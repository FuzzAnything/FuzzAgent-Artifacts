// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_obu_type_to_string at aom_codec.c:186:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_version_str at aom_codec.c:28:13 in aom_codec.h
// aom_codec_version_extra_str at aom_codec.c:30:13 in aom_codec.h
// aom_codec_build_config at aom_config.c:13:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_obu_type_to_string at aom_codec.c:186:13 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

/* Required Header Files */
#include <aom_decoder.h>
#include <aomdx.h>
#include <aom_external_partition.h>
#include <aom_integer.h>
#include <aom_image.h>
#include <aom_encoder.h>
#include <aomcx.h>
#include <aom.h>
#include <aom_codec.h>
#include <aom_frame_buffer.h>

/*
 * Fuzz driver for libaom API functions:
 * 1. aom_codec_iface_name
 * 2. aom_obu_type_to_string
 * 3. aom_codec_err_to_string
 * 4. aom_codec_version_extra_str
 * 5. aom_codec_build_config
 * 6. aom_codec_version_str
 *
 * These functions are primarily informational and do not require complex state
 * setup, but we will attempt to exercise them with various inputs derived from
 * the fuzzing data to ensure robustness.
 */

/* Helper to safely print strings to avoid potential issues in some environments,
 * though these functions return static strings. */
static void safe_print_string(const char *str) {
    if (str) {
        /* Just accessing the string to ensure it's valid and not NULL */
        size_t len = strlen(str);
        (void)len;
    }
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /*
     * 1. Test aom_codec_version_str
     * No arguments required. Always returns a static string.
     */
    const char *version_str = aom_codec_version_str();
    safe_print_string(version_str);

    /*
     * 2. Test aom_codec_version_extra_str
     * No arguments required. Returns build-specific details.
     */
    const char *version_extra = aom_codec_version_extra_str();
    safe_print_string(version_extra);

    /*
     * 3. Test aom_codec_build_config
     * No arguments required. Returns build configuration string.
     */
    const char *build_config = aom_codec_build_config();
    safe_print_string(build_config);

    /*
     * 4. Test aom_codec_err_to_string
     * We can iterate through possible error codes or derive one from input.
     * aom_codec_err_t is an enum. We'll try to map input bytes to error codes.
     * Common errors include AOM_CODEC_OK, AOM_CODEC_MEM_ERROR, etc.
     * We will use the first byte of input modulo a reasonable range to simulate
     * different error codes, including invalid ones.
     */
    aom_codec_err_t err_code = (aom_codec_err_t)(Data[0] % 20);
    /* Also test with a negative value if the enum allows, though typically unsigned/positive */
    if (Size > 1) {
        err_code = (aom_codec_err_t)(Data[1]);
    }
    const char *err_str = aom_codec_err_to_string(err_code);
    safe_print_string(err_str);

    /*
     * 5. Test aom_obu_type_to_string
     * OBU_TYPE is an enum. We'll derive a type from the input data.
     * We'll try a few different values from the input stream.
     */
    if (Size > 0) {
        OBU_TYPE obu_type = (OBU_TYPE)(Data[0] % 30); /* Assume OBU_TYPE values are within reasonable range */
        const char *obu_str = aom_obu_type_to_string(obu_type);
        safe_print_string(obu_str);
    }

    /*
     * 6. Test aom_codec_iface_name
     * This function requires an aom_codec_iface_t*.
     * We can pass NULL to test the "<invalid interface>" case.
     * We can also try to get a valid interface using aom_codec_av1_cx() or aom_codec_av1_dx()
     * if available in the linked library, but the task description implies we might not
     * have the actual codec implementations linked or initialized in a simple fuzz harness.
     * However, aom_codec_av1_cx() and aom_codec_av1_dx() are standard entry points.
     * Let's try to get a valid interface pointer.
     */
    
    /* Test with NULL first */
    const char *null_iface_name = aom_codec_iface_name(NULL);
    safe_print_string(null_iface_name);

    /* Try to get a valid AV1 encoder interface */
    aom_codec_iface_t *av1_enc_iface = aom_codec_av1_cx();
    if (av1_enc_iface) {
        const char *enc_name = aom_codec_iface_name(av1_enc_iface);
        safe_print_string(enc_name);
    }

    /* Try to get a valid AV1 decoder interface */
    aom_codec_iface_t *av1_dec_iface = aom_codec_av1_dx();
    if (av1_dec_iface) {
        const char *dec_name = aom_codec_iface_name(av1_dec_iface);
        safe_print_string(dec_name);
    }

    /*
     * Additional exploration:
     * If the input data is large enough, we can try to interpret parts of it
     * as error codes or OBU types again to ensure we cover edge cases.
     */
    if (Size > 2) {
        /* Use another byte for OBU type */
        OBU_TYPE obu_type_2 = (OBU_TYPE)(Data[2]);
        const char *obu_str_2 = aom_obu_type_to_string(obu_type_2);
        safe_print_string(obu_str_2);

        /* Use another byte for error code */
        aom_codec_err_t err_code_2 = (aom_codec_err_t)(Data[2]);
        const char *err_str_2 = aom_codec_err_to_string(err_code_2);
        safe_print_string(err_str_2);
    }

    return 0;
}