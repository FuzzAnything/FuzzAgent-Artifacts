// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aom.h>

static int fuzz_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;
    aom_codec_dec_cfg_t cfg;
    aom_codec_iter_t iter = NULL;

    iface = aom_codec_av1_dx();
    if (!iface) return 0;

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 64;
    cfg.h = 64;

    ret = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Feed data to decoder
    ret = aom_codec_decode(&ctx, data, size, NULL);
    if (ret != AOM_CODEC_OK) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Iterate over decoded frames
    while ((aom_codec_get_frame(&ctx, &iter)) != NULL) {
        int quantizer_val = 0;
        int base_q_idx = 0;
        int ref_used = 0;

        // AOMD_GET_LAST_QUANTIZER
        aom_codec_control(&ctx, AOMD_GET_LAST_QUANTIZER, &quantizer_val);
        
        // AOMD_GET_BASE_Q_IDX
        aom_codec_control(&ctx, AOMD_GET_BASE_Q_IDX, &base_q_idx);
        
        // AOMD_GET_LAST_REF_USED
        aom_codec_control(&ctx, AOMD_GET_LAST_REF_USED, &ref_used);
    }

    aom_codec_destroy(&ctx);
    return 0;
}

static int fuzz_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;
    aom_codec_enc_cfg_t cfg;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 1;

    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Set quantizer related controls
    int quantizer_val = 0;
    if (size > 0) {
        quantizer_val = data[0] % 64;
    }

    // AOME_SET_CQ_LEVEL
    aom_codec_control(&ctx, AOME_SET_CQ_LEVEL, quantizer_val);

    // AV1E_SET_QUANTIZER_ONE_PASS
    aom_codec_control(&ctx, AV1E_SET_QUANTIZER_ONE_PASS, quantizer_val);

    // AOME_GET_LAST_QUANTIZER
    int last_quantizer = 0;
    aom_codec_control(&ctx, AOME_GET_LAST_QUANTIZER, &last_quantizer);

    // Encode dummy frame if data is large enough
    if (size >= 64 * 64 * 3 / 2) {
        aom_image_t img;
        aom_img_alloc(&img, AOM_IMG_FMT_I420, 64, 64, 1);
        // Fill with data
        for (int i = 0; i < 64 * 64 * 3 / 2 && i < size; i++) {
            img.planes[0][i] = data[i];
        }

        aom_enc_frame_flags_t flags = 0;
        ret = aom_codec_encode(&ctx, &img, 0, 1, flags);
        if (ret == AOM_CODEC_OK) {
            // Try to get quantizer after encode
            aom_codec_control(&ctx, AOME_GET_LAST_QUANTIZER, &last_quantizer);
        }
        aom_img_free(&img);
    }

    aom_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Randomly choose between encoder and decoder fuzzing
    if (Data[0] & 1) {
        fuzz_decoder(Data, Size);
    } else {
        fuzz_encoder(Data, Size);
    }

    return 0;
}