// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control_typechecked_AV1_SET_INSPECTION_CALLBACK at aomdx.h:555:1 in aomdx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_ANGLE_DELTA at aomcx.h:2159:1 in aomcx.h
// aom_codec_control_typechecked_AOME_GET_LAST_QUANTIZER_64 at aomcx.h:1905:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_TRANSFER_CHARACTERISTICS at aomcx.h:1981:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_DIST_WTD_COMP at aomcx.h:2090:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_QUANT_B_ADAPT at aomcx.h:2207:1 in aomcx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>

int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Decoder Context
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return 0;

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 128;
    cfg.h = 128;

    aom_codec_err_t err = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Encoder Context
    aom_codec_ctx_t enc_ctx;
    memset(&enc_ctx, 0, sizeof(enc_ctx));
    aom_codec_iface_t *enc_iface = aom_codec_av1_cx();
    if (!enc_iface) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    aom_codec_enc_cfg_t enc_cfg;
    memset(&enc_cfg, 0, sizeof(enc_cfg));
    aom_codec_enc_config_default(enc_iface, &enc_cfg, 0);
    enc_cfg.g_w = 128;
    enc_cfg.g_h = 128;
    enc_cfg.rc_target_bitrate = 1000;

    err = aom_codec_enc_init(&enc_ctx, enc_iface, &enc_cfg, 0);
    if (err != AOM_CODEC_OK) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // 1. AV1_SET_INSPECTION_CALLBACK
    // Signature: aom_codec_control_typechecked_AV1_SET_INSPECTION_CALLBACK(ctx, ctrl, data)
    aom_inspect_init inspect_init;
    memset(&inspect_init, 0, sizeof(inspect_init));
    // The macro expands to require the control ID as the second argument
    aom_codec_control_typechecked_AV1_SET_INSPECTION_CALLBACK(&ctx, AV1_SET_INSPECTION_CALLBACK, &inspect_init);

    // 2. AV1E_SET_ENABLE_ANGLE_DELTA
    // Signature: aom_codec_control_typechecked_AV1E_SET_ENABLE_ANGLE_DELTA(ctx, ctrl, data)
    int angle_delta = (Size > 0) ? (Data[0] & 1) : 0;
    aom_codec_control_typechecked_AV1E_SET_ENABLE_ANGLE_DELTA(&enc_ctx, AV1E_SET_ENABLE_ANGLE_DELTA, angle_delta);

    // 3. AOME_GET_LAST_QUANTIZER_64
    // Signature: aom_codec_control_typechecked_AOME_GET_LAST_QUANTIZER_64(ctx, ctrl, data)
    int quant_val = 0;
    aom_codec_control_typechecked_AOME_GET_LAST_QUANTIZER_64(&enc_ctx, AOME_GET_LAST_QUANTIZER_64, &quant_val);

    // 4. AV1E_SET_TRANSFER_CHARACTERISTICS
    // Signature: aom_codec_control_typechecked_AV1E_SET_TRANSFER_CHARACTERISTICS(ctx, ctrl, data)
    int transfer_char = (Size > 1) ? (Data[1] % 20) : 0;
    aom_codec_control_typechecked_AV1E_SET_TRANSFER_CHARACTERISTICS(&enc_ctx, AV1E_SET_TRANSFER_CHARACTERISTICS, transfer_char);

    // 5. AV1E_SET_ENABLE_DIST_WTD_COMP
    // Signature: aom_codec_control_typechecked_AV1E_SET_ENABLE_DIST_WTD_COMP(ctx, ctrl, data)
    int dist_wtd = (Size > 2) ? (Data[2] & 1) : 0;
    aom_codec_control_typechecked_AV1E_SET_ENABLE_DIST_WTD_COMP(&enc_ctx, AV1E_SET_ENABLE_DIST_WTD_COMP, dist_wtd);

    // 6. AV1E_SET_QUANT_B_ADAPT
    // Signature: aom_codec_control_typechecked_AV1E_SET_QUANT_B_ADAPT(ctx, ctrl, data)
    int quant_b = (Size > 3) ? (Data[3] & 1) : 0;
    aom_codec_control_typechecked_AV1E_SET_QUANT_B_ADAPT(&enc_ctx, AV1E_SET_QUANT_B_ADAPT, quant_b);

    // Cleanup
    aom_codec_destroy(&ctx);
    aom_codec_destroy(&enc_ctx);

    return 0;
}