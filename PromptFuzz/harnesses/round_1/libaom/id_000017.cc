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
//<ID> 28
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_peek_stream_info","aom_codec_err_to_string","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_set_option","aom_codec_decode","aom_codec_err_to_string","aom_codec_get_stream_info","aom_codec_err_to_string","aom_codec_get_frame","aom_img_alloc","aom_img_alloc_with_border","aom_img_add_metadata","aom_img_flip","aom_img_free","aom_img_free","aom_uleb_encode_fixed_size","aom_uleb_decode","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_peek_stream_info","aom_codec_err_to_string","aom_codec_dec_init_ver","aom_codec_set_option","aom_codec_decode","aom_codec_err_to_string","aom_codec_get_stream_info","aom_codec_err_to_string","aom_codec_get_frame","aom_img_alloc","aom_img_alloc_with_border","aom_img_add_metadata","aom_img_flip","aom_img_free","aom_img_free","aom_uleb_encode_fixed_size","aom_uleb_decode","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if we don't have enough data for basic operations
    if (size < 4) {
	return 0;
	}
	

    // 1. Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // 2. Read the entire input file
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    if (file_size <= 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    uint8_t *file_data = new uint8_t[file_size];
    size_t read_size = fread(file_data, 1, file_size, in_file);
    assert_file_closed(&in_file);;

    if (read_size != (size_t)file_size) {
        delete[] file_data;
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Get the AV1 decoder interface
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) {
        delete[] file_data;
        assert_file_closed(&in_file);
	return 0;
    }

    // 4. Use aom_codec_get_caps to check decoder capabilities
    aom_codec_caps_t caps = aom_codec_get_caps(decoder_iface);
    (void)caps; // Use caps to avoid unused variable warning

    // 5. Peek stream info from the input data
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    aom_codec_err_t peek_err = aom_codec_peek_stream_info(decoder_iface, file_data, file_size, &stream_info);
    if (peek_err != AOM_CODEC_OK) {
        const char *err_str = aom_codec_err_to_string(peek_err);
        (void)err_str; // Use err_str to avoid unused variable warning
    }

    // 6. Initialize decoder configuration
    aom_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;
    
    if (stream_info.w > 0 && stream_info.h > 0) {
        dec_cfg.w = stream_info.w;
        dec_cfg.h = stream_info.h;
    } else {
        dec_cfg.w = 128;
        dec_cfg.h = 128;
    }

    // 7. Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_err_t init_err = aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    
    if (init_err != AOM_CODEC_OK) {
        const char *err_str = aom_codec_err_to_string(init_err);
        (void)err_str;
        delete[] file_data;
        assert_file_closed(&in_file);
	return 0;
    }

    // 8. Set decoder options (example: set error resilient mode)
    aom_codec_set_option(&decoder, "error-resilient", "1");

    // 9. Decode the input data
    aom_codec_err_t decode_err = aom_codec_decode(&decoder, file_data, file_size, nullptr);
    
    if (decode_err != AOM_CODEC_OK) {
        const char *err_str = aom_codec_err_to_string(decode_err);
        (void)err_str;
    }

    // 10. Get stream info after decoding
    aom_codec_err_t get_info_err = aom_codec_get_stream_info(&decoder, &stream_info);
    if (get_info_err != AOM_CODEC_OK) {
        const char *err_str = aom_codec_err_to_string(get_info_err);
        (void)err_str;
    }

    // 11. Get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t *img = nullptr;
    
    while ((img = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
        // 12. Allocate a new image with same format
        aom_image_t *allocated_img = aom_img_alloc(nullptr, img->fmt, img->d_w, img->d_h, 16);
        
        // 13. Also allocate an image with border
        unsigned int border = 32;
        aom_image_t *bordered_img = aom_img_alloc_with_border(nullptr, img->fmt, img->d_w, img->d_h, 16, 1, border);
        
        if (allocated_img) {
            // 14. Add metadata to the allocated image
            // Use first 4 bytes of input as metadata type and next 128 bytes as data
            uint32_t metadata_type = 0;
            if (size >= 4) {
                metadata_type = *(reinterpret_cast<const uint32_t*>(data));
            }
            
            size_t metadata_size = size > 132 ? 128 : (size > 4 ? size - 4 : 0);
            const uint8_t *metadata_data = size > 4 ? data + 4 : nullptr;
            
            if (metadata_size > 0 && metadata_data) {
                aom_img_add_metadata(allocated_img, metadata_type, metadata_data, metadata_size, AOM_MIF_KEY_FRAME);
            }
            
            // 15. Flip the image
            aom_img_flip(allocated_img);
            
            // 16. Free the allocated image
            aom_img_free(allocated_img);
        }
        
        if (bordered_img) {
            aom_img_free(bordered_img);
        }
    }

    // 17. ULEB encoding/decoding test using input data
    if (size >= 8) {
        // Use first 8 bytes as a 64-bit value
        uint64_t value = 0;
        for (int i = 0; i < 8 && i < (int)size; i++) {
            value |= (uint64_t)data[i] << (i * 8);
        }
        
        // Encode with fixed size
        uint8_t encoded_buffer[16];
        size_t encoded_size = 0;
        
        int encode_result = aom_uleb_encode_fixed_size(value, sizeof(encoded_buffer), 8, encoded_buffer, &encoded_size);
        
        if (encode_result == 0 && encoded_size > 0) {
            // Decode back
            uint64_t decoded_value = 0;
            size_t decoded_length = 0;
            
            int decode_result = aom_uleb_decode(encoded_buffer, encoded_size, &decoded_value, &decoded_length);
            (void)decode_result; // Use decode_result to avoid unused variable warning
        }
    }

    // 18. Clean up
    delete[] file_data;
    aom_codec_destroy(&decoder);
    
    assert_file_closed(&in_file);
	return 0;
}