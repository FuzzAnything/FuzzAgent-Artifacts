// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static int safe_get_int(const uint8_t *Data, size_t Size, size_t offset, int default_val) {
    if (Size > offset) {
        return Data[offset];
    }
    return default_val;
}

static int safe_get_int_range(const uint8_t *Data, size_t Size, size_t offset, int min_val, int max_val, int default_val) {
    if (Size > offset) {
        int val = Data[offset];
        if (val < min_val) val = min_val;
        if (val > max_val) val = max_val;
        return val;
    }
    return default_val;
}

int LLVMFuzzerTestOneInput_192(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize configuration with fuzz data safely
    cfg.g_w = safe_get_int_range(Data, Size, 0, 16, 4096, 128);
    cfg.g_h = safe_get_int_range(Data, Size, 1, 16, 4096, 128);
    cfg.g_timebase.num = safe_get_int_range(Data, Size, 2, 1, 100, 1);
    cfg.g_timebase.den = safe_get_int_range(Data, Size, 3, 1, 100, 30);
    cfg.g_threads = safe_get_int_range(Data, Size, 4, 1, 8, 1);
    cfg.g_error_resilient = safe_get_int_range(Data, Size, 5, 0, 2, 0);
    
    // Ensure bitrate is positive
    int bitrate_val = safe_get_int(Data, Size, 6, 1000);
    cfg.rc_target_bitrate = bitrate_val * 1000;
    
    cfg.rc_min_quantizer = safe_get_int_range(Data, Size, 7, 0, 63, 0);
    int max_q = safe_get_int(Data, Size, 8, 63);
    cfg.rc_max_quantizer = (max_q < cfg.rc_min_quantizer) ? cfg.rc_min_quantizer + 1 : max_q;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Control 1: AV1E_SET_ENABLE_SUPERRES (bool)
    int enable_superres = safe_get_int(Data, Size, 9, 0) % 2;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_SUPERRES, &enable_superres);

    // Control 2: AV1E_SET_ENABLE_FLIP_IDTX (bool)
    int enable_flip_idtx = safe_get_int(Data, Size, 10, 0) % 2;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, &enable_flip_idtx);

    // Control 3: AV1E_SET_NOISE_SENSITIVITY (int)
    int noise_sensitivity = safe_get_int(Data, Size, 11, 0) % 10;
    aom_codec_control(&ctx, AV1E_SET_NOISE_SENSITIVITY, &noise_sensitivity);

    // Control 4: AV1E_SET_RATE_DISTRIBUTION_INFO (int for simplicity)
    int rate_dist = safe_get_int(Data, Size, 12, 0);
    aom_codec_control(&ctx, AV1E_SET_RATE_DISTRIBUTION_INFO, &rate_dist);

    // Control 5: AV1E_SET_MAX_REFERENCE_FRAMES (int)
    int max_ref_frames = safe_get_int_range(Data, Size, 13, 1, 8, 4);
    aom_codec_control(&ctx, AV1E_SET_MAX_REFERENCE_FRAMES, &max_ref_frames);

    // Control 6: AV1E_SET_TARGET_SEQ_LEVEL_IDX (int)
    int target_seq_level = safe_get_int(Data, Size, 14, 0) % 10;
    aom_codec_control(&ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, &target_seq_level);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}