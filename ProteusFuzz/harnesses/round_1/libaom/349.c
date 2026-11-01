// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (ctx == NULL || data == NULL || size == 0) {
        return 0;
    }

    // Use data to generate random values for controls
    uint8_t val = data[0] % 2; // 0 or 1
    int8_t int_val = (int8_t)(data[1] - 128);
    int32_t int32_val = (int32_t)(data[2] * 256 + data[3]);

    // AV1E_SET_INTER_DCT_ONLY
    if (aom_codec_control(ctx, AV1E_SET_INTER_DCT_ONLY, val) != AOM_CODEC_OK) {
        // Ignore errors, just continue fuzzing
    }

    // AV1E_SET_ENABLE_INTERINTER_WEDGE
    if (aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, val) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // AV1E_SET_ENABLE_DNL_DENOISING
    if (aom_codec_control(ctx, AV1E_SET_ENABLE_DNL_DENOISING, val) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // AV1E_SET_ENABLE_CHROMA_DELTAQ
    if (aom_codec_control(ctx, AV1E_SET_ENABLE_CHROMA_DELTAQ, val) != AOM_CODEC_OK) {
        // Ignore errors
    }

    // AV1E_GET_SEQ_LEVEL_IDX
    int32_t level_idx = 0;
    if (aom_codec_control(ctx, AV1E_GET_SEQ_LEVEL_IDX, &level_idx) == AOM_CODEC_OK) {
        // Use level_idx to vary state if needed
        (void)level_idx;
    }

    // AV1E_SET_SVC_REF_FRAME_CONFIG
    if (size >= 24) {
        aom_svc_ref_frame_config_t svc_config;
        memset(&svc_config, 0, sizeof(svc_config));

        // Fill reference array (7 elements)
        for (int i = 0; i < 7; i++) {
            svc_config.reference[i] = (data[4 + i] % 2);
        }

        // Fill ref_idx array (7 elements)
        for (int i = 0; i < 7; i++) {
            svc_config.ref_idx[i] = data[11 + i] % 8;
        }

        // Fill refresh array (8 elements)
        for (int i = 0; i < 8; i++) {
            svc_config.refresh[i] = (data[18 + i] % 2);
        }

        if (aom_codec_control(ctx, AV1E_SET_SVC_REF_FRAME_CONFIG, &svc_config) != AOM_CODEC_OK) {
            // Ignore errors
        }
    }

    return 0;
}

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_349(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);

    // Use fuzz data to set some config parameters
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = (Data[2] % 255) + 1;
    cfg.g_timebase.den = (Data[3] % 255) + 1;
    cfg.rc_target_bitrate = (Data[4] * 256 + Data[5]) + 100;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(&ctx);
        return 0;
    }

    fuzz_encoder_control(&ctx, Data, Size);

    // Encode a dummy frame to exercise more states
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    
    // Fixed: Use 'stride' instead of 'd_stride'
    memset(img.planes[0], Data[6] % 256, img.stride[0] * cfg.g_h);
    memset(img.planes[1], Data[7] % 256, img.stride[1] * (cfg.g_h + 1));
    memset(img.planes[2], Data[8] % 256, img.stride[2] * (cfg.g_h + 1));

    // Fixed: Use AOME_SET_CPUUSED instead of AV1E_SET_CPUUSED
    if (Size > 9) {
        aom_codec_control(&ctx, AOME_SET_CPUUSED, Data[9] % 8);
    }

    // Fixed: aom_codec_encode takes 5 arguments
    aom_codec_encode(&ctx, &img, 0, 1, 0);

    aom_img_free(&img);
    cleanup_codec(&ctx);

    return 0;
}