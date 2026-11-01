// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static void init_config_from_fuzz(aom_codec_enc_cfg_t *cfg, const uint8_t *Data, size_t Size) {
    memset(cfg, 0, sizeof(aom_codec_enc_cfg_t));
    
    if (Size > 0) cfg->g_w = Data[0] + 1;
    if (Size > 1) cfg->g_h = Data[1] + 1;
    if (Size > 2) cfg->g_bit_depth = (Data[2] % 3 == 0) ? AOM_BITS_8 : ((Data[2] % 3 == 1) ? AOM_BITS_10 : AOM_BITS_12);
    if (Size > 3) cfg->g_input_bit_depth = cfg->g_bit_depth;
    
    cfg->g_timebase.num = (Size > 4) ? (Data[4] + 1) : 1;
    cfg->g_timebase.den = (Size > 5) ? (Data[5] + 1) : 1;
    
    if (Size > 6) cfg->g_threads = Data[6] % 4;
    if (Size > 7) cfg->rc_target_bitrate = (Data[7] * 100) + 100;
    if (Size > 8) cfg->rc_min_quantizer = Data[8] % 64;
    if (Size > 9) cfg->rc_max_quantizer = (Data[9] % 64) + cfg->rc_min_quantizer;
    
    if (Size > 10) cfg->kf_min_dist = Data[10] + 1;
    if (Size > 11) cfg->kf_max_dist = (Data[11] + 1) * 10;
    
    cfg->g_usage = 0;
    cfg->g_profile = 0;
    cfg->g_limit = 0;
    cfg->rc_end_usage = AOM_VBR;
    cfg->g_pass = AOM_RC_ONE_PASS;
    cfg->g_error_resilient = 0;
    cfg->g_lag_in_frames = 0;
}

int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_image_t img;
    aom_codec_err_t err;
    const char *err_str;
    const char *err_detail;
    
    // Initialize config from fuzz data
    init_config_from_fuzz(&cfg, Data, Size);

    // Get AV1 encoder interface using the correct function name
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize encoder context
    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        // Test error functions on failed init
        aom_codec_error(&ctx);
        aom_codec_error_detail(&ctx);
        aom_codec_err_to_string(err);
        return 0;
    }

    // Test aom_codec_set_option with fuzzed strings
    char key[64], val[64];
    size_t key_len = (Size > 12) ? Data[12] % 32 : 10;
    size_t val_len = (Size > 13) ? Data[13] % 32 : 10;
    
    if (key_len > 0 && val_len > 0) {
        memset(key, 'k', key_len);
        memset(val, 'v', val_len);
        key[key_len] = '\0';
        val[val_len] = '\0';
        aom_codec_set_option(&ctx, key, val);
    }
    
    // Test aom_codec_enc_config_set with potentially modified config
    if (Size > 14) {
        cfg.g_threads = Data[14];
        aom_codec_enc_config_set(&ctx, &cfg);
    }

    // Prepare image data
    memset(&img, 0, sizeof(img));
    img.fmt = AOM_IMG_FMT_I420;
    img.w = cfg.g_w;
    img.h = cfg.g_h;
    img.bit_depth = cfg.g_bit_depth;
    img.d_w = img.w;
    img.d_h = img.h;
    img.r_w = img.w;
    img.r_h = img.h;
    img.planes[0] = NULL;
    img.planes[1] = NULL;
    img.planes[2] = NULL;
    img.stride[0] = 0;
    img.stride[1] = 0;
    img.stride[2] = 0;

    // Allocate planes if size permits
    if (img.w > 0 && img.h > 0) {
        size_t plane_sz = (size_t)img.w * (size_t)img.h;
        size_t uv_sz = (plane_sz / 4) + 1;
        
        img.planes[0] = malloc(plane_sz);
        img.planes[1] = malloc(uv_sz);
        img.planes[2] = malloc(uv_sz);
        
        if (img.planes[0] && img.planes[1] && img.planes[2]) {
            img.stride[0] = (int)img.w;
            img.stride[1] = (int)(img.w / 2);
            img.stride[2] = (int)(img.w / 2);
            
            // Fill with fuzz data
            if (Size > 15) {
                uint8_t fill_val = Data[15];
                memset(img.planes[0], fill_val, plane_sz);
                memset(img.planes[1], fill_val, uv_sz);
                memset(img.planes[2], fill_val, uv_sz);
            }
        } else {
            // Cleanup partial allocation
            free(img.planes[0]);
            free(img.planes[1]);
            free(img.planes[2]);
            img.planes[0] = img.planes[1] = img.planes[2] = NULL;
        }
    }

    // Test encoding loop
    aom_codec_pts_t pts = 0;
    unsigned long duration = (Size > 16) ? (Data[16] + 1) : 1;
    if (duration > UINT32_MAX) duration = UINT32_MAX;
    
    // Encode a few frames or flush
    int encode_count = 0;
    int max_frames = (Size > 17) ? (Data[17] % 5) + 1 : 3;
    
    while (encode_count < max_frames) {
        const aom_image_t *input_img = (img.planes[0]) ? &img : NULL;
        
        err = aom_codec_encode(&ctx, input_img, pts, duration, 0);
        
        // Check error states
        if (err != AOM_CODEC_OK) {
            err_str = aom_codec_error(&ctx);
            err_detail = aom_codec_error_detail(&ctx);
            aom_codec_err_to_string(err);
        }
        
        // Consume output data to prevent memory leak in internal buffers
        aom_codec_cx_pkt_t *pkt;
        while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
            // No destroy function needed; data is managed by the context
            // Just consume it
            (void)pkt;
        }

        if (input_img == NULL) break; // Flushing
        pts += duration;
        encode_count++;
    }

    // Flush the encoder
    while (1) {
        err = aom_codec_encode(&ctx, NULL, 0, 0, 0);
        if (err != AOM_CODEC_OK) {
            aom_codec_error(&ctx);
            aom_codec_error_detail(&ctx);
            aom_codec_err_to_string(err);
        }
        
        aom_codec_cx_pkt_t *pkt;
        int has_data = 0;
        while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
            has_data = 1;
            // No destroy function needed
            (void)pkt;
        }
        
        if (!has_data) break;
    }

    // Cleanup image
    free(img.planes[0]);
    free(img.planes[1]);
    free(img.planes[2]);

    // Cleanup context
    aom_codec_destroy(&ctx);

    // Test error functions with null context
    aom_codec_error(NULL);
    aom_codec_error_detail(NULL);
    aom_codec_err_to_string(AOM_CODEC_INVALID_PARAM);
    aom_codec_err_to_string(AOM_CODEC_MEM_ERROR);
    aom_codec_err_to_string(AOM_CODEC_ERROR);

    return 0;
}