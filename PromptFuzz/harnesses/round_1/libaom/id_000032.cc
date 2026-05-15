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
//<ID> 69
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 0
//<Quality> {"density":24,"unique_branches":{},"library_calls":["aom_codec_version_extra_str","aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_destroy","aom_codec_destroy","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_set_rect","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_set_option","aom_codec_set_option","aom_codec_get_global_headers","aom_codec_encode","aom_codec_get_cx_data","aom_uleb_encode","aom_codec_decode","aom_codec_get_stream_info","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_version_extra_str","aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_set_rect","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_set_option","aom_codec_set_option","aom_codec_get_global_headers","aom_codec_encode","aom_codec_get_cx_data","aom_uleb_encode","aom_codec_decode","aom_codec_get_stream_info","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/*Looking at the provided APIs and the instruction to process libaom output data, I can see this is about creating a fuzzer that tests AV1 codec functionality - specifically decoding and potentially re-encoding video data. Let me create a comprehensive fuzzer that exercises these APIs.

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit for too small inputs
    if (size < 1) {
        return 0;
    }

    // Track resources to clean up
    aom_codec_ctx_t decoder = {0};
    aom_codec_ctx_t encoder = {0};
    aom_image_t* decoded_image = nullptr;
    aom_fixed_buf_t* global_headers = nullptr;
    FILE* in_file = nullptr;
    FILE* out_file = nullptr;
    
    // 1. Get extra version string (required API call)
    const char* extra_version = aom_codec_version_extra_str();
    (void)extra_version; // Prevent unused variable warning
    
    // 2. Get decoder interface
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) {
        return 0;
    }
    
    // 3. Try to peek stream info to understand the input
    aom_codec_stream_info_t stream_info = {0};
    aom_codec_peek_stream_info(decoder_iface, data, size, &stream_info);
    
    // 4. Initialize decoder
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1; // Use single thread for fuzzing
    dec_cfg.allow_lowbitdepth = 1;
    
    if (stream_info.w > 0 && stream_info.h > 0) {
        dec_cfg.w = stream_info.w;
        dec_cfg.h = stream_info.h;
    }
    
    aom_codec_err_t res = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 
                                                  0, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        // Try without config
        res = aom_codec_dec_init_ver(&decoder, decoder_iface, nullptr, 
                                      0, AOM_DECODER_ABI_VERSION);
        if (res != AOM_CODEC_OK) {
            return 0;
        }
    }
    
    // 5. Set frame buffer functions (even if we use default)
    aom_codec_set_frame_buffer_functions(&decoder, nullptr, nullptr, nullptr);
    
    // 6. Create input file from memory buffer
    in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 7. Read and decode the data
    // We'll try to decode in chunks to handle different input sizes
    size_t bytes_read = 0;
    const size_t chunk_size = 4096;
    uint8_t* buffer = (uint8_t*)malloc(chunk_size);
    if (!buffer) {
        assert_file_closed(&in_file);;
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }
    
    while (bytes_read < size) {
        size_t to_read = (size - bytes_read) < chunk_size ? 
                         (size - bytes_read) : chunk_size;
        
        memcpy(buffer, data + bytes_read, to_read);
        
        // Decode the chunk
        aom_codec_decode(&decoder, buffer, to_read, nullptr);
        bytes_read += to_read;
        
        // Break if we've processed enough
        if (to_read < chunk_size) {
            break;
        }
    }
    
    free(buffer);
    assert_file_closed(&in_file);;
    in_file = nullptr;
    
    // 8. Try to get stream info after decoding
    aom_codec_get_stream_info(&decoder, &stream_info);
    
    // 9. Try to get decoded frames
    aom_codec_iter_t iter = nullptr;
    decoded_image = aom_codec_get_frame(&decoder, &iter);
    
    // 10. If we got a frame, process it
    if (decoded_image) {
        // Apply a rectangle operation (required API call)
        // Use safe values based on image dimensions
        unsigned int x = 0;
        unsigned int y = 0;
        unsigned int w = decoded_image->d_w;
        unsigned int h = decoded_image->d_h;
        
        // Ensure dimensions are valid
        if (w > 0 && h > 0) {
            // Use smaller rectangle if image is large
            if (w > 100) w = 100;
            if (h > 100) h = 100;
            
            aom_img_set_rect(decoded_image, x, y, w, h, 0);
        }
        
        // 11. Get encoder interface
        aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
        if (encoder_iface && decoded_image->d_w > 0 && decoded_image->d_h > 0) {
            // 12. Initialize encoder with default config
            aom_codec_enc_cfg_t enc_cfg;
            if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0) == AOM_CODEC_OK) {
                // Set dimensions from decoded image
                enc_cfg.g_w = decoded_image->d_w;
                enc_cfg.g_h = decoded_image->d_h;
                
                // Initialize encoder
                if (aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 
                                           0, AOM_ENCODER_ABI_VERSION) == AOM_CODEC_OK) {
                    
                    // 13. Set some encoder options (required API call)
                    aom_codec_set_option(&encoder, "cpu-used", "5");
                    aom_codec_set_option(&encoder, "end-usage", "cq");
                    aom_codec_set_option(&encoder, "cq-level", "32");
                    
                    // 14. Get global headers
                    global_headers = aom_codec_get_global_headers(&encoder);
                    
                    // Create output file
                    out_file = fopen("output_file", "wb");
                    if (out_file) {
                        // 15. Encode the frame (required API call)
                        res = aom_codec_encode(&encoder, decoded_image, 0, 1, 0);
                        
                        if (res == AOM_CODEC_OK) {
                            // 16. Get encoded data
                            aom_codec_iter_t enc_iter = nullptr;
                            const aom_codec_cx_pkt_t* pkt;
                            
                            while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                                if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                                    // Write to file (simulating output)
                                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                                }
                            }
                        }
                        
                        assert_file_closed(&out_file);;
                        out_file = nullptr;
                    }
                    
                    // 17. Test uleb encoding (required API call)
                    uint8_t uleb_buffer[10];
                    size_t coded_size = 0;
                    uint64_t test_value = size; // Use input size as test value
                    aom_uleb_encode(test_value, sizeof(uleb_buffer), 
                                   uleb_buffer, &coded_size);
                    
                    // 18. Decode again with encoder output if we have data
                    if (global_headers && global_headers->buf && global_headers->sz > 0) {
                        // This simulates re-decoding encoded data
                        aom_codec_decode(&decoder, (const uint8_t*)global_headers->buf, 
                                       global_headers->sz, nullptr);
                        
                        // Get stream info again
                        aom_codec_get_stream_info(&decoder, &stream_info);
                    }
                }
            }
        }
    }
    
    // 19. Clean up (required API calls for destroy)
    if (encoder.iface) {
        aom_codec_destroy(&encoder);
    }
    
    // Free global headers if allocated
    if (global_headers) {
        free(global_headers->buf);
        free(global_headers);
    }
    
    // Destroy decoder
    aom_codec_destroy(&decoder);
    
    // Note: aom_img_free is not needed here because aom_codec_get_frame
    // returns images owned by the codec
    
    // Clean up any remaining file handles
    if (in_file) assert_file_closed(&in_file);;
    if (out_file) assert_file_closed(&out_file);;
    
    assert_file_closed(&in_file);
	return 0;
}