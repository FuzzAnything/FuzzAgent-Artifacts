// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Uses standard aom_codec_control API with AV1E_* constants.
 */

static int try_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return 0;
    aom_codec_err_t res = aom_codec_control(ctx, control_id, value);
    return (res == AOM_CODEC_OK);
}

int LLVMFuzzerTestOneInput_221(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;

    iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return 0;
    }

    aom_codec_enc_config_default(iface, &cfg, 0);
    
    cfg.g_w = (Data[0] % 128) + 16;
    cfg.g_h = (Data[1] % 128) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Size % 1000) + 1;
    cfg.rc_target_bitrate = (Size % 10000) + 100;
    cfg.g_pass = 0;

    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    int tier = (Size > 2) ? (Data[2] % 2) : 0;
    int force_video_mode = (Size > 3) ? (Data[3] % 2) : 0;
    int enable_cdef = (Size > 4) ? (Data[4] % 2) : 0;
    int enable_intrabc = (Size > 5) ? (Data[5] % 2) : 0;
    int superblock_size = (Size > 6) ? (Data[6] % 3) : 0;
    int enable_keyframe_filtering = (Size > 7) ? (Data[7] % 2) : 0;

    int sb_size_val = 0;
    if (superblock_size == 0) sb_size_val = 0; 
    else if (superblock_size == 1) sb_size_val = 1; 
    else sb_size_val = 0;

    // AV1E_SET_TIER_MASK is the specific constant for setting the tier mask
    // The previous error was due to using AV1E_SET_TIER which might not be the exact constant name
    // or available in this specific build context. Using the requested constant name directly.
    try_control(&ctx, AV1E_SET_TIER_MASK, tier);

    try_control(&ctx, AV1E_SET_FORCE_VIDEO_MODE, force_video_mode);
    try_control(&ctx, AV1E_SET_ENABLE_CDEF, enable_cdef);
    try_control(&ctx, AV1E_SET_ENABLE_INTRABC, enable_intrabc);
    try_control(&ctx, AV1E_SET_SUPERBLOCK_SIZE, sb_size_val);
    try_control(&ctx, AV1E_SET_ENABLE_KEYFRAME_FILTERING, enable_keyframe_filtering);

    aom_codec_destroy(&ctx);

    return 0;
}