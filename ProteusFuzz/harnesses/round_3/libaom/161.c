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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_161(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    int enabled = 0;
    int ret;

    if (Size < 1) {
        return 0;
    }

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (!init_encoder(&ctx, &cfg)) {
        return 0;
    }

    // Use input data to derive values for control functions
    int value = Data[0] % 2; // 0 or 1 for boolean-like settings
    int partition_size = 0;
    if (Size >= 2) {
        partition_size = (Data[1] % 4) + 1; // 1 to 4
    }

    // Fuzz AV1E_SET_ENABLE_KEYFRAME_FILTERING
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_KEYFRAME_FILTERING, value);
    (void)ret;

    // Fuzz AV1E_SET_INTRA_DCT_ONLY
    ret = aom_codec_control(&ctx, AV1E_SET_INTRA_DCT_ONLY, value);
    (void)ret;

    // Fuzz AV1E_SET_FORCE_VIDEO_MODE
    ret = aom_codec_control(&ctx, AV1E_SET_FORCE_VIDEO_MODE, value);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_CDEF
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_CDEF, value);
    (void)ret;

    // Fuzz AV1E_SET_MAX_PARTITION_SIZE
    ret = aom_codec_control(&ctx, AV1E_SET_MAX_PARTITION_SIZE, partition_size);
    (void)ret;

    cleanup_codec(&ctx);

    return 0;
}