// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_codec_version_str at aom_codec.c:28:13 in aom_codec.h
// aom_codec_version_extra_str at aom_codec.c:30:13 in aom_codec.h
// aom_codec_build_config at aom_config.c:13:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_obu_type_to_string at aom_codec.c:186:13 in aom_codec.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Required Header Files */
#include "aomcx.h"
#include "aomdx.h"
#include "aom.h"
#include "aom_codec.h"
#include "aom_external_partition.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom_frame_buffer.h"
#include "aom_encoder.h"
#include "aom_integer.h"

/*
 * Fuzz driver for libaom API functions:
 * - aom_codec_version_extra_str
 * - aom_codec_err_to_string
 * - aom_obu_type_to_string
 * - aom_codec_version_str
 * - aom_codec_build_config
 * - aom_codec_iface_name
 */

static void test_version_functions(void) {
    /* Test aom_codec_version_str */
    const char *version_str = aom_codec_version_str();
    if (version_str != NULL) {
        /* Just ensure it's accessible; no cleanup needed as it's static */
        (void)strlen(version_str);
    }

    /* Test aom_codec_version_extra_str */
    const char *version_extra_str = aom_codec_version_extra_str();
    if (version_extra_str != NULL) {
        (void)strlen(version_extra_str);
    }

    /* Test aom_codec_build_config */
    const char *build_config = aom_codec_build_config();
    if (build_config != NULL) {
        (void)strlen(build_config);
    }
}

static void test_error_string_conversion(const uint8_t *Data, size_t Size) {
    /* Fuzz aom_codec_err_to_string with various error codes */
    /* We use the input data to generate different error code values */
    for (size_t i = 0; i < Size && i < 100; i++) {
        aom_codec_err_t err = (aom_codec_err_t)Data[i];
        const char *err_str = aom_codec_err_to_string(err);
        if (err_str != NULL) {
            (void)strlen(err_str);
        }
    }
}

static void test_obu_type_string_conversion(const uint8_t *Data, size_t Size) {
    /* Fuzz aom_obu_type_to_string with various OBU_TYPE values */
    for (size_t i = 0; i < Size && i < 100; i++) {
        OBU_TYPE type = (OBU_TYPE)Data[i];
        const char *type_str = aom_obu_type_to_string(type);
        if (type_str != NULL) {
            (void)strlen(type_str);
        }
    }
}

static void test_codec_iface_name(const uint8_t *Data, size_t Size) {
    /* Test aom_codec_iface_name with NULL and valid interfaces */
    
    /* Test with NULL */
    const char *null_iface_name = aom_codec_iface_name(NULL);
    if (null_iface_name != NULL) {
        (void)strlen(null_iface_name);
    }

    /* Try to get valid interfaces if available */
    /* Note: We don't initialize a codec here as it's complex and not strictly needed
     * for testing the iface_name function's robustness with NULL and potential valid pointers.
     * However, to explore more states, we can try to get standard interfaces if the library provides them.
     * Since we cannot easily create a valid aom_codec_iface_t without initialization,
     * we rely on the NULL test and the fact that the function handles NULL gracefully.
     * In a real fuzzing scenario, we might try to initialize a codec if the input data suggests it,
     * but for these specific read-only string functions, NULL testing is primary.
     */
    
    /* Attempt to get AV1 decoder interface if available */
    aom_codec_iface_t *av1_dec_iface = aom_codec_av1_dx();
    if (av1_dec_iface != NULL) {
        const char *av1_dec_name = aom_codec_iface_name(av1_dec_iface);
        if (av1_dec_name != NULL) {
            (void)strlen(av1_dec_name);
        }
    }

    /* Attempt to get AV1 encoder interface if available */
    aom_codec_iface_t *av1_enc_iface = aom_codec_av1_cx();
    if (av1_enc_iface != NULL) {
        const char *av1_enc_name = aom_codec_iface_name(av1_enc_iface);
        if (av1_enc_name != NULL) {
            (void)strlen(av1_enc_name);
        }
    }
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    /* Prevent empty input issues if any logic depends on it, though these functions don't */
    if (Size == 0) {
        return 0;
    }

    /* 1. Test version and build config functions (no args) */
    test_version_functions();

    /* 2. Test error code to string conversion using input data */
    test_error_string_conversion(Data, Size);

    /* 3. Test OBU type to string conversion using input data */
    test_obu_type_string_conversion(Data, Size);

    /* 4. Test codec interface name function */
    test_codec_iface_name(Data, Size);

    /* No cleanup required as all functions return static strings or handle NULL safely */
    return 0;
}