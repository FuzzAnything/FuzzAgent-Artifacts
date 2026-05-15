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
//<ID> 323
//<Prompt> []
/*<Combination>: [
*/
//<score> 85, nr_unique_branch: 63
//<Quality> {"density":10,"unique_branches":{"der_tag":[[227,6,227,24,0,0,4,1]],"get_next_format_from_precision":[[143,6,143,24,0,0,4,1],[150,6,150,15,0,0,4,1],[155,6,155,15,0,0,4,1],[160,6,160,15,0,0,4,1]],"file_fmtcheck":[[170,6,170,16,0,0,4,0]],"magic_buffer":[[561,6,561,52,0,0,4,0]],"magiccheck":[[2216,3,2216,10,0,0,4,0],[2233,3,2233,11,0,0,4,0],[2241,3,2241,11,0,0,4,0],[2249,3,2249,10,0,0,4,0]],"fmtcheck":[[247,7,247,30,0,0,4,0]],"msetoffset":[[1573,7,1573,30,0,0,4,0]],"match":[[276,4,276,11,0,0,4,0]],"file_softmagic":[[141,3,141,10,0,0,4,0]],"cvt_double":[[1137,6,1137,17,1,0,4,0],[1138,11,1138,37,1,0,4,1],[1139,3,1139,18,1,0,4,1],[1142,3,1142,20,1,0,4,0],[1145,3,1145,23,1,0,4,1],[1148,3,1148,21,1,0,4,1]],"gettag":[[166,7,166,14,0,0,4,0]],"mprint":[[634,4,634,21,0,0,4,0],[641,4,641,20,0,0,4,0],[644,4,644,18,0,0,4,0],[645,4,645,20,0,0,4,0],[646,4,646,20,0,0,4,0],[654,7,654,21,0,0,4,0],[684,2,684,16,0,0,4,0],[686,2,686,18,0,0,4,0],[687,2,687,18,0,0,4,0],[694,2,694,19,0,0,4,0],[705,2,705,19,0,0,4,0],[711,2,711,18,0,0,4,0],[713,2,713,20,0,0,4,0],[728,2,728,17,0,0,4,0],[748,2,748,20,0,0,4,0],[749,2,749,20,0,0,4,0],[809,2,809,21,0,0,4,0],[823,2,823,17,0,0,4,0],[825,7,825,52,0,0,4,1]],"moffset":[[848,4,848,21,0,0,4,0],[850,2,850,21,0,0,4,0],[862,4,862,20,0,0,4,0],[866,4,866,18,0,0,4,0],[867,4,867,20,0,0,4,0],[868,4,868,20,0,0,4,0],[873,4,873,21,0,0,4,0],[876,2,876,17,0,0,4,0],[894,2,894,16,0,0,4,0],[896,2,896,18,0,0,4,0],[897,2,897,18,0,0,4,0],[910,2,910,19,0,0,4,0],[914,2,914,18,0,0,4,0],[916,2,916,20,0,0,4,0],[920,4,920,19,0,0,4,0],[927,4,927,22,0,0,4,0],[928,4,928,22,0,0,4,0]],"file_mdump":[[89,8,89,41,0,0,4,0]],"getlength":[[216,6,216,27,0,0,4,1],[216,31,216,43,0,0,4,0],[216,31,216,43,0,0,4,1]],"cvt_32":[[1098,6,1098,33,4,0,4,0]]},"library_calls":["magic_getpath","magic_open","magic_getflags","magic_load_buffers","magic_check","magic_errno","magic_close","magic_descriptor","magic_buffer","magic_list","magic_close"],"critical_calls":["magic_getpath","magic_open","magic_getflags","magic_load_buffers","magic_check","magic_errno","magic_descriptor","magic_buffer","magic_list","magic_close"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 2) {
        return 0; // Not enough data to process
    }

    // 1. Split input data into two parts for different purposes
    size_t magic_data_size = size / 2;
    size_t buffer_data_size = size - magic_data_size;
    const uint8_t *magic_data = data;
    const uint8_t *buffer_data = data + magic_data_size;

    // 2. Create a memory buffer for magic database
    std::vector<const void*> magic_buffers = {magic_data};
    std::vector<size_t> buffer_sizes = {magic_data_size};

    // 3. Get default magic database path
    const char *default_path = magic_getpath(nullptr, 0);
    
    // 4. Open magic cookie with default flags
    magic_t cookie = magic_open(MAGIC_NONE);
    if (!cookie) {
        return 0;
    }

    // 5. Get current flags
    int current_flags = magic_getflags(cookie);

    // 6. Load magic database from buffer
    int load_result = magic_load_buffers(cookie, 
                                        const_cast<void**>(magic_buffers.data()), 
                                        buffer_sizes.data(), 
                                        magic_buffers.size());

    // 7. Check if magic database is valid
    int check_result = magic_check(cookie, "input_file");

    // 8. Get error number if any
    int error_num = magic_errno(cookie);

    // 9. Create input file from buffer data
    FILE *in_file = fmemopen((void*)buffer_data, buffer_data_size, "rb");
    if (!in_file) {
        magic_close(cookie);
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // 10. Get file descriptor
    int fd = fuzz_fileno(in_file);

    // 11. Use magic_descriptor to identify file type
    const char *descriptor_result = magic_descriptor(cookie, fd);

    // 12. Use magic_buffer to identify buffer data
    const char *buffer_result = magic_buffer(cookie, buffer_data, buffer_data_size);

    // 13. List magic entries to output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        int list_result = magic_list(cookie, "output_file");
        assert_file_closed(&out_file);;
    }

    // 14. Clean up
    assert_file_closed(&in_file);;
    magic_close(cookie);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}