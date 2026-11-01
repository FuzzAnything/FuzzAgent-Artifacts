// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_obu_type_to_string at aom_codec.c:186:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_build_config at aom_config.c:13:13 in aom_codec.h
// aom_codec_version_extra_str at aom_codec.c:30:13 in aom_codec.h
// aom_codec_version_str at aom_codec.c:28:13 in aom_codec.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_obu_type_to_string at aom_codec.c:186:13 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom.h"
#include "aom_codec.h"
#include "aom_encoder.h"
#include "aom_frame_buffer.h"
#include "aom_decoder.h"
#include "aom_external_partition.h"
#include "aomcx.h"
#include "aom_image.h"
#include "aomdx.h"
#include "aom_integer.h"

// Helper to safely consume input data to avoid unused variable warnings
static void consume_input(const uint8_t *data, size_t size) {
    volatile const uint8_t *p = data;
    volatile size_t i = size;
    (void)p;
    (void)i;
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    // 1. Prepare environment and consume input
    consume_input(Data, Size);

    // 2. Invoke target functions diversely

    // Function 1: aom_codec_build_config
    const char *build_config = aom_codec_build_config();
    if (build_config != NULL) {
        volatile size_t len = strlen(build_config);
        (void)len;
    }

    // Function 2: aom_codec_version_extra_str
    const char *version_extra = aom_codec_version_extra_str();
    if (version_extra != NULL) {
        volatile size_t len = strlen(version_extra);
        (void)len;
    }

    // Function 6: aom_codec_version_str
    const char *version_str = aom_codec_version_str();
    if (version_str != NULL) {
        volatile size_t len = strlen(version_str);
        (void)len;
    }

    // Function 3: aom_codec_iface_name
    // Test with NULL pointer
    const char *iface_name_null = aom_codec_iface_name(NULL);
    if (iface_name_null != NULL) {
        volatile size_t len = strlen(iface_name_null);
        (void)len;
    }

    // Test with valid interfaces if available
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    const char *iface_name_valid = aom_codec_iface_name(iface);
    if (iface_name_valid != NULL) {
        volatile size_t len = strlen(iface_name_valid);
        (void)len;
    }

    aom_codec_iface_t *iface_enc = aom_codec_av1_cx();
    const char *iface_name_enc = aom_codec_iface_name(iface_enc);
    if (iface_name_enc != NULL) {
        volatile size_t len = strlen(iface_name_enc);
        (void)len;
    }

    // Function 4: aom_obu_type_to_string
    // Fuzz the enum value using input data
    if (Size > 0) {
        uint8_t byte_val = Data[0];
        OBU_TYPE obu_type = (OBU_TYPE)byte_val;
        const char *obu_str = aom_obu_type_to_string(obu_type);
        if (obu_str != NULL) {
            volatile size_t len = strlen(obu_str);
            (void)len;
        }
    }
    
    // Try specific known values (using only defined constants)
    // OBU_TYPE enum values from aom.h
    OBU_TYPE known_types[] = {
        OBU_SEQUENCE_HEADER,
        OBU_TEMPORAL_DELIMITER,
        OBU_FRAME_HEADER,
        OBU_TILE_GROUP,
        OBU_METADATA,
        OBU_FRAME,
        OBU_REDUNDANT_FRAME_HEADER,
        OBU_TILE_LIST,
        OBU_PADDING
    };
    int num_known = sizeof(known_types) / sizeof(known_types[0]);
    for (int i = 0; i < num_known; i++) {
        const char *str = aom_obu_type_to_string(known_types[i]);
        if (str != NULL) {
            volatile size_t len = strlen(str);
            (void)len;
        }
    }

    // Function 5: aom_codec_err_to_string
    // Fuzz the error code using input data
    if (Size > 0) {
        uint8_t err_byte = Data[Size > 1 ? 1 : 0];
        aom_codec_err_t err = (aom_codec_err_t)err_byte;
        const char *err_str = aom_codec_err_to_string(err);
        if (err_str != NULL) {
            volatile size_t len = strlen(err_str);
            (void)len;
        }
    }

    // Try specific known error codes (using only defined constants available in standard libaom)
    // Removed undefined macros based on compiler error
    aom_codec_err_t known_errors[] = {
        AOM_CODEC_OK,
        AOM_CODEC_ERROR,
        AOM_CODEC_MEM_ERROR,
        AOM_CODEC_INVALID_PARAM,
        AOM_CODEC_UNSUP_FEATURE
    };
    int num_errors = sizeof(known_errors) / sizeof(known_errors[0]);
    for (int i = 0; i < num_errors; i++) {
        const char *str = aom_codec_err_to_string(known_errors[i]);
        if (str != NULL) {
            volatile size_t len = strlen(str);
            (void)len;
        }
    }

    // 3. Cleanup
    // No explicit cleanup required for these specific API calls.

    return 0;
}