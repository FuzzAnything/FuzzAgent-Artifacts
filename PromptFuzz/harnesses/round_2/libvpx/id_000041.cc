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
//<ID> 151
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["vpx_codec_vp8_dx","vpx_codec_vp9_dx","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_destroy","vpx_codec_vp8_cx","vpx_codec_vp9_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_dx","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_destroy","vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_destroy","vpx_codec_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libvpx APIs to decode an encoded video stream and extract stream information:

*/


extern "C" {
}

// Frame buffer callback structures
struct FrameBufferData {
    std::vector<uint8_t> buffer;
    size_t buffer_size;
};

extern "C" int get_frame_buffer(void *cb_priv, size_t min_size, 
                               vpx_codec_frame_buffer_t *fb) {
    FrameBufferData *fb_data = static_cast<FrameBufferData*>(cb_priv);
    
    // Resize buffer if needed
    if (fb_data->buffer.size() < min_size) {
        fb_data->buffer.resize(min_size);
        fb_data->buffer_size = min_size;
    }
    
    fb->data = fb_data->buffer.data();
    fb->size = fb_data->buffer_size;
    fb->priv = nullptr;
    
    return 0;
}

extern "C" int release_frame_buffer(void *cb_priv, 
                                   vpx_codec_frame_buffer_t *fb) {
    // Nothing to do - buffer is managed by FrameBufferData
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
        return 0;  // Too small to be valid video data
    }
    
    FILE *in_file = nullptr;
    std::vector<uint8_t> file_data;
    vpx_codec_ctx_t decoder = {0};
    vpx_codec_enc_cfg_t enc_cfg = {0};
    vpx_codec_ctx_t encoder = {0};
    vpx_codec_stream_info_t stream_info = {0};
    vpx_codec_iface_t *codec_iface = nullptr;
    FrameBufferData frame_buffer_data;
    
    // Try both VP8 and VP9 decoders
    const vpx_codec_iface_t* decoder_ifaces[] = {
        vpx_codec_vp8_dx(),
        vpx_codec_vp9_dx()
    };
    
    const char* codec_names[] = {"VP8", "VP9"};
    
    for (int i = 0; i < 2; i++) {
        codec_iface = const_cast<vpx_codec_iface_t*>(decoder_ifaces[i]);
        
        // 1. Get codec interface name
        const char *iface_name = vpx_codec_iface_name(codec_iface);
        if (!iface_name) {
            continue;
        }
        
        // 2. Get codec capabilities
        vpx_codec_caps_t caps = vpx_codec_get_caps(codec_iface);
        (void)caps;  // Mark as used
        
        // Reset decoder context
        memset(&decoder, 0, sizeof(decoder));
        
        // Try to decode with current interface
        vpx_codec_err_t dec_result = vpx_codec_dec_init_ver(
            &decoder, codec_iface, nullptr, 0, VPX_DECODER_ABI_VERSION);
        
        if (dec_result != VPX_CODEC_OK) {
            continue;
        }
        
        // 3. Set frame buffer functions
        frame_buffer_data.buffer.resize(size * 2);  // Start with reasonable size
        frame_buffer_data.buffer_size = frame_buffer_data.buffer.size();
        
        vpx_codec_err_t fb_result = vpx_codec_set_frame_buffer_functions(
            &decoder, get_frame_buffer, release_frame_buffer, &frame_buffer_data);
        
        if (fb_result != VPX_CODEC_OK) {
            vpx_codec_destroy(&decoder);
            continue;
        }
        
        // 4. Initialize encoder for the same codec type (using encoder interface)
        vpx_codec_iface_t *encoder_iface = nullptr;
        if (i == 0) {
            encoder_iface = vpx_codec_vp8_cx();
        } else {
            encoder_iface = vpx_codec_vp9_cx();
        }
        
        // Get encoder interface name
        const char *enc_iface_name = vpx_codec_iface_name(encoder_iface);
        (void)enc_iface_name;  // Mark as used
        
        // Initialize encoder with default configuration
        vpx_codec_err_t enc_result = vpx_codec_enc_config_default(
            encoder_iface, &enc_cfg, 0);
        
        if (enc_result == VPX_CODEC_OK) {
            // Set reasonable dimensions for encoder
            enc_cfg.g_w = 320;
            enc_cfg.g_h = 240;
            enc_cfg.rc_target_bitrate = 1000;
            
            // 5. Initialize encoder
            enc_result = vpx_codec_enc_init_ver(
                &encoder, encoder_iface, &enc_cfg, 0, VPX_ENCODER_ABI_VERSION);
            
            if (enc_result != VPX_CODEC_OK) {
                // Don't fail if encoder init fails, just continue
            }
        }
        
        // 6. Decode the input data
        vpx_codec_err_t decode_result = vpx_codec_decode(
            &decoder, data, static_cast<unsigned int>(size), nullptr, 0);
        
        if (decode_result == VPX_CODEC_OK) {
            // 7. Get stream information
            stream_info.sz = sizeof(stream_info);
            vpx_codec_err_t info_result = vpx_codec_get_stream_info(
                &decoder, &stream_info);
            
            if (info_result == VPX_CODEC_OK) {
                // Successfully decoded and got stream info
                // Extract some frames to exercise the decoder more
                vpx_codec_iter_t iter = nullptr;
                const vpx_image_t *img;
                int frame_count = 0;
                
                while ((img = vpx_codec_get_frame(&decoder, &iter)) != nullptr && 
                       frame_count < 5) {
                    frame_count++;
                    // Just access some basic image properties
                    (void)img->d_w;
                    (void)img->d_h;
                    (void)img->fmt;
                }
                
                // Clean up and exit loop since we found a working decoder
                if (encoder.err == VPX_CODEC_OK) {
                    vpx_codec_destroy(&encoder);
                }
                vpx_codec_destroy(&decoder);
                break;
            }
        }
        
        // Clean up for this iteration
        if (encoder.err == VPX_CODEC_OK) {
            vpx_codec_destroy(&encoder);
            memset(&encoder, 0, sizeof(encoder));
        }
        vpx_codec_destroy(&decoder);
    }
    
    // Clean up any remaining resources
    if (encoder.err == VPX_CODEC_OK) {
        vpx_codec_destroy(&encoder);
    }
    
    if (decoder.err == VPX_CODEC_OK) {
        vpx_codec_destroy(&decoder);
    }
    
    return 0;
}