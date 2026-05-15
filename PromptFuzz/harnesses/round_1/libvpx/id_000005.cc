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
//<ID> 10
//<Prompt> []
/*<Combination>: [
*/
//<score> 10.5, nr_unique_branch: 0
//<Quality> {"density":21,"unique_branches":{},"library_calls":["vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_codec_destroy","vpx_codec_enc_init_ver","vpx_codec_vp9_cx","vpx_codec_destroy","vpx_codec_enc_config_set","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_get_caps","vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_vp9_cx","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_get_frame","vpx_img_alloc","vpx_img_flip","vpx_codec_encode","vpx_img_free","vpx_codec_decode","vpx_img_alloc","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_dec_init_ver","vpx_codec_vp9_dx","vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_codec_enc_init_ver","vpx_codec_vp9_cx","vpx_codec_enc_config_set","vpx_codec_get_caps","vpx_codec_vp9_dx","vpx_codec_get_caps","vpx_codec_vp9_cx","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_get_frame","vpx_img_alloc","vpx_img_flip","vpx_codec_encode","vpx_img_free","vpx_codec_decode","vpx_img_alloc","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified libvpx APIs to decode VP9 video, flip the decoded frames, and re-encode them:

*/


extern "C" {
}

// Custom structure to hold context information
struct FuzzerContext {
    uint8_t* buffer;
    size_t buffer_size;
    size_t buffer_offset;
};

// Callback function for put_slice (minimal implementation)
static void  id_000005_put_slice_callback(void* user_priv, const vpx_image_t* img,
                              const vpx_image_rect_t* slice,
                              const vpx_image_rect_t* clipped_slice) {
    // Empty callback - just to satisfy API requirements
    (void)user_priv;
    (void)img;
    (void)slice;
    (void)clipped_slice;
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if input is too small
    if (size < 10) {
	return 0;
	}
	
    
    // Create a copy of the input data that we can modify
    uint8_t* input_copy = static_cast<uint8_t*>(malloc(size));
    if (!input_copy) {
	return 0;
	}
	
    memcpy(input_copy, data, size);
    
    // Initialize contexts
    FuzzerContext ctx;
    ctx.buffer = input_copy;
    ctx.buffer_size = size;
    ctx.buffer_offset = 0;
    
    // Open input as FILE* for reading (even though we won't use it much)
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        free(input_copy);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    vpx_codec_err_t res = vpx_codec_dec_init_ver(&decoder, vpx_codec_vp9_dx(), 
                                                  &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        free(input_copy);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize encoder
    vpx_codec_ctx_t encoder;
    vpx_codec_enc_cfg_t enc_cfg;
    
    // Get default encoder configuration
    res = vpx_codec_enc_config_default(vpx_codec_vp9_cx(), &enc_cfg, 0);
    if (res != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        free(input_copy);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set basic encoder parameters
    enc_cfg.g_w = 320;
    enc_cfg.g_h = 240;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 256;
    enc_cfg.g_pass = VPX_RC_ONE_PASS;
    
    // Initialize encoder
    res = vpx_codec_enc_init_ver(&encoder, vpx_codec_vp9_cx(), &enc_cfg, 0, 
                                  VPX_ENCODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        free(input_copy);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set encoder configuration (using the same config we just initialized with)
    res = vpx_codec_enc_config_set(&encoder, &enc_cfg);
    if (res != VPX_CODEC_OK) {
        vpx_codec_destroy(&encoder);
        vpx_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        free(input_copy);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Check codec capabilities
    vpx_codec_caps_t decoder_caps = vpx_codec_get_caps(vpx_codec_vp9_dx());
    vpx_codec_caps_t encoder_caps = vpx_codec_get_caps(vpx_codec_vp9_cx());
    (void)decoder_caps;  // Mark as used
    (void)encoder_caps;  // Mark as used
    
    // Register put_slice callback
    res = vpx_codec_register_put_slice_cb(&encoder,  id_000005_put_slice_callback, &ctx);
    if (res != VPX_CODEC_OK) {
        // Continue even if callback registration fails
    }
    
    // Try to decode the input data
    res = vpx_codec_decode(&decoder, input_copy, static_cast<unsigned int>(size), 
                           nullptr, 0);
    
    vpx_codec_iter_t iter = nullptr;
    vpx_image_t* decoded_image = nullptr;
    
    if (res == VPX_CODEC_OK) {
        // Get decoded frame
        decoded_image = vpx_codec_get_frame(&decoder, &iter);
    }
    
    // If we got a valid decoded image, process it
    if (decoded_image) {
        // Create a copy of the image to flip
        vpx_image_t* image_to_encode = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 
                                                      decoded_image->d_w, 
                                                      decoded_image->d_h, 16);
        if (image_to_encode) {
            // Copy the decoded image
            for (int plane = 0; plane < 3; ++plane) {
                uint8_t* src = decoded_image->planes[plane];
                uint8_t* dst = image_to_encode->planes[plane];
                int src_stride = decoded_image->stride[plane];
                int dst_stride = image_to_encode->stride[plane];
                int height = (plane == 0) ? decoded_image->d_h : 
                            (decoded_image->d_h + 1) >> 1;
                int width = (plane == 0) ? decoded_image->d_w : 
                           (decoded_image->d_w + 1) >> 1;
                
                for (int y = 0; y < height; ++y) {
                    memcpy(dst, src, width);
                    src += src_stride;
                    dst += dst_stride;
                }
            }
            
            // Flip the image vertically
            vpx_img_flip(image_to_encode);
            
            // Try to encode the flipped image
            res = vpx_codec_encode(&encoder, image_to_encode, 0, 1, 0, 
                                   VPX_DL_REALTIME);
            
            // Clean up the copied image
            vpx_img_free(image_to_encode);
        }
    }
    
    // Try to decode and encode with some variations if we have enough data
    if (size > 100) {
        // Use a subset of data to test partial decoding
        size_t partial_size = size / 2;
        vpx_codec_decode(&decoder, input_copy, static_cast<unsigned int>(partial_size), 
                        nullptr, 0);
        
        // Try encoding a dummy image if decoding failed
        if (!decoded_image && size > 1000) {
            // Create a minimal test image
            vpx_image_t* test_img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, 64, 64, 16);
            if (test_img) {
                // Fill with some data
                for (int i = 0; i < test_img->d_h; ++i) {
                    memset(test_img->planes[0] + i * test_img->stride[0], 
                           i % 256, test_img->d_w);
                }
                for (int i = 0; i < test_img->d_h / 2; ++i) {
                    memset(test_img->planes[1] + i * test_img->stride[1], 
                           128, test_img->d_w / 2);
                    memset(test_img->planes[2] + i * test_img->stride[2], 
                           128, test_img->d_w / 2);
                }
                
                vpx_codec_encode(&encoder, test_img, 0, 1, 0, VPX_DL_REALTIME);
                vpx_img_free(test_img);
            }
        }
    }
    
    // Cleanup
    vpx_codec_destroy(&encoder);
    vpx_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    free(input_copy);
    
    assert_file_closed(&in_file);
	return 0;
}