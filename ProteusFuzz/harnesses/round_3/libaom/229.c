// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
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
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomcx.h>

/* Helper to safely initialize encoder context */
static aom_codec_ctx_t *init_encoder_context(aom_codec_flags_t flags) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;

    aom_codec_err_t err = aom_codec_enc_init(ctx, iface, &cfg, flags);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

/* Helper to create a dummy frame */
static aom_image_t *create_dummy_frame(int w, int h) {
    aom_image_t *img = (aom_image_t *)malloc(sizeof(aom_image_t));
    if (!img) return NULL;
    memset(img, 0, sizeof(aom_image_t));

    if (aom_img_alloc(img, AOM_IMG_FMT_I420, w, h, 1) != 0) {
        free(img);
        return NULL;
    }
    return img;
}

/* Helper to cleanup encoder and frame */
static void cleanup_encoder(aom_codec_ctx_t *ctx, aom_image_t *img) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
    if (img) {
        aom_img_free(img);
        free(img);
    }
}

int LLVMFuzzerTestOneInput_229(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    /* Initialize encoder context */
    aom_codec_ctx_t *ctx = init_encoder_context(0);
    if (!ctx) return 0;

    aom_image_t *img = create_dummy_frame(32, 32);
    if (!img) {
        cleanup_encoder(ctx, NULL);
        return 0;
    }

    /* Use input data to diversify control calls */
    uint8_t val = Data[0];
    int enable_tx_size_search = (val % 2);
    int enable_cfl_intra = ((val >> 1) % 2);
    int enable_intrabc = ((val >> 2) % 2);
    int transfer_char = (val >> 3) % 10; /* AV1 transfer characteristics enum range */

    /* 1. AV1E_SET_ENABLE_TX_SIZE_SEARCH */
    aom_codec_control(ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, enable_tx_size_search);

    /* 2. AV1E_SET_ENABLE_CFL_INTRA */
    aom_codec_control(ctx, AV1E_SET_ENABLE_CFL_INTRA, enable_cfl_intra);

    /* 5. AV1E_SET_ENABLE_INTRABC */
    aom_codec_control(ctx, AV1E_SET_ENABLE_INTRABC, enable_intrabc);

    /* 3. AV1E_SET_TRANSFER_CHARACTERISTICS */
    aom_codec_control(ctx, AV1E_SET_TRANSFER_CHARACTERISTICS, transfer_char);

    /* 4. AOME_SET_ROI_MAP */
    if (Size >= 10) {
        aom_roi_map_t roi_map;
        memset(&roi_map, 0, sizeof(roi_map));
        roi_map.enabled = (Data[1] % 2);
        roi_map.rows = (Data[2] % 10) + 1;
        roi_map.cols = (Data[3] % 10) + 1;
        size_t roi_size = roi_map.rows * roi_map.cols;
        if (roi_size > 0 && roi_size <= 1000) {
            roi_map.roi_map = (unsigned char *)malloc(roi_size);
            if (roi_map.roi_map) {
                for (size_t i = 0; i < roi_size; i++) {
                    roi_map.roi_map[i] = Data[4 + i % (Size - 4)] % 8;
                }
                aom_codec_control(ctx, AOME_SET_ROI_MAP, &roi_map);
                free(roi_map.roi_map);
            }
        }
    }

    /* 6. AOME_SET_ACTIVEMAP */
    if (Size >= 5) {
        aom_active_map_t active_map;
        memset(&active_map, 0, sizeof(active_map));
        active_map.rows = (Data[4] % 10) + 1;
        active_map.cols = (Data[5] % 10) + 1;
        size_t map_size = active_map.rows * active_map.cols;
        if (map_size > 0 && map_size <= 1000) {
            active_map.active_map = (unsigned char *)malloc(map_size);
            if (active_map.active_map) {
                for (size_t i = 0; i < map_size; i++) {
                    active_map.active_map[i] = (Data[6 + i % (Size - 6)] % 2);
                }
                aom_codec_control(ctx, AOME_SET_ACTIVEMAP, &active_map);
                free(active_map.active_map);
            }
        }
    }

    /* Encode a frame to exercise the state changes */
    /* Fixed: aom_codec_encode takes 5 arguments: ctx, img, pts, duration, flags */
    aom_codec_encode(ctx, img, 0, 1, 0);

    /* Cleanup */
    cleanup_encoder(ctx, img);

    return 0;
}