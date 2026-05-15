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
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["aom_codec_version","aom_codec_version_str","aom_uleb_size_in_bytes","aom_uleb_encode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_control","aom_codec_control","aom_codec_control","aom_codec_destroy","aom_obu_type_to_string"],"critical_calls":["aom_codec_version","aom_codec_version_str","aom_uleb_size_in_bytes","aom_uleb_encode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_control","aom_codec_control","aom_codec_control","aom_codec_destroy","aom_obu_type_to_string"],"visited":0}
/**/


// Include necessary libaom headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return for insufficient data
    if (size < 4) {
        return 0;
    }

    // 1. Get libaom version information
    int version = aom_codec_version();
    const char* version_str = aom_codec_version_str();
    
    // Use the version values to prevent optimization removal
    if (version > 0 && version_str != nullptr) {
        // Do nothing, just using the values
    }

    // 2. ULEB encoding operations
    // Extract a 64-bit value from input data
    uint64_t uleb_value = 0;
    size_t bytes_to_copy = (size > 8) ? 8 : size;
    memcpy(&uleb_value, data, bytes_to_copy);
    
    // Calculate ULEB size
    size_t uleb_size = aom_uleb_size_in_bytes(uleb_value);
    
    // Encode ULEB value
    uint8_t* coded_buffer = nullptr;
    size_t coded_size = 0;
    
    if (uleb_size > 0 && uleb_size <= size) {
        coded_buffer = (uint8_t*)malloc(uleb_size);
        if (coded_buffer) {
            int encode_result = aom_uleb_encode(
                uleb_value, 
                uleb_size, 
                coded_buffer, 
                &coded_size
            );
            
            // Use the result to prevent optimization removal
            if (encode_result == 0 && coded_size > 0) {
                // Do nothing with the encoded data
            }
            
            free(coded_buffer);
        }
    }

    // 3. Create a decoder context to use with aom_codec_control
    aom_codec_ctx_t decoder;
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    
    if (decoder_iface) {
        aom_codec_dec_cfg_t cfg = {0};
        cfg.threads = 1;
        
        // Initialize decoder
        if (aom_codec_dec_init(&decoder, decoder_iface, &cfg, 0) == AOM_CODEC_OK) {
            
            // 4. Use aom_codec_control with various control IDs
            // Try different control operations that are safe for fuzzing
            
            // Control ID 1: AOMD_GET_LAST_REF_UPDATES
            int ref_updates = 0;
            aom_codec_control(&decoder, 1, &ref_updates);
            
            // Control ID 2: AOMD_GET_FRAME_CORRUPTED
            int frame_corrupted = 0;
            aom_codec_control(&decoder, 2, &frame_corrupted);
            
            // Control ID 8: AOMD_GET_DISPLAY_SIZE
            int display_size[2] = {0, 0};
            aom_codec_control(&decoder, 8, display_size);
            
            // Clean up decoder
            aom_codec_destroy(&decoder);
        }
    }

    // 5. Convert OBU type to string
    // Use input data to get an OBU type (modulo to keep within valid range)
    if (size > 0) {
        int obu_type_value = data[0] % 16; // OBU types are 0-15
        const char* obu_type_str = aom_obu_type_to_string((OBU_TYPE)obu_type_value);
        
        // Use the string to prevent optimization removal
        if (obu_type_str != nullptr) {
            // Do nothing, just using the value
        }
    }

    // 6. Process input data as if it were a libaom output stream
    // Create a FILE* from input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        // Read and process some data from the file
        uint8_t buffer[256];
        size_t bytes_read = fread(buffer, 1, sizeof(buffer), in_file);
        
        // Use the read data to prevent optimization removal
        if (bytes_read > 0) {
            // Do nothing with the data
        }
        
        assert_file_closed(&in_file);;
    }

    // 7. Create an output file (not strictly necessary for fuzzing but follows instructions)
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some dummy data
        uint8_t dummy_data[] = {0x01, 0x02, 0x03};
        fwrite(dummy_data, 1, sizeof(dummy_data), out_file);
        assert_file_closed(&out_file);;
    }

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}