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
//<ID> 369
//<Prompt> []
/*<Combination>: [
*/
//<score> 51, nr_unique_branch: 2
//<Quality> {"density":17,"unique_branches":{"vpx_codec_enc_config_default":[[162,25,162,35,0,0,4,0]],"vpx_codec_encode":[[201,12,201,23,0,0,4,0]]},"library_calls":["vpx_codec_vp9_cx","vpx_codec_get_caps","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_err_to_string","vpx_codec_error_detail","vpx_codec_set_frame_buffer_functions","vpx_codec_enc_config_set","vpx_codec_get_preview_frame","vpx_codec_control_","vpx_codec_control_","vpx_img_alloc","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_cx","vpx_codec_get_caps","vpx_codec_enc_config_default","vpx_codec_enc_init_multi_ver","vpx_codec_err_to_string","vpx_codec_error_detail","vpx_codec_set_frame_buffer_functions","vpx_codec_enc_config_set","vpx_codec_get_preview_frame","vpx_codec_control_","vpx_codec_control_","vpx_img_alloc","vpx_codec_encode","vpx_img_free","vpx_codec_destroy","vpx_codec_vp9_dx","vpx_codec_dec_init_ver","vpx_codec_peek_stream_info","vpx_codec_decode","vpx_codec_error_detail","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Custom frame buffer management structure
struct FrameBufferData {
    std::vector<uint8_t> buffer;
    size_t in_use;
};

// Frame buffer get callback
int  id_000040_get_frame_buffer_cb(void *cb_priv, size_t min_size, vpx_codec_frame_buffer_t *fb) {
    FrameBufferData *fb_data = static_cast<FrameBufferData*>(cb_priv);
    
    if (fb_data->in_use >= fb_data->buffer.size()) {
        fb_data->buffer.resize(min_size * 2);
    }
    
    fb->data = fb_data->buffer.data() + fb_data->in_use;
    fb->size = min_size;
    fb_data->in_use += min_size;
    
    return 0;
}

// Frame buffer release callback
int  id_000040_release_frame_buffer_cb(void *cb_priv, vpx_codec_frame_buffer_t *fb) {
    // For simplicity, we don't actually release/free memory
    // In real usage, you'd manage buffer reuse here
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<64) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_9, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	 // Too small to be meaningful
    
    // Event: Initialize multiple encoders with configuration from input data,
    // decode a preview frame, and manage frame buffers with custom callbacks
    
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Read initial configuration values from input
    unsigned int width = 320, height = 240;
    unsigned int usage = 0;
    int num_encoders = 2;
    
    if (size >= 8) {
        width = (data[0] << 8) | data[1];
        height = (data[2] << 8) | data[3];
        usage = data[4] % 4;
        num_encoders = 1 + (data[5] % 3);
        
        // Clamp to reasonable values
        if (width < 16) width = 16;
        if (width > 4096) width = 4096;
        if (height < 16) height = 16;
        if (height > 4096) height = 4096;
        if (num_encoders > 4) num_encoders = 4;
    }
    
    // Get codec capabilities
    vpx_codec_iface_t *encoder_iface = vpx_codec_vp9_cx();
    vpx_codec_caps_t caps = vpx_codec_get_caps(encoder_iface);
    
    // Initialize encoder configuration
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err = vpx_codec_enc_config_default(encoder_iface, &cfg, fuzz_uint32_t_1);
    if (err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set configuration from input
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.rc_target_bitrate = 256; // Default bitrate
    cfg.g_threads = 1;
    
    // Create multiple encoder contexts
    std::vector<vpx_codec_ctx_t> encoders(num_encoders);
    vpx_rational_t dsf = {1, 1}; // Default scaling factor
    
    for (int i = 0; i < num_encoders; i++) {
        err = vpx_codec_enc_init_multi_ver(&encoders[i], encoder_iface, &cfg, 
                                          num_encoders, fuzz_int64_t_2, &dsf, VPX_ENCODER_ABI_VERSION);
        
        // Use error handling APIs
        if (err != VPX_CODEC_OK) {
            const char* error_str = vpx_codec_err_to_string(err);
            const char* error_detail = vpx_codec_error_detail(&encoders[i]);
            // We could log these, but for fuzzing we just continue
        }
    }
    
    // Set frame buffer functions for first encoder
    FrameBufferData fb_data;
    err = vpx_codec_set_frame_buffer_functions(&encoders[0], 
                                                id_000040_get_frame_buffer_cb,
                                                id_000040_release_frame_buffer_cb,
                                               &fb_data);
    
    // Set encoder configuration (again with potentially modified values)
    cfg.rc_target_bitrate = 512;
    err = vpx_codec_enc_config_set(&encoders[0], &cfg);
    
    // Try to get a preview frame (if encoder supports it)
    const vpx_image_t *preview_frame = vpx_codec_get_preview_frame(&encoders[0]);
    
    // Use codec control API - try different control IDs based on input
    if (size > 6) {
        int ctrl_id = (data[6] % 20) + VP8E_SET_CPUUSED; // Various control IDs
        err = vpx_codec_control_(&encoders[0], ctrl_id, data[7] % 20);
        
        // Try another control
        ctrl_id = VP8E_SET_NOISE_SENSITIVITY;
        err = vpx_codec_control_(&encoders[0], ctrl_id, data[8] % 4);
    }
    
    // Create a simple test image
    vpx_image_t *img = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, width, height, fuzz_uint32_t_3);
    if (img) {
        // Fill with some data
        for (unsigned int plane = 0; plane < 3; ++plane) {
            uint8_t *plane_ptr = img->planes[plane];
            unsigned int plane_size = (plane == 0) ? width * height : width * height / 4;
            
            for (unsigned int i = 0; i < plane_size && i < 100; ++i) {
                if (i < size) {
                    plane_ptr[i] = data[i % size];
                }
            }
        }
        
        // Try to encode with first encoder
        err = vpx_codec_encode(&encoders[0], img, fuzz_int64_t_4, fuzz_uint64_t_5, fuzz_int64_t_6, fuzz_uint64_t_7);
        
        vpx_img_free(img);
    }
    
    // Cleanup encoders
    for (int i = 0; i < num_encoders; i++) {
        if (encoders[i].err == VPX_CODEC_OK || encoders[i].err == VPX_CODEC_INVALID_PARAM) {
            vpx_codec_destroy(&encoders[i]);
        }
    }
    
    // Now try decoding the input as a compressed stream
    vpx_codec_ctx_t decoder;
    vpx_codec_iface_t *decoder_iface = vpx_codec_vp9_dx();
    
    err = vpx_codec_dec_init_ver(&decoder, decoder_iface, nullptr, fuzz_int64_t_8, VPX_DECODER_ABI_VERSION);
    if (err == VPX_CODEC_OK) {
        // Try to peek at stream info
        vpx_codec_stream_info_t si;
        vpx_codec_peek_stream_info(decoder_iface, data, 
                                   size > 1000 ? 1000 : size, &si);
        
        // Try to decode the input data
        err = vpx_codec_decode(&decoder, data, size, nullptr, fuzz_int64_t_9);
        
        // Get error detail if decoding failed
        if (err != VPX_CODEC_OK) {
            const char* error_detail = vpx_codec_error_detail(&decoder);
        }
        
        vpx_codec_destroy(&decoder);
    }
    
    assert_file_closed(&in_file);;
    
    // Try to write output (though we don't actually need it for fuzzing)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some dummy data
        fwrite(data, 1, size > 100 ? 100 : size, out_file);
        assert_file_closed(&out_file);;
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}