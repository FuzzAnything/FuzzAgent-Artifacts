// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aomdx.h"
#include "aom_encoder.h"
#include "aom_frame_buffer.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom.h"
#include "aom_integer.h"
#include "aom_codec.h"
#include "aomcx.h"
#include "aom_external_partition.h"

// Helper to initialize a decoder context
static int init_decoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_dec_cfg_t cfg;
    aom_codec_iface_t *iface;

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;

    iface = aom_codec_av1_dx();
    if (!iface) return 0;

    if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Feed some data to initialize the decoder state if possible
    if (size > 0) {
        aom_codec_err_t res = aom_codec_decode(ctx, data, size, NULL);
        (void)res;
    }

    return 1;
}

// Helper to initialize an encoder context
static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;

    memset(&cfg, 0, sizeof(cfg));
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_lag_in_frames = 0;
    cfg.g_error_resilient = 1;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Try to encode a dummy frame to ensure state is active
    if (size > 0) {
        aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
        aom_image_t img;
        aom_img_alloc(&img, fmt, 64, 64, 1);
        // Fill with data if available
        if (size >= 64 * 64 * 3 / 2) {
             memcpy(img.planes[0], data, 64*64);
             memcpy(img.planes[1], data + 64*64, 64*64/4);
             memcpy(img.planes[2], data + 64*64 + 64*64/4, 64*64/4);
        } else {
             memset(img.planes[0], 0, 64*64);
             memset(img.planes[1], 0, 64*64/4);
             memset(img.planes[2], 0, 64*64/4);
        }

        // Fixed: aom_codec_encode takes 5 arguments
        aom_codec_encode(ctx, &img, 0, 1, 0);
        aom_img_free(&img);
    }

    return 1;
}

int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // We need to test both decoder and encoder controls
    // The functions listed are AV1 specific controls.
    // We will try to invoke them on both decoder and encoder contexts.

    // 1. Test Decoder Controls
    aom_codec_ctx_t dec_ctx;
    memset(&dec_ctx, 0, sizeof(dec_ctx));
    
    if (init_decoder(&dec_ctx, Data, Size)) {
        int val;
        int ret;
        
        // Control 1: AV1_INVERT_TILE_DECODE_ORDER
        // This is a set/get control. Try setting 0 and 1.
        // Safe access check
        if (Size >= 1) {
            val = Data[0] % 2;
            // Fixed: Use the enum value directly as the control ID
            ret = aom_codec_control(&dec_ctx, AV1_INVERT_TILE_DECODE_ORDER, val);
            (void)ret; 
        }

        // Control 2: AV1_SET_DECODE_TILE_COL
        // Set column index
        if (Size >= 2) {
            val = Data[1] % 64; 
            ret = aom_codec_control(&dec_ctx, AV1_SET_DECODE_TILE_COL, val);
            (void)ret;
        }

        // Control 3: AV1_SET_DECODE_TILE_ROW
        // Set row index
        if (Size >= 3) {
            val = Data[2] % 64;
            ret = aom_codec_control(&dec_ctx, AV1_SET_DECODE_TILE_ROW, val);
            (void)ret;
        }

        // Control 4: AV1_SET_TILE_MODE
        // Set tile mode (0 or 1 usually)
        if (Size >= 4) {
            val = Data[3] % 2;
            ret = aom_codec_control(&dec_ctx, AV1_SET_TILE_MODE, val);
            (void)ret;
        }

        // Control 6: AV1D_GET_TILE_COUNT
        // This is a get control, we pass a pointer to store the result
        int tile_count = 0;
        ret = aom_codec_control(&dec_ctx, AV1D_GET_TILE_COUNT, &tile_count);
        (void)ret;

        aom_codec_destroy(&dec_ctx);
    }

    // 2. Test Encoder Controls
    aom_codec_ctx_t enc_ctx;
    memset(&enc_ctx, 0, sizeof(enc_ctx));

    if (init_encoder(&enc_ctx, Data, Size)) {
        int val;
        int ret;

        // Control 5: AV1E_SET_SINGLE_TILE_DECODING
        // Set single tile decoding mode (0 or 1)
        if (Size >= 5) {
            val = Data[4] % 2;
            ret = aom_codec_control(&enc_ctx, AV1E_SET_SINGLE_TILE_DECODING, val);
            (void)ret;
        }

        // Try AV1_SET_TILE_MODE on encoder just in case it's supported
        if (Size >= 6) {
            val = Data[5] % 2;
            ret = aom_codec_control(&enc_ctx, AV1_SET_TILE_MODE, val);
            (void)ret;
        }

        aom_codec_destroy(&enc_ctx);
    }

    return 0;
}