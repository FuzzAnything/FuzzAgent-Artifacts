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
//<ID> 428
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 13
//<Quality> {"density":7,"unique_branches":{"handle_mime":[[93,6,93,30,0,0,4,0],[94,7,94,45,0,0,4,1],[96,7,96,35,0,0,4,1],[100,6,100,34,0,0,4,1]],"file_buffer":[[459,7,459,37,0,0,4,0]],"file_fsmagic":[[126,6,126,38,0,0,4,1],[173,6,173,11,0,0,4,1],[406,7,406,39,0,0,4,1],[407,8,407,12,0,0,4,0],[408,9,408,47,0,0,4,1],[424,17,424,22,0,0,4,1]],"print_sep":[[2558,6,2558,15,0,0,4,1]],"match":[[311,8,311,13,0,0,4,1]]},"library_calls":["magic_open","magic_getflags","magic_setflags","magic_load_buffers","magic_check","magic_file","magic_close"],"critical_calls":["magic_open","magic_getflags","magic_setflags","magic_load_buffers","magic_check","magic_file","magic_close"],"visited":0}
/**/


// Include libmagic headers

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Step 1: Create a magic cookie
    magic_t magic_cookie = magic_open(MAGIC_NONE);
    if (magic_cookie == nullptr) {
        return 0;
    }

    // Step 2: Get current flags
    int current_flags = magic_getflags(magic_cookie);

    // Step 3: Set flags - toggle some flag bits using input data
    // We'll use the first byte of input to modify flags
    int new_flags = current_flags ^ (data[0] & 0xFF);
    magic_setflags(magic_cookie, new_flags);

    // Step 4: Create a temporary file to write the magic database
    // We'll use the input data as a magic database buffer
    FILE *out_file = fopen("output_file", "wb");
    if (out_file != nullptr) {
        // Write data to create a magic database file
        fwrite(data, 1, size, out_file);
        assert_file_closed(&out_file);;
    }

    // Step 5: Load magic database from buffer
    // Prepare buffer array for magic_load_buffers
    void *buffers[1] = {const_cast<void*>(static_cast<const void*>(data))};
    size_t sizes[1] = {size};
    
    // Try to load from buffers - this may fail if data isn't valid magic database
    int load_result = magic_load_buffers(magic_cookie, buffers, sizes, 1);
    
    // Step 6: Check the magic database
    // Use the output file we created as the database to check
    int check_result = magic_check(magic_cookie, "output_file");

    // Step 7: Use magic_file to identify a file
    // Create an input file using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file != nullptr) {
        // Get file descriptor for magic_descriptor (alternative to magic_file)
        int fd = fuzz_fileno(in_file);
        
        // Use magic_file to identify our output file
        const char *file_type = magic_file(magic_cookie, "output_file");
        
        // Clean up the input file
        assert_file_closed(&in_file);;
    }

    // Step 8: Clean up
    // Remove the temporary output file
    remove("output_file");
    
    // Close the magic cookie
    magic_close(magic_cookie);

    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	return 0;
}