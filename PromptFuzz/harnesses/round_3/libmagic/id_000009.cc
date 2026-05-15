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
#include <magic.h>
}
//<ID> 93
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["magic_open","magic_getflags","magic_setparam","magic_getparam","magic_load","magic_compile","magic_load","magic_descriptor","magic_buffer","magic_list","magic_close"],"critical_calls":["magic_open","magic_getflags","magic_setparam","magic_getparam","magic_load","magic_descriptor","magic_buffer","magic_list","magic_close"],"visited":0}
/*Here's a step-by-step C++ fuzz driver using the specified libmagic APIs to achieve file type identification with parameter manipulation:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<25) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size == 0) {
	return 0;
	}
	
    
    // 1. Create input file with fuzzing data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Open magic database with default flags
    magic_t magic_cookie = magic_open(fuzz_int32_t_1);
    if (!magic_cookie) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Get current flags (for demonstration)
    int current_flags = magic_getflags(magic_cookie);
    
    // 4. Set a parameter - MAGIC_PARAM_BYTES_MAX
    size_t bytes_max = size > 1000 ? 1000 : size;
    magic_setparam(magic_cookie, fuzz_int32_t_2, &bytes_max);
    
    // 5. Get the parameter back to verify
    size_t retrieved_bytes_max = 0;
    magic_getparam(magic_cookie, fuzz_int32_t_3, &retrieved_bytes_max);
    
    // 6. Load default magic database
    if (magic_load(magic_cookie, NULL) != 0) {
        // Fallback: try to compile input as magic file
        rewind(in_file);
        assert_file_closed(&in_file);;
        
        // Create temporary magic file
        FILE *magic_file = fopen("input_file", "wb");
        if (magic_file) {
            fwrite(data, 1, size, magic_file);
            assert_file_closed(&magic_file);;
            
            // 7. Try to compile the input as magic file
            magic_compile(magic_cookie, fuzz_str_4);
            remove("input_file");
            
            // Reload with compiled database
            magic_load(magic_cookie, NULL);
        }
    } else {
        // 8. Use magic_descriptor with file descriptor
        int fd = fuzz_fileno(in_file);
        const char *result_descriptor = magic_descriptor(magic_cookie, fd);
        
        // 9. Use magic_buffer with direct data
        const char *result_buffer = magic_buffer(magic_cookie, data, size);
        
        // 10. List magic entries to output file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            magic_list(magic_cookie, "output_file");
            assert_file_closed(&out_file);;
            remove("output_file");
        }
        
        assert_file_closed(&in_file);;
    }
    
    // 11. Clean up
    magic_close(magic_cookie);
    
    assert_file_closed(&in_file);
	return 0;
}