// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_OVERLAY at aomcx.h:2150:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_CFL_INTRA at aomcx.h:2144:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_WARPED_MOTION at aomcx.h:2129:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_RECT_PARTITIONS at aomcx.h:2060:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_PALETTE at aomcx.h:2153:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_LOOPFILTER_CONTROL at aomcx.h:2297:1 in aomcx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>

static void setup_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) return;

    ret = aom_codec_enc_config_default(iface, cfg, 0);
    if (ret != AOM_CODEC_OK) return;

    ret = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) return;
}

int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    int value;
    aom_codec_err_t ret;

    if (Size < 1) return 0;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    setup_encoder(&ctx, &cfg);
    if (ctx.iface == NULL) return 0;

    // Use input data to determine which control to test and what value to set
    int control_type = Data[0] % 6;
    int enable_val = (Data[0] & 1); // 0 or 1

    switch (control_type) {
        case 0:
            ret = aom_codec_control_typechecked_AV1E_SET_ENABLE_OVERLAY(&ctx, AV1E_SET_ENABLE_OVERLAY, enable_val);
            break;
        case 1:
            ret = aom_codec_control_typechecked_AV1E_SET_ENABLE_CFL_INTRA(&ctx, AV1E_SET_ENABLE_CFL_INTRA, enable_val);
            break;
        case 2:
            ret = aom_codec_control_typechecked_AV1E_SET_ENABLE_WARPED_MOTION(&ctx, AV1E_SET_ENABLE_WARPED_MOTION, enable_val);
            break;
        case 3:
            ret = aom_codec_control_typechecked_AV1E_SET_ENABLE_RECT_PARTITIONS(&ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, enable_val);
            break;
        case 4:
            ret = aom_codec_control_typechecked_AV1E_SET_ENABLE_PALETTE(&ctx, AV1E_SET_ENABLE_PALETTE, enable_val);
            break;
        case 5:
            // For loopfilter control, assuming it takes an int value based on pattern, 
            // though typically loopfilter control might use a struct. 
            // Using the macro constant as the second argument.
            ret = aom_codec_control_typechecked_AV1E_SET_LOOPFILTER_CONTROL(&ctx, AV1E_SET_LOOPFILTER_CONTROL, enable_val);
            break;
        default:
            ret = AOM_CODEC_ERROR;
            break;
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}