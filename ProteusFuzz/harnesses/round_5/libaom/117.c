// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx = {0};
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    
    // Use input data to vary configuration slightly to explore states
    cfg.g_w = (Size % 2048) + 16;
    cfg.g_h = (Size % 2048) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = (Size % 10000) + 100;

    aom_codec_err_t err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz aom_codec_get_global_headers
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&ctx);
    if (global_headers) {
        if (global_headers->buf) {
            free(global_headers->buf);
        }
        free(global_headers);
    }

    // AV1E_SET_FILM_GRAIN_TEST_VECTOR
    // Pass raw data if size is sufficient, otherwise skip to avoid invalid pointer issues
    if (Size >= 1) {
        aom_codec_control(&ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, (void*)(uintptr_t)Data);
    }

    // AV1E_SET_PARTITION_INFO_PATH
    // This expects a null-terminated string. We must ensure the buffer is null-terminated.
    if (Size > 0) {
        char *path_buf = (char *)malloc(Size + 1);
        if (path_buf) {
            memcpy(path_buf, Data, Size);
            path_buf[Size] = '\0'; // Ensure null termination
            aom_codec_control(&ctx, AV1E_SET_PARTITION_INFO_PATH, path_buf);
            free(path_buf);
        }
    }

    // AV1E_SET_ERROR_RESILIENT_MODE
    // Likely expects an integer.
    if (Size >= 4) {
        int32_t mode = *(const int32_t *)Data;
        aom_codec_control(&ctx, AV1E_SET_ERROR_RESILIENT_MODE, &mode);
    } else if (Size > 0) {
        int32_t mode = (int32_t)Data[0];
        aom_codec_control(&ctx, AV1E_SET_ERROR_RESILIENT_MODE, &mode);
    }

    // AV1E_SET_FILM_GRAIN_TABLE
    // The crash occurred here because the library likely treats the input as a string
    // or expects a specific structure with a string member, and reads past the buffer
    // looking for a null terminator.
    // To safely fuzz this, we should either:
    // 1. Ensure the data is null-terminated (like a string).
    // 2. Or, if it expects a specific struct, we cannot easily fuzz it with raw bytes
    //    without knowing the struct layout.
    // Given the crash log shows `strlen` being called inside the library for this command,
    // it implies the argument is treated as a string (or contains a string).
    // We will create a null-terminated copy for this command as well.
    if (Size > 0) {
        char *fg_table_buf = (char *)malloc(Size + 1);
        if (fg_table_buf) {
            memcpy(fg_table_buf, Data, Size);
            fg_table_buf[Size] = '\0'; // Ensure null termination to prevent heap-buffer-overflow
            aom_codec_control(&ctx, AV1E_SET_FILM_GRAIN_TABLE, fg_table_buf);
            free(fg_table_buf);
        }
    }

    // AV1E_SET_GF_MIN_PYRAMID_HEIGHT
    // Likely expects an integer.
    if (Size >= 4) {
        int32_t height = *(const int32_t *)Data;
        aom_codec_control(&ctx, AV1E_SET_GF_MIN_PYRAMID_HEIGHT, &height);
    } else if (Size > 0) {
        int32_t height = (int32_t)Data[0];
        aom_codec_control(&ctx, AV1E_SET_GF_MIN_PYRAMID_HEIGHT, &height);
    }

    cleanup_codec(&ctx);
    return 0;
}