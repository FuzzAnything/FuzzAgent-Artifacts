// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>

/*
 * Note: The function names provided in the prompt (e.g., aom_codec_control_typechecked_AV1E_SET_RENDER_SIZE)
 * are not standard public API functions in libaom. The standard API uses aom_codec_control() with specific
 * control codes (e.g., AV1E_SET_RENDER_SIZE).
 *
 * This fuzz driver implements the standard aom_codec_control() interface to exercise the underlying
 * logic corresponding to the requested features (Render Size, Diagonal Intra, DNL Denoising,
 * Seq Level, Angle Delta, Paeth Intra).
 */

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;
    uint32_t width, height;

    // Fixed: Use aom_codec_av1_cx() instead of non-existent aom_codec_av1_enc_cx()
    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return NULL;

    // Derive dimensions from input data to vary test states
    if (size >= 4) {
        width = (data[0] << 8) | data[1];
        height = (data[2] << 8) | data[3];
        if (width == 0) width = 64;
        if (height == 0) height = 64;
        if (width > 4096) width = 4096;
        if (height > 4096) height = 4096;
    } else {
        width = 64;
        height = 64;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    ctx = (aom_codec_ctx_t *)calloc(1, sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_159(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_err_t res;
    int val;

    if (Size < 1) return 0;

    ctx = init_encoder(Data, Size);
    if (!ctx) return 0;

    // Use the first byte to select which control to fuzz
    int control_type = Data[0] % 6;

    switch (control_type) {
        case 0:
            // AV1E_SET_RENDER_SIZE (Requires width, height)
            if (Size >= 5) {
                int w = (Data[1] << 8) | Data[2];
                int h = (Data[3] << 8) | Data[4];
                if (w > 0 && h > 0) {
                    res = aom_codec_control(ctx, AV1E_SET_RENDER_SIZE, w, h);
                    (void)res;
                }
            }
            break;

        case 1:
            // AV1E_SET_ENABLE_DIAGONAL_INTRA (Requires 0 or 1)
            val = (Data[1] % 2);
            res = aom_codec_control(ctx, AV1E_SET_ENABLE_DIAGONAL_INTRA, val);
            (void)res;
            break;

        case 2:
            // AV1E_SET_ENABLE_DNL_DENOISING (Requires 0 or 1)
            val = (Data[1] % 2);
            res = aom_codec_control(ctx, AV1E_SET_ENABLE_DNL_DENOISING, val);
            (void)res;
            break;

        case 3:
            // AV1E_GET_SEQ_LEVEL_IDX (Returns value)
            res = aom_codec_control(ctx, AV1E_GET_SEQ_LEVEL_IDX, &val);
            (void)res;
            (void)val;
            break;

        case 4:
            // AV1E_SET_ENABLE_ANGLE_DELTA (Requires 0 or 1)
            val = (Data[1] % 2);
            res = aom_codec_control(ctx, AV1E_SET_ENABLE_ANGLE_DELTA, val);
            (void)res;
            break;

        case 5:
            // AV1E_SET_ENABLE_PAETH_INTRA (Requires 0 or 1)
            val = (Data[1] % 2);
            res = aom_codec_control(ctx, AV1E_SET_ENABLE_PAETH_INTRA, val);
            (void)res;
            break;
    }

    // Additional diverse state exploration: Try setting invalid values or toggling
    if (Size > 2) {
        int toggle = (Data[2] % 2);
        // Try setting a generic flag again to ensure state changes are handled
        res = aom_codec_control(ctx, AV1E_SET_ENABLE_DIAGONAL_INTRA, toggle);
        (void)res;
    }

    cleanup_encoder(ctx);
    return 0;
}