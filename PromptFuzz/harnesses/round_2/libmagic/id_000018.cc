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
//<ID> 190
//<Prompt> []
/*<Combination>: [
*/
//<score> 225, nr_unique_branch: 76
//<Quality> {"density":18,"unique_branches":{"file_ascmagic_with_encoding":[[215,8,215,11,0,0,4,0],[220,9,220,42,0,0,4,1],[224,9,224,23,0,0,4,1]],"file_tryelf":[[38,2,38,14,19,0,4,0],[40,7,40,32,19,0,4,0],[40,7,40,32,19,0,4,1],[43,7,46,39,19,0,4,1],[53,7,53,32,21,0,4,0],[66,7,66,32,21,0,4,0]],"simh_parse":[[126,18,126,24,0,0,4,0],[144,7,144,23,0,0,4,1],[150,6,150,13,0,0,4,1]],"cdf_read_sat":[[484,7,484,21,0,0,4,1],[515,7,515,14,0,0,4,0],[526,15,526,29,0,0,4,1],[450,25,450,26,19,0,4,1]],"file_fsmagic":[[407,8,407,12,0,0,4,0],[408,9,408,47,0,0,4,1]],"looks_ucs16":[[464,43,464,56,3,0,4,0]],"json_parse_object":[[222,9,222,16,0,0,4,1]],"dophn_core":[[376,6,376,28,0,0,4,0]],"json_isxdigit":[[106,6,106,22,0,0,4,0],[109,2,109,10,0,0,4,0],[109,12,109,20,0,0,4,0],[109,22,109,30,0,0,4,0],[109,32,109,40,0,0,4,0],[109,42,109,50,0,0,4,0],[110,22,110,30,0,0,4,0],[110,42,110,50,0,0,4,0],[110,52,110,60,0,0,4,0]],"handle_mime":[[93,6,93,30,0,0,4,0],[94,7,94,45,0,0,4,1],[96,7,96,35,0,0,4,0],[96,39,97,26,0,0,4,1],[100,6,100,34,0,0,4,0],[100,38,100,69,0,0,4,1]],"file_getbuffer":[[595,8,595,26,0,0,4,1],[600,12,600,31,0,0,4,0],[600,12,600,31,0,0,4,1]],"json_parse_string":[[142,4,142,13,0,0,4,0],[144,4,144,12,0,0,4,0],[149,4,149,12,0,0,4,0],[150,4,150,12,0,0,4,0],[158,17,158,22,0,0,4,1]],"doshn":[[1442,6,1442,28,0,0,4,0]],"file_is_simh":[[166,6,166,25,0,0,4,1],[169,6,169,33,0,0,4,1],[172,6,172,10,0,0,4,0],[173,7,173,58,0,0,4,1]],"looks_ucs32":[[561,7,561,31,0,0,4,0]],"file_is_csv":[[153,6,153,10,0,0,4,0],[154,7,154,40,0,0,4,1]],"dophn_exec":[[1838,8,1838,30,0,0,4,0],[1848,8,1848,30,0,0,4,0],[1866,8,1866,30,0,0,4,0],[1871,6,1871,28,0,0,4,0]],"file_trycdf":[[638,18,638,45,0,0,4,0],[639,8,639,50,0,0,4,1],[696,13,696,40,0,0,4,0],[698,7,698,57,0,0,4,1],[42,21,42,54,7,0,4,1],[42,21,42,54,9,0,4,1]],"checkdone":[[290,6,290,39,0,0,4,1],[292,6,292,30,0,0,4,1]],"cdf_read_sector_chain":[[693,45,693,65,0,0,4,1]],"file_buffer":[[400,8,400,26,0,0,4,1],[411,8,411,26,0,0,4,1],[421,7,421,8,0,0,4,0],[422,8,422,26,0,0,4,1],[433,8,433,26,0,0,4,1]],"elf_printf":[[76,6,76,28,0,0,4,0]],"file_is_json":[[441,6,441,10,0,0,4,0],[442,7,443,43,0,0,4,1],[443,7,443,14,0,0,4,0],[443,7,443,14,0,0,4,1]],"json_parse":[[358,6,358,15,0,0,4,0]]},"library_calls":["magic_open","magic_getflags","magic_setflags","magic_setparam","magic_close","magic_close","magic_close","magic_load_buffers","magic_load","magic_check","magic_list","magic_file","magic_buffer","magic_descriptor","magic_error","magic_errno","magic_version","magic_getpath","magic_compile","magic_close"],"critical_calls":["magic_open","magic_getflags","magic_setflags","magic_setparam","magic_load_buffers","magic_load","magic_check","magic_list","magic_file","magic_buffer","magic_descriptor","magic_error","magic_errno","magic_version","magic_getpath","magic_compile","magic_close"],"visited":1}
/*Here's a C++ fuzz driver that uses all the specified libmagic APIs to achieve the event of analyzing input data with a custom magic database configuration:

*/


extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 10) {
        return 0; // Too small to be meaningful
    }

    magic_t cookie = nullptr;
    FILE *in_file = nullptr;
    FILE *out_file = nullptr;
    
    // 1. Open magic database with default flags
    cookie = magic_open(MAGIC_NONE);
    if (!cookie) {
        return 0;
    }

    // 2. Get initial flags
    int flags = magic_getflags(cookie);

    // 3. Set new flags (add some common ones)
    magic_setflags(cookie, flags | MAGIC_MIME | MAGIC_CONTINUE);

    // 4. Set a parameter (buffer size limit)
    size_t max_size = 1024 * 1024; // 1MB limit
    if (size > max_size) {
        max_size = size > (1024 * 1024 * 10) ? (1024 * 1024 * 10) : size;
    }
    magic_setparam(cookie, MAGIC_PARAM_BYTES_MAX, &max_size);

    // 5. Create input file from data
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        magic_close(cookie);
        assert_file_closed(&in_file);
	return 0;
    }

    // 6. Create output file
    out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        magic_close(cookie);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 7. Prepare buffer for magic_load_buffers
    // Split data into two parts for testing multiple buffers
    size_t first_part = size / 2;
    size_t second_part = size - first_part;
    
    void* buffers[2];
    size_t buffer_sizes[2];
    
    buffers[0] = malloc(first_part);
    buffers[1] = malloc(second_part);
    
    if (!buffers[0] || !buffers[1]) {
        free(buffers[0]);
        free(buffers[1]);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        magic_close(cookie);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    memcpy(buffers[0], data, first_part);
    memcpy(buffers[1], data + first_part, second_part);
    
    buffer_sizes[0] = first_part;
    buffer_sizes[1] = second_part;

    // 8. Try to load magic database from buffers (will likely fail but we call it)
    int load_result = magic_load_buffers(cookie, buffers, buffer_sizes, 2);
    
    // Free buffer memory
    free(buffers[0]);
    free(buffers[1]);

    // 9. If buffer load failed, try loading default database
    if (load_result != 0) {
        magic_load(cookie, nullptr); // Load default database
    }

    // 10. Check the magic database
    magic_check(cookie, nullptr);

    // 11. List magic entries to output file
    magic_list(cookie, "output_file");

    // 12. Get file descriptor for input file
    int fd = fuzz_fileno(in_file);

    // 13. Analyze using various methods
    
    // Method 1: Using magic_file with filename
    const char* result1 = magic_file(cookie, "input_file");
    if (result1) {
        // Can be used for something
    }

    // Method 2: Using magic_buffer with direct data
    const char* result2 = magic_buffer(cookie, data, size);
    if (result2) {
        // Can be used for something
    }

    // Method 3: Using magic_descriptor
    const char* result3 = magic_descriptor(cookie, fd);
    if (result3) {
        // Can be used for something
    }

    // 14. Get error if any
    const char* error = magic_error(cookie);
    if (error) {
        // Error handling if needed
    }

    // 15. Get error number
    int err = magic_errno(cookie);
    (void)err; // Use err to avoid unused variable warning

    // 16. Get version
    int version = magic_version();
    (void)version; // Use version to avoid unused variable warning

    // 17. Get magic path
    const char* magic_path = magic_getpath(nullptr, 0);
    (void)magic_path; // Use magic_path to avoid unused variable warning

    // 18. Clean up resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Try to compile magic database (might fail but we call it)
    magic_compile(cookie, nullptr);
    
    magic_close(cookie);

    // Remove created files
    remove("input_file");
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}