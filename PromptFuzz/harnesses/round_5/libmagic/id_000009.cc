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
//<ID> 148
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["magic_getpath","magic_open","magic_open","magic_close","magic_load","magic_error","magic_file","magic_error","magic_descriptor","magic_error","magic_buffer","magic_error","magic_list","magic_error","magic_load_buffers","magic_error","magic_buffer","magic_error","magic_close","magic_close"],"critical_calls":["magic_getpath","magic_open","magic_open","magic_load","magic_error","magic_file","magic_error","magic_descriptor","magic_error","magic_buffer","magic_error","magic_list","magic_error","magic_load_buffers","magic_buffer","magic_error","magic_close","magic_close"],"visited":0}
/**/


// Include libmagic headers

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
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
        return 0;  // Early exit for empty input
    }

    // 1. Get the magic database path
    const char *db_path = magic_getpath(NULL, 0);
    if (db_path == NULL) {
        // Continue without database path
    }

    // 2. Create a magic cookie for main operations
    magic_t mymagic = magic_open(MAGIC_NONE);
    if (mymagic == NULL) {
        return 0;
    }

    // 3. Create another magic cookie for load_buffers testing
    magic_t magic2 = magic_open(MAGIC_NONE);
    if (magic2 == NULL) {
        magic_close(mymagic);
        return 0;
    }

    // 4. Load the default magic database
    if (magic_load(mymagic, NULL) != 0) {
        // Continue even if load fails, some operations might still work
        const char *error = magic_error(mymagic);
        if (error != NULL) {
            // Error message available
        }
    }

    // 5. Write input data to a file for file-based operations
    FILE *in_file = fopen("input_file", "wb");
    if (in_file != NULL) {
        fwrite(data, 1, size, in_file);
        assert_file_closed(&in_file);;
        
        // Use magic_file on the created file
        const char *file_result = magic_file(mymagic, "input_file");
        if (file_result == NULL) {
            const char *error = magic_error(mymagic);
            if (error != NULL) {
                // Error message available
            }
        }
        
        // Clean up the temporary file
        remove("input_file");
    }

    // 6. Create a memory file for descriptor operations
    FILE *mem_file = fmemopen((void *)data, size, "rb");
    if (mem_file != NULL) {
        int fd = fuzz_fileno(mem_file);
        if (fd != -1) {
            // Use magic_descriptor with the file descriptor
            const char *desc_result = magic_descriptor(mymagic, fd);
            if (desc_result == NULL) {
                const char *error = magic_error(mymagic);
                if (error != NULL) {
                    // Error message available
                }
            }
        }
        assert_file_closed(&mem_file);;
    }

    // 7. Use magic_buffer with the input data directly
    const char *buffer_result = magic_buffer(mymagic, data, size);
    if (buffer_result == NULL) {
        const char *error = magic_error(mymagic);
        if (error != NULL) {
            // Error message available
        }
    }

    // 8. Create output file for magic_list
    FILE *out_file = fopen("output_file", "wb");
    if (out_file != NULL) {
        assert_file_closed(&out_file);;
        
        // Use magic_list to output to file
        if (magic_list(mymagic, "output_file") != 0) {
            const char *error = magic_error(mymagic);
            if (error != NULL) {
                // Error message available
            }
        }
        
        // Clean up the output file
        remove("output_file");
    }

    // 9. Test magic_load_buffers with the input data
    // Prepare buffer array (single buffer in this case)
    void *buffers[1] = {(void *)data};
    size_t sizes[1] = {size};
    
    if (magic_load_buffers(magic2, buffers, sizes, 1) != 0) {
        const char *error = magic_error(magic2);
        if (error != NULL) {
            // Error message available
        }
    } else {
        // If load succeeded, test with the buffer
        const char *buffer_result2 = magic_buffer(magic2, data, size);
        if (buffer_result2 == NULL) {
            const char *error = magic_error(magic2);
            if (error != NULL) {
                // Error message available
            }
        }
    }

    // 10. Clean up
    magic_close(mymagic);
    magic_close(magic2);

    assert_file_closed(&out_file);
	assert_file_closed(&mem_file);
	assert_file_closed(&in_file);
	return 0;
}