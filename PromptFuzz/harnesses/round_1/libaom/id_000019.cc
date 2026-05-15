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
//<ID> 30
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_destroy","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_num_metadata","aom_img_plane_width","aom_img_remove_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_version_str","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_num_metadata","aom_img_plane_width","aom_img_remove_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


// Include libaom headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if data is too small for any meaningful processing
    if (size < 16) {
	return 0;
	}
	
    
    // 1. Get libaom version string
    const char* version_str = aom_codec_version_str();
    (void)version_str; // Mark as used to avoid compiler warnings
    
    // 2. Create input file from fuzzer data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 3. Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Variables to track resources
    aom_codec_ctx_t decoder = {0};
    aom_codec_ctx_t encoder = {0};
    aom_codec_stream_info_t stream_info = {0};
    aom_codec_enc_cfg_t encoder_cfg = {0};
    aom_image_t* image = nullptr;
    aom_codec_iter_t iter = nullptr;
    
    // 4. Initialize decoder with AV1 interface
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    if (aom_codec_dec_init(&decoder, decoder_iface, nullptr, 0) != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 5. Read input data and decode
    std::vector<uint8_t> input_buffer(data, data + size);
    
    // Try to decode the input data
    if (aom_codec_decode(&decoder, input_buffer.data(), input_buffer.size(), nullptr) != AOM_CODEC_OK) {
        // Clean up and exit if decode fails
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 6. Get stream information
    if (aom_codec_get_stream_info(&decoder, &stream_info) == AOM_CODEC_OK) {
        // Use stream_info fields
        (void)stream_info.w;
        (void)stream_info.h;
        (void)stream_info.is_kf;
    }
    
    // 7. Get decoded frames
    while ((image = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
        // 8. Get image metadata count
        size_t metadata_count = aom_img_num_metadata(image);
        (void)metadata_count;
        
        // 9. Get plane width for Y plane (plane 0)
        int plane_width = aom_img_plane_width(image, 0);
        (void)plane_width;
        
        // 10. Remove metadata from image
        aom_img_remove_metadata(image);
        
        // 11. Initialize encoder configuration
        aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
        if (aom_codec_enc_config_default(encoder_iface, &encoder_cfg, 0) == AOM_CODEC_OK) {
            // Set basic encoder configuration
            encoder_cfg.g_w = image->d_w;
            encoder_cfg.g_h = image->d_h;
            encoder_cfg.g_timebase.num = 1;
            encoder_cfg.g_timebase.den = 30;
            encoder_cfg.rc_target_bitrate = 1000;
            encoder_cfg.g_pass = AOM_RC_ONE_PASS;
            encoder_cfg.g_lag_in_frames = 0;
            
            // 12. Initialize encoder
            if (aom_codec_enc_init(&encoder, encoder_iface, &encoder_cfg, 0) == AOM_CODEC_OK) {
                // 13. Encode the image
                if (aom_codec_encode(&encoder, image, 0, 1, 0) == AOM_CODEC_OK) {
                    // 14. Get encoded data packets
                    const aom_codec_cx_pkt_t* pkt = nullptr;
                    while ((pkt = aom_codec_get_cx_data(&encoder, &iter)) != nullptr) {
                        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                            // Write encoded data to output file
                            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                            break; // Just get first packet for fuzzing
                        }
                    }
                }
                
                // 15. Destroy encoder
                aom_codec_destroy(&encoder);
            }
        }
    }
    
    // 16. Clean up decoder
    aom_codec_destroy(&decoder);
    
    // 17. Clean up file resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // 18. Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}