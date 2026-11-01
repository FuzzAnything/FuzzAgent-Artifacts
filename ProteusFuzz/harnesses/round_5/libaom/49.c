// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Target functions:
 * - AV1E_SET_INTER_DCT_ONLY
 * - AV1E_SET_SVC_LAYER_ID
 * - AV1E_SET_ENABLE_FILTER_INTRA
 * - AV1E_SET_ENABLE_AB_PARTITIONS
 * - AV1E_SET_ENABLE_INTERINTRA_WEDGE
 * - AV1E_SET_ENABLE_INTERINTER_WEDGE
 */

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_lag_in_frames = 0;

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    /* Create a dummy image frame to allow encoding if needed */
    if (size > 0) {
        aom_image_t img;
        aom_img_alloc(&img, AOM_IMG_FMT_I420, 16, 16, 1);
        
        if (size >= 16 * 16 * 3 / 2) {
            memcpy(img.planes[0], data, 16 * 16);
            memcpy(img.planes[1], data + 16 * 16, 8 * 8);
            memcpy(img.planes[2], data + 16 * 16 + 8 * 8, 8 * 8);
        }
        
        /* aom_codec_encode signature: (ctx, img, pts, duration, flags) */
        aom_codec_encode(ctx, &img, 0, 1, 0);
        aom_img_free(&img);
    }

    return ctx;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (!ctx) return;
    aom_codec_destroy(ctx);
    free(ctx);
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t *ctx;
    aom_svc_layer_id_t svc_id;
    int val_int;

    ctx = init_encoder(Data, Size);
    if (!ctx) {
        return 0;
    }

    if (Size >= 1) {
        val_int = Data[0];
    } else {
        val_int = 0;
    }

    /* 1. AV1E_SET_INTER_DCT_ONLY */
    if (val_int <= 1) {
        aom_codec_control(ctx, AV1E_SET_INTER_DCT_ONLY, val_int);
    }

    /* 2. AV1E_SET_SVC_LAYER_ID */
    if (Size >= 2) {
        svc_id.spatial_layer_id = (int)Data[0];
        svc_id.temporal_layer_id = (int)Data[1];
        aom_codec_control(ctx, AV1E_SET_SVC_LAYER_ID, &svc_id);
    }

    /* 3. AV1E_SET_ENABLE_FILTER_INTRA */
    if (Size >= 3) {
        val_int = Data[2];
        if (val_int <= 1) {
            aom_codec_control(ctx, AV1E_SET_ENABLE_FILTER_INTRA, val_int);
        }
    }

    /* 4. AV1E_SET_ENABLE_AB_PARTITIONS */
    if (Size >= 4) {
        val_int = Data[3];
        if (val_int <= 1) {
            aom_codec_control(ctx, AV1E_SET_ENABLE_AB_PARTITIONS, val_int);
        }
    }

    /* 5. AV1E_SET_ENABLE_INTERINTRA_WEDGE */
    if (Size >= 5) {
        val_int = Data[4];
        if (val_int <= 1) {
            aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, val_int);
        }
    }

    /* 6. AV1E_SET_ENABLE_INTERINTER_WEDGE */
    if (Size >= 6) {
        val_int = Data[5];
        if (val_int <= 1) {
            aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, val_int);
        }
    }

    cleanup_encoder(ctx);

    return 0;
}