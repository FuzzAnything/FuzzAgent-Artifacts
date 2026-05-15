#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include <algorithm>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "vpx/vp8cx.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_image.h"

namespace harness_002 {
/*
 * Fuzzing harness for libvpx multi-resolution encoding and SVC (Scalable Video Coding)
 * Targets vpx_codec_enc_init_multi_ver, SVC layer configuration, and multi-resolution encoding
 */




// Minimum input size for meaningful testing
const size_t MIN_INPUT_SIZE = 128;

// Maximum number of encoders for multi-resolution encoding (as per API limits)
const int MAX_NUM_ENCODERS = 16;

// Helper function to allocate and fill image with fuzzed data for multi-resolution encoding
static int fill_multi_res_image(vpx_image_t *img, FuzzedDataProvider *fdp, int downsample_factor_num, int downsample_factor_den) {
    if (img == nullptr || fdp == nullptr || downsample_factor_num <= 0 || downsample_factor_den <= 0) return 0;
    
    // For simplicity, we only handle 8-bit I420 format
    if (img->bit_depth != 8 || img->fmt != VPX_IMG_FMT_I420) {
        return 0;
    }
    
    // Calculate scaled dimensions based on downsampling factor
    int scaled_w = img->d_w * downsample_factor_den / downsample_factor_num;
    int scaled_h = img->d_h * downsample_factor_den / downsample_factor_num;
    
    // Ensure dimensions are valid
    if (scaled_w <= 0 || scaled_h <= 0 || scaled_w > img->d_w || scaled_h > img->d_h) {
        return 0;
    }
    
    size_t total_bytes = 0;
    for (int plane = 0; plane < 3; ++plane) {
        unsigned char *buf = img->planes[plane];
        const int stride = img->stride[plane];
        int w = (plane == 0) ? scaled_w : (scaled_w + 1) / 2;
        int h = (plane == 0) ? scaled_h : (scaled_h + 1) / 2;
        
        for (int y = 0; y < h; ++y) {
            if (fdp->remaining_bytes() < (size_t)w) {
                return total_bytes;
            }
            std::vector<uint8_t> row_data = fdp->ConsumeBytes<uint8_t>(w);
            memcpy(buf, row_data.data(), w);
            buf += stride;
            total_bytes += w;
        }
    }
    return total_bytes;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size < MIN_INPUT_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters from fuzzer input
    uint8_t codec_choice = fdp.ConsumeIntegral<uint8_t>() % 2;  // 0=VP8, 1=VP9
    uint8_t width_idx = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t height_idx = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t num_encoders = fdp.ConsumeIntegralInRange<uint8_t>(1, 4);  // Limit to 4 for performance
    uint8_t encoding_mode = fdp.ConsumeIntegral<uint8_t>() % 3;  // 0=multi-res, 1=SVC, 2=both
    uint8_t spatial_layers = fdp.ConsumeIntegralInRange<uint8_t>(1, 5);  // VPX_SS_MAX_LAYERS = 5
    uint8_t temporal_layers = fdp.ConsumeIntegralInRange<uint8_t>(1, 5);  // VPX_TS_MAX_LAYERS = 5
    
    // Map width/height indices to actual dimensions
    unsigned int widths[] = {64, 128, 256, 512};
    unsigned int heights[] = {48, 96, 192, 384};
    unsigned int width = widths[width_idx % 4];
    unsigned int height = heights[height_idx % 4];
    
    // Choose codec interface based on fuzzer input
    const vpx_codec_iface_t *encoder_iface = nullptr;
    
    if (codec_choice == 0) {
        // VP8
        encoder_iface = vpx_codec_vp8_cx();
    } else {
        // VP9
        encoder_iface = vpx_codec_vp9_cx();
    }
    
    if (!encoder_iface) {
        return 0;
    }
    
    // Create encoder configurations
    std::vector<vpx_codec_enc_cfg_t> cfgs(num_encoders);
    std::vector<vpx_rational_t> dsf(num_encoders);
    std::vector<vpx_codec_ctx_t> ctx_array(num_encoders);
    
    // Initialize default configuration for the first encoder
    if (vpx_codec_enc_config_default(encoder_iface, &cfgs[0], 0) != VPX_CODEC_OK) {
        return 0;
    }
    
    // Set base configuration parameters
    cfgs[0].g_w = width;
    cfgs[0].g_h = height;
    cfgs[0].g_timebase.num = 1;
    cfgs[0].g_timebase.den = 30;
    cfgs[0].rc_target_bitrate = 200;
    cfgs[0].g_error_resilient = 1;
    
    // Configure downsampling factors for multi-resolution encoding
    for (int i = 0; i < num_encoders; ++i) {
        if (i > 0) {
            // Copy base config for other encoders
            cfgs[i] = cfgs[0];
            
            // Set different downsampling factors
            uint8_t dsf_num = fdp.ConsumeIntegralInRange<uint8_t>(1, 4);
            uint8_t dsf_den = fdp.ConsumeIntegralInRange<uint8_t>(dsf_num, 8);  // Ensure den >= num
            
            dsf[i].num = dsf_num;
            dsf[i].den = dsf_den;
            
            // Adjust dimensions for this encoder
            cfgs[i].g_w = width * dsf[i].den / dsf[i].num;
            cfgs[i].g_h = height * dsf[i].den / dsf[i].num;
            
            // Ensure dimensions are valid
            if (cfgs[i].g_w < 16) cfgs[i].g_w = 16;
            if (cfgs[i].g_h < 16) cfgs[i].g_h = 16;
        } else {
            // Base layer (full resolution)
            dsf[i].num = 1;
            dsf[i].den = 1;
        }
    }
    
    vpx_codec_flags_t flags = 0;
    
    if (encoding_mode == 0) {
        // Test multi-resolution encoding with vpx_codec_enc_init_multi_ver
        // Note: According to documentation, this is only supported by VP8
        if (codec_choice == 0) {  // VP8
            vpx_codec_err_t init_result = vpx_codec_enc_init_multi_ver(
                ctx_array.data(), 
                const_cast<vpx_codec_iface_t*>(encoder_iface),
                cfgs.data(),
                num_encoders,
                flags,
                dsf.data(),
                VPX_ENCODER_ABI_VERSION
            );
            
            if (init_result == VPX_CODEC_OK) {
                // Create and fill images for each encoder
                for (int i = 0; i < num_encoders; ++i) {
                    vpx_image_t *img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 
                                                     cfgs[i].g_w, cfgs[i].g_h, 1);
                    if (img) {
                        // Fill image with fuzzed data using appropriate downsampling
                        fill_multi_res_image(img, &fdp, dsf[i].num, dsf[i].den);
                        
                        // Encode a frame
                        vpx_codec_encode(&ctx_array[i], img, 0, 1, 0, VPX_DL_GOOD_QUALITY);
                        
                        // Get encoded data
                        vpx_codec_iter_t iter = nullptr;
                        const vpx_codec_cx_pkt_t *pkt;
                        while ((pkt = vpx_codec_get_cx_data(&ctx_array[i], &iter)) != nullptr) {
                            // Process packet if needed
                            (void)pkt;
                        }
                        
                        vpx_img_free(img);
                    }
                }
                
                // Destroy all encoders
                for (int i = 0; i < num_encoders; ++i) {
                    vpx_codec_destroy(&ctx_array[i]);
                }
            }
        }
    } else if (encoding_mode == 1 && codec_choice == 1) {
        // Test VP9 SVC encoding (VP9 only)
        // Initialize single encoder with SVC configuration
        vpx_codec_ctx_t codec;
        
        if (vpx_codec_enc_init(&codec, encoder_iface, &cfgs[0], flags) == VPX_CODEC_OK) {
            // Set SVC parameters
            vpx_svc_layer_id_t layer_id;
            memset(&layer_id, 0, sizeof(layer_id));
            
            // Configure spatial and temporal layers
            layer_id.spatial_layer_id = fdp.ConsumeIntegralInRange<int>(0, spatial_layers - 1);
            
            for (int i = 0; i < spatial_layers; ++i) {
                layer_id.temporal_layer_id_per_spatial[i] = fdp.ConsumeIntegralInRange<int>(0, temporal_layers - 1);
            }
            
            vpx_codec_control(&codec, VP9E_SET_SVC_LAYER_ID, &layer_id);
            
            // Set SVC reference frame configuration
            vpx_svc_ref_frame_config_t ref_config;
            memset(&ref_config, 0, sizeof(ref_config));
            
            for (int i = 0; i < spatial_layers; ++i) {
                ref_config.lst_fb_idx[i] = fdp.ConsumeIntegralInRange<int>(0, 7);
                ref_config.gld_fb_idx[i] = fdp.ConsumeIntegralInRange<int>(0, 7);
                ref_config.alt_fb_idx[i] = fdp.ConsumeIntegralInRange<int>(0, 7);
                ref_config.update_buffer_slot[i] = fdp.ConsumeIntegral<uint8_t>();
                ref_config.reference_last[i] = fdp.ConsumeBool();
                ref_config.reference_golden[i] = fdp.ConsumeBool();
                ref_config.reference_alt_ref[i] = fdp.ConsumeBool();
            }
            
            vpx_codec_control(&codec, VP9E_SET_SVC_REF_FRAME_CONFIG, &ref_config);
            
            // Create and fill image
            vpx_image_t *img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, width, height, 1);
            if (img) {
                // Fill image planes
                for (int plane = 0; plane < 3; ++plane) {
                    unsigned char *buf = img->planes[plane];
                    const int stride = img->stride[plane];
                    int w = (plane == 0) ? width : (width + 1) / 2;
                    int h = (plane == 0) ? height : (height + 1) / 2;
                    
                    for (int y = 0; y < h; ++y) {
                        if (fdp.remaining_bytes() < (size_t)w) {
                            break;
                        }
                        std::vector<uint8_t> row_data = fdp.ConsumeBytes<uint8_t>(w);
                        memcpy(buf, row_data.data(), w);
                        buf += stride;
                    }
                }
                
                // Encode frame with SVC
                vpx_codec_encode(&codec, img, 0, 1, 0, VPX_DL_GOOD_QUALITY);
                
                // Get encoded data
                vpx_codec_iter_t iter = nullptr;
                const vpx_codec_cx_pkt_t *pkt;
                while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != nullptr) {
                    // Process packet if needed
                    (void)pkt;
                }
                
                vpx_img_free(img);
            }
            
            vpx_codec_destroy(&codec);
        }
    } else if (encoding_mode == 2 && codec_choice == 1) {
        // Test combination: initialize encoder and set some SVC controls
        vpx_codec_ctx_t codec;
        
        if (vpx_codec_enc_init(&codec, encoder_iface, &cfgs[0], flags) == VPX_CODEC_OK) {
            // Test various SVC-related controls
            vpx_svc_layer_id_t layer_id;
            memset(&layer_id, 0, sizeof(layer_id));
            layer_id.spatial_layer_id = fdp.ConsumeIntegralInRange<int>(0, 2);
            
            vpx_codec_control(&codec, VP9E_SET_SVC_LAYER_ID, &layer_id);
            
            // Test getting layer ID back
            vpx_svc_layer_id_t get_layer_id;
            memset(&get_layer_id, 0, sizeof(get_layer_id));
            vpx_codec_control(&codec, VP9E_GET_SVC_LAYER_ID, &get_layer_id);
            
            // Test frame dropping configuration
            vpx_svc_frame_drop_t frame_drop;
            memset(&frame_drop, 0, sizeof(frame_drop));
            frame_drop.framedrop_mode = fdp.ConsumeBool() ? CONSTRAINED_LAYER_DROP : LAYER_DROP;
            frame_drop.max_consec_drop = fdp.ConsumeIntegralInRange<int>(0, 10);
            
            for (int i = 0; i < spatial_layers; ++i) {
                frame_drop.framedrop_thresh[i] = fdp.ConsumeIntegralInRange<int>(0, 100);
            }
            
            vpx_codec_control(&codec, VP9E_SET_SVC_FRAME_DROP_LAYER, &frame_drop);
            
            // Test spatial layer sync
            vpx_svc_spatial_layer_sync_t spatial_sync;
            memset(&spatial_sync, 0, sizeof(spatial_sync));
            spatial_sync.base_layer_intra_only = fdp.ConsumeBool();
            
            for (int i = 0; i < spatial_layers; ++i) {
                spatial_sync.spatial_layer_sync[i] = fdp.ConsumeBool();
            }
            
            vpx_codec_control(&codec, VP9E_SET_SVC_SPATIAL_LAYER_SYNC, &spatial_sync);
            
            vpx_codec_destroy(&codec);
        }
    }
    
    // Additional test: Try to get global headers (if supported)
    if (fdp.remaining_bytes() > 32 && codec_choice == 1) {
        vpx_codec_ctx_t codec;
        
        if (vpx_codec_enc_init(&codec, encoder_iface, &cfgs[0], flags) == VPX_CODEC_OK) {
            vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&codec);
            (void)global_headers;  // Mark as used
            
            vpx_codec_destroy(&codec);
        }
    }
    
    return 0;
}

} // namespace harness_002
