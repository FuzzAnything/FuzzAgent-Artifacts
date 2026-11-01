// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder_context(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 1;
    }

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return 1;
    }

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return 1;
    }

    return 0;
}

static void free_global_headers(aom_fixed_buf_t *buf) {
    if (buf) {
        if (buf->buf) {
            free(buf->buf);
        }
        free(buf);
    }
}

int LLVMFuzzerTestOneInput_290(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg = {0};

    // Initialize configuration from input data
    cfg.g_w = (Size > 4) ? (Data[0] | (Data[1] << 8)) : 128;
    cfg.g_h = (Size > 6) ? (Data[2] | (Data[3] << 8)) : 128;
    if (cfg.g_w == 0 || cfg.g_h == 0) {
        cfg.g_w = 128;
        cfg.g_h = 128;
    }
    cfg.g_timebase.num = (Size > 8) ? Data[4] : 1;
    cfg.g_timebase.den = (Size > 9) ? Data[5] : 1;
    if (cfg.g_timebase.num == 0) cfg.g_timebase.num = 1;
    if (cfg.g_timebase.den == 0) cfg.g_timebase.den = 1;
    cfg.rc_target_bitrate = (Size > 10) ? (Data[6] | (Data[7] << 8)) : 1000;
    cfg.g_error_resilient = (Size > 12) ? Data[8] : 0;

    if (init_encoder_context(&ctx, &cfg) != 0) {
        return 0;
    }

    // Fuzz AV1E_SET_MATRIX_COEFFICIENTS
    if (Size > 16) {
        aom_codec_control(&ctx, AV1E_SET_MATRIX_COEFFICIENTS, 
                          (void*)(uintptr_t)Data[9], (void*)(uintptr_t)Data[10]);
    }

    // Fuzz AV1E_SET_PARTITION_INFO_PATH
    if (Size > 20) {
        char dummy_path[256];
        size_t path_len = (Size > 21) ? Data[11] : 10;
        if (path_len > 255) path_len = 255;
        memcpy(dummy_path, Data + 12, path_len);
        dummy_path[path_len] = '\0';
        aom_codec_control(&ctx, AV1E_SET_PARTITION_INFO_PATH, dummy_path);
    }

    // Fuzz AV1E_SET_LOOPFILTER_CONTROL
    if (Size > 24) {
        int lf_level = Data[13];
        int lf_sharpness = Data[14];
        aom_codec_control(&ctx, AV1E_SET_LOOPFILTER_CONTROL, 
                          lf_level, lf_sharpness);
    }

    // Fuzz AV1E_SET_FILM_GRAIN_TEST_VECTOR
    if (Size > 28) {
        int fg_test = Data[15];
        aom_codec_control(&ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, fg_test);
    }

    // Fuzz AV1E_SET_FILM_GRAIN_TABLE
    if (Size > 32) {
        int fg_table_size = Data[16];
        if (fg_table_size > 0 && Size > 32 + fg_table_size) {
            aom_codec_control(&ctx, AV1E_SET_FILM_GRAIN_TABLE, 
                              Data + 17, fg_table_size);
        }
    }

    // Fuzz aom_codec_get_global_headers
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&ctx);
    if (global_headers) {
        // Use the data to trigger potential issues
        if (global_headers->buf && global_headers->sz > 0) {
            volatile uint8_t *ptr = (volatile uint8_t *)global_headers->buf;
            for (size_t i = 0; i < global_headers->sz && i < 1024; i++) {
                (void)ptr[i];
            }
        }
        free_global_headers(global_headers);
    }

    cleanup_encoder(&ctx);
    return 0;
}