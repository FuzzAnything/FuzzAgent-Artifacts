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
//<ID> 88
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_img_num_metadata","aom_img_remove_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc","aom_codec_encode","aom_codec_get_cx_data","aom_codec_encode","aom_codec_destroy","aom_img_free","aom_codec_destroy","aom_img_alloc","aom_img_add_metadata","aom_img_num_metadata","aom_img_remove_metadata","aom_img_free"],"critical_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_add_metadata","aom_img_num_metadata","aom_img_remove_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc","aom_codec_encode","aom_codec_get_cx_data","aom_codec_encode","aom_codec_destroy","aom_img_free","aom_codec_destroy","aom_img_alloc","aom_img_add_metadata","aom_img_num_metadata","aom_img_remove_metadata","aom_img_free"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful processing

    // 1. Print version (using first API)
    const char *version = aom_codec_version_str();
    // Just reference to avoid unused variable warning
    (void)version;

    // 2. Try to decode the input as AV1 bitstream
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_ctx_t decoder_ctx;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;

    aom_codec_err_t res = aom_codec_dec_init_ver(&decoder_ctx, decoder_iface, &dec_cfg, 0, 
                                                 AOM_DECODER_ABI_VERSION);

    // 3. Try to peek stream info (before actual decode)
    aom_codec_stream_info_t stream_info;
    if (res == AOM_CODEC_OK) {
        aom_codec_peek_stream_info(decoder_iface, data, size, &stream_info);
    }

    // 4. Actually decode
    if (res == AOM_CODEC_OK) {
        aom_codec_decode(&decoder_ctx, data, size, nullptr);
        
        // 5. Get stream info after decode
        aom_codec_get_stream_info(&decoder_ctx, &stream_info);
        
        // 6. Try to get decoded frames
        aom_codec_iter_t iter = nullptr;
        aom_image_t *decoded_img = nullptr;
        std::vector<aom_image_t*> decoded_frames;
        
        while ((decoded_img = aom_codec_get_frame(&decoder_ctx, &iter)) != nullptr) {
            decoded_frames.push_back(decoded_img);
            
            // 7. Use image APIs on decoded frame
            int plane_width = aom_img_plane_width(decoded_img, 0);
            int plane_height = aom_img_plane_height(decoded_img, 0);
            (void)plane_width;
            (void)plane_height;
            
            // 8. Test metadata APIs
            uint8_t metadata_data[] = {1, 2, 3, 4};
            aom_img_add_metadata(decoded_img, 1, metadata_data, sizeof(metadata_data), 
                               AOM_MIF_ANY_FRAME);
            
            size_t num_metadata = aom_img_num_metadata(decoded_img);
            (void)num_metadata;
            
            aom_img_remove_metadata(decoded_img);
        }
        
        // If we got decoded frames, try to re-encode them
        if (!decoded_frames.empty() && size > 0) {
            // 9. Setup encoder
            aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
            aom_codec_ctx_t encoder_ctx;
            aom_codec_enc_cfg_t enc_cfg;
            
            res = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
            
            if (res == AOM_CODEC_OK) {
                // Configure encoder based on decoded frame
                aom_image_t *first_frame = decoded_frames[0];
                enc_cfg.g_w = first_frame->d_w;
                enc_cfg.g_h = first_frame->d_h;
                enc_cfg.g_timebase.num = 1;
                enc_cfg.g_timebase.den = 30;
                enc_cfg.rc_target_bitrate = 1000;
                enc_cfg.g_threads = 1;
                enc_cfg.g_pass = AOM_RC_ONE_PASS;
                enc_cfg.g_lag_in_frames = 0;
                
                res = aom_codec_enc_init_ver(&encoder_ctx, encoder_iface, &enc_cfg, 0,
                                            AOM_ENCODER_ABI_VERSION);
                
                if (res == AOM_CODEC_OK) {
                    // 10. Create test image for encoding (using different API)
                    aom_image_t test_img;
                    aom_img_alloc(&test_img, AOM_IMG_FMT_I420, 64, 64, 16);
                    
                    // Fill with some data from input
                    size_t bytes_to_copy = std::min(size, (size_t)(test_img.stride[0] * test_img.h * 3 / 2));
                    if (bytes_to_copy > 0) {
                        memcpy(test_img.planes[0], data, 
                               std::min(bytes_to_copy, (size_t)(test_img.stride[0] * test_img.h)));
                    }
                    
                    // 11. Encode the test image
                    aom_codec_encode(&encoder_ctx, &test_img, 0, 1, 0);
                    
                    // 12. Get encoded data
                    aom_codec_iter_t enc_iter = nullptr;
                    const aom_codec_cx_pkt_t *pkt;
                    
                    while ((pkt = aom_codec_get_cx_data(&encoder_ctx, &enc_iter)) != nullptr) {
                        // Just process packet type
                        (void)pkt;
                    }
                    
                    // 13. Also try to encode one decoded frame
                    if (decoded_frames[0]) {
                        aom_codec_encode(&encoder_ctx, decoded_frames[0], 1, 1, 0);
                    }
                    
                    // 14. Cleanup encoder
                    aom_codec_destroy(&encoder_ctx);
                    
                    // 15. Free test image
                    aom_img_free(&test_img);
                }
            }
        }
        
        // 16. Cleanup decoder
        aom_codec_destroy(&decoder_ctx);
    }
    
    // 17. Test with different approach: try to decode with FILE* if size is large enough
    if (size > 1000) {
        // Create a temporary file with the data
        FILE *in_file = fmemopen((void *)data, size, "rb");
        if (in_file) {
            // Read some data from the file to use for metadata
            uint8_t file_data[100];
            size_t read_count = fread(file_data, 1, sizeof(file_data), in_file);
            
            // Create another test image
            aom_image_t *img = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, 32, 32, 16);
            if (img) {
                // Add metadata from file data
                if (read_count > 0) {
                    aom_img_add_metadata(img, 2, file_data, 
                                        std::min(read_count, (size_t)50), 
                                        AOM_MIF_KEY_FRAME);
                }
                
                // Check metadata count
                size_t meta_count = aom_img_num_metadata(img);
                (void)meta_count;
                
                // Remove metadata
                aom_img_remove_metadata(img);
                
                // Free image
                aom_img_free(img);
            }
            
            assert_file_closed(&in_file);;
        }
        
        // Also test writing to output file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            // Write some data from input to output file
            if (size > 0) {
                fwrite(data, 1, std::min(size, (size_t)100), out_file);
            }
            assert_file_closed(&out_file);;
            
            // Get file descriptor (as an example, though not used directly with libaom APIs)
            out_file = fopen("output_file", "rb");
            if (out_file) {
                int fd = fuzz_fileno(out_file);
                (void)fd;  // Avoid unused warning
                assert_file_closed(&out_file);;
            }
        }
    }
    
    return 0;
}