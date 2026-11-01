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
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->priv) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return -1;
    }

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg = {0};
    int ret;

    // Initialize encoder
    if (init_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    // Use input data to diversify control calls
    uint8_t val1 = Data[0];
    uint8_t val2 = Data[1];

    // Fuzz AV1E_SET_DENOISE_BLOCK_SIZE
    ret = aom_codec_control(&ctx, AV1E_SET_DENOISE_BLOCK_SIZE, val1);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_TPL_MODEL
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_TPL_MODEL, val2);
    (void)ret;

    // Fuzz AV1E_SET_DELTAQ_MODE
    ret = aom_codec_control(&ctx, AV1E_SET_DELTAQ_MODE, val1);
    (void)ret;

    // Fuzz AV1E_SET_COEFF_COST_UPD_FREQ
    ret = aom_codec_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, val2);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_QM
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_QM, val1);
    (void)ret;

    // Fuzz AV1E_SET_QM_MAX
    ret = aom_codec_control(&ctx, AV1E_SET_QM_MAX, val2);
    (void)ret;

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}