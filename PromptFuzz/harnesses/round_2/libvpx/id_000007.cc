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
//<ID> 12
//<Prompt> []
/*<Combination>: [
*/
//<score> 105, nr_unique_branch: 93
//<Quality> {"density":10,"unique_branches":{"vpx_realloc_frame_buffer":[[220,11,220,59,0,0,4,0],[222,31,222,61,0,0,4,0]],"average_split_mvs":[[121,5,121,11,0,0,4,0]],"vpx_filter_block1d4_h8_avx2":[[1096,14,1096,19,0,0,4,0],[1096,14,1096,19,0,0,4,1],[1099,9,1099,14,0,0,4,0],[1099,9,1099,14,0,0,4,1]],"vpx_convolve8_avg_vert_avx2":[[41,9,41,70,0,0,4,0],[43,14,43,21,0,0,4,0],[43,14,43,21,0,0,4,1],[50,11,50,17,0,0,4,0],[50,11,50,17,0,0,4,1],[53,18,53,24,0,0,4,0],[53,18,53,24,0,0,4,1]],"vpx_convolve8_avg_avx2":[[109,9,109,62,0,0,4,0]],"vpx_filter_block1d8_v8_avx2":[[993,12,993,17,0,0,4,0],[993,12,993,17,0,0,4,1]],"extend_and_predict":[[543,7,543,50,0,0,4,1]],"vpx_filter_block1d16_v8_x_avx2":[[274,27,274,32,0,0,4,0],[274,27,274,32,0,0,4,1],[304,9,304,12,0,0,4,0],[304,9,304,12,0,0,4,1]],"vpx_filter_block1d16_h8_x_avx2":[[82,27,82,32,0,0,4,0],[82,27,82,32,0,0,4,1],[112,9,112,12,0,0,4,0],[112,9,112,12,0,0,4,1],[124,7,124,12,0,0,4,0],[124,7,124,12,0,0,4,1],[146,9,146,12,0,0,4,1]],"vpx_filter_block1d8_h8_avx2":[[186,10,186,15,0,0,4,0],[186,10,186,15,0,0,4,1],[193,10,193,15,0,0,4,0],[193,10,193,15,0,0,4,1],[199,7,199,12,0,0,4,0],[199,7,199,12,0,0,4,1]],"vpx_convolve8_avx2":[[109,9,109,62,0,0,4,0]],"vpx_convolve8_horiz_avx2":[[41,9,41,70,0,0,4,0],[43,14,43,21,0,0,4,0],[43,14,43,21,0,0,4,1],[50,11,50,17,0,0,4,0],[50,11,50,17,0,0,4,1],[53,18,53,24,0,0,4,0],[53,18,53,24,0,0,4,1]],"vpx_convolve8_avg_horiz_avx2":[[41,9,41,70,0,0,4,0],[43,14,43,21,0,0,4,0],[43,14,43,21,0,0,4,1],[50,11,50,17,0,0,4,0],[50,11,50,17,0,0,4,1],[53,18,53,24,0,0,4,0],[53,18,53,24,0,0,4,1]],"setup_frame_size_with_refs":[[1608,7,1616,25,0,0,4,0]],"build_mc_border":[[464,7,464,13,0,0,4,0],[464,7,464,13,0,0,4,1],[466,12,466,17,0,0,4,0],[466,12,466,17,0,0,4,1],[471,16,471,21,0,0,4,0],[471,16,471,21,0,0,4,1],[473,9,473,19,0,0,4,0],[473,9,473,19,0,0,4,1],[475,9,475,20,0,0,4,0],[475,9,475,20,0,0,4,1],[477,9,477,20,0,0,4,0],[477,9,477,20,0,0,4,1],[481,9,481,13,0,0,4,0],[481,9,481,13,0,0,4,1],[483,9,483,13,0,0,4,0],[483,9,483,13,0,0,4,1],[485,9,485,14,0,0,4,0],[485,9,485,14,0,0,4,1],[490,9,490,14,0,0,4,0],[490,9,490,14,0,0,4,1],[490,18,490,23,0,0,4,0],[490,18,490,23,0,0,4,1],[491,12,491,17,0,0,4,0],[491,12,491,17,0,0,4,1]],"round_mv_comp_q4":[[63,11,63,20,0,0,4,0],[63,11,63,20,0,0,4,1]],"vpx_filter_block1d4_v8_avx2":[[1193,10,1193,16,0,0,4,0],[1193,10,1193,16,0,0,4,1],[1238,7,1238,13,0,0,4,1]],"dec_build_inter_predictors":[[662,54,662,73,0,0,4,1],[663,7,663,27,0,0,4,0],[663,7,663,27,0,0,4,1],[702,7,702,50,0,0,4,1]],"vpx_convolve8_vert_avx2":[[41,9,41,70,0,0,4,0],[43,14,43,21,0,0,4,0],[43,14,43,21,0,0,4,1],[50,11,50,17,0,0,4,0],[50,11,50,17,0,0,4,1],[53,18,53,24,0,0,4,0],[53,18,53,24,0,0,4,1]],"inverse_transform_block_inter":[[177,7,177,50,0,0,4,1],[199,9,199,21,0,0,4,0]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_alloc","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_vp8_dx","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_get_frame","vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_alloc","vpx_img_free","vpx_codec_destroy"],"visited":1}
/*We'll create a fuzzer that decodes VP8/VP9 video frames using libvpx with custom frame buffer management and slice callbacks. Here's a complete implementation:

*/


// Custom structure to manage frame buffers
struct FrameBufferContext {
    std::vector<vpx_codec_frame_buffer_t*> allocated_buffers;
    const uint8_t* input_data;
    size_t input_size;
    size_t data_offset;
};

// Callback to get frame buffer
int  id_000007_get_frame_buffer(void* cb_priv, size_t min_size, vpx_codec_frame_buffer_t* fb) {
    FrameBufferContext* ctx = static_cast<FrameBufferContext*>(cb_priv);
    
    // Check if we have enough input data left
    if (ctx->data_offset >= ctx->input_size) {
        return -1;
    }
    
    // Calculate available size from input
    size_t available_size = ctx->input_size - ctx->data_offset;
    size_t alloc_size = (min_size < available_size) ? min_size : available_size;
    
    // Allocate buffer using input data
    fb->data = const_cast<uint8_t*>(ctx->input_data + ctx->data_offset);
    fb->size = alloc_size;
    
    // Store the buffer for tracking
    ctx->allocated_buffers.push_back(fb);
    ctx->data_offset += alloc_size;
    
    return 0;
}

// Callback to release frame buffer
int  id_000007_release_frame_buffer(void* cb_priv, vpx_codec_frame_buffer_t* fb) {
    FrameBufferContext* ctx = static_cast<FrameBufferContext*>(cb_priv);
    
    // Remove from allocated buffers list
    for (auto it = ctx->allocated_buffers.begin(); it != ctx->allocated_buffers.end(); ++it) {
        if (*it == fb) {
            ctx->allocated_buffers.erase(it);
            break;
        }
    }
    
    return 0;
}

// Put slice callback function
void  id_000007_put_slice_callback(void* user_priv, const vpx_image_t* img, 
                       const vpx_image_rect_t* rect, const vpx_image_rect_t* rect2) {
    // This callback is called when a slice is decoded
    // We can process or log slice information here
    (void)user_priv;
    (void)img;
    (void)rect;
    (void)rect2;
    
    // In a fuzzer, we might want to validate or inspect slices
    // but for fuzzing purposes, we just accept the slice
}

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for codec initialization
    
    // Initialize FrameBufferContext
    FrameBufferContext fb_ctx;
    fb_ctx.input_data = data;
    fb_ctx.input_size = size;
    fb_ctx.data_offset = 0;
    
    // Initialize decoder context
    vpx_codec_ctx_t codec;
    vpx_codec_iface_t* decoder_iface = nullptr;
    
    // Try VP9 decoder first, then VP8
    int ret = -1;
    decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_dec_cfg_t cfg = {0, 0, 0};
    
    // Initialize decoder with custom frame buffer functions
    ret = vpx_codec_dec_init_ver(&codec, decoder_iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    
    if (ret != VPX_CODEC_OK) {
        // Try VP8 if VP9 fails
        decoder_iface = vpx_codec_vp8_dx();
        ret = vpx_codec_dec_init_ver(&codec, decoder_iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
        if (ret != VPX_CODEC_OK) {
            // Cannot initialize decoder
            return 0;
        }
    }
    
    // Register put slice callback
    ret = vpx_codec_register_put_slice_cb(&codec,  id_000007_put_slice_callback, nullptr);
    
    // Set frame buffer functions using our custom callbacks
    ret = vpx_codec_set_frame_buffer_functions(&codec,  id_000007_get_frame_buffer, 
                                               id_000007_release_frame_buffer, &fb_ctx);
    
    // Prepare to decode frames
    vpx_codec_iter_t iter = nullptr;
    const vpx_image_t* img = nullptr;
    
    // Split input data into chunks for multiple decode attempts
    const size_t chunk_size = 4096;
    size_t decode_offset = 0;
    int decode_attempts = 0;
    
    while (decode_offset < size && decode_attempts < 10) {
        size_t remaining = size - decode_offset;
        size_t decode_size = (remaining < chunk_size) ? remaining : chunk_size;
        
        // Decode the chunk
        ret = vpx_codec_decode(&codec, data + decode_offset, decode_size, nullptr, 0);
        
        // Check for errors and get error detail if any
        if (ret != VPX_CODEC_OK) {
            const char* error_detail = vpx_codec_error_detail(&codec);
            (void)error_detail; // Use error detail if needed for debugging
        }
        
        // Get decoded frames
        while ((img = vpx_codec_get_frame(&codec, &iter)) != nullptr) {
            // Process the decoded frame if needed
            // For fuzzing, we just accept the frame
            
            // Note: We could use vpx_codec_get_cx_data here if this was an encoder,
            // but for decoder we use vpx_codec_get_frame
            
            // Simulate using vpx_codec_get_cx_data for completeness
            // (though it's typically used with encoders)
            // We'll create a mock encoder context to demonstrate the API
            vpx_codec_ctx_t mock_encoder;
            vpx_codec_iter_t mock_iter = nullptr;
            
            // Try to initialize VP9 encoder to use get_cx_data
            vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
            vpx_codec_enc_cfg_t enc_cfg;
            
            if (vpx_codec_enc_config_default(encoder_iface, &enc_cfg, 0) == VPX_CODEC_OK) {
                if (vpx_codec_enc_init_ver(&mock_encoder, encoder_iface, &enc_cfg, 0, 
                                          VPX_ENCODER_ABI_VERSION) == VPX_CODEC_OK) {
                    // Get CX data from mock encoder
                    const vpx_codec_cx_pkt_t* pkt;
                    while ((pkt = vpx_codec_get_cx_data(&mock_encoder, &mock_iter)) != nullptr) {
                        // Process the packet if needed
                        (void)pkt;
                    }
                    
                    // Clean up mock encoder
                    vpx_codec_destroy(&mock_encoder);
                }
            }
            
            // Free the image if we had allocated one
            // In this case, the image is managed by libvpx, so we don't free it
        }
        
        decode_offset += decode_size;
        decode_attempts++;
    }
    
    // Create and free a test image using vpx_img_free
    vpx_image_t* test_img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 64, 64, 1);
    if (test_img) {
        vpx_img_free(test_img);
    }
    
    // Clean up
    vpx_codec_destroy(&codec);
    
    return 0;
}