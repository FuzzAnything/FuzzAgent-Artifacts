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
//<ID> 36
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["magic_open","magic_setflags","magic_close","magic_load_buffers","magic_close","magic_file","magic_descriptor","magic_list","magic_close"],"critical_calls":["magic_open","magic_setflags","magic_load_buffers","magic_file","magic_descriptor","magic_list","magic_close"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libmagic APIs to process input data:

*/


extern "C" {
}

// Helper function to create a temporary file with input data
FILE*  id_000006_create_temp_file(const uint8_t* data, size_t size) {
    FILE* temp_file = tmpfile();
    if (temp_file) {
        fwrite(data, 1, size, temp_file);
        rewind(temp_file);
    }
    return temp_file;
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size == 0) {
	return 0;
	}
	

    // Create magic cookie
    magic_t magic_cookie = magic_open(MAGIC_NONE);
    if (!magic_cookie) {
        return 0;
    }

    // Set flags
    magic_setflags(magic_cookie, MAGIC_CONTINUE);

    // Prepare buffer for loading magic database
    std::vector<void*> buffers;
    std::vector<size_t> sizes;
    
    // Split input data into multiple parts to simulate multiple buffers
    size_t chunk_size = size / 3;
    if (chunk_size == 0) chunk_size = size;
    
    for (size_t offset = 0; offset < size; offset += chunk_size) {
        size_t current_chunk = std::min(chunk_size, size - offset);
        void* buffer = malloc(current_chunk);
        if (!buffer) {
            magic_close(magic_cookie);
            return 0;
        }
        memcpy(buffer, data + offset, current_chunk);
        buffers.push_back(buffer);
        sizes.push_back(current_chunk);
    }

    // Load magic database from buffers
    int load_result = magic_load_buffers(magic_cookie, buffers.data(), sizes.data(), buffers.size());

    // Create input file for testing
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        // Clean up buffers
        for (void* buffer : buffers) {
            free(buffer);
        }
        magic_close(magic_cookie);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor
    int fd = fuzz_fileno(in_file);

    // Test magic_file with input file
    const char* file_result = magic_file(magic_cookie, "input_file");
    // Test magic_descriptor
    const char* desc_result = magic_descriptor(magic_cookie, fd);
    
    // Create output file for magic_list
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        // Test magic_list
        magic_list(magic_cookie, "output_file");
        assert_file_closed(&out_file);;
        remove("output_file");
    }

    // Clean up
    assert_file_closed(&in_file);;
    
    // Free allocated buffers
    for (void* buffer : buffers) {
        free(buffer);
    }
    
    magic_close(magic_cookie);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}