// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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
#include <aom/aom_external_partition.h>

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size < 2) return 0;

    int val;
    int ret = 0;

    // AV1E_ENABLE_EXT_TILE_DEBUG (boolean)
    if (size > 0) {
        val = data[0] % 2;
        ret = aom_codec_control(ctx, AV1E_ENABLE_EXT_TILE_DEBUG, val);
        (void)ret;
    }

    // AV1E_SET_ENABLE_PALETTE (boolean)
    if (size > 1) {
        val = data[1] % 2;
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_PALETTE, val);
        (void)ret;
    }

    // AV1E_SET_ENABLE_DIAGONAL_INTRA (boolean)
    if (size > 2) {
        val = data[2] % 2;
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_DIAGONAL_INTRA, val);
        (void)ret;
    }

    // AV1E_SET_TILE_COLUMNS (uint32_t)
    if (size > 3) {
        uint32_t cols = (data[3] * 256 + data[4]) % 65 + 1; // 1 to 64
        ret = aom_codec_control(ctx, AV1E_SET_TILE_COLUMNS, cols);
        (void)ret;
    }

    // AV1E_SET_QM_U (uint8_t)
    if (size > 5) {
        uint8_t qm_u = data[5] % 128;
        ret = aom_codec_control(ctx, AV1E_SET_QM_U, qm_u);
        (void)ret;
    }

    // AV1_SET_TILE_MODE (uint32_t)
    if (size > 6) {
        uint32_t mode = (data[6] * 256 + data[7]) % 4; // 0 to 3
        ret = aom_codec_control(ctx, AV1_SET_TILE_MODE, mode);
        (void)ret;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_154(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_image_t img;
    aom_codec_cx_pkt_t pkt;
    aom_codec_err_t res;

    // Initialize default encoder configuration
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    // Set some basic config from fuzz data
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_bit_depth = 8;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = (Data[2] * 256 + Data[3]) + 100;
    cfg.rc_min_quantizer = Data[4] % 64;
    cfg.rc_max_quantizer = Data[5] % 64 + 64;
    cfg.g_error_resilient = Data[6] % 2;
    cfg.g_lag_in_frames = 0;
    cfg.g_threads = 1;

    // Initialize encoder
    res = aom_codec_enc_init(&codec, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Perform control fuzzing
    fuzz_encoder_controls(&codec, Data, Size);

    // Create a dummy image frame using aom_img_alloc
    // Fixed: aom_img_alloc returns a pointer, so we must assign to img pointer or use pointer arithmetic
    // However, aom_img_alloc returns a pointer to a struct, so we need to assign to a pointer variable or dereference
    // The correct way is to assign the return value to a pointer, but here we have a struct variable.
    // So we need to change img to a pointer or use a different approach.
    // Let's change img to a pointer.
    aom_image_t *img_ptr = aom_img_alloc(NULL, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    if (!img_ptr || !img_ptr->planes[0] || !img_ptr->planes[1] || !img_ptr->planes[2]) {
        aom_codec_destroy(&codec);
        return 0;
    }

    // Fill image with fuzz data
    memset(img_ptr->planes[0], Data[7], cfg.g_w * cfg.g_h);
    memset(img_ptr->planes[1], Data[8] % 256, cfg.g_w * cfg.g_h / 4);
    memset(img_ptr->planes[2], Data[9] % 256, cfg.g_w * cfg.g_h / 4);

    // Encode frame (5 arguments: ctx, img, pts, duration, flags)
    res = aom_codec_encode(&codec, img_ptr, 0, 1, 0);
    if (res != AOM_CODEC_OK) {
        aom_img_free(img_ptr);
        aom_codec_destroy(&codec);
        return 0;
    }

    // Drain encoder
    while (aom_codec_get_cx_data(&codec, &pkt) == AOM_CODEC_OK) {
        if (pkt.kind == AOM_CODEC_CX_FRAME_PKT) {
            if (pkt.data.frame.buf) {
                free(pkt.data.frame.buf);
            }
        }
    }

    // Cleanup
    aom_img_free(img_ptr);
    aom_codec_destroy(&codec);

    return 0;
}