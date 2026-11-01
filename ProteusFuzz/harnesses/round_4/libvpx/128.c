// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP9E_SET_TILE_COLUMNS__value at vp8cx.h:1024:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// VPXD_GET_LAST_QUANTIZER__value at vp8dx.h:214:1 in vp8dx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// VP8E_GET_LAST_QUANTIZER__value at vp8cx.h:996:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// VP9E_SET_TILE_ROWS__value at vp8cx.h:1026:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_image.h"

/*
 * Fuzz driver for libvpx API functions:
 * - VP9E_SET_TILE_ROWS
 * - VP9E_SET_TILE_COLUMNS
 * - VP8E_GET_LAST_QUANTIZER
 * - VPXD_GET_LAST_QUANTIZER
 * - vpx_codec_control_ (via vpx_codec_control macro)
 */

static int setup_encoder(vpx_codec_ctx_t *ctx, vpx_codec_iface_t *iface, const uint8_t *data, size_t size) {
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res;

    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Use fuzz data to configure width/height if possible, with bounds checking
    if (size >= 8) {
        uint32_t w = *(const uint32_t *)(data);
        uint32_t h = *(const uint32_t *)(data + 4);
        // Ensure reasonable bounds to avoid allocation failures or crashes
        if (w > 0 && w <= 4096 && h > 0 && h <= 4096) {
            cfg.g_w = w;
            cfg.g_h = h;
        }
    }
    // Default to 160x128 if data is insufficient or bounds check fails
    cfg.g_w = cfg.g_w ? cfg.g_w : 160;
    cfg.g_h = cfg.g_h ? cfg.g_h : 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_lag_in_frames = 0;
    cfg.rc_end_usage = VPX_CBR;
    cfg.rc_target_bitrate = 500;

    res = vpx_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    return 1;
}

static int setup_decoder(vpx_codec_ctx_t *ctx, vpx_codec_iface_t *iface) {
    vpx_codec_dec_cfg_t cfg;
    vpx_codec_err_t res;

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    // Removed allow_highbitdepth as it may not exist in all versions

    res = vpx_codec_dec_init(ctx, iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_128(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    vpx_codec_ctx_t enc_ctx = {0};
    vpx_codec_ctx_t dec_ctx = {0};
    vpx_codec_iface_t *enc_iface = NULL;
    vpx_codec_iface_t *dec_iface = NULL;
    vpx_codec_err_t res;
    int32_t val;

    // Determine which codec to test based on input data
    // First byte: 0 -> VP9 Encoder, 1 -> VP8 Decoder, 2 -> VP8 Encoder
    int mode = Data[0] % 3;

    // --- VP9 Encoder Tests ---
    if (mode == 0) {
        enc_iface = vpx_codec_vp9_cx();
        if (enc_iface && setup_encoder(&enc_ctx, enc_iface, Data, Size)) {
            // Test VP9E_SET_TILE_ROWS
            // Extract value from data, clamp to valid range [1, 16]
            int tile_rows = 1;
            if (Size > 1) {
                tile_rows = Data[1] % 16 + 1;
            }
            res = vpx_codec_control(&enc_ctx, VP9E_SET_TILE_ROWS, tile_rows);
            (void)res; // Ignore error for fuzzing, just exercise path

            // Test VP9E_SET_TILE_COLUMNS
            // Extract value from data, clamp to valid range [1, 16]
            int tile_cols = 1;
            if (Size > 2) {
                tile_cols = Data[2] % 16 + 1;
            }
            res = vpx_codec_control(&enc_ctx, VP9E_SET_TILE_COLUMNS, tile_cols);
            (void)res;

            // Cleanup encoder
            vpx_codec_destroy(&enc_ctx);
        }
    }

    // --- VP8 Decoder Tests ---
    if (mode == 1) {
        dec_iface = vpx_codec_vp8_dx();
        if (dec_iface && setup_decoder(&dec_ctx, dec_iface)) {
            // We need to decode at least one frame to get a quantizer
            // Create a dummy packet with fuzz data (might fail decode, which is fine)
            vpx_codec_iter_t iter = NULL;
            
            // Try to decode a small chunk if available
            // Fixed: Added deadline argument (VPX_DL_REALTIME)
            if (Size > 4) {
                vpx_codec_err_t dec_res = vpx_codec_decode(&dec_ctx, 
                    (const uint8_t *)(Data + 4), Size - 4, NULL, VPX_DL_REALTIME);
                (void)dec_res; // Ignore decode result, we just want to trigger state

                // Try to get last quantizer
                res = vpx_codec_control(&dec_ctx, VPXD_GET_LAST_QUANTIZER, &val);
                (void)res;
            }

            // Cleanup decoder
            vpx_codec_destroy(&dec_ctx);
        }
    }

    // --- VP8 Encoder Tests (for VP8E_GET_LAST_QUANTIZER) ---
    if (mode == 2) {
        vpx_codec_ctx_t vp8_enc_ctx = {0};
        vpx_codec_iface_t *vp8_enc_iface = vpx_codec_vp8_cx();
        
        if (vp8_enc_iface && setup_encoder(&vp8_enc_ctx, vp8_enc_iface, Data, Size)) {
            // Encode a frame to get quantizer
            // Fixed: Added deadline argument (VPX_DL_REALTIME)
            vpx_codec_err_t enc_res = vpx_codec_encode(&vp8_enc_ctx, NULL, 0, 1, 0, VPX_DL_REALTIME);
            (void)enc_res;

            // Test VP8E_GET_LAST_QUANTIZER
            res = vpx_codec_control(&vp8_enc_ctx, VP8E_GET_LAST_QUANTIZER, &val);
            (void)res;

            vpx_codec_destroy(&vp8_enc_ctx);
        }
    }

    return 0;
}