// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aom.h"
#include "aom/aom_image.h"

// Declare the AV1 encoder interface function since it might not be in headers
extern aom_codec_iface_t *aom_codec_av1_cx(void);

static void init_encoder_config(aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    // Initialize with reasonable defaults
    cfg->g_usage = 0;
    cfg->g_threads = 1;
    cfg->g_profile = 0;
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_limit = 30;
    cfg->g_forced_max_frame_width = 0;
    cfg->g_forced_max_frame_height = 0;
    cfg->g_bit_depth = AOM_BITS_8;
    cfg->g_input_bit_depth = 8;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_error_resilient = 0;
    cfg->g_pass = AOM_RC_ONE_PASS;
    cfg->g_lag_in_frames = 0;
    
    // Rate control settings
    cfg->rc_dropframe_thresh = 0;
    cfg->rc_resize_mode = 0;
    cfg->rc_resize_denominator = 8;
    cfg->rc_resize_kf_denominator = 8;
    cfg->rc_superres_mode = AOM_SUPERRES_NONE;
    cfg->rc_superres_denominator = 8;
    cfg->rc_superres_kf_denominator = 8;
    cfg->rc_superres_qthresh = 0;
    cfg->rc_superres_kf_qthresh = 0;
    cfg->rc_end_usage = AOM_VBR;
    cfg->rc_target_bitrate = 256;
    cfg->rc_min_quantizer = 2;
    cfg->rc_max_quantizer = 58;
    cfg->rc_undershoot_pct = 50;
    cfg->rc_overshoot_pct = 50;
    cfg->rc_buf_sz = 600;
    cfg->rc_buf_initial_sz = 400;
    cfg->rc_buf_optimal_sz = 500;
    cfg->rc_2pass_vbr_bias_pct = 50;
    cfg->rc_2pass_vbr_minsection_pct = 0;
    cfg->rc_2pass_vbr_maxsection_pct = 2000;
    
    // Keyframe settings
    cfg->fwd_kf_enabled = 0;
    cfg->kf_mode = AOM_KF_AUTO;
    cfg->kf_min_dist = 0;
    cfg->kf_max_dist = 9999;
    cfg->sframe_dist = 0;
    cfg->sframe_mode = 0;
    cfg->large_scale_tile = 0;
    cfg->monochrome = 0;
    cfg->full_still_picture_hdr = 0;
    cfg->save_as_annexb = 0;
    cfg->tile_width_count = 0;
    cfg->tile_height_count = 0;
    cfg->use_fixed_qp_offsets = 0;
    
    // Use fuzzer data to modify some parameters if we have enough data
    if (size >= 8) {
        cfg->g_w = 16 + (data[0] % 128);
        cfg->g_h = 16 + (data[1] % 128);
        cfg->rc_target_bitrate = 100 + (data[2] * 10);
        cfg->rc_min_quantizer = data[3] % 64;
        cfg->rc_max_quantizer = 2 + (data[4] % 62);
        if (cfg->rc_max_quantizer < cfg->rc_min_quantizer) {
            cfg->rc_max_quantizer = cfg->rc_min_quantizer;
        }
        cfg->g_threads = 1 + (data[5] % 4);
        cfg->g_profile = data[6] % 4;
        cfg->g_lag_in_frames = data[7] % 10;
    }
}

static aom_image_t *create_test_image(const uint8_t *data, size_t size, unsigned int width, unsigned int height) {
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, width, height, 1);
    if (!img) return NULL;
    
    // Fill image planes with fuzzer data
    size_t y_plane_size = width * height;
    size_t uv_plane_size = (width / 2) * (height / 2);
    
    if (size > 0) {
        // Fill Y plane
        size_t y_bytes = y_plane_size < size ? y_plane_size : size;
        memcpy(img->planes[AOM_PLANE_Y], data, y_bytes);
        
        // Fill U plane if we have more data
        if (size > y_bytes) {
            size_t u_bytes = uv_plane_size < (size - y_bytes) ? uv_plane_size : (size - y_bytes);
            memcpy(img->planes[AOM_PLANE_U], data + y_bytes, u_bytes);
        }
        
        // Fill V plane if we have even more data
        if (size > y_bytes + uv_plane_size) {
            size_t v_bytes = uv_plane_size < (size - y_bytes - uv_plane_size) ? uv_plane_size : (size - y_bytes - uv_plane_size);
            memcpy(img->planes[AOM_PLANE_V], data + y_bytes + uv_plane_size, v_bytes);
        }
    }
    
    return img;
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    
    if (!iface) return 0;
    
    // Initialize encoder configuration
    init_encoder_config(&cfg, Data, Size);
    
    // Initialize encoder
    aom_codec_err_t res = aom_codec_enc_init_ver(&codec, iface, &cfg, 0, AOM_ENCODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        return 0;
    }
    
    // Try to reconfigure with slightly modified settings
    if (Size >= 12) {
        cfg.rc_target_bitrate = 50 + (Data[8] * 20);
        cfg.g_w = 32 + (Data[9] % 96);
        cfg.g_h = 32 + (Data[10] % 96);
        aom_codec_enc_config_set(&codec, &cfg);
    }
    
    // Try some codec controls using fuzzer data
    if (Size >= 16) {
        int ctrl_id = 1 + (Data[11] % 100);  // Various control IDs
        int ctrl_value = Data[12];
        aom_codec_control(&codec, ctrl_id, ctrl_value);
        
        // Try another control
        ctrl_id = 100 + (Data[13] % 100);
        ctrl_value = Data[14];
        aom_codec_control(&codec, ctrl_id, ctrl_value);
    }
    
    // Create and encode a test image
    aom_image_t *img = create_test_image(Data, Size, cfg.g_w, cfg.g_h);
    if (img) {
        // Encode the image
        aom_codec_encode(&codec, img, 0, 1, 0);
        
        // Flush the encoder
        aom_codec_encode(&codec, NULL, 0, 1, 0);
        
        // Get encoded data
        aom_codec_iter_t iter = NULL;
        const aom_codec_cx_pkt_t *pkt;
        while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != NULL) {
            // Process packet if needed
        }
        
        // Try setting an output buffer
        if (Size >= 20) {
            aom_fixed_buf_t buf;
            unsigned char buffer[1024];
            buf.buf = buffer;
            buf.sz = sizeof(buffer);
            unsigned int pad_before = Data[15] % 32;
            unsigned int pad_after = Data[16] % 32;
            aom_codec_set_cx_data_buf(&codec, &buf, pad_before, pad_after);
            
            // Encode another frame with buffer set
            aom_codec_encode(&codec, img, 1, 1, 0);
            
            // Get data again
            iter = NULL;
            while ((pkt = aom_codec_get_cx_data(&codec, &iter)) != NULL) {
                // Process packet
            }
        }
        
        aom_img_free(img);
    }
    
    // Clean up
    aom_codec_destroy(&codec);
    
    return 0;
}