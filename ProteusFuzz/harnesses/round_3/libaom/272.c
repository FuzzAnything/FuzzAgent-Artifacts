// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_272(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface = NULL;
    aom_codec_err_t err;
    const char *err_str;
    const char *detail_str;

    // Test aom_codec_error with NULL context
    err_str = aom_codec_error(NULL);
    if (err_str != NULL) {
        // Just consume the string to avoid unused warning
        size_t len = strlen(err_str);
        if (len > 0) {
            volatile char c = err_str[0];
            (void)c;
        }
    }

    // Test aom_codec_err_to_string with various error codes
    // We can't iterate all possible enums easily without full header, 
    // so we test a few known ones and derived from input
    aom_codec_err_t test_err = (aom_codec_err_t)(Data[0] & 0xFF);
    err_str = aom_codec_err_to_string(test_err);
    if (err_str != NULL) {
        size_t len = strlen(err_str);
        if (len > 0) {
            volatile char c = err_str[0];
            (void)c;
        }
    }

    // Try to initialize an encoder context to test aom_codec_error with valid context
    // We use aom_codec_av1_cx() to get the AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (iface == NULL) {
        // If interface is not available, we can't proceed with context tests
        // but we have already tested the NULL case and err_to_string
        return 0;
    }

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (ctx == NULL) {
        return 0;
    }
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    // Initialize configuration
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    // Use input data to influence configuration slightly to explore states
    if (Size > 0) {
        cfg.g_w = (Data[0] % 1024) + 16; // Ensure width is at least 16
        cfg.g_h = (Data[1 % Size] % 1024) + 16; // Ensure height is at least 16
        cfg.g_timebase.num = (Data[2 % Size] % 100) + 1;
        cfg.g_timebase.den = (Data[3 % Size] % 100) + 1;
        cfg.rc_target_bitrate = (Data[4 % Size] % 1000) + 100;
    }

    // Initialize the encoder
    err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        // Even if init fails, we can test aom_codec_error on the context
        err_str = aom_codec_error(ctx);
        if (err_str != NULL) {
            size_t len = strlen(err_str);
            if (len > 0) {
                volatile char c = err_str[0];
                (void)c;
            }
        }
        detail_str = aom_codec_error_detail(ctx);
        if (detail_str != NULL) {
            size_t len = strlen(detail_str);
            if (len > 0) {
                volatile char c = detail_str[0];
                (void)c;
            }
        }
        cleanup_codec(ctx);
        return 0;
    }

    // At this point, ctx is initialized. Test aom_codec_error on valid context
    err_str = aom_codec_error(ctx);
    if (err_str != NULL) {
        size_t len = strlen(err_str);
        if (len > 0) {
            volatile char c = err_str[0];
            (void)c;
        }
    }

    // Try to trigger some control operations if possible, though the specific
    // typechecked functions provided in the prompt had empty signatures.
    // We rely on the fact that aom_codec_control might be called internally
    // or we can try standard controls if we had their signatures.
    // Since the specific control functions are undefined in the prompt,
    // we focus on the error reporting functions which are well-defined.

    // Simulate an error condition by passing invalid data to encode if possible
    // However, aom_codec_encode requires an image. We can't easily create one
    // from raw fuzz data without more complex logic.
    // Instead, we just destroy the context and check for errors after destruction
    // (though typically no error is set after successful destroy).

    cleanup_codec(ctx);

    return 0;
}