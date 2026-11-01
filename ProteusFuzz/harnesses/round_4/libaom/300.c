// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
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

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size == 0) return 0;

    int32_t val;
    int ret = 0;

    // Try to extract a value from the fuzzed data if possible
    if (size >= sizeof(int32_t)) {
        memcpy(&val, data, sizeof(int32_t));
    } else {
        val = 0;
    }

    // AV1E_SET_CHROMA_SUBSAMPLING_X
    ret = aom_codec_control(ctx, AV1E_SET_CHROMA_SUBSAMPLING_X, val);
    (void)ret;

    // AV1E_SET_ENABLE_QM
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_QM, val);
    (void)ret;

    // AV1E_SET_ENABLE_KEYFRAME_FILTERING
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_KEYFRAME_FILTERING, val);
    (void)ret;

    // AV1E_SET_CHROMA_SUBSAMPLING_Y
    ret = aom_codec_control(ctx, AV1E_SET_CHROMA_SUBSAMPLING_Y, val);
    (void)ret;

    // AV1E_SET_LOSSLESS
    ret = aom_codec_control(ctx, AV1E_SET_LOSSLESS, val);
    (void)ret;

    // AV1E_SET_TIER_MASK
    ret = aom_codec_control(ctx, AV1E_SET_TIER_MASK, val);
    (void)ret;

    return 0;
}

int LLVMFuzzerTestOneInput_300(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize encoder
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions
    fuzz_encoder_control(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}