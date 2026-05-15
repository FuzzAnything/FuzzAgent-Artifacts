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
//<ID> 192
//<Prompt> []
/*<Combination>: [
*/
//<score> 25, nr_unique_branch: 0
//<Quality> {"density":25,"unique_branches":{},"library_calls":["vpx_codec_vp8_cx","vpx_codec_vp8_dx","vpx_codec_get_caps","vpx_codec_get_caps","vpx_codec_iface_name","vpx_codec_iface_name","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_register_put_slice_cb","vpx_img_alloc","vpx_img_set_rect","vpx_img_wrap","vpx_codec_encode","vpx_codec_set_cx_data_buf","vpx_codec_get_cx_data","vpx_codec_control_","vpx_codec_error","vpx_codec_destroy","vpx_codec_error","vpx_codec_destroy","vpx_codec_error","vpx_img_free"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_vp8_dx","vpx_codec_get_caps","vpx_codec_get_caps","vpx_codec_iface_name","vpx_codec_iface_name","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_decode","vpx_codec_get_frame","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_register_put_slice_cb","vpx_img_alloc","vpx_img_set_rect","vpx_img_wrap","vpx_codec_encode","vpx_codec_set_cx_data_buf","vpx_codec_get_cx_data","vpx_codec_control_","vpx_codec_error","vpx_codec_destroy","vpx_codec_destroy","vpx_img_free"],"visited":0}
/**/


// Include libvpx headers

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if we don't have enough data for basic operations
    if (size < 16) {
        return 0;
    }

    // Create file handle for input data using fmemopen
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor for input file
    int in_fd = fuzz_fileno(in_file);
    if (in_fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Variables to track cleanup
    bool decoder_initialized = false;
    bool encoder_initialized = false;
    vpx_image_t* allocated_image = nullptr;
    vpx_image_t* wrapped_image = nullptr;
    unsigned char* img_buffer = nullptr;
    
    // Initialize codec interfaces
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp8_cx();
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp8_dx();
    
    // Get codec capabilities and names
    vpx_codec_caps_t enc_caps = vpx_codec_get_caps(encoder_iface);
    vpx_codec_caps_t dec_caps = vpx_codec_get_caps(decoder_iface);
    
    const char* enc_name = vpx_codec_iface_name(encoder_iface);
    const char* dec_name = vpx_codec_iface_name(decoder_iface);
    
    // Peek stream information from input data
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    vpx_codec_err_t peek_result = vpx_codec_peek_stream_info(
        decoder_iface, data, size, &stream_info);
    
    // Initialize decoder configuration
    vpx_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 2;
    dec_cfg.w = stream_info.w > 0 ? stream_info.w : 320;
    dec_cfg.h = stream_info.h > 0 ? stream_info.h : 240;
    
    // Initialize decoder context
    vpx_codec_ctx_t decoder;
    memset(&decoder, 0, sizeof(decoder));
    
    vpx_codec_err_t dec_init_result = vpx_codec_dec_init_ver(
        &decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    
    if (dec_init_result == VPX_CODEC_OK) {
        decoder_initialized = true;
        
        // Decode the input data
        vpx_codec_err_t decode_result = vpx_codec_decode(
            &decoder, data, size, nullptr, 0);
        
        // Get decoded frame
        vpx_codec_iter_t iter = nullptr;
        vpx_image_t* decoded_frame = vpx_codec_get_frame(&decoder, &iter);
        
        if (decoded_frame) {
            // Initialize encoder configuration
            vpx_codec_enc_cfg_t enc_cfg;
            memset(&enc_cfg, 0, sizeof(enc_cfg));
            
            vpx_codec_err_t config_result = vpx_codec_enc_config_default(
                encoder_iface, &enc_cfg, 0);  // 0 = good quality
            
            if (config_result == VPX_CODEC_OK) {
                // Configure encoder based on decoded frame dimensions
                enc_cfg.g_w = decoded_frame->d_w;
                enc_cfg.g_h = decoded_frame->d_h;
                enc_cfg.g_timebase.num = 1;
                enc_cfg.g_timebase.den = 30;
                enc_cfg.rc_target_bitrate = 256;
                enc_cfg.g_error_resilient = 1;
                
                // Initialize encoder context
                vpx_codec_ctx_t encoder;
                memset(&encoder, 0, sizeof(encoder));
                
                vpx_codec_err_t enc_init_result = vpx_codec_enc_init_ver(
                    &encoder, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
                
                if (enc_init_result == VPX_CODEC_OK) {
                    encoder_initialized = true;
                    
                    // Create a callback structure for put_slice (dummy implementation)
                    struct SliceCallback {
                        static void callback(void* user_priv, 
                                            const vpx_image_t* img,
                                            const vpx_image_rect_t* rect1,
                                            const vpx_image_rect_t* rect2) {
                            // Do nothing, just a placeholder
                        }
                    };
                    
                    // Register put_slice callback
                    vpx_codec_register_put_slice_cb(
                        &encoder, SliceCallback::callback, nullptr);
                    
                    // Allocate image for encoding
                    allocated_image = vpx_img_alloc(
                        nullptr, VPX_IMG_FMT_I420, 
                        decoded_frame->d_w, decoded_frame->d_h, 1);
                    
                    if (allocated_image) {
                        // Set a rectangular region on the image
                        unsigned int x = decoded_frame->d_w > 16 ? 8 : 0;
                        unsigned int y = decoded_frame->d_h > 16 ? 8 : 0;
                        unsigned int w = decoded_frame->d_w > 32 ? 16 : decoded_frame->d_w;
                        unsigned int h = decoded_frame->d_h > 32 ? 16 : decoded_frame->d_h;
                        
                        vpx_img_set_rect(allocated_image, x, y, w, h);
                        
                        // Create wrapped image from buffer
                        size_t buffer_size = decoded_frame->d_w * decoded_frame->d_h * 3 / 2;
                        img_buffer = new unsigned char[buffer_size];
                        memset(img_buffer, 128, buffer_size);
                        
                        wrapped_image = vpx_img_wrap(
                            nullptr, VPX_IMG_FMT_I420,
                            decoded_frame->d_w, decoded_frame->d_h, 1, img_buffer);
                        
                        // Try encoding the allocated image
                        vpx_codec_err_t encode_result = vpx_codec_encode(
                            &encoder, allocated_image, 0, 1, 0, 0);
                        
                        if (encode_result == VPX_CODEC_OK) {
                            // Prepare buffer for encoded data
                            vpx_fixed_buf_t encoded_buffer;
                            encoded_buffer.buf = malloc(size * 2);  // Allocate double input size
                            encoded_buffer.sz = size * 2;
                            
                            // Set buffer for encoded data
                            vpx_codec_set_cx_data_buf(&encoder, &encoded_buffer, 0, 0);
                            
                            // Get encoded data packets
                            vpx_codec_iter_t enc_iter = nullptr;
                            const vpx_codec_cx_pkt_t* pkt;
                            while ((pkt = vpx_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                                // Process packet if needed
                                if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                                    // Frame packet received
                                }
                            }
                            
                            free(encoded_buffer.buf);
                        }
                        
                        // Try a control operation (dummy control ID, will likely fail)
                        vpx_codec_control_(&encoder, 0);
                        
                        // Get error string (should be null if no error)
                        const char* enc_error = vpx_codec_error(&encoder);
                        
                        // Clean up encoder
                        vpx_codec_destroy(&encoder);
                        encoder_initialized = false;
                    }
                } else {
                    // Get encoder error
                    const char* enc_error = vpx_codec_error(&encoder);
                }
            }
        }
        
        // Clean up decoder
        vpx_codec_destroy(&decoder);
        decoder_initialized = false;
    } else {
        // Get decoder error
        const char* dec_error = vpx_codec_error(&decoder);
    }
    
    // Cleanup allocated resources
    if (allocated_image) {
        vpx_img_free(allocated_image);
    }
    
    if (wrapped_image) {
        // Note: vpx_img_free is not needed for wrapped images that don't own their data
        // But we clean up the buffer we allocated
        delete[] img_buffer;
    }
    
    // Close input file
    assert_file_closed(&in_file);;
    
    // Create and close output file (for completeness, though we didn't write to it)
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        int out_fd = fuzz_fileno(out_file);
        assert_file_closed(&out_file);;
        remove("output_file");  // Clean up the empty file
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}