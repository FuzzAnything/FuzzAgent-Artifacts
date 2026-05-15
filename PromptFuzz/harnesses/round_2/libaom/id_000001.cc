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
//<ID> 3
//<Prompt> []
/*<Combination>: [
*/
//<score> 39, nr_unique_branch: 4
//<Quality> {"density":13,"unique_branches":{"decoder_decode":[[700,7,700,32,0,0,4,1]],"release_pending_output_frames":[[569,7,569,24,0,0,4,0],[575,24,575,50,0,0,4,1],[580,24,580,62,0,0,4,1]]},"library_calls":["aom_codec_version","aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_destroy","aom_codec_destroy","aom_codec_decode","aom_codec_get_frame","aom_codec_get_global_headers","aom_uleb_decode","aom_uleb_size_in_bytes","aom_uleb_encode","aom_obu_type_to_string","aom_codec_peek_stream_info","aom_codec_destroy"],"critical_calls":["aom_codec_version","aom_codec_av1_dx","aom_codec_get_caps","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_codec_get_global_headers","aom_uleb_decode","aom_uleb_size_in_bytes","aom_uleb_encode","aom_obu_type_to_string","aom_codec_peek_stream_info","aom_codec_destroy"],"visited":0}
/*Looking at the provided APIs and requirements, I'll create a fuzz driver that processes AV1 bitstream data. The event will be: decoding AV1 bitstream data to understand its structure and metadata.

*/


// Include libaom headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Check if we have enough data
    if (size == 0 || data == nullptr) {
        return 0;
    }

    // 2. Get library version (first API usage)
    int version = aom_codec_version();
    // We don't use the version value, but the function is called as required

    // 3. Get decoder interface (second API usage)
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) {
        return 0;
    }

    // 4. Get decoder capabilities (third API usage)
    aom_codec_caps_t caps = aom_codec_get_caps(decoder_iface);
    // Capabilities value is not used further, but API is called

    // 5. Create decoder context
    aom_codec_ctx_t decoder;
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;  // Single thread for fuzzing

    // Initialize decoder
    aom_codec_err_t err = aom_codec_dec_init(&decoder, decoder_iface, &dec_cfg, 0);
    if (err != AOM_CODEC_OK) {
        // Cleanup and return if initialization fails
        return 0;
    }

    // 6. Create FILE* for input data using fmemopen (as per requirement #4)
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // 7. Create output file (as per requirement #4)
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        aom_codec_destroy(&decoder);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 8. Prepare to read and decode the bitstream
    // Read data in chunks
    const size_t buffer_size = 4096;
    uint8_t buffer[buffer_size];
    
    // Track position in input
    size_t bytes_processed = 0;
    
    while (bytes_processed < size) {
        size_t bytes_to_read = (size - bytes_processed) < buffer_size ? 
                               (size - bytes_processed) : buffer_size;
        
        // Read chunk from file
        size_t bytes_read = fread(buffer, 1, bytes_to_read, in_file);
        if (bytes_read == 0) {
            break;
        }
        
        // Decode the chunk
        err = aom_codec_decode(&decoder, buffer, bytes_read, NULL);
        
        // Get decoded frames
        aom_codec_iter_t iter = NULL;
        const aom_image_t *img;
        while ((img = aom_codec_get_frame(&decoder, &iter)) != NULL) {
            // Process the decoded frame if needed
            // For fuzzing, we just get the frame to exercise the API
        }
        
        bytes_processed += bytes_read;
    }

    // 9. Try to get global headers (fourth API usage)
    // This might fail if no global headers are present, but we call it anyway
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&decoder);
    if (global_headers) {
        // Optional: Write global headers to output file
        if (global_headers->buf && global_headers->sz > 0) {
            fwrite(global_headers->buf, 1, global_headers->sz, out_file);
        }
    }

    // 10. Process ULEB encoded values in the input (fifth API usage)
    // Try to find and decode ULEB values in the input
    size_t uleb_processed = 0;
    while (uleb_processed < size) {
        uint64_t uleb_value;
        size_t uleb_length;
        
        int uleb_result = aom_uleb_decode(data + uleb_processed, 
                                         size - uleb_processed, 
                                         &uleb_value, 
                                         &uleb_length);
        
        if (uleb_result == 0 && uleb_length > 0) {
            // Calculate ULEB size in bytes (first API usage)
            size_t uleb_size = aom_uleb_size_in_bytes(uleb_value);
            
            // Try to encode it back
            uint8_t encoded_buffer[16];
            size_t encoded_size;
            aom_uleb_encode(uleb_value, sizeof(encoded_buffer), 
                           encoded_buffer, &encoded_size);
            
            uleb_processed += uleb_length;
        } else {
            // Move forward by 1 byte if we can't decode ULEB
            uleb_processed++;
        }
    }

    // 11. Try to identify OBU types from the input data
    // The first 2 bytes of AV1 bitstream often contain OBU information
    if (size >= 2) {
        uint8_t obu_header = data[0];
        // Extract OBU type (bits 5-3 of first byte in AV1)
        int obu_type_value = (obu_header >> 3) & 0x7;
        
        // Convert OBU type to string (sixth API usage)
        const char *obu_type_str = aom_obu_type_to_string(
            static_cast<OBU_TYPE>(obu_type_value));
        // We don't use the string, but the API is called
        
        // Write OBU type info to output file
        if (obu_type_str) {
            fwrite(obu_type_str, 1, strlen(obu_type_str), out_file);
        }
    }

    // 12. Get stream information
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    // Try to peek stream info from the beginning
    if (size > 0) {
        aom_codec_peek_stream_info(decoder_iface, data, 
                                   size < 256 ? size : 256, &stream_info);
    }

    // 13. Clean up resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file if created
    remove("output_file");
    
    // Destroy decoder context
    aom_codec_destroy(&decoder);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}