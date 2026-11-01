// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"
#include "aom/aomcx.h"
#include "aom/aom_image.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom.h"

// Helper to initialize encoder context
static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) return 0;
    
    // Use fuzz data to configure encoder parameters safely
    if (size > 0) {
        cfg->g_w = (data[0] % 1024) + 16;
        cfg->g_h = (data[1] % 1024) + 16;
        cfg->g_timebase.num = (data[2] % 100) + 1;
        cfg->g_timebase.den = (data[3] % 100) + 1;
        cfg->rc_max_quantizer = (data[4] % 64) + 1;
        cfg->rc_min_quantizer = (data[5] % 64) + 1;
        cfg->rc_target_bitrate = (data[6] % 10000) + 100;
        cfg->g_lag_in_frames = (data[7] % 25) + 1;
    }

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) return 0;
    return 1;
}

// Helper to create a dummy frame from fuzz data
static aom_image_t *create_frame(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0);
    
    // Ensure minimal dimensions
    int w = (size > 0 ? data[0] : 16) % 1024 + 16;
    int h = (size > 1 ? data[1] : 16) % 1024 + 16;
    
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, w, h, 1);
    if (!img) return NULL;

    // Fill with fuzz data
    size_t plane_size = img->d_w * img->d_h;
    if (size > 0) {
        memcpy(img->planes[0], data, plane_size < size ? plane_size : size);
    }
    
    return img;
}

int LLVMFuzzerTestOneInput_200(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (!init_encoder(&ctx, &cfg, Data, Size)) {
        return 0;
    }

    // Create a frame to encode
    aom_image_t *img = create_frame(&ctx, Data, Size);
    if (!img) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Prepare frame flags
    aom_enc_frame_flags_t enc_flags = 0;
    if (Size > 10) {
        if (Data[10] & 1) {
            enc_flags = AOM_EFLAG_FORCE_KF;
        }
    }

    // Attempt to encode (5 arguments: ctx, img, pts, duration, flags)
    aom_codec_err_t ret = aom_codec_encode(&ctx, img, 0, 1, enc_flags);
    if (ret != AOM_CODEC_OK) {
        aom_img_free(img);
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Try to get global headers
    aom_fixed_buf_t *headers = aom_codec_get_global_headers(&ctx);
    if (headers) {
        // Headers are malloc'd, we should free them after use
        if (headers->buf) {
            free(headers->buf);
        }
        free(headers);
    }

    // Attempt to invoke control functions
    // 1. AV1E_SET_MATRIX_COEFFICIENTS
    int matrix_coeff = Size > 13 ? Data[13] : 0;
    aom_codec_control(&ctx, AV1E_SET_MATRIX_COEFFICIENTS, matrix_coeff);

    // 2. AV1E_SET_PARTITION_INFO_PATH
    const char *dummy_path = "./dummy_file";
    aom_codec_control(&ctx, AV1E_SET_PARTITION_INFO_PATH, dummy_path);

    // 3. AV1E_SET_ENABLE_FLIP_IDTX
    int flip_idtx = Size > 14 ? Data[14] : 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, flip_idtx);

    // 4. AV1E_SET_FILM_GRAIN_TABLE
    if (Size > 15) {
        aom_codec_control(&ctx, AV1E_SET_FILM_GRAIN_TABLE, (void*)&Data[15]);
    }

    // 5. AV1E_SET_INTER_DCT_ONLY
    int inter_dct_only = Size > 16 ? Data[16] : 0;
    aom_codec_control(&ctx, AV1E_SET_INTER_DCT_ONLY, inter_dct_only);

    // Cleanup
    aom_img_free(img);
    aom_codec_destroy(&ctx);

    return 0;
}