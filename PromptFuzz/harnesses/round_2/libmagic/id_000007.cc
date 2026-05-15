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
//<ID> 38
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 13
//<Quality> {"density":12,"unique_branches":{"handle_annotation":[[2537,39,2537,53,0,0,4,0],[2540,7,2540,17,0,0,4,1],[2542,7,2542,37,0,0,4,1],[2544,7,2544,57,0,0,4,1],[2548,7,2548,37,0,0,4,1]],"moffset":[[861,4,861,20,0,0,4,0],[877,25,877,39,0,0,4,1],[882,8,882,27,0,0,4,1],[885,8,885,31,0,0,4,1]],"match":[[295,7,295,54,0,0,4,0],[352,8,352,24,0,0,4,0],[353,9,353,24,0,0,4,1],[423,9,423,14,0,0,4,1]]},"library_calls":["magic_open","magic_close","magic_load_buffers","magic_close","magic_getflags","magic_setflags","magic_close","magic_close","magic_descriptor","magic_file","magic_list","magic_close"],"critical_calls":["magic_open","magic_load_buffers","magic_getflags","magic_setflags","magic_descriptor","magic_file","magic_list","magic_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Early exit if no data
    if (size == 0) {
        return 0;
    }
    
    // 1. Create magic handle
    magic_t magic_cookie = magic_open(MAGIC_NONE);
    if (!magic_cookie) {
        return 0;
    }
    
    // 2. Prepare buffer for magic_load_buffers
    void* buffer_copy = malloc(size);
    if (!buffer_copy) {
        magic_close(magic_cookie);
        return 0;
    }
    memcpy(buffer_copy, data, size);
    
    void* buffers[1] = {buffer_copy};
    size_t sizes[1] = {size};
    
    // 3. Load magic database from buffers
    if (magic_load_buffers(magic_cookie, buffers, sizes, 1) != 0) {
        free(buffer_copy);
        magic_close(magic_cookie);
        return 0;
    }
    
    // 4. Set flags (enable MIME type detection)
    int current_flags = magic_getflags(magic_cookie);
    magic_setflags(magic_cookie, current_flags | MAGIC_MIME);
    
    // 5. Create input file from data using fmemopen
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        free(buffer_copy);
        magic_close(magic_cookie);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 6. Get file descriptor for magic_descriptor
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        free(buffer_copy);
        magic_close(magic_cookie);
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 7. Use magic_descriptor API
    const char* descriptor_result = magic_descriptor(magic_cookie, fd);
    // Result is intentionally not used, just calling the API
    
    // 8. Use magic_file API
    const char* file_result = magic_file(magic_cookie, "input_file");
    // Result is intentionally not used, just calling the API
    
    // 9. Create output file for magic_list
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        // 10. Use magic_list API
        if (magic_list(magic_cookie, "output_file") != 0) {
            // Error occurred, but we continue cleanup
        }
        assert_file_closed(&out_file);;
        // Remove the output file to avoid accumulating files during fuzzing
        remove("output_file");
    }
    
    // 11. Clean up
    assert_file_closed(&in_file);;
    free(buffer_copy);
    magic_close(magic_cookie);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}