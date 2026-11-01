// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Required headers as specified */
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

/* Helper function to initialize the encoder context */
static aom_codec_ctx_t* init_encoder_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    int ret;

    /* Get the AV1 encoder interface */
    iface = aom_codec_av1_cx();
    if (!iface) {
        return NULL;
    }

    /* Initialize the codec context */
    ret = aom_codec_enc_init(&ctx, iface, NULL, 0);
    if (ret != AOM_CODEC_OK) {
        return NULL;
    }

    /* Set default configuration */
    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        aom_codec_destroy(&ctx);
        return NULL;
    }

    /* Use input data to seed configuration if size permits */
    if (size >= sizeof(int)) {
        int width = 0, height = 0;
        memcpy(&width, data, sizeof(int));
        if (size >= 2 * sizeof(int)) {
            memcpy(&height, data + sizeof(int), sizeof(int));
        } else {
            height = width;
        }
        
        /* Ensure valid dimensions (min 16x16 for AV1) */
        if (width < 16) width = 16;
        if (height < 16) height = 16;
        
        cfg.g_w = width;
        cfg.g_h = height;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.g_threads = 1;
        cfg.rc_target_bitrate = 1000;
    }

    /* Re-initialize with specific config */
    aom_codec_destroy(&ctx);
    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return NULL;
    }

    return ctx;
}

/* Helper function to cleanup encoder context */
static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(&ctx);
    }
}

int LLVMFuzzerTestOneInput_224(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t *ctx = init_encoder_context(Data, Size);
    if (!ctx) {
        return 0;
    }

    /* Extract integers from fuzz data to use as control parameters */
    int val1 = 0, val2 = 0, val3 = 0, val4 = 0, val5 = 0, val6 = 0;
    
    if (Size >= 6 * sizeof(int)) {
        memcpy(&val1, Data, sizeof(int));
        memcpy(&val2, Data + sizeof(int), sizeof(int));
        memcpy(&val3, Data + 2 * sizeof(int), sizeof(int));
        memcpy(&val4, Data + 3 * sizeof(int), sizeof(int));
        memcpy(&val5, Data + 4 * sizeof(int), sizeof(int));
        memcpy(&val6, Data + 5 * sizeof(int), sizeof(int));
    } else if (Size >= sizeof(int)) {
        memcpy(&val1, Data, sizeof(int));
        val2 = val1;
        val3 = val1;
        val4 = val1;
        val5 = val1;
        val6 = val1;
    }

    /* Normalize values to 0 or 1 for boolean-like flags if needed, 
       but pass raw for testing boundary conditions */
    
    /* 1. AV1E_SET_ENABLE_ANGLE_DELTA */
    aom_codec_control(ctx, AV1E_SET_ENABLE_ANGLE_DELTA, val1);

    /* 2. AV1E_SET_MIN_PARTITION_SIZE */
    /* Ensure min partition size is within valid range (e.g., 8 to 64) */
    int min_part = (val2 % 57) + 8; 
    aom_codec_control(ctx, AV1E_SET_MIN_PARTITION_SIZE, min_part);

    /* 3. AV1E_SET_ENABLE_DNL_DENOISING */
    aom_codec_control(ctx, AV1E_SET_ENABLE_DNL_DENOISING, val3);

    /* 4. AV1E_SET_ENABLE_1TO4_PARTITIONS */
    aom_codec_control(ctx, AV1E_SET_ENABLE_1TO4_PARTITIONS, val4);

    /* 5. AV1E_SET_INTER_DCT_ONLY */
    aom_codec_control(ctx, AV1E_SET_INTER_DCT_ONLY, val5);

    /* 6. AV1E_SET_REDUCED_TX_TYPE_SET */
    aom_codec_control(ctx, AV1E_SET_REDUCED_TX_TYPE_SET, val6);

    /* Cleanup */
    cleanup_encoder_context(ctx);

    return 0;
}