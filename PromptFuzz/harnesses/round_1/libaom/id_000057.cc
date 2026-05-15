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
//<ID> 126
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_frame","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc","aom_img_alloc_with_border","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_img_remove_metadata","aom_codec_encode","aom_codec_get_cx_data","aom_codec_encode","aom_img_free","aom_img_free","aom_codec_error","aom_codec_error_detail","aom_codec_destroy","aom_codec_error","aom_codec_error_detail","aom_codec_destroy"],"critical_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_frame","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc","aom_img_alloc_with_border","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_img_remove_metadata","aom_codec_encode","aom_codec_get_cx_data","aom_codec_encode","aom_img_free","aom_img_free","aom_codec_error","aom_codec_error_detail","aom_codec_destroy","aom_codec_error","aom_codec_error_detail","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Get codec version (required API call)
    const char *version = aom_codec_version_str();
    (void)version; // Prevent unused variable warning

    // Create FILE* for input using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // First, try to decode the input as an AV1 bitstream
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    
    // Configure decoder
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    aom_codec_err_t res = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, 
                                                  AOM_DECODER_ABI_VERSION);
    
    if (res != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Try to peek stream info
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    
    // Read a chunk to peek at stream info
    std::vector<uint8_t> peek_buffer(4096);
    size_t bytes_read = fread(peek_buffer.data(), 1, 
                             std::min(peek_buffer.size(), size), in_file);
    fseek(in_file, 0, SEEK_SET); // Reset file pointer
    
    if (bytes_read > 0) {
        aom_codec_peek_stream_info(decoder_iface, peek_buffer.data(), 
                                   bytes_read, &si);
    }

    // Read and decode entire input
    std::vector<uint8_t> input_buffer(size);
    bytes_read = fread(input_buffer.data(), 1, size, in_file);
    assert_file_closed(&in_file);;
    
    if (bytes_read > 0) {
        aom_codec_decode(&decoder, input_buffer.data(), bytes_read, nullptr);
    }

    // Get decoded frames
    std::vector<aom_image_t*> decoded_frames;
    aom_codec_iter_t iter = nullptr;
    aom_image_t *img;
    while ((img = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
        decoded_frames.push_back(img);
    }

    // If we got any decoded frames, try to re-encode them
    if (!decoded_frames.empty()) {
        // Get encoder interface
        aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
        
        // Configure encoder
        aom_codec_enc_cfg_t enc_cfg;
        res = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
        
        if (res == AOM_CODEC_OK) {
            // Set basic encoding parameters
            enc_cfg.g_w = decoded_frames[0]->d_w;
            enc_cfg.g_h = decoded_frames[0]->d_h;
            enc_cfg.g_timebase.num = 1;
            enc_cfg.g_timebase.den = 30;
            enc_cfg.rc_target_bitrate = 1000;
            enc_cfg.g_pass = AOM_RC_ONE_PASS;
            enc_cfg.g_lag_in_frames = 0;
            enc_cfg.g_error_resilient = 0;
            
            aom_codec_ctx_t encoder;
            res = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0,
                                         AOM_ENCODER_ABI_VERSION);
            
            if (res == AOM_CODEC_OK) {
                // Allocate images with and without border for testing
                aom_image_t *img_no_border = aom_img_alloc(nullptr, AOM_IMG_FMT_I420,
                                                          enc_cfg.g_w, enc_cfg.g_h, 16);
                
                aom_image_t *img_with_border = aom_img_alloc_with_border(nullptr,
                                                                        AOM_IMG_FMT_I420,
                                                                        enc_cfg.g_w,
                                                                        enc_cfg.g_h,
                                                                        16, 32, 16);
                
                // Test plane dimensions API
                if (img_no_border) {
                    int plane_width = aom_img_plane_width(img_no_border, 0);
                    int plane_height = aom_img_plane_height(img_no_border, 0);
                    (void)plane_width;
                    (void)plane_height;
                    
                    // Test metadata APIs
                    size_t metadata_count = aom_img_num_metadata(img_no_border);
                    (void)metadata_count;
                    
                    // Remove metadata (even if there isn't any)
                    aom_img_remove_metadata(img_no_border);
                }
                
                // Encode each decoded frame
                for (size_t i = 0; i < decoded_frames.size(); i++) {
                    aom_codec_encode(&encoder, decoded_frames[i], i, 1, 0);
                    
                    // Get encoded data
                    const aom_codec_cx_pkt_t *pkt;
                    aom_codec_iter_t enc_iter = nullptr;
                    
                    while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                        // Packet received, but we don't need to do anything with it for fuzzing
                        (void)pkt;
                    }
                }
                
                // Flush encoder
                aom_codec_encode(&encoder, nullptr, 0, 0, 0);
                
                // Clean up allocated images
                if (img_no_border) {
                    aom_img_free(img_no_border);
                }
                if (img_with_border) {
                    aom_img_free(img_with_border);
                }
                
                // Get error info (just to call the API)
                const char *error_str = aom_codec_error(&encoder);
                const char *error_detail = aom_codec_error_detail(&encoder);
                (void)error_str;
                (void)error_detail;
                
                aom_codec_destroy(&encoder);
            }
        }
    }
    
    // Get error info from decoder (just to call the API)
    const char *dec_error_str = aom_codec_error(&decoder);
    const char *dec_error_detail = aom_codec_error_detail(&decoder);
    (void)dec_error_str;
    (void)dec_error_detail;
    
    // Clean up
    aom_codec_destroy(&decoder);
    
    assert_file_closed(&in_file);
	return 0;
}