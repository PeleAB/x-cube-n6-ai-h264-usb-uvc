/**
 * SysobjUart Protocol Logic for TypeScript
 * Matches the C implementation in sysobj_uart.c/h
 */

export const SysobjUartMsgType = {
    MANAGE: 0x00,
    CONFIG: 0x01,
    TEST: 0x02,
    CRITICAL: 0x03,
} as const;

export const SysobjUartManageSubtype = {
    SET_LED:   0x01,
    TELEMETRY: 0x02,
    GET_STATE: 0x03,
} as const;

export const SysobjUartConfigSubtype = {
    PARAM_READ:          0x01,
    PARAM_WRITE:         0x02,
    ENTER_CONFIG:        0x03,
    EXIT_CONFIG:         0x04,
    MODEL_SELECT:        0x05,
    ENROLL:              0x06,
    COMMIT_ENROLL:       0x07,
    CLEAR_EMBEDDINGS:    0x08,
} as const;

export const SysobjUartState = {
    BOOT:   0,
    ON:     1,
    CONFIG: 2,
} as const;
export type SysobjUartStateValue = typeof SysobjUartState[keyof typeof SysobjUartState];

export interface SysobjUartMsg {
    src_id: number;
    dst_id: number;
    is_ack: number; // 4 bits
    need_ack: number; // 4 bits
    msg_type: number;
    msg_subtype: number;
    data?: Uint8Array;
}

const SYSOBJ_UART_SOF = 0xAA;
const SYSOBJ_UART_PAYLOAD_HEADER_SIZE = 5;

let crc32Table: Uint32Array | null = null;

function initCrc32Table() {
    const polynomial = 0xedb88320;
    crc32Table = new Uint32Array(256);
    for (let i = 0; i < 256; i++) {
        let crc = i;
        for (let j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >>> 1) ^ polynomial;
            } else {
                crc >>>= 1;
            }
        }
        crc32Table[i] = crc;
    }
}

export function calculateCrc32(data: Uint8Array): number {
    if (!crc32Table) {
        initCrc32Table();
    }
    let crc = 0xffffffff;
    for (let i = 0; i < data.length; i++) {
        const lookupIdx = (crc ^ data[i]) & 0xff;
        crc = (crc >>> 8) ^ crc32Table![lookupIdx];
    }
    return (crc ^ 0xffffffff) >>> 0;
}

export function generatePacket(msg: SysobjUartMsg): Uint8Array {
    const dataLen = msg.data ? msg.data.length : 0;
    const payloadSize = SYSOBJ_UART_PAYLOAD_HEADER_SIZE + dataLen;
    const totalLen = 3 + payloadSize + 4;

    const buffer = new Uint8Array(totalLen);

    // Wrapper Header
    buffer[0] = SYSOBJ_UART_SOF;
    buffer[1] = payloadSize & 0xff;
    buffer[2] = (buffer[0] + buffer[1]) & 0xff;

    // Payload Header
    const payload = buffer.subarray(3, 3 + payloadSize);
    payload[0] = msg.src_id & 0xff;
    payload[1] = msg.dst_id & 0xff;
    payload[2] = ((msg.is_ack & 0x0f) << 4) | (msg.need_ack & 0x0f);
    payload[3] = msg.msg_type & 0xff;
    payload[4] = msg.msg_subtype & 0xff;

    // Payload Data
    if (msg.data) {
        payload.set(msg.data, 5);
    }

    // CRC32
    const crc = calculateCrc32(payload);
    const crcOffset = 3 + payloadSize;
    buffer[crcOffset] = crc & 0xff;
    buffer[crcOffset + 1] = (crc >>> 8) & 0xff;
    buffer[crcOffset + 2] = (crc >>> 16) & 0xff;
    buffer[crcOffset + 3] = (crc >>> 24) & 0xff;

    return buffer;
}

export function createSetLedMsg(ledId: number, state: number): SysobjUartMsg {
    return {
        src_id: 0x01, // Usually PC/App
        dst_id: 0x02, // Usually MCU
        is_ack: 0,
        need_ack: 1,
        msg_type: SysobjUartMsgType.MANAGE,
        msg_subtype: SysobjUartManageSubtype.SET_LED,
        data: new Uint8Array([ledId, state]),
    };
}

export function createRequestTelemetryMsg(): SysobjUartMsg {
    return {
        src_id: 0x01,
        dst_id: 0x02,
        is_ack: 0,
        need_ack: 0,
        msg_type: SysobjUartMsgType.MANAGE,
        msg_subtype: SysobjUartManageSubtype.TELEMETRY,
    };
}

export function formatHex(data: Uint8Array): string {
    return Array.from(data)
        .map(b => b.toString(16).padStart(2, '0').toUpperCase())
        .join(' ');
}

export function parsePacket(data: Uint8Array): { msg?: SysobjUartMsg; raw: string; error?: string } {
    const raw = formatHex(data);

    if (data.length < 8) {
        return { raw, error: 'Packet too short' };
    }

    if (data[0] !== SYSOBJ_UART_SOF) {
        return { raw, error: 'Invalid SOF' };
    }

    const payloadSize = data[1];
    const checksum = data[2];
    if (((data[0] + data[1]) & 0xff) !== checksum) {
        return { raw, error: 'Invalid checksum' };
    }

    if (data.length < 3 + payloadSize + 4) {
        return { raw, error: 'Incomplete packet' };
    }

    const payload = data.subarray(3, 3 + payloadSize);
    const receivedCrc = (data[3 + payloadSize]) |
        (data[3 + payloadSize + 1] << 8) |
        (data[3 + payloadSize + 2] << 16) |
        (data[3 + payloadSize + 3] << 24);

    const calculatedCrc = calculateCrc32(payload);
    if ((calculatedCrc >>> 0) !== (receivedCrc >>> 0)) {
        return { raw, error: `CRC mismatch (calc: ${calculatedCrc.toString(16)}, recv: ${receivedCrc.toString(16)})` };
    }

    return {
        raw,
        msg: {
            src_id: payload[0],
            dst_id: payload[1],
            is_ack: (payload[2] >> 4) & 0x0f,
            need_ack: payload[2] & 0x0f,
            msg_type: payload[3],
            msg_subtype: payload[4],
            data: payload.length > 5 ? payload.slice(5) : undefined
        }
    };
}

export function extractPackets(buffer: number[]): { msg?: SysobjUartMsg; raw: string; error?: string }[] {
    const results: { msg?: SysobjUartMsg; raw: string; error?: string }[] = [];

    while (buffer.length > 0) {
        // Find next SOF
        if (buffer[0] !== 0xAA) {
            buffer.shift();
            continue;
        }

        // Minimum header size to determine length
        if (buffer.length < 3) {
            break; // Wait for more data
        }

        const payloadSize = buffer[1];
        const checksum = buffer[2];

        // Checksum mismatch -> false SOF, drop it and resync
        if (((0xAA + payloadSize) & 0xff) !== checksum) {
            buffer.shift();
            continue;
        }

        const totalLen = 3 + payloadSize + 4; // SOF + Size + CHK + Payload + CRC32

        // Check if we received enough bytes for the declared payload size
        if (buffer.length < totalLen) {
            break; // Wait for the rest of the packet
        }

        // We have a full packet, extract it
        const packetBytes = new Uint8Array(buffer.slice(0, totalLen));
        buffer.splice(0, totalLen); // Remove it from the incoming stream

        // Use the existing strict parser
        results.push(parsePacket(packetBytes));
    }

    return results;
}

// ---------------------------------------------------------------------------
// CONFIG message helpers
// ---------------------------------------------------------------------------

/**
 * Build a PARAM_READ request.
 * Payload: param_id[0..1] LE
 */
export function createParamReadMsg(paramId: number): SysobjUartMsg {
    const data = new Uint8Array(2);
    data[0] = paramId & 0xff;
    data[1] = (paramId >> 8) & 0xff;
    return {
        src_id: 0x01,
        dst_id: 0x02,
        is_ack: 0,
        need_ack: 0,
        msg_type: SysobjUartMsgType.CONFIG,
        msg_subtype: SysobjUartConfigSubtype.PARAM_READ,
        data,
    };
}

/**
 * Build a PARAM_WRITE request.
 * Payload: param_id[0..1] LE, type[2]=0 (U32), value[3..10] LE uint64
 */
export function createParamWriteMsg(paramId: number, value: number): SysobjUartMsg {
    const data = new Uint8Array(11);
    data[0] = paramId & 0xff;
    data[1] = (paramId >> 8) & 0xff;
    data[2] = 0; // PARAM_TYPE_U32
    // 32-bit value in LE, upper 4 bytes = 0
    data[3] = (value >>> 0) & 0xff;
    data[4] = (value >>> 8) & 0xff;
    data[5] = (value >>> 16) & 0xff;
    data[6] = (value >>> 24) & 0xff;
    // data[7..10] = 0 already
    return {
        src_id: 0x01,
        dst_id: 0x02,
        is_ack: 0,
        need_ack: 0,
        msg_type: SysobjUartMsgType.CONFIG,
        msg_subtype: SysobjUartConfigSubtype.PARAM_WRITE,
        data,
    };
}

export function createEnterConfigMsg(): SysobjUartMsg {
    return {
        src_id: 0x01,
        dst_id: 0x02,
        is_ack: 0,
        need_ack: 0,
        msg_type: SysobjUartMsgType.CONFIG,
        msg_subtype: SysobjUartConfigSubtype.ENTER_CONFIG,
    };
}

export function createExitConfigMsg(): SysobjUartMsg {
    return {
        src_id: 0x01,
        dst_id: 0x02,
        is_ack: 0,
        need_ack: 0,
        msg_type: SysobjUartMsgType.CONFIG,
        msg_subtype: SysobjUartConfigSubtype.EXIT_CONFIG,
    };
}

export function createGetStateMsg(): SysobjUartMsg {
    return {
        src_id: 0x01,
        dst_id: 0x02,
        is_ack: 0,
        need_ack: 0,
        msg_type: SysobjUartMsgType.MANAGE,
        msg_subtype: SysobjUartManageSubtype.GET_STATE,
    };
}

export interface StateResponse {
    status: number;
    state: number;  // SysobjUartState value
}

/**
 * Parse a CONFIG/ENTER_CONFIG, EXIT_CONFIG, or GET_STATE response (2 bytes):
 * [0]=status (0=OK, 0x10=wrong state), [1]=current state
 */
export function parseStateResponse(data: Uint8Array): StateResponse | null {
    if (!data || data.length < 2) return null;
    return { status: data[0], state: data[1] };
}

export interface ParamReadResponse {
    status: number;   // params_status_t (0 = OK)
    paramId: number;
    type: number;     // param_type_t (0 = U32)
    value: number;    // uint32 value
    wasDefault: boolean;
}

/**
 * Parse a CONFIG/PARAM_READ response payload (17 bytes):
 * [0]=status, [1..2]=param_id LE, [3]=type, [4..11]=value LE u64,
 * [12..15]=entry_crc32, [16]=was_default
 */
export function parseParamReadResponse(data: Uint8Array): ParamReadResponse | null {
    if (!data || data.length < 17) return null;
    const status = data[0];
    const paramId = data[1] | (data[2] << 8);
    const type = data[3];
    const value = ((data[4] | (data[5] << 8) | (data[6] << 16) | (data[7] << 24)) >>> 0);
    const wasDefault = data[16] !== 0;
    return { status, paramId, type, value, wasDefault };
}

export interface ParamWriteResponse {
    status: number;   // params_status_t (0 = OK)
    paramId: number;
}

/**
 * Parse a CONFIG/PARAM_WRITE response payload (3 bytes):
 * [0]=status, [1..2]=param_id LE
 */
export function parseParamWriteResponse(data: Uint8Array): ParamWriteResponse | null {
    if (!data || data.length < 3) return null;
    const status = data[0];
    const paramId = data[1] | (data[2] << 8);
    return { status, paramId };
}

export function getMsgTypeName(type: number): string {
    for (const [name, value] of Object.entries(SysobjUartMsgType)) {
        if (value === type) return name;
    }
    return `UNKNOWN(0x${type.toString(16).toUpperCase()})`;
}

export function getMsgSubtypeName(type: number, subtype: number): string {
    if (type === SysobjUartMsgType.MANAGE) {
        for (const [name, value] of Object.entries(SysobjUartManageSubtype)) {
            if (value === subtype) return name;
        }
    }
    if (type === SysobjUartMsgType.CONFIG) {
        for (const [name, value] of Object.entries(SysobjUartConfigSubtype)) {
            if (value === subtype) return name;
        }
    }
    return `0x${subtype.toString(16).toUpperCase()}`;
}

export function getStateName(state: number): string {
    for (const [name, value] of Object.entries(SysobjUartState)) {
        if (value === state) return name;
    }
    return 'UNKNOWN';
}

// ---------------------------------------------------------------------------
// MODEL_SELECT message helpers
// ---------------------------------------------------------------------------

/**
 * Build a MODEL_SELECT request.
 * Payload: model_id[0..1] LE
 */
export function createModelSelectMsg(modelId: number): SysobjUartMsg {
    const data = new Uint8Array(2);
    data[0] = modelId & 0xff;
    data[1] = (modelId >> 8) & 0xff;
    return {
        src_id: 0x01,
        dst_id: 0x02,
        is_ack: 0,
        need_ack: 0,
        msg_type: SysobjUartMsgType.CONFIG,
        msg_subtype: SysobjUartConfigSubtype.MODEL_SELECT,
        data,
    };
}

// ---------------------------------------------------------------------------
// Face recognition enrollment message helpers
// ---------------------------------------------------------------------------

/** Request enrollment of the next detected face (no payload). */
export function createEnrollMsg(): SysobjUartMsg {
    return {
        src_id: 0x01,
        dst_id: 0x02,
        is_ack: 0,
        need_ack: 0,
        msg_type: SysobjUartMsgType.CONFIG,
        msg_subtype: SysobjUartConfigSubtype.ENROLL,
    };
}

/** Request commit of enrolled samples to NOR flash (no payload). */
export function createCommitEnrollMsg(): SysobjUartMsg {
    return {
        src_id: 0x01,
        dst_id: 0x02,
        is_ack: 0,
        need_ack: 0,
        msg_type: SysobjUartMsgType.CONFIG,
        msg_subtype: SysobjUartConfigSubtype.COMMIT_ENROLL,
    };
}

/** Request clearing of all enrolled embeddings (no payload). */
export function createClearEmbeddingsMsg(): SysobjUartMsg {
    return {
        src_id: 0x01,
        dst_id: 0x02,
        is_ack: 0,
        need_ack: 0,
        msg_type: SysobjUartMsgType.CONFIG,
        msg_subtype: SysobjUartConfigSubtype.CLEAR_EMBEDDINGS,
    };
}

export interface EnrollResponse {
    status: number;  // 0 = OK, 0x10 = wrong state
}

/** Parse a CONFIG/ENROLL response payload (1 byte): [0]=status */
export function parseEnrollResponse(data: Uint8Array): EnrollResponse | null {
    if (!data || data.length < 1) return null;
    return { status: data[0] };
}

export interface CommitEnrollResponse {
    status: number;    // 0 = OK, 0x10 = wrong state
    sampleCount: number;
}

/**
 * Parse a CONFIG/COMMIT_ENROLL response payload (5 bytes):
 * [0]=status, [1..4]=sample_count LE uint32
 */
export function parseCommitEnrollResponse(data: Uint8Array): CommitEnrollResponse | null {
    if (!data || data.length < 5) return null;
    const status = data[0];
    const sampleCount = (data[1] | (data[2] << 8) | (data[3] << 16) | (data[4] << 24)) >>> 0;
    return { status, sampleCount };
}

export interface ModelSelectResponse {
    status: number;   // 0 = OK, 0x10 = wrong state
    modelId: number;
}

/**
 * Parse a CONFIG/MODEL_SELECT response payload (3 bytes):
 * [0]=status, [1..2]=model_id LE
 */
export function parseModelSelectResponse(data: Uint8Array): ModelSelectResponse | null {
    if (!data || data.length < 3) return null;
    const status = data[0];
    const modelId = data[1] | (data[2] << 8);
    return { status, modelId };
}
