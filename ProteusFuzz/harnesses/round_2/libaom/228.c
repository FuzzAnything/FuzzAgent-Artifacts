// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
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
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static int fuzz_encoder_controls(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    int ret = 0;

    if (iface == NULL) return 0;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = (Size > 0) ? (Data[0] % 512) + 16 : 16;
    cfg.g_h = (Size > 1) ? (Data[1] % 512) + 16 : 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Size > 2) ? (Data[2] % 1000) + 1 : 1000;
    cfg.rc_target_bitrate = (Size > 3) ? (Data[3] % 10000) + 1000 : 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz AV1E_ENABLE_SB_QP_SWEEP
    int enable_sb_qp_sweep = (Size > 4) ? (Data[4] % 2) : 0;
    ret = aom_codec_control(&ctx, AV1E_ENABLE_SB_QP_SWEEP, enable_sb_qp_sweep);
    (void)ret;

    // Fuzz AV1E_SET_QUANTIZER_ONE_PASS
    int quantizer = (Size > 5) ? (Data[5] % 64) : 0;
    ret = aom_codec_control(&ctx, AV1E_SET_QUANTIZER_ONE_PASS, quantizer);
    (void)ret;

    // Encode a dummy frame to trigger state changes
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    memset(img.planes[0], 0, img.stride[0] * cfg.g_h);
    memset(img.planes[1], 128, img.stride[1] * (cfg.g_h + 1));
    memset(img.planes[2], 128, img.stride[2] * (cfg.g_h + 1));

    // Fixed: aom_codec_encode takes 5 arguments
    aom_codec_encode(&ctx, &img, 0, 1, 0);

    // Fuzz AOME_GET_LAST_QUANTIZER
    int last_quantizer = 0;
    ret = aom_codec_control(&ctx, AOME_GET_LAST_QUANTIZER, &last_quantizer);
    (void)ret;

    // Fuzz AOME_GET_LAST_QUANTIZER_64
    int64_t last_quantizer_64 = 0;
    ret = aom_codec_control(&ctx, AOME_GET_LAST_QUANTIZER_64, &last_quantizer_64);
    (void)ret;

    aom_img_free(&img);
    aom_codec_destroy(&ctx);
    return 0;
}

static int fuzz_decoder_controls(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    int ret = 0;

    if (iface == NULL) return 0;

    if (aom_codec_dec_init(&ctx, iface, NULL, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Prepare dummy input data for decoding
    if (Size < 10) return 0;
    
    // Fixed: Removed call to undeclared aom_codec_dec_cfg_default
    // Feed some data to initialize decoder state
    aom_codec_decode(&ctx, Data, Size, NULL);

    // Fuzz AOMD_GET_BASE_Q_IDX
    int base_q_idx = 0;
    ret = aom_codec_control(&ctx, AOMD_GET_BASE_Q_IDX, &base_q_idx);
    (void)ret;

    // Fuzz AOMD_GET_LAST_QUANTIZER
    int last_quantizer = 0;
    ret = aom_codec_control(&ctx, AOMD_GET_LAST_QUANTIZER, &last_quantizer);
    (void)ret;

    aom_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_228(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Alternate between encoder and decoder fuzzing based on input
    if (Data[0] & 1) {
        fuzz_encoder_controls(Data, Size);
    } else {
        fuzz_decoder_controls(Data, Size);
    }

    return 0;
}