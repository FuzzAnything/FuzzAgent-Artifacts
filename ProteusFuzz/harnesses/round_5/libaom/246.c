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
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_246(const uint8_t *Data, size_t Size) {
    if (Size < 30) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to vary configuration slightly with bounds checking
    cfg.g_w = 16 + (Data[0] % 64);
    cfg.g_h = 16 + (Data[1] % 64);
    cfg.g_threads = 1 + (Data[2] % 4);
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz AV1E_SET_SVC_REF_FRAME_CONFIG
    aom_svc_ref_frame_config_t svc_cfg;
    memset(&svc_cfg, 0, sizeof(svc_cfg));
    for (int i = 0; i < 7; i++) {
        svc_cfg.reference[i] = (Data[3 + i] % 2);
        svc_cfg.ref_idx[i] = (Data[10 + i] % 8);
    }
    for (int i = 0; i < 8; i++) {
        svc_cfg.refresh[i] = (Data[17 + i] % 2);
    }
    aom_codec_control(&ctx, AV1E_SET_SVC_REF_FRAME_CONFIG, &svc_cfg);

    // Fuzz AV1E_SET_ENABLE_DIRECTIONAL_INTRA
    int enable_dir_intra = (Data[25] % 2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_DIRECTIONAL_INTRA, &enable_dir_intra);

    // Fuzz AV1E_GET_SEQ_LEVEL_IDX
    int seq_level_idx = 0;
    aom_codec_control(&ctx, AV1E_GET_SEQ_LEVEL_IDX, &seq_level_idx);
    (void)seq_level_idx; // Use the value to avoid unused variable warning

    // Fuzz AV1E_SET_ENABLE_SUPERRES
    int enable_superres = (Data[27] % 2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_SUPERRES, &enable_superres);

    // Fuzz AV1E_SET_ENABLE_SMOOTH_INTERINTRA
    int enable_smooth_interintra = (Data[28] % 2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_SMOOTH_INTERINTRA, &enable_smooth_interintra);

    // Fuzz AV1E_SET_ENABLE_DNL_DENOISING
    int enable_dnl_denoising = (Data[29] % 2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_DNL_DENOISING, &enable_dnl_denoising);

    cleanup_codec(&ctx);
    return 0;
}