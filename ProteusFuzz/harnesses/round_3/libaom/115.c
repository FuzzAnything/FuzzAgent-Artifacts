// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

/* Helper to safely free a codec context */
static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

/* Helper to create a dummy image from fuzz data */
static aom_image_t *create_dummy_image(const uint8_t *data, size_t size) {
    if (!data || size == 0) return NULL;

    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 16, 16, 1);
    if (!img) return NULL;

    /* Fill image planes with fuzz data if possible */
    size_t plane_size = 0;
    for (int i = 0; i < 3; i++) {
        if (img->planes[i]) {
            plane_size = img->stride[i] * img->h;
            if (size > plane_size) {
                memcpy(img->planes[i], data + plane_size, plane_size);
            } else {
                memset(img->planes[i], 0, plane_size);
            }
        }
    }

    return img;
}

int LLVMFuzzerTestOneInput_115(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    aom_image_t *img = NULL;
    aom_codec_cx_pkt_t *pkt;
    int frame_count = 0;
    int flush_count = 0;

    /* Initialize context */
    memset(&ctx, 0, sizeof(ctx));

    /* Get AV1 encoder interface */
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    /* Set default config */
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_profile = 0;

    /* Initialize encoder */
    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    /* Extract fuzz data for control commands */
    int val_int = Data[0];
    int val_int2 = Data[1];
    int val_int3 = Data[2];
    int val_int4 = Data[3];

    /* Test AV1E_SET_VMAF_MODEL_PATH */
    const char *dummy_path = "./dummy_file";
    ret = aom_codec_control(&ctx, AV1E_SET_VMAF_MODEL_PATH, dummy_path);
    (void)ret;

    /* Test AV1E_SET_ENABLE_AB_PARTITIONS */
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_AB_PARTITIONS, val_int & 1);
    (void)ret;

    /* Test AV1E_SET_SVC_PARAMS */
    aom_svc_params_t svc_params;
    memset(&svc_params, 0, sizeof(svc_params));
    svc_params.number_spatial_layers = (val_int2 % 3) + 1;
    svc_params.number_temporal_layers = (val_int3 % 3) + 1;
    for (int i = 0; i < AOM_MAX_LAYERS; i++) {
        svc_params.max_quantizers[i] = val_int4 + i;
        svc_params.min_quantizers[i] = (val_int4 + i) / 2;
    }
    ret = aom_codec_control(&ctx, AV1E_SET_SVC_PARAMS, &svc_params);
    (void)ret;

    /* Test AV1E_SET_ENABLE_INTRABC */
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, val_int & 1);
    (void)ret;

    /* Test aom_codec_set_option with fuzzed strings */
    char key[32], value[32];
    size_t key_len = (Size > 10) ? (Size / 10) : 1;
    size_t val_len = (Size > 10) ? (Size / 10) : 1;
    
    if (key_len > 31) key_len = 31;
    if (val_len > 31) val_len = 31;

    memcpy(key, Data + 4, key_len);
    key[key_len] = '\0';
    
    memcpy(value, Data + 4 + key_len, val_len);
    value[val_len] = '\0';

    ret = aom_codec_set_option(&ctx, key, value);
    (void)ret;

    /* Create dummy image for encoding */
    img = create_dummy_image(Data, Size);

    /* Encode loop */
    aom_codec_pts_t pts = 0;
    unsigned long duration = (Size > 0) ? (Size % 100) + 1 : 1;
    if (duration == 0) duration = 1;

    while (frame_count < 5) {
        if (img) {
            ret = aom_codec_encode(&ctx, img, pts, duration, 0);
            if (ret != AOM_CODEC_OK) break;
            pts += duration;
            frame_count++;
        }
    }

    /* Flush encoder */
    while (flush_count < 5) {
        ret = aom_codec_encode(&ctx, NULL, 0, 0, 0);
        if (ret != AOM_CODEC_OK) break;
        
        /* Drain packets */
        while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                /* pkt->data.frame is an aom_image_t*, no need to free it here
                   as the encoder owns the memory or it's handled by the codec */
                /* In libaom, we do not call aom_img_free on frames returned by get_cx_data */
            }
            /* aom_codec_cx_pkt_free does not exist in standard libaom API.
               The packet memory is managed internally or the user must not free it.
               We simply continue to the next packet. */
        }
        flush_count++;
    }

    /* Cleanup */
    if (img) {
        aom_img_free(img);
    }
    cleanup_codec(&ctx);

    return 0;
}