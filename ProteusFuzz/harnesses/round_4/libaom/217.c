// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static uint8_t get_byte_safe(const uint8_t *Data, size_t Size, size_t index) {
    if (Size > index) {
        return Data[index];
    }
    return 0;
}

int LLVMFuzzerTestOneInput_217(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    // Use fuzzer data to configure basic settings safely
    cfg.g_w = (Size % 2048) + 16;
    cfg.g_h = (Size % 2048) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = (Size % 10000) + 100;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    // Fuzz AV1E_SET_NOISE_SENSITIVITY
    int noise_sensitivity = (get_byte_safe(Data, Size, 0) % 100) - 50;
    aom_codec_control(ctx, AV1E_SET_NOISE_SENSITIVITY, noise_sensitivity);

    // Fuzz AV1E_SET_MAX_REFERENCE_FRAMES
    int max_ref_frames = (get_byte_safe(Data, Size, 1) % 8) + 1;
    aom_codec_control(ctx, AV1E_SET_MAX_REFERENCE_FRAMES, max_ref_frames);

    // Fuzz AV1E_SET_SINGLE_TILE_DECODING
    int single_tile_decoding = (get_byte_safe(Data, Size, 2) % 2);
    aom_codec_control(ctx, AV1E_SET_SINGLE_TILE_DECODING, single_tile_decoding);

    // Fuzz AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR
    int max_consec_drop = (get_byte_safe(Data, Size, 3) % 10) + 1;
    aom_codec_control(ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR, max_consec_drop);

    // Fuzz AV1E_SET_RATE_DISTRIBUTION_INFO
    // This control typically expects a pointer to a struct, we simulate with dummy data
    if (Size > 4) {
        int rate_dist_info[4] = { 
            (int)get_byte_safe(Data, Size, 4), 
            (int)get_byte_safe(Data, Size, 5), 
            (int)get_byte_safe(Data, Size, 6), 
            (int)get_byte_safe(Data, Size, 7) 
        };
        aom_codec_control(ctx, AV1E_SET_RATE_DISTRIBUTION_INFO, rate_dist_info);
    }

    // Fuzz AV1E_SET_ENABLE_SUPERRES
    int enable_superres = (get_byte_safe(Data, Size, 8) % 2);
    aom_codec_control(ctx, AV1E_SET_ENABLE_SUPERRES, enable_superres);

    // Cleanup
    cleanup_codec(ctx);

    return 0;
}