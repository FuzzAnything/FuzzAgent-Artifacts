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
//<ID> 25
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_err_to_string","aom_codec_set_option","aom_codec_set_option","aom_codec_destroy","aom_codec_decode","aom_codec_err_to_string","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_alloc_with_border","aom_img_flip","aom_img_add_metadata","aom_img_alloc","aom_uleb_encode_fixed_size","aom_uleb_decode","aom_img_free","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_get_caps","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_set_option","aom_codec_set_option","aom_codec_decode","aom_codec_err_to_string","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_alloc_with_border","aom_img_flip","aom_img_add_metadata","aom_img_alloc","aom_uleb_encode_fixed_size","aom_uleb_decode","aom_img_free","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create a FILE pointer for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. Get decoder interface
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Get codec capabilities
    aom_codec_caps_t caps = aom_codec_get_caps(iface);

    // 3. Peek stream info from the input data
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    // Read some data for peek_stream_info
    uint8_t peek_buffer[4096];
    size_t peek_size = fread(peek_buffer, 1, sizeof(peek_buffer), in_file);
    fseek(in_file, 0, SEEK_SET);  // Reset to beginning
    
    aom_codec_err_t err = aom_codec_peek_stream_info(iface, peek_buffer, peek_size, &stream_info);

    // 4. Initialize decoder
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;
    
    err = aom_codec_dec_init_ver(&decoder, iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        const char* err_str = aom_codec_err_to_string(err);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 5. Set decoder options
    aom_codec_set_option(&decoder, "tile-rows", "1");
    aom_codec_set_option(&decoder, "tile-columns", "1");

    // 6. Decode the input data
    fseek(in_file, 0, SEEK_SET);
    uint8_t *decode_buffer = (uint8_t*)malloc(size);
    if (!decode_buffer) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t read_size = fread(decode_buffer, 1, size, in_file);
    err = aom_codec_decode(&decoder, decode_buffer, read_size, NULL);
    
    if (err != AOM_CODEC_OK) {
        const char* err_str = aom_codec_err_to_string(err);
    }

    // 7. Get stream info after decoding
    aom_codec_stream_info_t post_stream_info;
    memset(&post_stream_info, 0, sizeof(post_stream_info));
    aom_codec_get_stream_info(&decoder, &post_stream_info);

    // 8. Get decoded frames
    aom_codec_iter_t iter = NULL;
    aom_image_t *decoded_img = aom_codec_get_frame(&decoder, &iter);
    
    if (decoded_img) {
        // 9. Create a new image with border
        aom_image_t *new_img = aom_img_alloc_with_border(
            NULL, 
            decoded_img->fmt,
            decoded_img->d_w,
            decoded_img->d_h,
            1,  // align
            16, // size_align
            8   // border
        );
        
        if (new_img) {
            // 10. Flip the new image
            aom_img_flip(new_img);
            
            // 11. Add metadata to the new image
            // Use first 16 bytes of input as metadata
            size_t meta_size = size > 16 ? 16 : size;
            aom_img_add_metadata(
                new_img,
                1,  // type
                data,
                meta_size,
                AOM_MIF_ANY_FRAME
            );
            
            // 12. Create another image without border
            aom_image_t *simple_img = aom_img_alloc(
                NULL,
                AOM_IMG_FMT_I420,
                64,  // d_w
                64,  // d_h
                1    // align
            );
            
            if (simple_img) {
                // 13. Test ULEB encoding/decoding
                uint64_t test_value = size;
                uint8_t uleb_buffer[16];
                size_t coded_size = 0;
                
                // Encode
                int encode_result = aom_uleb_encode_fixed_size(
                    test_value,
                    sizeof(uleb_buffer),
                    8,  // pad to 8 bytes
                    uleb_buffer,
                    &coded_size
                );
                
                // Decode
                uint64_t decoded_value = 0;
                size_t decoded_length = 0;
                if (encode_result == 0) {
                    aom_uleb_decode(
                        uleb_buffer,
                        coded_size,
                        &decoded_value,
                        &decoded_length
                    );
                }
                
                aom_img_free(simple_img);
            }
            
            aom_img_free(new_img);
        }
    }

    // 14. Clean up
    free(decode_buffer);
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}