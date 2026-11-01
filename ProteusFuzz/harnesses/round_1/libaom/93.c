// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom.h"
#include "aom_codec.h"
#include "aom_encoder.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom_integer.h"
#include "aomdx.h"
#include "aomcx.h"
#include "aom_external_partition.h"
#include "aom_frame_buffer.h"

/* Helper to safely read integers from fuzz input */
static int read_int(const uint8_t **data, size_t *size) {
    if (*size < sizeof(int)) return 0;
    int val;
    memcpy(&val, *data, sizeof(int));
    *data += sizeof(int);
    *size -= sizeof(int);
    return val;
}

/* Cleanup function for codec context */
static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    /* Initialize codec context */
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return 0;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    /* Initialize encoder config */
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(NULL, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return 0;
    }

    /* Initialize encoder context */
    if (aom_codec_enc_init(ctx, aom_codec_av1_cx(), &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return 0;
    }

    /* Prepare dummy frame for encoding to ensure encoder is in valid state */
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 16, 16, 1);
    memset(img.planes[0], 128, img.d_w * img.d_h);
    memset(img.planes[1], 128, img.d_w * img.d_h / 4);
    memset(img.planes[2], 128, img.d_w * img.d_h / 4);

    /* Consume some input to determine which test to run */
    const uint8_t *ptr = Data;
    size_t rem = Size;
    int mode = read_int(&ptr, &rem);
    mode = mode % 7; /* 0-6 for different control commands */

    switch (mode) {
        case 0: {
            /* AV1E_SET_SVC_LAYER_ID */
            aom_svc_layer_id_t layer_id;
            layer_id.spatial_layer_id = read_int(&ptr, &rem) % 4;
            layer_id.temporal_layer_id = read_int(&ptr, &rem) % 4;
            aom_codec_control(ctx, AV1E_SET_SVC_LAYER_ID, &layer_id);
            break;
        }
        case 1: {
            /* AV1E_SET_SVC_PARAMS */
            aom_svc_params_t svc_params;
            memset(&svc_params, 0, sizeof(svc_params));
            svc_params.number_spatial_layers = (read_int(&ptr, &rem) % 3) + 1;
            svc_params.number_temporal_layers = (read_int(&ptr, &rem) % 3) + 1;
            for (int i = 0; i < AOM_MAX_LAYERS; i++) {
                svc_params.max_quantizers[i] = read_int(&ptr, &rem) % 255;
                svc_params.min_quantizers[i] = read_int(&ptr, &rem) % 255;
                svc_params.layer_target_bitrate[i] = read_int(&ptr, &rem) % 10000;
            }
            for (int i = 0; i < AOM_MAX_SS_LAYERS; i++) {
                svc_params.scaling_factor_num[i] = read_int(&ptr, &rem) % 100;
                svc_params.scaling_factor_den[i] = (read_int(&ptr, &rem) % 100) + 1;
            }
            for (int i = 0; i < AOM_MAX_TS_LAYERS; i++) {
                svc_params.framerate_factor[i] = read_int(&ptr, &rem) % 100;
            }
            aom_codec_control(ctx, AV1E_SET_SVC_PARAMS, &svc_params);
            break;
        }
        case 2: {
            /* AV1E_SET_ENABLE_INTRABC */
            int enable = read_int(&ptr, &rem) % 2;
            aom_codec_control(ctx, AV1E_SET_ENABLE_INTRABC, &enable);
            break;
        }
        case 3: {
            /* AV1E_SET_ENABLE_PAETH_INTRA */
            int enable = read_int(&ptr, &rem) % 2;
            aom_codec_control(ctx, AV1E_SET_ENABLE_PAETH_INTRA, &enable);
            break;
        }
        case 4: {
            /* AV1E_SET_ENABLE_AB_PARTITIONS */
            int enable = read_int(&ptr, &rem) % 2;
            aom_codec_control(ctx, AV1E_SET_ENABLE_AB_PARTITIONS, &enable);
            break;
        }
        case 5: {
            /* AV1E_SET_ENABLE_DIAGONAL_INTRA */
            int enable = read_int(&ptr, &rem) % 2;
            aom_codec_control(ctx, AV1E_SET_ENABLE_DIAGONAL_INTRA, &enable);
            break;
        }
        case 6: {
            /* Fuzz multiple controls in sequence */
            int val = read_int(&ptr, &rem);
            int enable = val % 2;
            aom_codec_control(ctx, AV1E_SET_ENABLE_INTRABC, &enable);
            aom_codec_control(ctx, AV1E_SET_ENABLE_PAETH_INTRA, &enable);
            aom_codec_control(ctx, AV1E_SET_ENABLE_AB_PARTITIONS, &enable);
            aom_codec_control(ctx, AV1E_SET_ENABLE_DIAGONAL_INTRA, &enable);
            
            aom_svc_layer_id_t layer_id;
            layer_id.spatial_layer_id = val % 4;
            layer_id.temporal_layer_id = val % 4;
            aom_codec_control(ctx, AV1E_SET_SVC_LAYER_ID, &layer_id);
            break;
        }
        default:
            break;
    }

    /* Encode a frame to ensure state consistency */
    aom_enc_frame_flags_t flags = 0;
    aom_codec_encode(ctx, &img, 0, 1, flags);

    /* Cleanup */
    aom_img_free(&img);
    cleanup_codec(ctx);

    return 0;
}