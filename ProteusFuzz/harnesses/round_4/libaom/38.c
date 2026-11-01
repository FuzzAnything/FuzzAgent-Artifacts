// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_codec_version_str at aom_codec.c:28:13 in aom_codec.h
// aom_codec_version_extra_str at aom_codec.c:30:13 in aom_codec.h
// aom_obu_type_to_string at aom_codec.c:186:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_build_config at aom_config.c:13:13 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

/*
 * LLVMFuzzerTestOneInput is the entry point for the fuzzer.
 * It receives Data and Size from the fuzzer.
 */
int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    // Prevent empty inputs from causing issues in logic that might depend on Size
    if (Size == 0) {
        return 0;
    }

    // 1. aom_codec_version_str
    // No arguments required. Returns static string.
    const char *ver_str = aom_codec_version_str();
    if (ver_str != NULL) {
        // Consume the string to ensure it's valid and prevent unused variable warnings
        // We do not free this as it is static.
        size_t len = strlen(ver_str);
        if (len > 0) {
            // Simple check to ensure the string is readable
            volatile char c = ver_str[0];
            (void)c;
        }
    }

    // 2. aom_codec_version_extra_str
    // No arguments required. Returns static string.
    const char *extra_str = aom_codec_version_extra_str();
    if (extra_str != NULL) {
        size_t len = strlen(extra_str);
        if (len > 0) {
            volatile char c = extra_str[0];
            (void)c;
        }
    }

    // 3. aom_obu_type_to_string
    // Takes an OBU_TYPE enum. We fuzz the integer value to cover valid and invalid cases.
    // OBU_TYPE is an enum, so casting from uint8_t or uint32_t is safe for testing.
    OBU_TYPE obu_type = (OBU_TYPE)Data[0];
    const char *obu_str = aom_obu_type_to_string(obu_type);
    if (obu_str != NULL) {
        size_t len = strlen(obu_str);
        if (len > 0) {
            volatile char c = obu_str[0];
            (void)c;
        }
    }

    // 4. aom_codec_err_to_string
    // Takes an aom_codec_err_t. We fuzz the integer value.
    // Ensure we have enough data for a 32-bit integer
    aom_codec_err_t err_code = 0;
    if (Size >= sizeof(aom_codec_err_t)) {
        memcpy(&err_code, Data, sizeof(aom_codec_err_t));
    } else if (Size > 0) {
        err_code = (aom_codec_err_t)Data[0];
    }
    
    const char *err_str = aom_codec_err_to_string(err_code);
    if (err_str != NULL) {
        size_t len = strlen(err_str);
        if (len > 0) {
            volatile char c = err_str[0];
            (void)c;
        }
    }

    // 5. aom_codec_build_config
    // No arguments required. Returns static string.
    const char *build_cfg = aom_codec_build_config();
    if (build_cfg != NULL) {
        size_t len = strlen(build_cfg);
        if (len > 0) {
            volatile char c = build_cfg[0];
            (void)c;
        }
    }

    // 6. aom_codec_iface_name
    // Takes a pointer to aom_codec_iface_t.
    // We test with NULL (explicitly handled by API) and a valid interface if available.
    
    // Case A: NULL pointer
    const char *iface_name_null = aom_codec_iface_name(NULL);
    if (iface_name_null != NULL) {
        size_t len = strlen(iface_name_null);
        if (len > 0) {
            volatile char c = iface_name_null[0];
            (void)c;
        }
    }

    // Case B: Valid interface (AV1 Decoder)
    // We attempt to get the AV1 decoder interface.
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (iface != NULL) {
        const char *iface_name = aom_codec_iface_name(iface);
        if (iface_name != NULL) {
            size_t len = strlen(iface_name);
            if (len > 0) {
                volatile char c = iface_name[0];
                (void)c;
            }
        }
    }

    // Case C: Valid interface (AV1 Encoder)
    aom_codec_iface_t *iface_enc = aom_codec_av1_cx();
    if (iface_enc != NULL) {
        const char *iface_name_enc = aom_codec_iface_name(iface_enc);
        if (iface_name_enc != NULL) {
            size_t len = strlen(iface_name_enc);
            if (len > 0) {
                volatile char c = iface_name_enc[0];
                (void)c;
            }
        }
    }

    // 7. File handling (as per instructions, though not strictly required by these specific APIs)
    // If the code requires a file, use "./dummy_file".
    // Since these APIs are stateless or use internal statics, we don't strictly need a file,
    // but we can write the input to it to satisfy the instruction pattern if needed.
    // However, writing to a file in a tight fuzzing loop can be slow. 
    // We will skip writing unless the API explicitly demands it, which these do not.
    // The instruction says "If the code requires a file...". These functions do not.

    return 0;
}