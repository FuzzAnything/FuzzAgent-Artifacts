// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_codec_iface_name at aom_codec.c:32:13 in aom_codec.h
// aom_codec_build_config at aom_config.c:13:13 in aom_codec.h
// aom_codec_version_str at aom_codec.c:28:13 in aom_codec.h
// aom_codec_version_extra_str at aom_codec.c:30:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_obu_type_to_string at aom_codec.c:186:13 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
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

#include "aom_encoder.h"
#include "aom_external_partition.h"
#include "aom_codec.h"
#include "aomdx.h"
#include "aom_integer.h"
#include "aom_frame_buffer.h"
#include "aom_image.h"
#include "aom_decoder.h"
#include "aom.h"
#include "aomcx.h"

/*
 * Fuzz driver for libaom API functions:
 * - aom_codec_build_config
 * - aom_codec_version_str
 * - aom_codec_iface_name
 * - aom_codec_err_to_string
 * - aom_obu_type_to_string
 * - aom_codec_version_extra_str
 *
 * These functions are mostly stateless or return static strings.
 * The fuzzer explores different inputs where applicable (e.g., error codes, OBU types,
 * and interface pointers derived from available codec interfaces).
 */

static void test_codec_info_functions(void) {
    const char *build_config = aom_codec_build_config();
    const char *version_str = aom_codec_version_str();
    const char *version_extra = aom_codec_version_extra_str();

    /* Ensure returned pointers are not NULL (they should be static strings) */
    if (build_config != NULL) {
        /* Consume the string to avoid unused variable warnings and explore memory */
        size_t len = strlen(build_config);
        (void)len;
    }
    if (version_str != NULL) {
        size_t len = strlen(version_str);
        (void)len;
    }
    if (version_extra != NULL) {
        size_t len = strlen(version_extra);
        (void)len;
    }
}

static void test_error_string_conversion(const uint8_t *data, size_t size) {
    /* Interpret input bytes as potential error codes */
    if (size >= sizeof(aom_codec_err_t)) {
        aom_codec_err_t err;
        memcpy(&err, data, sizeof(aom_codec_err_t));
        const char *err_str = aom_codec_err_to_string(err);
        if (err_str != NULL) {
            size_t len = strlen(err_str);
            (void)len;
        }
    }

    /* Also test with some known error constants if available */
    const char *mem_err_str = aom_codec_err_to_string(AOM_CODEC_MEM_ERROR);
    if (mem_err_str != NULL) {
        size_t len = strlen(mem_err_str);
        (void)len;
    }
}

static void test_obu_type_string_conversion(const uint8_t *data, size_t size) {
    if (size >= sizeof(OBU_TYPE)) {
        OBU_TYPE type;
        memcpy(&type, data, sizeof(OBU_TYPE));
        const char *type_str = aom_obu_type_to_string(type);
        if (type_str != NULL) {
            size_t len = strlen(type_str);
            (void)len;
        }
    }

    /* Test with a few known OBU_TYPE values if defined */
    /* Note: OBU_TYPE enum values are not explicitly listed in the prompt,
       so we rely on the fuzzer to provide diverse values. */
}

static void test_codec_iface_name(const uint8_t *data, size_t size) {
    /* Try to get valid codec interfaces from libaom */
    aom_codec_iface_t *av1_enc_iface = aom_codec_av1_cx();
    aom_codec_iface_t *av1_dec_iface = aom_codec_av1_dx();

    /* Test with valid interfaces */
    if (av1_enc_iface != NULL) {
        const char *name = aom_codec_iface_name(av1_enc_iface);
        if (name != NULL) {
            size_t len = strlen(name);
            (void)len;
        }
    }

    if (av1_dec_iface != NULL) {
        const char *name = aom_codec_iface_name(av1_dec_iface);
        if (name != NULL) {
            size_t len = strlen(name);
            (void)len;
        }
    }

    /* Test with NULL interface */
    const char *null_name = aom_codec_iface_name(NULL);
    if (null_name != NULL) {
        size_t len = strlen(null_name);
        (void)len;
    }

    /* Attempt to interpret input data as a pointer (unsafe in general, but for fuzzing
       we can try to cast it to aom_codec_iface_t* to see how the function handles
       invalid pointers. This may cause a crash, which is useful for fuzzing.) */
    if (size >= sizeof(aom_codec_iface_t*)) {
        aom_codec_iface_t *fake_iface;
        memcpy(&fake_iface, data, sizeof(aom_codec_iface_t*));
        /* This call may dereference an invalid pointer; it's intentional for fuzzing */
        const char *fake_name = aom_codec_iface_name(fake_iface);
        if (fake_name != NULL) {
            size_t len = strlen(fake_name);
            (void)len;
        }
    }
}

int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size) {
    /* Always test the stateless info functions */
    test_codec_info_functions();

    /* Test error string conversion */
    test_error_string_conversion(Data, Size);

    /* Test OBU type string conversion */
    test_obu_type_string_conversion(Data, Size);

    /* Test codec interface name function */
    test_codec_iface_name(Data, Size);

    return 0;
}