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
//<ID> 146
//<Prompt> []
/*<Combination>: [
*/
//<score> 40, nr_unique_branch: 2
//<Quality> {"density":20,"unique_branches":{"aom_codec_enc_init_ver":[[74,14,74,34,0,0,4,0],[75,14,75,55,0,0,4,0]]},"library_calls":["aom_codec_version_str","aom_uleb_decode","aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_img_alloc","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_codec_destroy","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_free"],"critical_calls":["aom_codec_version_str","aom_uleb_decode","aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_img_alloc","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy","aom_codec_destroy","aom_img_alloc","aom_img_plane_width","aom_img_plane_height","aom_img_free"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	 // Too small for meaningful processing

    // 1. Get version string
    const char* version = aom_codec_version_str();
    (void)version; // Use version to avoid unused variable warning

    // 2. Try to decode a ULEB value from the beginning of input
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    size_t uleb_available = std::min(size, (size_t)10);
    int uleb_result = aom_uleb_decode(data, uleb_available, &uleb_value, &uleb_length);
    (void)uleb_result; // Result may be error, that's OK for fuzzing

    // 3. Create input FILE* from data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 4. Create output FILE* (will be cleaned up)
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 5. Decoder setup
    aom_codec_ctx_t decoder;
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;
    
    // Get decoder capabilities
    aom_codec_caps_t dec_caps = aom_codec_get_caps(decoder_iface);
    (void)dec_caps;
    
    aom_codec_err_t res = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    
    if (res == AOM_CODEC_OK) {
        // Try to decode the input data
        res = aom_codec_decode(&decoder, data, size, NULL);
        
        // Get stream info
        aom_codec_stream_info_t stream_info;
        memset(&stream_info, 0, sizeof(stream_info));
        aom_codec_get_stream_info(&decoder, &stream_info);
        
        // Try to get decoded frames
        aom_codec_iter_t iter = NULL;
        aom_image_t* decoded_frame = aom_codec_get_frame(&decoder, &iter);
        
        if (decoded_frame) {
            // Get plane dimensions
            int plane_width = aom_img_plane_width(decoded_frame, 0);
            int plane_height = aom_img_plane_height(decoded_frame, 0);
            (void)plane_width;
            (void)plane_height;
            
            // Get metadata count
            size_t metadata_count = aom_img_num_metadata(decoded_frame);
            (void)metadata_count;
            
            // 6. Encoder setup
            aom_codec_ctx_t encoder;
            aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
            aom_codec_enc_cfg_t enc_cfg;
            
            // Get default encoder configuration
            res = aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
            
            if (res == AOM_CODEC_OK) {
                // Set encoder dimensions from decoded frame
                enc_cfg.g_w = decoded_frame->d_w;
                enc_cfg.g_h = decoded_frame->d_h;
                enc_cfg.g_threads = 1;
                enc_cfg.g_bit_depth = (aom_bit_depth_t)decoded_frame->bit_depth;
                enc_cfg.rc_end_usage = AOM_Q;
                
                // Initialize encoder
                res = aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
                
                if (res == AOM_CODEC_OK) {
                    // Set an encoder option
                    aom_codec_set_option(&encoder, "cpu-used", "5");
                    
                    // Create an image for encoding (using same format as decoded frame)
                    aom_image_t* enc_image = aom_img_alloc(NULL, decoded_frame->fmt, 
                                                          decoded_frame->d_w, decoded_frame->d_h, 16);
                    
                    if (enc_image) {
                        // Copy data from decoded frame to encoder image (simplified)
                        // In real usage, you would copy actual pixel data
                        
                        // Encode the frame
                        res = aom_codec_encode(&encoder, enc_image, 0, 1, 0);
                        
                        // Get encoded data
                        aom_codec_iter_t enc_iter = NULL;
                        const aom_codec_cx_pkt_t* pkt;
                        while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != NULL) {
                            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                                // Write to output file
                                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                            }
                        }
                        
                        // Free the encoder image
                        aom_img_free(enc_image);
                    }
                    
                    // Destroy encoder
                    aom_codec_destroy(&encoder);
                }
            }
        }
        
        // Destroy decoder
        aom_codec_destroy(&decoder);
    }

    // 7. Test with a simple image allocation
    aom_image_t* test_image = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 64, 64, 16);
    if (test_image) {
        // Get plane dimensions
        int test_width = aom_img_plane_width(test_image, 0);
        int test_height = aom_img_plane_height(test_image, 0);
        (void)test_width;
        (void)test_height;
        
        // Free the test image
        aom_img_free(test_image);
    }

    // 8. Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Clean up output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}