// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>

int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_image_t *img;
    
    memset(&ctx, 0, sizeof(ctx));
    
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    uint32_t w = Data[0] | (Data[1] << 8);
    uint32_t h = Data[2] | (Data[3] << 8);
    
    if (w == 0) w = 16;
    if (h == 0) h = 16;
    
    cfg.g_w = w;
    cfg.g_h = h;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_profile = 0;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, w, h, 1);
    if (!img) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // AV1E_SET_ENABLE_QM
    aom_codec_control(&ctx, AV1E_SET_ENABLE_QM, 0);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_QM, 1);
    if (Size > 4) aom_codec_control(&ctx, AV1E_SET_ENABLE_QM, Data[4] & 0xFF);

    // AV1E_SET_DELTAQ_MODE
    aom_codec_control(&ctx, AV1E_SET_DELTAQ_MODE, 0);
    if (Size > 5) aom_codec_control(&ctx, AV1E_SET_DELTAQ_MODE, Data[5] & 0xFF);

    // AV1E_SET_QM_V
    int qm[64];
    memset(qm, 0, sizeof(qm));
    for (int i = 0; i < 64 && (6 + i) < Size; i++) {
        qm[i] = Data[6 + i];
    }
    aom_codec_control(&ctx, AV1E_SET_QM_V, qm);
    aom_codec_control(&ctx, AV1E_SET_QM_V, NULL);

    // AV1E_SET_ENABLE_TPL_MODEL
    aom_codec_control(&ctx, AV1E_SET_ENABLE_TPL_MODEL, 0);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_TPL_MODEL, 1);
    if (Size > 7) aom_codec_control(&ctx, AV1E_SET_ENABLE_TPL_MODEL, Data[7] & 0xFF);

    // AV1E_SET_QM_MAX
    aom_codec_control(&ctx, AV1E_SET_QM_MAX, 0);
    aom_codec_control(&ctx, AV1E_SET_QM_MAX, 63);
    if (Size > 8) aom_codec_control(&ctx, AV1E_SET_QM_MAX, Data[8] & 0xFF);

    // AV1E_SET_DELTAQ_STRENGTH
    aom_codec_control(&ctx, AV1E_SET_DELTAQ_STRENGTH, 0);
    aom_codec_control(&ctx, AV1E_SET_DELTAQ_STRENGTH, 100);
    if (Size > 9) aom_codec_control(&ctx, AV1E_SET_DELTAQ_STRENGTH, Data[9] & 0xFF);

    aom_codec_encode(&ctx, img, 0, 1, 0);

    aom_img_free(img);
    aom_codec_destroy(&ctx);

    return 0;
}