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
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
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
        return 1;
    }

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return 1;
    }

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return 1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_129(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    int ret;
    int val;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (init_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    // Extract a value from the input data to use as the control parameter
    val = Data[0];

    // Fuzz AV1E_SET_ENABLE_SMOOTH_INTERINTRA
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_SMOOTH_INTERINTRA, val);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_INTERINTRA_COMP
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, val);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_DIRECTIONAL_INTRA
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_DIRECTIONAL_INTRA, val);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_INTERINTRA_WEDGE
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, val);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_DIAGONAL_INTRA
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_DIAGONAL_INTRA, val);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_PAETH_INTRA
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_PAETH_INTRA, val);
    (void)ret;

    cleanup_codec(&ctx);

    return 0;
}