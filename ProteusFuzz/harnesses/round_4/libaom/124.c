// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int id, int value) {
    if (ctx == NULL) return 0;
    // aom_codec_control takes an int for the control ID and a void* for the argument.
    // We cast the int value to void* to satisfy the function signature.
    return aom_codec_control(ctx, id, (void*)(intptr_t)value);
}

int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    // Parse input to determine configuration
    int width = (Data[0] % 16) + 16;
    int height = (Data[1] % 16) + 16;
    int fps_num = (Data[2] % 60) + 1;
    int fps_den = (Data[3] % 60) + 1;
    int threads = (Data[4] % 8) + 1;
    
    // Extract control values
    int val1 = (int)(Data[6]);
    int val2 = (int)(Data[7]);
    int val3 = (int)(Data[8]);
    // AV1E_SET_ENABLE_DIST_8X8 does not exist, skipping or using a valid alternative if needed.
    // For now, we skip this specific control to avoid compilation errors, 
    // or we could map it to a valid one if the user intended a specific feature.
    // Since the task requires fuzzing the listed APIs but one is invalid, 
    // we will comment out the invalid call to ensure compilation.
    int val4 = (int)(Data[9]); 
    int val5 = (int)(Data[10]);
    int val6 = (int)(Data[11]);

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;

    // Initialize encoder configuration
    if (aom_codec_enc_config_default(NULL, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = fps_num;
    cfg.g_timebase.den = fps_den;
    cfg.g_threads = threads;
    cfg.g_lag_in_frames = 0;
    cfg.g_error_resilient = 0;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_undershoot_pct = 0;
    cfg.rc_overshoot_pct = 0;

    // Get AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    // Initialize encoder context
    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz various control functions using the correct enum names
    // AV1E_SET_COEFF_COST_UPD_FREQ
    fuzz_encoder_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, val1);
    
    // AV1E_SET_FRAME_PERIODIC_BOOST
    fuzz_encoder_control(&ctx, AV1E_SET_FRAME_PERIODIC_BOOST, val2);
    
    // AV1E_SET_REDUCED_REFERENCE_SET
    fuzz_encoder_control(&ctx, AV1E_SET_REDUCED_REFERENCE_SET, val3);
    
    // AV1E_SET_ENABLE_DIST_8X8 is invalid in this version of libaom.
    // Skipping to prevent compilation error. 
    // fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_DIST_8X8, val4);
    
    // AOME_SET_ENABLEAUTOBWDREF
    fuzz_encoder_control(&ctx, AOME_SET_ENABLEAUTOBWDREF, val5);
    
    // AV1E_SET_ENABLE_TPL_MODEL
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_TPL_MODEL, val6);

    // Encode a dummy frame to trigger more code paths
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, 1);
    
    // Fill with dummy data
    memset(img.planes[0], Data[12], width * height);
    memset(img.planes[1], Data[13], (width * height) / 4);
    memset(img.planes[2], Data[14], (width * height) / 4);

    // Corrected type: aom_enc_frame_flags_t
    aom_enc_frame_flags_t flags = 0;
    if (Size > 15 && Data[15] & 1) flags |= AOM_EFLAG_FORCE_KF;
    // AOM_EFLAG_NO_DELAY does not exist, using AOM_EFLAG_NO_DELAY is invalid.
    // We'll skip setting it or use a valid flag if available. 
    // For now, we just use the FORCE_KF flag logic.

    // Corrected function call: aom_codec_encode takes 5 arguments
    aom_codec_encode(&ctx, &img, 0, 1, flags);

    // Cleanup
    aom_codec_destroy(&ctx);
    aom_img_free(&img);

    return 0;
}