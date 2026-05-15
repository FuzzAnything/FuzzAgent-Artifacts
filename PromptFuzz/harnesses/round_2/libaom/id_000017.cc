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
//<ID> 32
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_set_frame_buffer_functions","aom_codec_error_detail","aom_codec_decode","aom_codec_get_frame","aom_codec_enc_config_default","aom_codec_av1_cx","aom_codec_enc_init_ver","aom_codec_av1_cx","aom_codec_enc_config_set","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_uleb_encode","aom_uleb_encode_fixed_size","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_dec_init_ver","aom_codec_av1_dx","aom_codec_set_frame_buffer_functions","aom_codec_error_detail","aom_codec_decode","aom_codec_get_frame","aom_codec_enc_config_default","aom_codec_av1_cx","aom_codec_enc_init_ver","aom_codec_av1_cx","aom_codec_enc_config_set","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_uleb_encode","aom_uleb_encode_fixed_size","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Frame buffer callback structure
struct FrameBufferData {
    std::vector<uint8_t> buffer;
    size_t size;
};

// Frame buffer get callback
int  id_000017_get_frame_buffer(void* cb_priv, size_t min_size, aom_codec_frame_buffer_t* fb) {
    FrameBufferData* fb_data = static_cast<FrameBufferData*>(cb_priv);
    
    if (min_size > fb_data->buffer.size()) {
        fb_data->buffer.resize(min_size);
        fb_data->size = min_size;
    }
    
    fb->data = fb_data->buffer.data();
    fb->size = fb_data->buffer.size();
    return 0;
}

// Frame buffer release callback
int  id_000017_release_frame_buffer(void* cb_priv, aom_codec_frame_buffer_t* fb) {
    // Nothing to do - we manage the buffer in FrameBufferData
    (void)cb_priv;
    (void)fb;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // Create input file from fuzzer data
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
    
    // Create decoder context
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg = aom_codec_dec_cfg_t();
    dec_cfg.threads = 1;
    
    // Initialize decoder
    if (aom_codec_dec_init(&decoder, aom_codec_av1_dx(), &dec_cfg, 0) != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set frame buffer functions
    FrameBufferData fb_data;
    if (aom_codec_set_frame_buffer_functions(&decoder, 
                                              id_000017_get_frame_buffer, 
                                              id_000017_release_frame_buffer, 
                                             &fb_data) != AOM_CODEC_OK) {
        // Get error detail
        const char* error_detail = aom_codec_error_detail(&decoder);
        (void)error_detail; // Use error detail
    }
    
    // Read and decode data
    std::vector<uint8_t> buffer(size);
    size_t bytes_read = fread(buffer.data(), 1, size, in_file);
    
    if (bytes_read > 0) {
        // Decode the frame
        aom_codec_decode(&decoder, buffer.data(), bytes_read, nullptr);
        
        // Get decoded frame
        aom_codec_iter_t iter = nullptr;
        aom_image_t* img = aom_codec_get_frame(&decoder, &iter);
        
        if (img) {
            // Create encoder context
            aom_codec_ctx_t encoder;
            aom_codec_enc_cfg_t enc_cfg;
            
            // Initialize encoder with default configuration
            if (aom_codec_enc_config_default(aom_codec_av1_cx(), &enc_cfg, 0) == AOM_CODEC_OK) {
                enc_cfg.g_w = img->d_w;
                enc_cfg.g_h = img->d_h;
                enc_cfg.g_threads = 1;
                enc_cfg.rc_end_usage = AOM_Q;
                
                // Initialize encoder
                if (aom_codec_enc_init(&encoder, aom_codec_av1_cx(), &enc_cfg, 0) == AOM_CODEC_OK) {
                    // Set encoder configuration
                    aom_codec_enc_config_set(&encoder, &enc_cfg);
                    
                    // Create output buffer for encoded data
                    aom_fixed_buf_t output_buf;
                    output_buf.sz = 1024 * 1024; // 1MB buffer
                    output_buf.buf = malloc(output_buf.sz);
                    
                    if (output_buf.buf) {
                        // Set CX data buffer with padding
                        aom_codec_set_cx_data_buf(&encoder, &output_buf, 16, 16);
                        
                        // Encode the frame
                        aom_codec_encode(&encoder, img, 0, 1, 0);
                        
                        // Get encoded data
                        aom_codec_iter_t enc_iter = nullptr;
                        const aom_codec_cx_pkt_t* pkt = nullptr;
                        
                        while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                                // Write encoded data to output file
                                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                                
                                // Use ULEB encoding to encode frame size
                                uint8_t uleb_buffer[10];
                                size_t uleb_size = 0;
                                
                                // Encode frame size using ULEB
                                aom_uleb_encode(pkt->data.frame.sz, 
                                               sizeof(uleb_buffer),
                                               uleb_buffer,
                                               &uleb_size);
                                
                                // Also try fixed size encoding
                                uint8_t fixed_uleb_buffer[10];
                                size_t fixed_uleb_size = 0;
                                aom_uleb_encode_fixed_size(pkt->data.frame.sz,
                                                          sizeof(fixed_uleb_buffer),
                                                          5,
                                                          fixed_uleb_buffer,
                                                          &fixed_uleb_size);
                                
                                // Write ULEB encoded size to output
                                fwrite(uleb_buffer, 1, uleb_size, out_file);
                            }
                        }
                        
                        free(output_buf.buf);
                    }
                    
                    aom_codec_destroy(&encoder);
                }
            }
        }
    }
    
    // Cleanup
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}