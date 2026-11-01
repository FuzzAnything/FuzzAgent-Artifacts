// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
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
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_214(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    aom_codec_err_t res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Prepare a dummy frame
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    
    // Fill image data with fuzzed input if size permits
    // Corrected access to planes: img.planes[0] is a pointer to uint8_t
    size_t img_size = (size_t)img.stride[0] * img.h;
    if (Size > img_size) {
        memcpy(img.planes[0], Data, img_size);
    } else if (Size > 0) {
        memset(img.planes[0], Data[0], img_size);
    }

    // Extract values from input to diversify control calls
    int32_t val1 = (Size > 0) ? (int32_t)Data[0] : 0;
    int32_t val2 = (Size > 1) ? (int32_t)Data[1] : 0;
    int32_t val3 = (Size > 2) ? (int32_t)Data[2] : 0;
    int32_t val4 = (Size > 3) ? (int32_t)Data[3] : 0;
    int32_t val5 = (Size > 4) ? (int32_t)Data[4] : 0;
    int32_t val6 = (Size > 5) ? (int32_t)Data[5] : 0;

    // Call target API functions with fuzzed values
    // 1. aom_codec_control_typechecked_AV1E_SET_QM_MIN
    aom_codec_control(&ctx, AV1E_SET_QM_MIN, val1);

    // 2. aom_codec_control_typechecked_AV1E_SET_VBR_CORPUS_COMPLEXITY_LAP
    aom_codec_control(&ctx, AV1E_SET_VBR_CORPUS_COMPLEXITY_LAP, val2);

    // 3. aom_codec_control_typechecked_AV1E_SET_COEFF_COST_UPD_FREQ
    aom_codec_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, val3);

    // 4. aom_codec_control_typechecked_AOME_SET_SHARPNESS
    aom_codec_control(&ctx, AOME_SET_SHARPNESS, val4);

    // 5. aom_codec_control_typechecked_AV1E_SET_MV_COST_UPD_FREQ
    aom_codec_control(&ctx, AV1E_SET_MV_COST_UPD_FREQ, val5);

    // 6. aom_codec_control_typechecked_AV1E_SET_MODE_COST_UPD_FREQ
    aom_codec_control(&ctx, AV1E_SET_MODE_COST_UPD_FREQ, val6);

    // Attempt to encode to exercise the state
    aom_codec_encode(&ctx, &img, 0, 1, 0);

    // Cleanup
    aom_img_free(&img);
    cleanup_codec(&ctx);

    return 0;
}