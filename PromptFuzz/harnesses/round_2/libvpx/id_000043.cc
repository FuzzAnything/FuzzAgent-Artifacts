#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <vpx/vp8cx.h>
}
extern "C" {
#include <vpx/vp8dx.h>
}
extern "C" {
#include <vpx/vpx_decoder.h>
}
//<ID> 183
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp9_cx","vpx_codec_get_caps","vpx_img_alloc","vpx_img_set_rect","vpx_codec_enc_config_default","vpx_img_free","vpx_codec_enc_init_multi_ver","vpx_codec_error","vpx_img_free","vpx_codec_register_put_slice_cb","vpx_codec_register_put_frame_cb","vpx_codec_enc_config_set","vpx_codec_error","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_vp9_cx","vpx_codec_get_caps","vpx_img_alloc","vpx_img_set_rect","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_register_put_slice_cb","vpx_codec_register_put_frame_cb","vpx_codec_enc_config_set","vpx_codec_error","vpx_codec_encode","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free"],"visited":0}
/*Looking at the provided APIs and the requirement to process libvpx output data, I'll create a fuzzer that:
1. Parses encoded video stream information
2. Initializes a multi-encoder configuration
3. Sets up image processing with callbacks
4. Uses the input data as encoded video frames

*/


extern "C" {
}

// Custom callback structures
typedef struct {
    unsigned int frame_count;
    unsigned int slice_count;
} callback_context_t;

// Dummy callbacks for slice and frame notifications
static void  id_000043_put_slice_callback(void *user_priv, 
                               const vpx_image_t *img,
                               const vpx_image_rect_t *slice_rect,
                               const vpx_image_rect_t *preview_rect) {
    callback_context_t *ctx = (callback_context_t *)user_priv;
    if (ctx) {
        ctx->slice_count++;
    }
    // Do nothing - just a dummy callback
}

static void  id_000043_put_frame_callback(void *user_priv, 
                               const vpx_image_t *img) {
    callback_context_t *ctx = (callback_context_t *)user_priv;
    if (ctx) {
        ctx->frame_count++;
    }
    // Do nothing - just a dummy callback
}

extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit for insufficient data
    if (size < 10) {
	return 0;
	}
	
    
    FILE *in_file = nullptr;
    vpx_codec_ctx_t codec_ctx = {0};
    vpx_codec_enc_cfg_t enc_cfg = {0};
    vpx_image_t *image = nullptr;
    vpx_codec_stream_info_t stream_info = {0};
    callback_context_t cb_ctx = {0};
    vpx_fixed_buf_t two_pass_stats = {0};
    uint8_t *config_data = nullptr;
    
    // Use fmemopen to read input data
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 1. First, peek at the stream information from the input data
    // Use both VP8 and VP9 codecs to check compatibility
    vpx_codec_iface_t *decoder_ifaces[] = {
        vpx_codec_vp8_dx(),
        vpx_codec_vp9_dx()
    };
    
    stream_info.sz = sizeof(stream_info);
    for (int i = 0; i < 2; i++) {
        vpx_codec_err_t peek_result = vpx_codec_peek_stream_info(
            decoder_ifaces[i], data, size, &stream_info);
        
        if (peek_result == VPX_CODEC_OK) {
            break;  // Found compatible codec
        }
    }
    
    // 2. Get codec capabilities for VP9 encoder
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    vpx_codec_caps_t caps = vpx_codec_get_caps(encoder_iface);
    
    // 3. Allocate image using the stream dimensions (or defaults if not found)
    unsigned int width = stream_info.w > 0 ? stream_info.w : 640;
    unsigned int height = stream_info.h > 0 ? stream_info.h : 480;
    
    image = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, width, height, 1);
    if (!image) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Set rectangle on the image
    unsigned int rect_x = 0;
    unsigned int rect_y = 0;
    unsigned int rect_w = width / 2;
    unsigned int rect_h = height / 2;
    
    // Ensure rectangle is within bounds
    if (rect_w > 0 && rect_h > 0 && rect_x + rect_w <= width && rect_y + rect_h <= height) {
        vpx_img_set_rect(image, rect_x, rect_y, rect_w, rect_h);
    }
    
    // 5. Initialize multi-encoder with default configuration
    unsigned int usage = 0;  // Good quality mode
    if (vpx_codec_enc_config_default(encoder_iface, &enc_cfg, usage) != VPX_CODEC_OK) {
        vpx_img_free(image);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set encoder configuration based on stream info
    enc_cfg.g_w = width;
    enc_cfg.g_h = height;
    enc_cfg.g_threads = 1;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000;
    
    // Create configuration data for two-pass encoding
    size_t config_size = sizeof(vpx_codec_enc_cfg_t);
    config_data = new (std::nothrow) uint8_t[config_size];
    if (config_data) {
        memcpy(config_data, &enc_cfg, config_size);
        two_pass_stats.buf = config_data;
        two_pass_stats.sz = config_size;
        enc_cfg.rc_twopass_stats_in = two_pass_stats;
    }
    
    // Initialize the encoder context with multi-encoder configuration
    // Using single encoder for simplicity
    int num_enc = 1;
    vpx_codec_flags_t flags = 0;
    vpx_rational_t dsf = {1, 1};  // No downsampling
    
    vpx_codec_err_t init_result = vpx_codec_enc_init_multi_ver(
        &codec_ctx, encoder_iface, &enc_cfg, num_enc, flags, &dsf, VPX_ENCODER_ABI_VERSION);
    
    if (init_result != VPX_CODEC_OK) {
        // Get error message
        const char *error_str = vpx_codec_error(&codec_ctx);
        if (error_str) {
            // Error string is available but we don't use it in fuzzing
        }
        
        vpx_img_free(image);
        delete[] config_data;
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 6. Register callbacks
    vpx_codec_register_put_slice_cb(&codec_ctx,  id_000043_put_slice_callback, &cb_ctx);
    vpx_codec_register_put_frame_cb(&codec_ctx,  id_000043_put_frame_callback, &cb_ctx);
    
    // 7. Re-set the encoder configuration (testing the API)
    if (vpx_codec_enc_config_set(&codec_ctx, &enc_cfg) != VPX_CODEC_OK) {
        // Get error message
        const char *error_str = vpx_codec_error(&codec_ctx);
        if (error_str) {
            // Error string is available but we don't use it in fuzzing
        }
    }
    
    // 8. Try to encode a dummy frame using the input data pattern
    if (image && size >= image->bps) {
        // Fill image planes with patterned data from input
        for (int plane = 0; plane < 3; plane++) {
            unsigned int plane_height = (plane == 0) ? height : height / 2;
            unsigned int plane_width = (plane == 0) ? width : width / 2;
            
            for (unsigned int y = 0; y < plane_height; y++) {
                size_t data_offset = (y * plane_width) % size;
                uint8_t pattern = data[data_offset];
                
                for (unsigned int x = 0; x < plane_width; x++) {
                    image->planes[plane][y * image->stride[plane] + x] = 
                        (uint8_t)(pattern ^ (x + y));
                }
            }
        }
        
        // Attempt to encode the frame
        vpx_codec_encode(&codec_ctx, image, 0, 1, 0, VPX_DL_REALTIME);
        
        // Get encoded data
        vpx_codec_iter_t iter = nullptr;
        const vpx_codec_cx_pkt_t *pkt = nullptr;
        while ((pkt = vpx_codec_get_cx_data(&codec_ctx, &iter)) != nullptr) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                // Frame packet generated
                cb_ctx.frame_count++;
            }
        }
    }
    
    // Cleanup
    vpx_codec_destroy(&codec_ctx);
    vpx_img_free(image);
    delete[] config_data;
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}