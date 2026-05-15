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
//<ID> 656
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 1
//<Quality> {"density":13,"unique_branches":{"string_modifier_check":[[1787,7,1787,63,0,0,4,0]]},"library_calls":["magic_version","magic_open","magic_setflags","magic_close","magic_getpath","magic_close","magic_close","magic_close","magic_close","magic_load_buffers","magic_load","magic_check","magic_descriptor","magic_close"],"critical_calls":["magic_version","magic_open","magic_setflags","magic_getpath","magic_load_buffers","magic_load","magic_check","magic_descriptor","magic_close"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libmagic APIs to achieve the event of loading magic database from buffers and checking/validating it:

*/


// Include libmagic headers

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<21) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
	//fuzzer shim end}




    // 1. Get libmagic version
    int version = magic_version();
    if (version <= 0) {
        return 0;
    }

    // 2. Create a magic cookie
    magic_t magic_cookie = magic_open(fuzz_int32_t_1);
    if (!magic_cookie) {
        return 0;
    }

    // 3. Set flags for the magic cookie
    int flags = MAGIC_CHECK | MAGIC_ERROR;
    if (magic_setflags(magic_cookie, flags) != 0) {
        magic_close(magic_cookie);
        return 0;
    }

    // 4. Get the default magic database path
    const char *default_magic_path = magic_getpath(nullptr, fuzz_int32_t_2);
    if (!default_magic_path) {
        magic_close(magic_cookie);
        return 0;
    }

    // 5. Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        magic_close(magic_cookie);
        assert_file_closed(&in_file);
	return 0;
    }

    // 6. Get file descriptor for the input file
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        magic_close(magic_cookie);
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 7. Prepare buffer for magic_load_buffers
    // We'll split the input data into chunks to simulate multiple buffers
    std::vector<void*> buffers;
    std::vector<size_t> buffer_sizes;
    
    // Split data into chunks of up to 1024 bytes
    const size_t chunk_size = 1024;
    size_t remaining = size;
    const uint8_t* ptr = data;
    
    while (remaining > 0) {
        size_t current_chunk = (remaining > chunk_size) ? chunk_size : remaining;
        void* buffer = malloc(current_chunk);
        if (!buffer) {
            // Cleanup allocated buffers
            for (size_t i = 0; i < buffers.size(); ++i) {
                free(buffers[i]);
            }
            assert_file_closed(&in_file);;
            magic_close(magic_cookie);
            assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
        }
        memcpy(buffer, ptr, current_chunk);
        buffers.push_back(buffer);
        buffer_sizes.push_back(current_chunk);
        
        ptr += current_chunk;
        remaining -= current_chunk;
    }

    // 8. Load magic database from buffers
    if (buffers.size() > 0) {
        // Convert vectors to arrays for the API call
        void** buffers_array = buffers.data();
        size_t* sizes_array = buffer_sizes.data();
        size_t num_buffers = buffers.size();
        
        int result = magic_load_buffers(magic_cookie, buffers_array, sizes_array, num_buffers);
        // We continue even if this fails, as the input might not be valid magic data
    }

    // 9. Also try to load from the default magic file
    // This might fail if the default file doesn't exist, but we still call it
    magic_load(magic_cookie, default_magic_path);

    // 10. Check the magic database
    // We'll check against the input file we created
    magic_check(magic_cookie, fuzz_str_3);

    // 11. Get description of the input file using its descriptor
    const char* description = magic_descriptor(magic_cookie, fd);
    // We don't need to use the description, but the API is called

    // 12. Clean up allocated resources
    for (size_t i = 0; i < buffers.size(); ++i) {
        free(buffers[i]);
    }
    
    assert_file_closed(&in_file);;
    magic_close(magic_cookie);

    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}