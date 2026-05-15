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
//<ID> 20
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"aom_uleb_encode_fixed_size":[[83,34,83,66,0,0,4,0]]},"library_calls":["aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_set_option","aom_codec_set_option","aom_img_alloc","aom_codec_destroy","aom_uleb_encode_fixed_size","aom_img_add_metadata","aom_img_flip","aom_codec_decode","aom_codec_get_frame","aom_img_free","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_set_option","aom_codec_set_option","aom_img_alloc","aom_uleb_encode_fixed_size","aom_img_add_metadata","aom_img_flip","aom_codec_decode","aom_codec_get_frame","aom_img_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
        return 0;  // Need minimum data
    }

    // Open input data as file stream using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. First use aom_codec_peek_stream_info to analyze input stream
    aom_codec_iface_t *codec_iface = aom_codec_av1_dx();
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    // Read a chunk from input file for stream analysis
    uint8_t header_data[100];
    size_t bytes_read = fread(header_data, 1, (size < 100) ? size : 100, in_file);
    fseek(in_file, 0, SEEK_SET);  // Reset to beginning
    
    aom_codec_err_t err = aom_codec_peek_stream_info(
        codec_iface, 
        header_data, 
        bytes_read, 
        &stream_info
    );
    
    // 2. Create a codec context and set options
    aom_codec_ctx_t codec_ctx;
    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    
    err = aom_codec_dec_init_ver(&codec_ctx, codec_iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Use aom_codec_set_option to configure the decoder
    aom_codec_set_option(&codec_ctx, "error-concealment", "1");
    aom_codec_set_option(&codec_ctx, "skip-film-grain", "0");
    
    // 3. Create an image to work with
    unsigned int width = stream_info.w > 0 ? stream_info.w : 320;
    unsigned int height = stream_info.h > 0 ? stream_info.h : 240;
    aom_image_t *image = aom_img_alloc(NULL, AOM_IMG_FMT_I420, width, height, 1);
    if (!image) {
        aom_codec_destroy(&codec_ctx);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Use aom_uleb_encode_fixed_size to encode a value
    // Use a portion of input data as the value to encode
    uint64_t uleb_value = 0;
    size_t value_size = (size > 8) ? 8 : size;
    memcpy(&uleb_value, data, value_size);
    
    uint8_t coded_buffer[16];
    size_t coded_size = 0;
    size_t pad_size = 10;  // Pad to 10 bytes
    
    int encode_result = aom_uleb_encode_fixed_size(
        uleb_value, 
        sizeof(coded_buffer), 
        pad_size, 
        coded_buffer, 
        &coded_size
    );
    
    if (encode_result == 0) {  // Success
        // 5. Add metadata to the image using encoded ULEB data
        aom_img_add_metadata(
            image, 
            0x1234,  // Arbitrary metadata type
            coded_buffer, 
            coded_size, 
            AOM_MIF_ANY_FRAME
        );
    }
    
    // 6. Flip the image
    aom_img_flip(image);
    
    // 7. Try to decode the input data
    uint8_t *decode_buffer = (uint8_t*)malloc(size);
    if (decode_buffer) {
        fread(decode_buffer, 1, size, in_file);
        
        // Decode the data
        aom_codec_decode(&codec_ctx, decode_buffer, size, NULL);
        
        // Try to get decoded frame
        aom_codec_iter_t iter = NULL;
        aom_image_t *decoded_img = aom_codec_get_frame(&codec_ctx, &iter);
        
        if (decoded_img) {
            // We could process the decoded image here if needed
        }
        
        free(decode_buffer);
    }
    
    // Cleanup
    aom_img_free(image);
    aom_codec_destroy(&codec_ctx);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}