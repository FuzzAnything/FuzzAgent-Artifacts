// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_QM at aomcx.h:2032:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_MAX_PARTITION_SIZE at aomcx.h:2072:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_FORCE_VIDEO_MODE at aomcx.h:2023:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_MTU at aomcx.h:2057:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_KEYFRAME_FILTERING at aomcx.h:1951:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_CDEF at aomcx.h:2017:1 in aomcx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->priv) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    int ret;

    // Initialize encoder config
    if (aom_codec_enc_config_default(NULL, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure width/height to avoid division by zero or invalid sizes
    uint32_t width = (Data[0] % 256) + 16;
    uint32_t height = (Data[1] % 256) + 16;
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = Data[2] % 100 + 1;
    cfg.g_timebase.den = Data[3] % 100 + 1;
    cfg.g_threads = 1;

    // Initialize encoder context
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz AV1E_SET_ENABLE_QM (expects int ctrl, unsigned int data)
    if (Size >= 4) {
        unsigned int val_qm = *(uint32_t *)(Data + 4);
        ret = aom_codec_control_typechecked_AV1E_SET_ENABLE_QM(&ctx, AV1E_SET_ENABLE_QM, val_qm);
        (void)ret;
    }

    // Fuzz AV1E_SET_MAX_PARTITION_SIZE (expects int ctrl, int data)
    if (Size >= 8) {
        int val_max_part = *(int32_t *)(Data + 8);
        ret = aom_codec_control_typechecked_AV1E_SET_MAX_PARTITION_SIZE(&ctx, AV1E_SET_MAX_PARTITION_SIZE, val_max_part);
        (void)ret;
    }

    // Fuzz AV1E_SET_FORCE_VIDEO_MODE (expects int ctrl, unsigned int data)
    if (Size >= 12) {
        unsigned int val_video_mode = *(uint32_t *)(Data + 12);
        ret = aom_codec_control_typechecked_AV1E_SET_FORCE_VIDEO_MODE(&ctx, AV1E_SET_FORCE_VIDEO_MODE, val_video_mode);
        (void)ret;
    }

    // Fuzz AV1E_SET_MTU (expects int ctrl, unsigned int data)
    if (Size >= 16) {
        unsigned int val_mtu = *(uint32_t *)(Data + 16);
        ret = aom_codec_control_typechecked_AV1E_SET_MTU(&ctx, AV1E_SET_MTU, val_mtu);
        (void)ret;
    }

    // Fuzz AV1E_SET_ENABLE_KEYFRAME_FILTERING (expects int ctrl, unsigned int data)
    if (Size >= 20) {
        unsigned int val_keyframe = *(uint32_t *)(Data + 20);
        ret = aom_codec_control_typechecked_AV1E_SET_ENABLE_KEYFRAME_FILTERING(&ctx, AV1E_SET_ENABLE_KEYFRAME_FILTERING, val_keyframe);
        (void)ret;
    }

    // Fuzz AV1E_SET_ENABLE_CDEF (expects int ctrl, unsigned int data)
    if (Size >= 24) {
        unsigned int val_cdef = *(uint32_t *)(Data + 24);
        ret = aom_codec_control_typechecked_AV1E_SET_ENABLE_CDEF(&ctx, AV1E_SET_ENABLE_CDEF, val_cdef);
        (void)ret;
    }

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}