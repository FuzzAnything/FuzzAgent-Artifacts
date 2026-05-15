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
//<ID> 353
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 2
//<Quality> {"density":15,"unique_branches":{"magic_setparam":[[619,2,619,28,0,0,4,0],[640,2,640,28,0,0,4,1]]},"library_calls":["magic_open","magic_setflags","magic_setparam","magic_getflags","magic_load","magic_error","magic_errno","magic_close","magic_buffer","magic_error","magic_errno","magic_getparam","magic_descriptor","magic_file","magic_close"],"critical_calls":["magic_open","magic_setflags","magic_setparam","magic_getflags","magic_load","magic_buffer","magic_error","magic_errno","magic_getparam","magic_descriptor","magic_file","magic_close"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified libmagic APIs to achieve the event of identifying file type/format from a buffer while handling errors and managing configuration:

*/


extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // 1. Open magic cookie with specific flags
    int open_flags = MAGIC_NONE | MAGIC_MIME | MAGIC_SYMLINK;
    magic_t cookie = magic_open(open_flags);
    if (cookie == nullptr) {
        return 0;
    }

    // 2. Set additional flags
    int set_flags = MAGIC_CHECK | MAGIC_PRESERVE_ATIME;
    int set_result = magic_setflags(cookie, set_flags);
    
    // 3. Set a parameter (buffer size limit)
    size_t param_value = size;  // Use input size as parameter
    int param_result = magic_setparam(cookie, fuzz_int32_t_1, &param_value);

    // 4. Get current flags after setting
    int current_flags = magic_getflags(cookie);

    // 5. Try to load the default magic database
    if (magic_load(cookie, nullptr) != 0) {
        // If loading fails, try to use built-in database
        const char *error_msg = magic_error(cookie);
        int err_no = magic_errno(cookie);
        magic_close(cookie);
        return 0;
    }

    // 6. Use magic_buffer to identify the input data
    const char *description = magic_buffer(cookie, data, size);
    
    // 7. Get error information if any
    const char *error_msg = nullptr;
    int err_no = 0;
    
    if (description == nullptr) {
        error_msg = magic_error(cookie);
        err_no = magic_errno(cookie);
    }

    // 8. Get parameter we set earlier
    size_t retrieved_param = 0;
    int getparam_result = magic_getparam(cookie, fuzz_int32_t_2, &retrieved_param);

    // 9. Additional testing: Try to identify using file descriptor
    if (size > 0) {
        FILE *in_file = fmemopen((void *)data, size, "rb");
        if (in_file != nullptr) {
            int fd = fuzz_fileno(in_file);
            if (fd >= 0) {
                const char *desc_from_fd = magic_descriptor(cookie, fd);
                // Don't use the result, just exercise the API
            }
            assert_file_closed(&in_file);;
        }
    }

    // 10. Additional testing: Try to identify file from path
    // Write data to a temporary file first
    FILE *out_file = fopen("output_file", "wb");
    if (out_file != nullptr) {
        fwrite(data, 1, size, out_file);
        assert_file_closed(&out_file);;
        
        const char *file_desc = magic_file(cookie, "output_file");
        // Don't use the result, just exercise the API
        
        // Clean up temporary file
        remove("output_file");
    }

    // 11. Release all allocated resources
    magic_close(cookie);

    assert_file_closed(&out_file);
	return 0;
}