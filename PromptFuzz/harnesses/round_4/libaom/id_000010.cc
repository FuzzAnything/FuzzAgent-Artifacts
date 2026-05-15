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
//<ID> 29
//<Prompt> []
/*<Combination>: [
*/
//<score> 39, nr_unique_branch: 21
//<Quality> {"density":13,"unique_branches":{"aom_obu_type_to_string":[[188,5,188,29,0,0,4,0],[188,5,188,29,0,0,4,1],[189,5,189,32,0,0,4,0],[189,5,189,32,0,0,4,1],[190,5,190,26,0,0,4,0],[190,5,190,26,0,0,4,1],[191,5,191,36,0,0,4,0],[191,5,191,36,0,0,4,1],[192,5,192,19,0,0,4,0],[192,5,192,19,0,0,4,1],[193,5,193,24,0,0,4,0],[193,5,193,24,0,0,4,1],[194,5,194,22,0,0,4,0],[194,5,194,22,0,0,4,1],[195,5,195,23,0,0,4,0],[195,5,195,23,0,0,4,1],[196,5,196,21,0,0,4,1],[197,5,197,12,0,0,4,0],[197,5,197,12,0,0,4,1]],"aom_uleb_encode_fixed_size":[[82,7,82,34,0,0,4,0],[83,34,83,66,0,0,4,0]]},"library_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_img_alloc","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_get_global_headers","aom_codec_destroy","aom_uleb_encode_fixed_size","aom_img_flip","aom_obu_type_to_string","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_img_alloc","aom_img_metadata_alloc","aom_img_add_metadata","aom_img_metadata_free","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_get_global_headers","aom_codec_destroy","aom_uleb_encode_fixed_size","aom_img_flip","aom_obu_type_to_string","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<56) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimal data
    
    // 1. First use aom_codec_get_stream_info to analyze the input stream
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    aom_codec_ctx_t decoder_ctx;
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    
    // Get codec capabilities first
    aom_codec_caps_t caps = aom_codec_get_caps(decoder_iface);
    (void)caps; // Use caps to satisfy API requirement
    
    // Try to peek at stream info
    aom_codec_peek_stream_info(decoder_iface, data, size, &stream_info);
    
    // Initialize decoder
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    
    if (aom_codec_dec_init_ver(&decoder_ctx, decoder_iface, &dec_cfg, fuzz_int64_t_1, AOM_DECODER_ABI_VERSION) 
        != AOM_CODEC_OK) {
        return 0;
    }
    
    // Decode to get stream info properly
    aom_codec_decode(&decoder_ctx, data, size, nullptr);
    aom_codec_get_stream_info(&decoder_ctx, &stream_info);
    
    // 2. Create an image and add metadata using input data
    aom_image_t img;
    memset(&img, 0, sizeof(img));
    
    // Create a small dummy image
    const unsigned int width = 64;
    const unsigned int height = 64;
    const unsigned int align = 32;
    
    aom_img_alloc(&img, AOM_IMG_FMT_I420, fuzz_uint32_t_2, fuzz_uint32_t_3, fuzz_uint32_t_4);
    
    // Use a portion of input data for metadata
    size_t metadata_size = std::min(size / 4, (size_t)100);
    uint32_t metadata_type = 1; // Example type
    
    // First allocate metadata separately
    aom_metadata_t* metadata = aom_img_metadata_alloc(
        fuzz_uint32_t_5, 
        data, 
        metadata_size, 
        AOM_MIF_ANY_FRAME
    );
    
    if (metadata) {
        // Add metadata to image
        aom_img_add_metadata(&img, fuzz_uint32_t_6, data, metadata_size, AOM_MIF_ANY_FRAME);
        aom_img_metadata_free(metadata);
    }
    
    // 3. Get global headers from encoder
    aom_codec_ctx_t encoder_ctx;
    aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t enc_cfg;
    
    if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, fuzz_uint32_t_7) == AOM_CODEC_OK) {
        enc_cfg.g_w = width;
        enc_cfg.g_h = height;
        enc_cfg.g_threads = 1;
        enc_cfg.g_profile = 0;
        enc_cfg.rc_end_usage = AOM_Q;
        enc_cfg.rc_min_quantizer = 20;
        enc_cfg.rc_max_quantizer = 20;
        
        if (aom_codec_enc_init_ver(&encoder_ctx, encoder_iface, &enc_cfg, fuzz_int64_t_8, 
                                   AOM_ENCODER_ABI_VERSION) == AOM_CODEC_OK) {
            // Get global headers
            aom_fixed_buf_t* global_headers = aom_codec_get_global_headers(&encoder_ctx);
            if (global_headers) {
                free(global_headers->buf);
                free(global_headers);
            }
            
            aom_codec_destroy(&encoder_ctx);
        }
    }
    
    // 4. Use aom_uleb_encode_fixed_size with input data
    if (size >= 8) {
        uint64_t value = 0;
        for (int i = 0; i < 8 && i < (int)size; i++) {
            value = (value << 8) | data[i];
        }
        
        uint8_t encoded_buffer[16];
        size_t encoded_size = 0;
        aom_uleb_encode_fixed_size(fuzz_uint64_t_9, sizeof(encoded_buffer), fuzz_uint64_t_10, 
                                   encoded_buffer, &encoded_size);
    }
    
    // 5. Flip the image
    aom_img_flip(&img);
    
    // 6. Demonstrate aom_obu_type_to_string for different OBU types
    const char* obu_str = nullptr;
    for (int i = 0; i <= 10; i++) {
        obu_str = aom_obu_type_to_string(static_cast<OBU_TYPE>(i));
        (void)obu_str; // Use to satisfy requirement
    }
    
    // 7. Read additional data from input using FILE* if needed
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        uint8_t buffer[100];
        fread(buffer, 1, sizeof(buffer), in_file);
        assert_file_closed(&in_file);;
    }
    
    // 8. Write some output using FILE*
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        fwrite(data, 1, std::min(size, (size_t)100), out_file);
        assert_file_closed(&out_file);;
        
        // Optional: remove the output file
        remove("output_file");
    }
    
    // Cleanup
    aom_img_free(&img);
    aom_codec_destroy(&decoder_ctx);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}