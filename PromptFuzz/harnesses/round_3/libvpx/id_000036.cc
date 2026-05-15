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
//<ID> 105
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_error_detail","vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_enc_init_multi_ver","vpx_codec_vp8_cx","vpx_codec_control_","vpx_codec_control_","vpx_img_alloc","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_dx","vpx_codec_iface_name","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_frame_cb","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_error_detail","vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_codec_enc_init_multi_ver","vpx_codec_vp8_cx","vpx_codec_control_","vpx_codec_control_","vpx_img_alloc","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/*To create a fuzzer that uses libvpx APIs for decoding and re-encoding video data, I'll design a program that:

1. Decodes input video data
2. Registers callbacks for frame processing
3. Re-encodes the decoded frames using multiple encoders
4. Handles errors and properly cleans up resources

*/


// Include necessary libvpx headers
extern "C" {
}

// Structure to hold decoded frame information
struct FrameData {
    std::vector<uint8_t> buffer;
    int width;
    int height;
    vpx_img_fmt_t format;
};

// Global vector to store decoded frames
static std::vector<FrameData>  id_000036_g_decoded_frames;
static size_t  id_000036_g_max_frames_to_store = 10;

// Callback function for put_frame
static void  id_000036_put_frame_callback(void* user_priv, const vpx_image_t* img) {
    if ( id_000036_g_decoded_frames.size() >=  id_000036_g_max_frames_to_store) {
        return;
    }
    
    FrameData frame;
    frame.width = img->d_w;
    frame.height = img->d_h;
    frame.format = img->fmt;
    
    // Calculate total size
    size_t total_size = 0;
    for (int plane = 0; plane < 3; plane++) {
        total_size += img->stride[plane] * (img->d_h >> (plane > 0 ? 1 : 0));
    }
    
    frame.buffer.resize(total_size);
    
    // Copy image data
    size_t offset = 0;
    for (int plane = 0; plane < 3; plane++) {
        int plane_height = img->d_h >> (plane > 0 ? 1 : 0);
        size_t plane_size = img->stride[plane] * plane_height;
        
        if (img->planes[plane]) {
            memcpy(frame.buffer.data() + offset, img->planes[plane], plane_size);
            offset += plane_size;
        }
    }
    
     id_000036_g_decoded_frames.push_back(frame);
}

// Frame buffer callback functions
static int  id_000036_get_frame_buffer_cb(void* cb_priv, size_t min_size, 
                              vpx_codec_frame_buffer_t* fb) {
    static uint8_t frame_buffer_data[1024 * 1024]; // 1MB buffer
    static size_t buffer_size = sizeof(frame_buffer_data);
    
    if (min_size > buffer_size) {
        return -1;
    }
    
    fb->data = frame_buffer_data;
    fb->size = buffer_size;
    return 0;
}

static int  id_000036_release_frame_buffer_cb(void* cb_priv, 
                                  vpx_codec_frame_buffer_t* fb) {
    // Nothing to release for static buffer
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<64) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
	//fuzzer shim end}




    if (size < 4) {
        return 0;
    }
    
    // Clear global state
     id_000036_g_decoded_frames.clear();
    
    // Try both VP8 and VP9 decoders
    vpx_codec_iface_t* decoder_ifaces[] = {
        vpx_codec_vp8_dx(),
        vpx_codec_vp9_dx()
    };
    
    const char* iface_names[] = {"VP8", "VP9"};
    
    for (int decoder_idx = 0; decoder_idx < 2; decoder_idx++) {
        vpx_codec_iface_t* iface = decoder_ifaces[decoder_idx];
        
        // 1. Get interface name
        const char* name = vpx_codec_iface_name(iface);
        // name is unused but we called the API
        
        // 2. Peek stream info
        vpx_codec_stream_info_t stream_info;
        memset(&stream_info, 0, sizeof(stream_info));
        stream_info.sz = sizeof(stream_info);
        
        vpx_codec_err_t peek_result = vpx_codec_peek_stream_info(
            iface, data, (unsigned int)size, &stream_info);
        
        if (peek_result != VPX_CODEC_OK) {
            continue; // Try next decoder
        }
        
        // 3. Initialize decoder
        vpx_codec_ctx_t decoder;
        vpx_codec_dec_cfg_t dec_cfg;
        memset(&dec_cfg, 0, sizeof(dec_cfg));
        dec_cfg.threads = 1;
        
        vpx_codec_err_t init_result = vpx_codec_dec_init_ver(
            &decoder, iface, &dec_cfg, fuzz_int64_t_1, VPX_DECODER_ABI_VERSION);
        
        if (init_result != VPX_CODEC_OK) {
            continue;
        }
        
        // 4. Set frame buffer functions
        vpx_codec_set_frame_buffer_functions(
            &decoder,  id_000036_get_frame_buffer_cb,  id_000036_release_frame_buffer_cb, nullptr);
        
        // 5. Register put frame callback
        vpx_codec_register_put_frame_cb(&decoder,  id_000036_put_frame_callback, nullptr);
        
        // 6. Decode the input data
        vpx_codec_decode(&decoder, data, (unsigned int)size, nullptr, fuzz_int64_t_2);
        
        // 7. Get stream info from decoder
        vpx_codec_stream_info_t decoder_info;
        memset(&decoder_info, 0, sizeof(decoder_info));
        decoder_info.sz = sizeof(decoder_info);
        
        vpx_codec_get_stream_info(&decoder, &decoder_info);
        
        // 8. Get error details (even if no error)
        const char* error_detail = vpx_codec_error_detail(&decoder);
        // error_detail is unused but we called the API
        
        // Now try to re-encode the decoded frames
        if (! id_000036_g_decoded_frames.empty() && decoder_info.w > 0 && decoder_info.h > 0) {
            // 9. Initialize multiple encoders
            vpx_codec_ctx_t encoder1, encoder2;
            vpx_codec_enc_cfg_t enc_cfg;
            
            // Get default encoder configuration
            vpx_codec_enc_config_default(vpx_codec_vp8_cx(), &enc_cfg, fuzz_uint32_t_3);
            
            // Set dimensions
            enc_cfg.g_w = decoder_info.w;
            enc_cfg.g_h = decoder_info.h;
            enc_cfg.g_timebase.num = 1;
            enc_cfg.g_timebase.den = 30;
            enc_cfg.rc_target_bitrate = 1000;
            enc_cfg.g_pass = VPX_RC_ONE_PASS;
            
            // Downsampling factors
            vpx_rational_t dsf[2];
            dsf[0].num = 1;
            dsf[0].den = 1;
            dsf[1].num = 2;
            dsf[1].den = 1;
            
            // 10. Initialize multi-encoder
            vpx_codec_err_t enc_init_result = vpx_codec_enc_init_multi_ver(
                &encoder1, vpx_codec_vp8_cx(), &enc_cfg, 2, fuzz_int64_t_4, dsf, 
                VPX_ENCODER_ABI_VERSION);
            
            if (enc_init_result == VPX_CODEC_OK) {
                // 11. Use control API to set some parameters
                vpx_codec_control_(&encoder1, VP8E_SET_CPUUSED, 6);
                vpx_codec_control_(&encoder1, VP8E_SET_NOISE_SENSITIVITY, 0);
                
                // Encode a few frames
                for (size_t i = 0; i <  id_000036_g_decoded_frames.size() && i < 2; i++) {
                    const FrameData& frame =  id_000036_g_decoded_frames[i];
                    
                    // Create vpx_image from frame data
                    vpx_image_t img;
                    vpx_img_alloc(&img, VPX_IMG_FMT_I420, frame.width, 
                                 frame.height, fuzz_uint32_t_5);
                    
                    // Copy data (simplified - assuming I420 format)
                    if (frame.buffer.size() >= 
                        (size_t)(frame.width * frame.height * 3 / 2)) {
                        memcpy(img.planes[0], frame.buffer.data(), 
                               frame.width * frame.height);
                        memcpy(img.planes[1], 
                               frame.buffer.data() + frame.width * frame.height,
                               frame.width * frame.height / 4);
                        memcpy(img.planes[2], 
                               frame.buffer.data() + frame.width * frame.height * 5 / 4,
                               frame.width * frame.height / 4);
                    }
                    
                    // Encode the frame
                    vpx_codec_encode(&encoder1, &img, fuzz_int64_t_6, fuzz_uint64_t_7, fuzz_int64_t_8, fuzz_uint64_t_9);
                    
                    vpx_img_free(&img);
                }
                
                // Clean up encoder
                vpx_codec_destroy(&encoder1);
            }
        }
        
        // Clean up decoder
        vpx_codec_destroy(&decoder);
        
        // If we successfully processed with this decoder, break
        if (peek_result == VPX_CODEC_OK) {
            break;
        }
    }
    
    return 0;
}