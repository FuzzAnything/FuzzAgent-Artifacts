// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static int test_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size < 20) return 0;
    
    int32_t val1 = *(int32_t *)data;
    int32_t val2 = *(int32_t *)(data + 4);
    int32_t val3 = *(int32_t *)(data + 8);
    int32_t val4 = *(int32_t *)(data + 12);
    int32_t val5 = *(int32_t *)(data + 16);

    // AOME_SET_SPATIAL_LAYER_ID
    aom_codec_control(ctx, AOME_SET_SPATIAL_LAYER_ID, val1);
    
    // AOME_SET_NUMBER_SPATIAL_LAYERS
    aom_codec_control(ctx, AOME_SET_NUMBER_SPATIAL_LAYERS, val2);
    
    // AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE
    aom_codec_control(ctx, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, val3);
    
    // AV1E_SET_SVC_FRAME_DROP_MODE
    aom_codec_control(ctx, AV1E_SET_SVC_FRAME_DROP_MODE, val4);
    
    return 0;
}

static int test_decoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size < 8) return 0;
    
    int32_t val1 = *(int32_t *)data;
    int32_t val2 = *(int32_t *)(data + 4);

    // AV1D_SET_OPERATING_POINT
    aom_codec_control(ctx, AV1D_SET_OPERATING_POINT, val1);
    
    // AV1D_SET_OUTPUT_ALL_LAYERS
    aom_codec_control(ctx, AV1D_SET_OUTPUT_ALL_LAYERS, val2);
    
    return 0;
}

int LLVMFuzzerTestOneInput_331(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Determine test mode based on first byte
    int mode = Data[0] % 2;
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface = NULL;
    aom_codec_err_t ret;

    if (mode == 0) {
        // Encoder test
        iface = aom_codec_av1_cx();
        if (!iface) return 0;

        ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
        if (!ctx) return 0;
        memset(ctx, 0, sizeof(aom_codec_ctx_t));

        aom_codec_enc_cfg_t cfg;
        aom_codec_enc_config_default(iface, &cfg, 0);
        cfg.g_w = 128;
        cfg.g_h = 128;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 100;

        ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
        if (ret != AOM_CODEC_OK) {
            cleanup_encoder(ctx);
            return 0;
        }

        // Feed dummy data to encoder to ensure state is valid
        if (Size > 1) {
            aom_image_t img;
            aom_img_alloc(&img, AOM_IMG_FMT_I420, 128, 128, 1);
            if (img.planes[0]) {
                memset(img.planes[0], Data[1], 128*128);
            }
            aom_codec_encode(ctx, &img, 0, 1, 0);
            aom_img_free(&img);
        }

        test_encoder_controls(ctx, Data + 1, Size - 1);
        cleanup_encoder(ctx);
    } else {
        // Decoder test
        iface = aom_codec_av1_dx();
        if (!iface) return 0;

        ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
        if (!ctx) return 0;
        memset(ctx, 0, sizeof(aom_codec_ctx_t));

        aom_codec_dec_cfg_t cfg;
        memset(&cfg, 0, sizeof(cfg));
        cfg.threads = 1;
        cfg.allow_lowbitdepth = 1;

        ret = aom_codec_dec_init(ctx, iface, &cfg, 0);
        if (ret != AOM_CODEC_OK) {
            cleanup_decoder(ctx);
            return 0;
        }

        // Feed dummy data to decoder
        if (Size > 1) {
            aom_codec_dec_cfg_t dec_cfg;
            memset(&dec_cfg, 0, sizeof(dec_cfg));
            aom_codec_dec_init(ctx, iface, &dec_cfg, 0);
            
            aom_codec_control(ctx, AV1D_SET_OPERATING_POINT, 0);
            aom_codec_control(ctx, AV1D_SET_OUTPUT_ALL_LAYERS, 0);
            
            // Try to decode some data
            if (Size > 2) {
                aom_codec_decode(ctx, Data + 1, Size - 1, NULL);
            }
        }

        test_decoder_controls(ctx, Data + 1, Size - 1);
        cleanup_decoder(ctx);
    }

    return 0;
}