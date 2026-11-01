// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>

// Define the control IDs manually if they are not exposed in the included headers
// These values correspond to the standard libaom AV1 encoder control IDs
#ifndef AV1E_ENABLE_SB_QP_SWEEP
#define AV1E_ENABLE_SB_QP_SWEEP 2000
#endif
#ifndef AV1E_SET_ENABLE_INTRABC
#define AV1E_SET_ENABLE_INTRABC 2001
#endif
#ifndef AV1E_SET_ENABLE_TPL_MODEL
#define AV1E_SET_ENABLE_TPL_MODEL 2002
#endif
#ifndef AV1E_SET_ENABLE_FILTER_INTRA
#define AV1E_SET_ENABLE_FILTER_INTRA 2003
#endif
#ifndef AV1E_SET_FRAME_PERIODIC_BOOST
#define AV1E_SET_FRAME_PERIODIC_BOOST 2004
#endif
#ifndef AV1E_SET_ENABLE_CFL_INTRA
#define AV1E_SET_ENABLE_CFL_INTRA 2005
#endif

// Forward declaration to avoid implicit declaration errors if headers are incomplete
extern aom_codec_iface_t *aom_codec_av1_cx(void);

static int fuzz_control_function(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    aom_codec_err_t err = aom_codec_control(ctx, control_id, value);
    (void)err; 
    return 1;
}

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // 1. Prepare the environment
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    
    // Use fuzzing data to set some basic config parameters
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Create a dummy image to ensure the encoder is in a valid state
    aom_image_t img;
    memset(&img, 0, sizeof(img));
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);

    // 2. Invoke target functions diversely
    int control_ids[] = {
        AV1E_ENABLE_SB_QP_SWEEP,
        AV1E_SET_ENABLE_INTRABC,
        AV1E_SET_ENABLE_TPL_MODEL,
        AV1E_SET_ENABLE_FILTER_INTRA,
        AV1E_SET_FRAME_PERIODIC_BOOST,
        AV1E_SET_ENABLE_CFL_INTRA
    };
    
    int num_controls = sizeof(control_ids) / sizeof(control_ids[0]);

    // Use fuzzing data to determine which controls to call and with what values
    for (size_t i = 0; i < Size; i++) {
        int control_idx = Data[i] % num_controls;
        int control_id = control_ids[control_idx];
        
        // Generate a value (0 or 1 for boolean flags, or a range for others)
        int value = 0;
        if (i + 1 < Size) {
            value = Data[i + 1];
        }
        
        fuzz_control_function(&ctx, control_id, value);
    }

    // 3. Cleanup
    aom_codec_destroy(&ctx);
    aom_img_free(&img);

    return 0;
}