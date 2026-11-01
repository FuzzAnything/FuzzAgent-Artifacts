// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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

int LLVMFuzzerTestOneInput_261(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t enc_ctx;
    aom_codec_ctx_t dec_ctx;
    aom_codec_err_t res;
    aom_codec_iface_t *enc_iface = NULL;
    aom_codec_iface_t *dec_iface = NULL;
    aom_codec_enc_cfg_t cfg;
    aom_codec_dec_cfg_t dec_cfg;
    aom_image_t *img = NULL;
    aom_codec_cx_pkt_t *pkt;
    aom_codec_iter_t iter = NULL;
    
    // Initialize Encoder
    enc_iface = aom_codec_av1_cx();
    if (enc_iface == NULL) return 0;
    
    res = aom_codec_enc_config_default(enc_iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }
    
    // Use input data to configure encoder settings
    if (Size > 0) cfg.g_w = (Data[0] % 128) + 16;
    if (Size > 1) cfg.g_h = (Data[1] % 128) + 16;
    if (Size > 2) cfg.g_timebase.num = (Data[2] % 255) + 1;
    if (Size > 3) cfg.g_timebase.den = (Data[3] % 255) + 1;
    
    res = aom_codec_enc_init(&enc_ctx, enc_iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }
    
    // Initialize Decoder
    dec_iface = aom_codec_av1_dx();
    if (dec_iface == NULL) {
        aom_codec_destroy(&enc_ctx);
        return 0;
    }
    
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    res = aom_codec_dec_init(&dec_ctx, dec_iface, &dec_cfg, 0);
    if (res != AOM_CODEC_OK) {
        aom_codec_destroy(&enc_ctx);
        return 0;
    }

    // Prepare dummy image for encoding
    img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    if (img == NULL) {
        aom_codec_destroy(&enc_ctx);
        aom_codec_destroy(&dec_ctx);
        return 0;
    }
    
    // Fill image with fuzz data
    size_t img_size = img->d_w * img->d_h * 3 / 2;
    if (Size > img_size) Size = img_size;
    memcpy(img->planes[0], Data, Size);
    // Fill chroma planes with pattern
    memset(img->planes[1], 0, img->d_w * img->d_h / 4);
    memset(img->planes[2], 0, img->d_w * img->d_h / 4);

    // Encode a frame
    aom_codec_control(&enc_ctx, AOME_SET_CPUUSED, 0);
    
    res = aom_codec_encode(&enc_ctx, img, 0, 1, 0);
    if (res != AOM_CODEC_OK) {
        aom_img_free(img);
        aom_codec_destroy(&enc_ctx);
        aom_codec_destroy(&dec_ctx);
        return 0;
    }
    
    // Try to get packets
    iter = NULL;
    while ((pkt = aom_codec_get_cx_data(&enc_ctx, &iter)) != NULL) {
        // Process packet
        aom_codec_destroy(&enc_ctx); // Cleanup after use
        break;
    }
    
    // Attempt to call the specific control functions
    int val_int = 0;
    int64_t val_int64 = 0;
    aom_tile_info tile_info;
    
    // AOME_GET_LAST_QUANTIZER
    res = aom_codec_control(&enc_ctx, AOME_GET_LAST_QUANTIZER, &val_int);
    (void)res;
    
    // AOME_GET_LAST_QUANTIZER_64
    res = aom_codec_control(&enc_ctx, AOME_GET_LAST_QUANTIZER_64, &val_int64);
    (void)res;
    
    // AV1E_GET_LUMA_CDEF_STRENGTH requires an array of int, not a single int.
    // CDEF_MAX_STRENGTHS is typically 8.
    int cdef_strengths[8];
    res = aom_codec_control(&enc_ctx, AV1E_GET_LUMA_CDEF_STRENGTH, cdef_strengths);
    (void)res;
    
    // Try Decoder Controls
    // AOMD_GET_BASE_Q_IDX
    res = aom_codec_control(&dec_ctx, AOMD_GET_BASE_Q_IDX, &val_int);
    (void)res;
    
    // AOMD_GET_TILE_INFO
    res = aom_codec_control(&dec_ctx, AOMD_GET_TILE_INFO, &tile_info);
    (void)res;
    
    // AOMD_GET_LAST_QUANTIZER
    res = aom_codec_control(&dec_ctx, AOMD_GET_LAST_QUANTIZER, &val_int);
    (void)res;

    // Cleanup
    aom_img_free(img);
    aom_codec_destroy(&enc_ctx);
    aom_codec_destroy(&dec_ctx);
    
    return 0;
}