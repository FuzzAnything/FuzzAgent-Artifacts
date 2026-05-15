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
#include <aom/aom.h>
}
extern "C" {
#include <aom/aom_codec.h>
}
extern "C" {
#include <aom/aom_decoder.h>
}
extern "C" {
#include <aom/aom_encoder.h>
}
extern "C" {
#include <aom/aom_ext_ratectrl.h>
}
extern "C" {
#include <aom/aom_external_partition.h>
}
extern "C" {
#include <aom/aom_frame_buffer.h>
}
extern "C" {
#include <aom/aom_image.h>
}
extern "C" {
#include <aom/aomcx.h>
}
extern "C" {
#include <aom/aomdx.h>
}
//<ID> 11
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["aom_codec_build_config","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_iface_name","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_error","aom_codec_err_to_string","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_error","aom_codec_destroy","aom_codec_error","aom_codec_err_to_string","aom_codec_destroy"],"critical_calls":["aom_codec_build_config","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_iface_name","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_error","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Frame buffer callback structure
struct FrameBufferData {
    std::vector<uint8_t> buffer;
};

// Frame buffer get callback
int  id_000007_get_frame_buffer_cb(void *cb_priv, size_t min_size, aom_codec_frame_buffer_t *fb) {
    FrameBufferData *fb_data = static_cast<FrameBufferData*>(cb_priv);
    if (!fb_data) return -1;
    
    fb_data->buffer.resize(min_size);
    fb->data = fb_data->buffer.data();
    fb->size = min_size;
    return 0;
}

// Frame buffer release callback
int  id_000007_release_frame_buffer_cb(void *cb_priv, aom_codec_frame_buffer_t *fb) {
    // Nothing to do here - the buffer is managed by FrameBufferData
    (void)cb_priv;
    (void)fb;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Get build configuration
    const char *build_config = aom_codec_build_config();
    (void)build_config; // Use to avoid unused variable warning
    
    // Create a file from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = 128;
    dec_cfg.h = 128;
    dec_cfg.allow_lowbitdepth = 1;
    
    aom_codec_err_t res = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 
                                                 0, AOM_DECODER_ABI_VERSION);
    
    // Get decoder interface name
    const char *iface_name = aom_codec_iface_name(decoder_iface);
    (void)iface_name;
    
    // Set frame buffer functions
    FrameBufferData fb_data;
    res = aom_codec_set_frame_buffer_functions(&decoder,  id_000007_get_frame_buffer_cb, 
                                                id_000007_release_frame_buffer_cb, &fb_data);
    
    // Try to decode the input data
    if (res == AOM_CODEC_OK) {
        // Read some data from the file
        std::vector<uint8_t> buffer(4096);
        size_t bytes_read = fread(buffer.data(), 1, std::min(buffer.size(), size), in_file);
        
        if (bytes_read > 0) {
            res = aom_codec_decode(&decoder, buffer.data(), bytes_read, nullptr);
            
            // Get error string if decoding failed
            if (res != AOM_CODEC_OK) {
                const char *error_str = aom_codec_error(&decoder);
                (void)error_str;
                const char *err_to_str = aom_codec_err_to_string(res);
                (void)err_to_str;
            } else {
                // Try to get a frame
                aom_codec_iter_t iter = nullptr;
                aom_image_t *img = aom_codec_get_frame(&decoder, &iter);
                
                if (img) {
                    // Try to get metadata from the image
                    size_t num_metadata = aom_img_num_metadata(img);
                    if (num_metadata > 0) {
                        const aom_metadata_t *metadata = aom_img_get_metadata(img, 0);
                        (void)metadata;
                    }
                    
                    // Now set up encoder to re-encode the image
                    aom_codec_ctx_t encoder;
                    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
                    aom_codec_enc_cfg_t enc_cfg;
                    
                    if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0) == AOM_CODEC_OK) {
                        enc_cfg.g_w = img->d_w;
                        enc_cfg.g_h = img->d_h;
                        enc_cfg.g_timebase.num = 1;
                        enc_cfg.g_timebase.den = 30;
                        enc_cfg.rc_target_bitrate = 1000;
                        enc_cfg.g_error_resilient = 0;
                        
                        if (aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 
                                                   0, AOM_ENCODER_ABI_VERSION) == AOM_CODEC_OK) {
                            
                            // Set up output buffer for encoded data
                            aom_fixed_buf_t output_buf;
                            output_buf.buf = malloc(4096);
                            output_buf.sz = 4096;
                            
                            if (output_buf.buf) {
                                // Set the output buffer for compressed data
                                res = aom_codec_set_cx_data_buf(&encoder, &output_buf, 0, 0);
                                
                                // Try to encode the image
                                if (res == AOM_CODEC_OK) {
                                    res = aom_codec_encode(&encoder, img, 0, 1, 0);
                                    
                                    // Check for encoder errors
                                    if (res != AOM_CODEC_OK) {
                                        const char *encoder_error = aom_codec_error(&encoder);
                                        (void)encoder_error;
                                    }
                                }
                                
                                free(output_buf.buf);
                            }
                            
                            aom_codec_destroy(&encoder);
                        }
                    }
                }
            }
        }
    } else {
        // Get initialization error
        const char *init_error = aom_codec_error(&decoder);
        const char *err_to_str = aom_codec_err_to_string(res);
        (void)init_error;
        (void)err_to_str;
    }
    
    // Clean up decoder
    aom_codec_destroy(&decoder);
    
    // Clean up file
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}