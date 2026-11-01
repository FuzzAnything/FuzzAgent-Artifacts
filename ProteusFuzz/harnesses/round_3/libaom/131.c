// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aomcx.h"

static int fuzzer_run_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    
    aom_codec_err_t res = aom_codec_control(ctx, control_id, value);
    // Return 0 on error or success, as we just want to exercise the code path
    return 0;
}

int LLVMFuzzerTestOneInput_131(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Extract parameters from fuzz input
    int width = Data[0] + 1;
    int height = Data[1] + 1;
    int bitrate = (Size > 2) ? Data[2] : 1000;
    int fps_num = (Size > 3) ? Data[3] : 30;
    int fps_den = (Size > 4) ? Data[4] : 1;
    
    // Ensure reasonable bounds to avoid allocation failures or hangs
    if (width > 4096 || height > 4096 || width < 16 || height < 16) return 0;
    if (bitrate < 1) bitrate = 1;
    if (fps_den < 1) fps_den = 1;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    aom_image_t img;
    aom_codec_cx_pkt_t pkt;

    // 1. Prepare the environment
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = fps_num;
    cfg.g_timebase.den = fps_den;
    cfg.rc_target_bitrate = bitrate;
    cfg.g_lag_in_frames = 0; // Reduce latency for faster fuzzing
    cfg.g_threads = 1;

    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    // Initialize a dummy image frame
    aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, 1);
    memset(img.planes[0], 0, width * height);
    memset(img.planes[1], 128, (width * height) / 4);
    memset(img.planes[2], 128, (width * height) / 4);

    // 2. Invoke target functions diversely
    // AV1E_SET_ENABLE_OVERLAY
    int val_overlay = (Size > 5) ? Data[5] : 0;
    fuzzer_run_encoder_control(&ctx, AV1E_SET_ENABLE_OVERLAY, val_overlay);

    // AV1E_SET_MV_COST_UPD_FREQ
    int val_mv_cost = (Size > 6) ? Data[6] : 0;
    fuzzer_run_encoder_control(&ctx, AV1E_SET_MV_COST_UPD_FREQ, val_mv_cost);

    // AV1E_SET_ENABLE_GLOBAL_MOTION
    int val_gm = (Size > 7) ? Data[7] : 0;
    fuzzer_run_encoder_control(&ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, val_gm);

    // AV1E_SET_ENABLE_PALETTE
    int val_palette = (Size > 8) ? Data[8] : 0;
    fuzzer_run_encoder_control(&ctx, AV1E_SET_ENABLE_PALETTE, val_palette);

    // AV1E_SET_MODE_COST_UPD_FREQ
    int val_mode_cost = (Size > 9) ? Data[9] : 0;
    fuzzer_run_encoder_control(&ctx, AV1E_SET_MODE_COST_UPD_FREQ, val_mode_cost);

    // AV1E_SET_QM_MIN
    int val_qm_min = (Size > 10) ? Data[10] : 0;
    fuzzer_run_encoder_control(&ctx, AV1E_SET_QM_MIN, val_qm_min);

    // 3. Explore more program states by encoding a frame
    res = aom_codec_encode(&ctx, &img, 0, 1, 0);
    if (res != AOM_CODEC_OK) {
        // Encoding might fail, but we continue to drain
    }

    // Drain the encoder
    while (aom_codec_get_cx_data(&ctx, &pkt) == AOM_CODEC_OK) {
        if (pkt.kind == AOM_CODEC_CX_FRAME_PKT) {
            // Free the packet data if it contains a frame
            if (pkt.data.frame.buf) {
                aom_img_free(pkt.data.frame.buf);
            }
        }
    }

    // 4. Cleanup
    aom_img_free(&img);
    aom_codec_destroy(&ctx);

    return 0;
}