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
//<ID> 0
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 21
//<Quality> {"density":2,"unique_branches":{"aom_obu_type_to_string":[[188,5,188,29,0,0,4,0],[188,5,188,29,0,0,4,1],[189,5,189,32,0,0,4,0],[189,5,189,32,0,0,4,1],[190,5,190,26,0,0,4,0],[190,5,190,26,0,0,4,1],[191,5,191,36,0,0,4,0],[191,5,191,36,0,0,4,1],[192,5,192,19,0,0,4,0],[192,5,192,19,0,0,4,1],[193,5,193,24,0,0,4,0],[193,5,193,24,0,0,4,1],[194,5,194,22,0,0,4,0],[194,5,194,22,0,0,4,1],[195,5,195,23,0,0,4,0],[195,5,195,23,0,0,4,1],[196,5,196,21,0,0,4,0],[196,5,196,21,0,0,4,1],[197,5,197,12,0,0,4,0],[197,5,197,12,0,0,4,1]],"aom_uleb_encode":[[62,7,62,27,0,0,4,0]]},"library_calls":["aom_codec_version","aom_codec_version_str","aom_uleb_size_in_bytes","aom_uleb_encode","aom_codec_control","aom_obu_type_to_string"],"critical_calls":["aom_codec_version","aom_codec_version_str","aom_uleb_size_in_bytes","aom_uleb_encode","aom_codec_control","aom_obu_type_to_string"],"visited":0}
/**/


// Include necessary libaom headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // 1. Get library version information
    int version = aom_codec_version();
    const char *version_str = aom_codec_version_str();
    
    // Use version information to prevent compiler warnings
    (void)version;
    (void)version_str;

    // 2. ULEB encoding operations
    // Use the first 8 bytes of input as a 64-bit value
    uint64_t uleb_value = 0;
    size_t bytes_to_copy = (size < sizeof(uint64_t)) ? size : sizeof(uint64_t);
    memcpy(&uleb_value, data, bytes_to_copy);
    
    // Calculate ULEB size
    size_t uleb_size = aom_uleb_size_in_bytes(uleb_value);
    
    // Prepare buffer for ULEB encoding
    size_t available = (size > 32) ? 32 : size; // Use reasonable buffer size
    uint8_t *coded_value = (uint8_t *)malloc(available);
    if (!coded_value) {
        return 0;
    }
    
    size_t coded_size = 0;
    int encode_result = aom_uleb_encode(uleb_value, available, coded_value, &coded_size);
    (void)encode_result;
    
    // 3. Create a dummy codec context for control operations
    aom_codec_ctx_t codec_ctx;
    memset(&codec_ctx, 0, sizeof(codec_ctx));
    
    // Try to use various control operations
    // Note: We use minimal safe operations since we don't have a real codec instance
    aom_codec_err_t control_result = aom_codec_control(&codec_ctx, 0);
    (void)control_result;
    
    // 4. OBU type string conversion
    // Use a valid OBU type from input (mask to get valid OBU type range)
    int obu_type_value = data[0] & 0x0F; // OBU types are 0-15
    const char *obu_type_str = aom_obu_type_to_string((OBU_TYPE)obu_type_value);
    (void)obu_type_str;
    
    // 5. Create FILE* from input data for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Get file descriptor
        int fd = fuzz_fileno(in_file);
        (void)fd;
        
        // Read some data from the file
        uint8_t buffer[64];
        size_t read_count = fread(buffer, 1, sizeof(buffer), in_file);
        (void)read_count;
        
        assert_file_closed(&in_file);;
    }
    
    // 6. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Get file descriptor
        int out_fd = fuzz_fileno(out_file);
        (void)out_fd;
        
        // Write encoded ULEB data to file
        if (coded_value && coded_size > 0) {
            fwrite(coded_value, 1, coded_size, out_file);
        }
        
        assert_file_closed(&out_file);;
    }
    
    // 7. Clean up allocated resources
    free(coded_value);
    
    // Remove output file if it exists
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}