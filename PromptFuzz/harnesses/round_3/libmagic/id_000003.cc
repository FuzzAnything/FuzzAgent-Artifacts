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
//<ID> 34
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 63
//<Quality> {"density":9,"unique_branches":{"mprint":[[633,4,633,19,0,0,4,0],[634,4,634,21,0,0,4,0],[638,4,638,18,0,0,4,0],[645,4,645,20,0,0,4,0],[646,4,646,20,0,0,4,0],[647,2,647,18,0,0,4,0],[684,2,684,16,0,0,4,0],[685,2,685,18,0,0,4,0],[688,7,689,59,0,0,4,1],[694,2,694,19,0,0,4,0],[696,2,696,19,0,0,4,0],[697,7,699,9,0,0,4,1],[703,2,703,17,0,0,4,0],[705,2,705,19,0,0,4,0],[711,2,711,18,0,0,4,0],[712,2,712,20,0,0,4,0],[713,2,713,20,0,0,4,0],[714,7,715,70,0,0,4,1],[719,2,719,18,0,0,4,0],[720,2,720,20,0,0,4,0],[722,7,724,12,0,0,4,1],[728,2,728,17,0,0,4,0],[729,2,729,19,0,0,4,0],[730,2,730,19,0,0,4,0],[733,3,733,10,0,0,4,1],[735,3,735,9,0,0,4,1],[740,3,740,10,0,0,4,0],[741,8,741,52,0,0,4,1],[810,2,810,23,0,0,4,0],[811,2,811,23,0,0,4,0],[817,2,817,23,0,0,4,0],[818,2,818,23,0,0,4,0],[819,7,820,52,0,0,4,1],[823,2,823,17,0,0,4,0],[825,7,825,52,0,0,4,1],[602,2,602,9,6,0,4,1],[604,2,604,8,6,0,4,1],[615,2,615,9,6,0,4,0],[616,7,616,25,6,0,4,0],[616,7,616,25,6,0,4,1],[617,14,618,28,6,0,4,1],[621,14,622,28,6,0,4,1],[602,2,602,9,11,0,4,1],[604,2,604,8,11,0,4,1],[615,2,615,9,11,0,4,0],[616,7,616,25,11,0,4,0],[617,14,618,28,11,0,4,1]],"file_buffer":[[400,8,400,26,0,0,4,0],[411,8,411,26,0,0,4,0],[422,8,422,26,0,0,4,0]],"moffset":[[852,2,852,23,0,0,4,0],[894,2,894,16,0,0,4,0],[904,2,904,19,0,0,4,0],[908,2,908,17,0,0,4,0],[920,4,920,19,0,0,4,0],[921,4,921,21,0,0,4,0]],"file_printable":[[851,7,851,35,0,0,4,0]],"cdf_read":[[385,6,385,46,0,0,4,1]],"file_getbuffer":[[554,6,554,27,0,0,4,0]],"cvt_double":[[1137,6,1137,17,1,0,4,1]],"close_and_restore":[[371,20,371,59,0,0,4,1]],"magiccheck":[[2204,3,2204,11,0,0,4,0],[2237,3,2237,11,0,0,4,0]]},"library_calls":["magic_version","magic_open","magic_setflags","magic_close","magic_close","magic_close","magic_load_buffers","magic_check","magic_file","magic_close"],"critical_calls":["magic_version","magic_open","magic_setflags","magic_load_buffers","magic_check","magic_file","magic_close"],"visited":2}
/*Here's a C++ fuzz driver that uses the specified libmagic APIs in a coherent way:

*/


extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<17) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // 1. Get libmagic version first
    int version = magic_version();
    
    // 2. Open a magic handle with default flags
    magic_t cookie = magic_open(fuzz_int32_t_1);
    if (!cookie) {
        return 0;  // Failed to open magic handle
    }
    
    // 3. Set some flags (use a combination of flags)
    // Using MAGIC_CHECK | MAGIC_PRESERVE_ATIME | MAGIC_RAW as an example
    int flags = MAGIC_CHECK | MAGIC_PRESERVE_ATIME | MAGIC_RAW;
    if (magic_setflags(cookie, flags) == -1) {
        magic_close(cookie);
        return 0;
    }
    
    // 4. Create a temporary FILE* to read the input data as a magic database
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        magic_close(cookie);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 5. Create a temporary file for output
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        magic_close(cookie);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 6. Use magic_load_buffers to load the input as a magic database
    // We'll split the input into two buffers for demonstration
    void *buffers[2];
    size_t buffer_sizes[2];
    size_t num_buffers = 2;
    
    // Split input into two parts (if size is large enough)
    size_t split_point = size / 2;
    buffers[0] = (void *)data;
    buffer_sizes[0] = split_point;
    buffers[1] = (void *)(data + split_point);
    buffer_sizes[1] = size - split_point;
    
    // Try to load the buffers as a magic database
    int load_result = magic_load_buffers(cookie, buffers, buffer_sizes, num_buffers);
    
    // 7. Check the loaded magic database for consistency
    int check_result = magic_check(cookie, fuzz_str_2);
    
    // 8. Use magic_file to get the file type of our input file
    const char *file_type = magic_file(cookie, "input_file");
    
    // 9. Clean up resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove the temporary output file
    remove("output_file");
    
    // Close the magic handle
    magic_close(cookie);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}