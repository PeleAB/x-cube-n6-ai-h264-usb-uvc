# cmake/nn_model.cmake
# NN model selection helper for x-cube-n6 firmware.
#
# Usage (after include("mx-generated.cmake")):
#   include(cmake/nn_model.cmake)
#   setup_nn_model(${CMAKE_PROJECT_NAME})
#
# Cache variables:
#   NN_MODEL  — model name; "network" uses the legacy Model/network.c at the
#               repository root (backward-compatible default).  Any other value
#               must match a sub-directory under Model/  e.g. "centerface" →
#               Model/centerface/*.c + Model/centerface/*.h.
#
# Compile definitions injected into the target:
#   NN_MODEL_INSTANCE — ATON instance name token (Default for legacy "network",
#                       otherwise equals NN_MODEL).
#   NN_MODEL_HEADER   — quoted header filename, e.g. "network.h".
#
# app_pipeline.c uses these to select the right header and declare the right
# ATON instance without any hard-coded "Default" references.

set(NN_MODEL "network" CACHE STRING
    "NN model to compile ('network' = legacy root-level model; \
otherwise = sub-directory of Model/ containing the generated .c/.h files)")

# ---------------------------------------------------------------------------
# setup_face_models(TARGET)
#
# Adds the face_detection (CenterFace) and face_recognition (MobileFaceNet)
# model directories to the build.  The model .c/.h files are copied from
# the STM32N6-FaceRecognition reference app into Model/face_detection/ and
# Model/face_recognition/.
#
# Symbol naming: models use unique name prefixes so that
# LL_ATON_DECLARE_NAMED_NN_INSTANCE_AND_INTERFACE(face_detection) and
# LL_ATON_DECLARE_NAMED_NN_INSTANCE_AND_INTERFACE(face_recognition) resolve
# without colliding with the primary YOLO model symbols.
# ---------------------------------------------------------------------------
function(setup_face_models TARGET)
    set(_root "${CMAKE_SOURCE_DIR}/Model")
    set(_face_models "face_detection" "face_recognition")

    foreach(_name IN LISTS _face_models)
        set(_model_dir "${_root}/${_name}")
        if(NOT EXISTS "${_model_dir}")
            message(WARNING
                "setup_face_models: Model/${_name}/ not found — "
                "face recognition pipeline will fail to link. "
                "Generate the model with STM32Cube.AI and deploy to Model/${_name}/.")
            continue()
        endif()

        file(GLOB _model_srcs "${_model_dir}/*.c")
        if(NOT _model_srcs)
            message(WARNING
                "setup_face_models: no .c files in Model/${_name}/")
            continue()
        endif()

        target_sources(${TARGET} PRIVATE ${_model_srcs})
        target_include_directories(${TARGET} PRIVATE "${_model_dir}")
    endforeach()
endfunction()

function(setup_nn_model TARGET)
    set(_name "${NN_MODEL}")
    set(_root "${CMAKE_SOURCE_DIR}/Model")

    if(_name STREQUAL "network")
        # ── Legacy layout ────────────────────────────────────────────────────
        # Model/network.c is already added by mx-generated.cmake.
        # We just expose the Model/ directory for #include "network.h" and
        # pass the default instance/header compile definitions.
        target_include_directories(${TARGET} PRIVATE "${_root}")
        target_compile_definitions(${TARGET} PRIVATE
            NN_MODEL_INSTANCE=Default
            "NN_MODEL_HEADER=\"network.h\""
        )
    else()
        # ── Custom model layout ──────────────────────────────────────────────
        set(_model_dir "${_root}/${_name}")
        if(NOT EXISTS "${_model_dir}")
            message(FATAL_ERROR
                "setup_nn_model: directory not found: ${_model_dir}\n"
                "  Deploy the model first via the model_converter 'Deploy to Firmware' button,\n"
                "  or copy the generated files to Model/${_name}/")
        endif()

        # Suppress legacy Model/network.c (already listed in mx-generated.cmake)
        set_source_files_properties("${_root}/network.c"
            PROPERTIES HEADER_FILE_ONLY TRUE)

        # Add all generated .c files from the model subdirectory
        file(GLOB _model_srcs "${_model_dir}/*.c")
        if(NOT _model_srcs)
            message(FATAL_ERROR
                "setup_nn_model: no .c files in ${_model_dir}")
        endif()
        target_sources(${TARGET} PRIVATE ${_model_srcs})

        # Expose the model's include directory
        target_include_directories(${TARGET} PRIVATE "${_model_dir}")

        # Pass model-specific definitions
        target_compile_definitions(${TARGET} PRIVATE
            NN_MODEL_INSTANCE=${_name}
            "NN_MODEL_HEADER=\"${_name}.h\""
        )
    endif()
endfunction()
