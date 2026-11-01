// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
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
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Fuzz some configuration parameters based on input data
    if (size > 0) {
        cfg.g_w = (data[0] % 1024) + 16;
        if (size > 1) cfg.g_h = (data[1] % 1024) + 16;
        if (size > 2) cfg.g_timebase.num = (data[2] % 100) + 1;
        if (size > 3) cfg.g_timebase.den = (data[3] % 100) + 1;
        if (size > 4) cfg.rc_target_bitrate = (data[4] % 1000) + 100;
        if (size > 5) cfg.g_error_resilient = (data[5] % 2);
    }

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

int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t *ctx = init_encoder(Data, Size);
    if (!ctx) return 0;

    // Prepare dummy frame for encoding
    aom_image_t frame;
    aom_img_alloc(&frame, AOM_IMG_FMT_I420, 16, 16, 1);
    // Fill frame with fuzz data if available
    if (Size > 0) {
        size_t copy_size = (Size < 16*16) ? Size : 16*16;
        memcpy(frame.planes[0], Data, copy_size);
    }

    // Set control parameters using the target APIs
    int val;
    
    if (Size > 0) {
        val = Data[0] % 10;
        aom_codec_control(ctx, AV1E_SET_MV_COST_UPD_FREQ, val);
    }

    if (Size > 1) {
        val = Data[1] % 3;
        aom_codec_control(ctx, AV1E_SET_CDF_UPDATE_MODE, val);
    }

    if (Size > 2) {
        val = Data[2] % 2;
        aom_codec_control(ctx, AV1E_SET_ENABLE_PALETTE, val);
    }

    if (Size > 3) {
        val = Data[3] % 10;
        aom_codec_control(ctx, AV1E_SET_MODE_COST_UPD_FREQ, val);
    }

    if (Size > 4) {
        val = Data[4] % 64;
        aom_codec_control(ctx, AV1E_SET_QM_MIN, val);
    }

    if (Size > 5) {
        val = Data[5] % 64;
        aom_codec_control(ctx, AV1E_SET_QM_U, val);
    }

    // Attempt to encode the frame to exercise the state changes
    aom_codec_cx_pkt_t pkt;
    aom_codec_err_t res = aom_codec_encode(ctx, &frame, 0, 1, 0);
    if (res == AOM_CODEC_OK) {
        while (aom_codec_get_cx_data(ctx, &pkt) == AOM_CODEC_OK) {
            if (pkt.kind == AOM_CODEC_CX_FRAME_PKT) {
                if (pkt.data.frame.sz) {
                    // Process encoded data (fuzzing doesn't need to do anything specific)
                }
            }
        }
    }

    aom_img_free(&frame);
    cleanup_encoder(ctx);

    return 0;
}