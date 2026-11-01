// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
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

#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static int initialize_encoder(aom_codec_ctx_t **ctx) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t *codec_ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!codec_ctx) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(codec_ctx);
        return 0;
    }

    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_bit_depth = 8;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    if (aom_codec_enc_init(codec_ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(codec_ctx);
        return 0;
    }

    *ctx = codec_ctx;
    return 1;
}

int LLVMFuzzerTestOneInput_141(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t *ctx = NULL;
    
    if (!initialize_encoder(&ctx)) {
        return 0;
    }

    int cmd_idx = Data[0] % 6;
    int value = (Data[1] % 2); 

    aom_codec_err_t err;
    int32_t control_val = value;
    
    // For CHROMA_SAMPLE_POSITION, valid values are 0, 1, 2.
    if (cmd_idx == 4) {
        control_val = (Data[1] % 3);
    }

    switch (cmd_idx) {
        case 0: // AV1E_SET_ENABLE_RECT_TX
            err = aom_codec_control(ctx, AV1E_SET_ENABLE_RECT_TX, control_val);
            break;
        case 1: // AV1E_SET_ENABLE_CFL_INTRA
            err = aom_codec_control(ctx, AV1E_SET_ENABLE_CFL_INTRA, control_val);
            break;
        case 2: // AV1E_SET_ENABLE_INTRABC
            err = aom_codec_control(ctx, AV1E_SET_ENABLE_INTRABC, control_val);
            break;
        case 3: // AV1E_SET_ENABLE_AB_PARTITIONS
            err = aom_codec_control(ctx, AV1E_SET_ENABLE_AB_PARTITIONS, control_val);
            break;
        case 4: // AV1E_SET_CHROMA_SAMPLE_POSITION
            err = aom_codec_control(ctx, AV1E_SET_CHROMA_SAMPLE_POSITION, control_val);
            break;
        case 5: // AV1E_SET_ENABLE_RECT_PARTITIONS
            err = aom_codec_control(ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, control_val);
            break;
        default:
            err = AOM_CODEC_OK;
            break;
    }

    // Optional: Try to encode a frame to ensure the state change didn't break the encoder
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 32, 32, 1);
    
    // Fill with random data from input if available
    if (Size > 100) {
        memcpy(img.planes[0], Data + 1, 32 * 32);
        memcpy(img.planes[1], Data + 1, 16 * 16);
        memcpy(img.planes[2], Data + 1, 16 * 16);
    }

    // Use standard common controls if needed, or skip to avoid undefined enums
    // aom_codec_control(ctx, AOME_SET_CPUUSED, 0); 

    aom_codec_encode(ctx, &img, 0, 1, 0);

    aom_img_free(&img);

    cleanup_codec(ctx);

    return 0;
}