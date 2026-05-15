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
//<ID> 24
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 1
//<Quality> {"density":10,"unique_branches":{"aom_uleb_encode_fixed_size":[[87,7,87,21,0,0,4,0]]},"library_calls":["aom_codec_av1_dx","aom_codec_peek_stream_info","aom_uleb_encode_fixed_size","aom_codec_dec_init_ver","aom_codec_set_option","aom_codec_decode","aom_codec_get_frame","aom_img_add_metadata","aom_img_flip","aom_codec_get_stream_info","aom_codec_destroy","aom_img_alloc","aom_img_add_metadata","aom_img_flip","aom_img_free","aom_uleb_decode","aom_uleb_encode_fixed_size"],"critical_calls":["aom_codec_av1_dx","aom_codec_peek_stream_info","aom_uleb_encode_fixed_size","aom_codec_dec_init_ver","aom_codec_set_option","aom_codec_decode","aom_codec_get_frame","aom_img_add_metadata","aom_img_flip","aom_codec_get_stream_info","aom_codec_destroy","aom_img_alloc","aom_img_add_metadata","aom_img_flip","aom_img_free","aom_uleb_decode","aom_uleb_encode_fixed_size"],"visited":1}
/*Looking at the provided APIs, I can create a fuzz driver that simulates processing AV1 encoded data, adding metadata, and potentially manipulating the decoded images. The event could be: "Process encoded AV1 stream to extract frame information, decode frames, add metadata to decoded frames, and manipulate frame orientation."

Here's the complete fuzzer implementation:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if input is too small
    if (size < 4) {
        return 0;
    }

    // Create output file for encoded ULEB data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&out_file);
	return 0;
    }

    // 1. First, peek at stream information using the encoded data
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    aom_codec_err_t peek_result = aom_codec_peek_stream_info(
        decoder_iface, data, size, &stream_info);
    
    // 2. Encode a ULEB value from the input data
    // Extract first 8 bytes as potential value, or use size if available
    uint64_t uleb_value = 0;
    size_t value_bytes = (size > 8) ? 8 : size;
    for (size_t i = 0; i < value_bytes; i++) {
        uleb_value |= (uint64_t)data[i] << (i * 8);
    }
    
    // Reserve buffer for encoded ULEB
    const size_t max_encoded_size = 16;
    uint8_t encoded_buffer[max_encoded_size];
    size_t encoded_size = 0;
    
    // Use fixed size encoding (pad to 4 bytes)
    int encode_result = aom_uleb_encode_fixed_size(
        uleb_value, max_encoded_size, 4, 
        encoded_buffer, &encoded_size);
    
    if (encode_result == 0 && encoded_size > 0) {
        fwrite(encoded_buffer, 1, encoded_size, out_file);
    }
    
    // 3. Create decoder context and set options
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;
    
    aom_codec_err_t init_result = aom_codec_dec_init_ver(
        &decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    
    if (init_result == AOM_CODEC_OK) {
        // Set some decoder options using the input data
        // Use first non-zero byte as option value
        char option_value[32];
        for (size_t i = 0; i < size && i < 4; i++) {
            if (data[i] != 0) {
                snprintf(option_value, sizeof(option_value), "%u", data[i]);
                aom_codec_set_option(&decoder, "tile-rows", option_value);
                break;
            }
        }
        
        // Try to decode the input data
        aom_codec_decode(&decoder, data, size, nullptr);
        
        // Get decoded frames
        aom_codec_iter_t iter = nullptr;
        aom_image_t *img = nullptr;
        
        while ((img = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
            // 4. Add metadata to the decoded image
            // Use middle portion of input data as metadata
            size_t metadata_offset = size / 2;
            size_t metadata_size = (size - metadata_offset) / 2;
            
            if (metadata_size > 0 && metadata_offset < size) {
                // Use first byte as metadata type
                uint32_t metadata_type = (metadata_offset < size) ? data[metadata_offset] : 1;
                
                // Add metadata to image
                aom_img_add_metadata(
                    img, 
                    metadata_type,
                    data + metadata_offset,
                    (metadata_size > 256) ? 256 : metadata_size,
                    AOM_MIF_ANY_FRAME
                );
                
                // 5. Flip the image
                aom_img_flip(img);
            }
            
            // Write image dimensions to output file
            uint32_t dims[2] = {img->d_w, img->d_h};
            fwrite(dims, sizeof(uint32_t), 2, out_file);
            
            // Try to get stream info from decoder context
            aom_codec_get_stream_info(&decoder, &stream_info);
        }
        
        // Clean up decoder
        aom_codec_destroy(&decoder);
    }
    
    // 6. Create an image for additional testing
    // Use input data to determine dimensions
    unsigned int width = 64;
    unsigned int height = 64;
    
    if (size >= 8) {
        width = (data[0] << 8) | data[1];
        height = (data[2] << 8) | data[3];
        
        // Clamp to reasonable values
        if (width > 2048) width = 2048;
        if (height > 2048) height = 2048;
        if (width < 16) width = 16;
        if (height < 16) height = 16;
    }
    
    aom_image_t test_img;
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    
    // Allocate image
    if (aom_img_alloc(&test_img, fmt, width, height, 16)) {
        // Add metadata to test image
        size_t meta_size = (size > 100) ? 100 : size;
        aom_img_add_metadata(
            &test_img, 
            2,  // Custom metadata type
            data, 
            meta_size,
            AOM_MIF_KEY_FRAME
        );
        
        // Flip test image
        aom_img_flip(&test_img);
        
        // Write test image info
        uint32_t test_dims[2] = {test_img.d_w, test_img.d_h};
        fwrite(test_dims, sizeof(uint32_t), 2, out_file);
        
        // Clean up
        aom_img_free(&test_img);
    }
    
    // 7. Test ULEB decoding
    if (size >= 4) {
        uint64_t decoded_value = 0;
        size_t decode_length = 0;
        
        // Try to decode ULEB from beginning of input
        if (aom_uleb_decode(data, (size > 10) ? 10 : size, 
                           &decoded_value, &decode_length) == 0) {
            // Encode the decoded value back
            uint8_t reencoded[16];
            size_t reencoded_size = 0;
            
            aom_uleb_encode_fixed_size(
                decoded_value, 
                sizeof(reencoded), 
                4, 
                reencoded, 
                &reencoded_size
            );
            
            if (reencoded_size > 0) {
                fwrite(reencoded, 1, reencoded_size, out_file);
            }
        }
    }
    
    // Clean up resources
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_name_closed("output_file");
	return 0;
}