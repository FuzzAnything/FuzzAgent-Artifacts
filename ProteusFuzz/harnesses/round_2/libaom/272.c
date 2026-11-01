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
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_decoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (ctx == NULL || data == NULL || size == 0) {
        return 0;
    }

    int value = 0;
    if (size >= sizeof(int)) {
        memcpy(&value, data, sizeof(int));
    }

    // AV1E_SET_ENABLE_RECT_TX
    if (aom_codec_control(ctx, AV1E_SET_ENABLE_RECT_TX, value) != AOM_CODEC_OK) {
        return 0;
    }

    // AV1E_SET_ENABLE_DUAL_FILTER
    if (aom_codec_control(ctx, AV1E_SET_ENABLE_DUAL_FILTER, value) != AOM_CODEC_OK) {
        return 0;
    }

    // AV1E_SET_ENABLE_INTRA_EDGE_FILTER
    if (aom_codec_control(ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, value) != AOM_CODEC_OK) {
        return 0;
    }

    // AV1E_SET_ENABLE_MASKED_COMP
    if (aom_codec_control(ctx, AV1E_SET_ENABLE_MASKED_COMP, value) != AOM_CODEC_OK) {
        return 0;
    }

    // AV1E_SET_ENABLE_SMOOTH_INTERINTRA
    if (aom_codec_control(ctx, AV1E_SET_ENABLE_SMOOTH_INTERINTRA, value) != AOM_CODEC_OK) {
        return 0;
    }

    // AV1E_SET_ENABLE_INTERINTER_WEDGE
    if (aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, value) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_272(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);

    // Initialize encoder context
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Perform fuzzing on encoder control functions
    fuzz_encoder_control(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}