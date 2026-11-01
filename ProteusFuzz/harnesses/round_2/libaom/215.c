// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder_context(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;

    iface = aom_codec_av1_cx();
    if (!iface) return 1;

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 1;

    // Use fuzzing data to tweak configuration slightly to explore states
    if (size > 0) {
        cfg.g_w = (data[0] % 256) + 16;
        cfg.g_h = (data[1] % 256) + 16;
        cfg.g_timebase.num = (data[2] % 100) + 1;
        cfg.g_timebase.den = (data[3] % 100) + 1;
        cfg.rc_target_bitrate = (data[4] % 10000) + 100;
    }

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_215(const uint8_t *Data, size_t Size) {
    if (Size < 5) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    if (init_encoder_context(&ctx, Data, Size) != 0) {
        return 0;
    }

    // Fuzz aom_codec_set_option
    // Create a simple key-value pair from input data
    char key[64] = {0};
    char value[64] = {0};
    size_t key_len = (Size > 10) ? (Data[5] % 30) + 1 : 5;
    size_t val_len = (Size > 11) ? (Data[6] % 30) + 1 : 5;

    if (key_len < sizeof(key) && val_len < sizeof(value)) {
        memcpy(key, Data + 7, key_len);
        memcpy(value, Data + 7 + key_len, val_len);
        aom_codec_set_option(&ctx, key, value);
    }

    // Fuzz aom_codec_control_typechecked_AV1E_SET_ERROR_RESILIENT_MODE
    // Assuming it takes an integer value (0 or 1)
    int error_resilient_mode = (Size > 10) ? (Data[10] % 2) : 0;
    aom_codec_control(&ctx, AV1E_SET_ERROR_RESILIENT_MODE, error_resilient_mode);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_ADAPTIVE_SHARPNESS
    int adaptive_sharpness = (Size > 11) ? (Data[11] % 2) : 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_ADAPTIVE_SHARPNESS, adaptive_sharpness);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_FLIP_IDTX
    int flip_idtx = (Size > 12) ? (Data[12] % 2) : 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, flip_idtx);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_SUPERRES
    int superres = (Size > 13) ? (Data[13] % 2) : 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_SUPERRES, superres);

    // Fuzz aom_codec_get_global_headers
    aom_fixed_buf_t *headers = aom_codec_get_global_headers(&ctx);
    if (headers) {
        // Free the buffer content
        if (headers->buf) {
            free(headers->buf);
        }
        // Free the struct itself
        free(headers);
    }

    cleanup_codec(&ctx);
    return 0;
}