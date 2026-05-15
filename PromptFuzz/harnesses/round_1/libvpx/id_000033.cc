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
//<ID> 83
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_frame","vpx_img_set_rect","vpx_codec_get_preview_frame","vpx_codec_control_","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_peek_stream_info","vpx_codec_vp8_dx","vpx_codec_peek_stream_info","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_decode","vpx_codec_get_frame","vpx_img_set_rect","vpx_codec_get_preview_frame","vpx_codec_control_","vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Custom frame buffer management structure
struct FrameBufferData {
    std::vector<uint8_t> buffer;
    size_t size;
};

// Custom get frame buffer callback
static int  id_000033_get_frame_buffer_cb(void* cb_priv, size_t min_size, 
                               vpx_codec_frame_buffer_t* fb) {
    FrameBufferData* fb_data = static_cast<FrameBufferData*>(cb_priv);
    
    // Ensure buffer is large enough
    if (fb_data->buffer.size() < min_size) {
        fb_data->buffer.resize(min_size);
        fb_data->size = min_size;
    }
    
    fb->data = fb_data->buffer.data();
    fb->size = fb_data->size;
    return 0;
}

// Custom release frame buffer callback
static int  id_000033_release_frame_buffer_cb(void* cb_priv, 
                                   vpx_codec_frame_buffer_t* fb) {
    // Nothing to do here for this simple implementation
    (void)cb_priv;
    (void)fb;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }
    
    // Create FILE* from input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Frame buffer data for custom allocation
    FrameBufferData fb_data;
    
    // Codec context
    vpx_codec_ctx_t codec;
    memset(&codec, 0, sizeof(codec));
    
    // Choose codec interface based on input
    vpx_codec_iface_t* iface = nullptr;
    
    // Try to determine which codec to use
    vpx_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    
    // Try VP9 first
    iface = vpx_codec_vp9_dx();
    vpx_codec_err_t peek_err = vpx_codec_peek_stream_info(iface, data, 
                                                         (unsigned int)size, &si);
    
    if (peek_err != VPX_CODEC_OK) {
        // Try VP8
        iface = vpx_codec_vp8_dx();
        peek_err = vpx_codec_peek_stream_info(iface, data, 
                                             (unsigned int)size, &si);
    }
    
    // Get codec interface name
    const char* codec_name = vpx_codec_iface_name(iface);
    (void)codec_name; // Prevent unused warning
    
    // Get codec capabilities
    vpx_codec_caps_t caps = vpx_codec_get_caps(iface);
    (void)caps; // Prevent unused warning
    
    // Initialize decoder configuration
    vpx_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1; // Single thread for fuzzing
    dec_cfg.w = si.w;
    dec_cfg.h = si.h;
    
    // Initialize decoder
    vpx_codec_err_t init_err = vpx_codec_dec_init_ver(&codec, iface, &dec_cfg, 
                                                      0, VPX_DECODER_ABI_VERSION);
    
    if (init_err == VPX_CODEC_OK) {
        // Set custom frame buffer functions
        vpx_codec_err_t set_buf_err = vpx_codec_set_frame_buffer_functions(
            &codec,  id_000033_get_frame_buffer_cb,  id_000033_release_frame_buffer_cb, &fb_data);
        (void)set_buf_err; // Prevent unused warning
        
        // Decode the input data
        vpx_codec_err_t decode_err = vpx_codec_decode(&codec, data, 
                                                     (unsigned int)size, 
                                                     nullptr, 0);
        
        if (decode_err == VPX_CODEC_OK) {
            vpx_codec_iter_t iter = nullptr;
            vpx_image_t* img = vpx_codec_get_frame(&codec, &iter);
            
            if (img) {
                // Set a rectangle on the image (use small values for safety)
                unsigned int x = 0;
                unsigned int y = 0;
                unsigned int w = (img->d_w > 1) ? 1 : img->d_w;
                unsigned int h = (img->d_h > 1) ? 1 : img->d_h;
                
                int rect_err = vpx_img_set_rect(img, x, y, w, h);
                (void)rect_err; // Prevent unused warning
                
                // Try to get preview frame (this may fail but we attempt it)
                const vpx_image_t* preview = vpx_codec_get_preview_frame(&codec);
                (void)preview; // Prevent unused warning
                
                // Try some control commands
                // Use a safe control ID based on input
                int control_id = (data[0] % 100) + VP8_DECODER_CTRL_ID_START;
                vpx_codec_err_t control_err = vpx_codec_control_(&codec, control_id);
                (void)control_err; // Prevent unused warning
            }
        }
        
        // Initialize encoder configuration for re-encoding
        vpx_codec_iface_t* encoder_iface = vpx_codec_vp8_cx();
        vpx_codec_enc_cfg_t enc_cfg;
        memset(&enc_cfg, 0, sizeof(enc_cfg));
        
        // Get default encoder configuration
        unsigned int usage = 0; // Good quality
        vpx_codec_err_t cfg_err = vpx_codec_enc_config_default(encoder_iface, 
                                                              &enc_cfg, usage);
        (void)cfg_err; // Prevent unused warning
    }
    
    // Clean up
    vpx_codec_destroy(&codec);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}