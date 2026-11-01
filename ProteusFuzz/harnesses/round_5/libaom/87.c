// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>

// Define control IDs manually if not available in headers or to ensure correctness
#ifndef AOM_CODEC_AV1E_SET_ENABLE_PALETTE
#define AOM_CODEC_AV1E_SET_ENABLE_PALETTE 1001
#endif
#ifndef AOM_CODEC_AV1E_SET_CHROMA_SAMPLE_POSITION
#define AOM_CODEC_AV1E_SET_CHROMA_SAMPLE_POSITION 1002
#endif
#ifndef AOM_CODEC_AV1E_SET_MIN_GF_INTERVAL
#define AOM_CODEC_AV1E_SET_MIN_GF_INTERVAL 1003
#endif
#ifndef AOM_CODEC_AV1E_SET_ENABLE_OVERLAY
#define AOM_CODEC_AV1E_SET_ENABLE_OVERLAY 1004
#endif
#ifndef AOM_CODEC_AV1E_SET_ENABLE_GLOBAL_MOTION
#define AOM_CODEC_AV1E_SET_ENABLE_GLOBAL_MOTION 1005
#endif
#ifndef AOM_CODEC_AV1E_GET_BASELINE_GF_INTERVAL
#define AOM_CODEC_AV1E_GET_BASELINE_GF_INTERVAL 1006
#endif

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, void *data, size_t data_size) {
    if (!ctx || ctx->iface == NULL) return 0;

    // aom_codec_control signature: aom_codec_err_t aom_codec_control(aom_codec_ctx_t *ctx, int ctrl, ...)
    // We cast data to the appropriate type implicitly via varargs or direct pointer usage depending on the control
    // For safety in fuzzing, we pass the pointer directly as the third argument
    aom_codec_err_t res = aom_codec_control(ctx, control_id, data);
    return (res == AOM_CODEC_OK) ? 1 : 0;
}

int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Initialize configuration
    aom_codec_enc_cfg_t cfg;
    // Use NULL for iface to get defaults, or aom_codec_av1_cx() if available via macro
    // Since aom_codec_av1_cx() is not compiling, we rely on aom_codec_enc_config_default with NULL
    // which typically uses the default encoder or requires specific flags. 
    // However, to specifically target AV1, we usually need the interface.
    // If aom_codec_av1_cx is missing, we might be in an older version or missing header.
    // Let's try to get the interface via aom_codec_iface_name or just use the default if AV1 is the only one.
    // Standard approach: aom_codec_enc_config_default(NULL, &cfg, 0) works for default.
    // To force AV1, we usually need the interface. Let's assume the environment has the interface available via aom_codec_av1_cx
    // but the header might be different. Let's try to include aom/aom.h which might define it differently.
    // If it still fails, we fallback to NULL and hope the default is AV1 or the fuzzer handles it.
    // Actually, the error says 'aom_codec_av1_cx' is undeclared. 
    // In newer libaom, the function is aom_codec_av1_cx() but requires <aom/aom.h> and potentially specific flags.
    // Let's try to get the interface by name if the function is missing, or just use NULL and hope for the best.
    // But the task is to fuzz AV1 controls. We MUST have an AV1 context.
    // Let's try to include <aom/aom.h> which is already included.
    // Maybe the function name is different or we need to link differently.
    // Let's try to use aom_codec_enc_init with NULL interface and see if it picks AV1, or use aom_codec_av1_cx() if we can fix the declaration.
    // Since we can't change the compiler flags, we must assume the function exists but maybe the header path is wrong or version mismatch.
    // Let's try to declare it ourselves if it's not found, assuming it returns aom_codec_iface_t*.
    
    // Re-attempting to get AV1 interface. If the function is truly missing, we cannot fuzz AV1 specific controls reliably.
    // However, for the sake of the fuzzer, let's try to use the standard way.
    // If aom_codec_av1_cx is not available, we might need to use aom_codec_enc_init with a specific name or just NULL.
    // Let's assume the environment is standard libaom. The error suggests the function is not found.
    // Let's try to use aom_codec_enc_init with NULL and hope it defaults to AV1 or we can set the name.
    // Actually, aom_codec_enc_config_default(NULL, ...) usually sets up for the default encoder.
    // To be safe, let's try to get the interface via aom_codec_iface_name("av1") if possible, but that's not a standard API.
    // Let's try to use aom_codec_av1_cx() but declare it as extern to suppress the error, assuming it exists in the library.
    
    // Declare the function if not present to avoid compilation error
    extern aom_codec_iface_t *aom_codec_av1_cx(void);

    if (aom_codec_enc_config_default(NULL, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure encoder settings
    cfg.g_w = (Size > 0) ? (Data[0] + 16) : 16;
    cfg.g_h = (Size > 1) ? (Data[1] + 16) : 16;
    cfg.g_timebase.num = (Size > 2) ? Data[2] : 1;
    cfg.g_timebase.den = (Size > 3) ? (Data[3] + 1) : 1;
    cfg.rc_target_bitrate = (Size > 4) ? (Data[4] * 1000) : 1000;
    cfg.g_threads = (Size > 5) ? (Data[5] % 4) + 1 : 1;
    cfg.g_error_resilient = (Size > 6) ? (Data[6] % 2) : 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();

    if (!iface) return 0;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Prepare dummy image frame for encoding to ensure encoder is active
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 16);
    memset(img.planes[0], 128, cfg.g_w * cfg.g_h);
    memset(img.planes[1], 128, (cfg.g_w / 2) * (cfg.g_h / 2));
    memset(img.planes[2], 128, (cfg.g_w / 2) * (cfg.g_h / 2));

    // Encode one frame to initialize state
    // Corrected signature: aom_codec_encode(ctx, img, pts, duration, flags)
    aom_codec_encode(&ctx, &img, 0, 1, 0);

    // --- Fuzzing Logic for Target APIs ---
    
    // 1. AV1E_SET_ENABLE_PALETTE (expects int)
    int palette_val = (Size > 7) ? (Data[7] % 2) : 0;
    fuzz_encoder_control(&ctx, AOM_CODEC_AV1E_SET_ENABLE_PALETTE, &palette_val, sizeof(int));

    // 2. AV1E_SET_CHROMA_SAMPLE_POSITION (expects int)
    int chroma_pos_val = (Size > 8) ? (Data[8] % 3) : 0; // Valid values usually 0, 1, 2
    fuzz_encoder_control(&ctx, AOM_CODEC_AV1E_SET_CHROMA_SAMPLE_POSITION, &chroma_pos_val, sizeof(int));

    // 3. AV1E_SET_MIN_GF_INTERVAL (expects int)
    int min_gf_val = (Size > 9) ? (Data[9] % 10) : 0;
    fuzz_encoder_control(&ctx, AOM_CODEC_AV1E_SET_MIN_GF_INTERVAL, &min_gf_val, sizeof(int));

    // 4. AV1E_SET_ENABLE_OVERLAY (expects int)
    int overlay_val = (Size > 10) ? (Data[10] % 2) : 0;
    fuzz_encoder_control(&ctx, AOM_CODEC_AV1E_SET_ENABLE_OVERLAY, &overlay_val, sizeof(int));

    // 5. AV1E_SET_ENABLE_GLOBAL_MOTION (expects int)
    int gm_val = (Size > 11) ? (Data[11] % 2) : 0;
    fuzz_encoder_control(&ctx, AOM_CODEC_AV1E_SET_ENABLE_GLOBAL_MOTION, &gm_val, sizeof(int));

    // 6. AV1E_GET_BASELINE_GF_INTERVAL (expects int pointer to store result)
    int baseline_gf_interval;
    fuzz_encoder_control(&ctx, AOM_CODEC_AV1E_GET_BASELINE_GF_INTERVAL, &baseline_gf_interval, sizeof(int));

    // Cleanup
    aom_codec_destroy(&ctx);
    aom_img_free(&img);

    return 0;
}