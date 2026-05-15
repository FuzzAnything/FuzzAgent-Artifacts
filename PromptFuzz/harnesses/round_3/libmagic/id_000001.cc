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
//<ID> 31
//<Prompt> []
/*<Combination>: [
*/
//<score> 63, nr_unique_branch: 190
//<Quality> {"density":7,"unique_branches":{"save_cont":[[1601,6,1601,22,0,0,4,1]],"do_ops":[[1511,3,1511,20,0,0,4,0],[1514,3,1514,23,0,0,4,0],[1528,6,1528,24,0,0,4,0],[1529,7,1529,37,0,0,4,1]],"der_offs":[[267,25,267,41,0,0,4,0],[269,6,269,38,0,0,4,1],[277,6,277,21,0,0,4,1],[292,6,292,24,0,0,4,0],[292,6,292,24,0,0,4,1],[293,7,293,27,0,0,4,1]],"mconvert":[[1269,2,1269,18,0,0,4,0]],"file_ascmagic_with_encoding":[[158,7,159,28,0,0,4,1],[215,8,215,11,0,0,4,0],[220,9,220,42,0,0,4,1],[224,9,224,23,0,0,4,0],[224,9,224,23,0,0,4,1],[224,27,224,51,0,0,4,1]],"file_is_json":[[441,6,441,10,0,0,4,0],[442,7,443,43,0,0,4,1],[443,7,443,14,0,0,4,0],[443,7,443,14,0,0,4,1]],"file_trycdf":[[638,18,638,45,0,0,4,0],[639,8,639,50,0,0,4,1],[696,13,696,40,0,0,4,0],[698,7,698,57,0,0,4,1],[42,21,42,54,7,0,4,1],[42,21,42,54,9,0,4,1]],"mkdbname":[[3528,34,3528,42,0,0,4,1],[3533,6,3533,14,0,0,4,1]],"elf_printf":[[76,6,76,28,0,0,4,0]],"cvt_8":[[1065,6,1065,17,3,0,4,1],[1079,3,1079,20,3,0,4,0],[1065,6,1065,17,4,0,4,1]],"mget":[[1752,8,1752,61,0,0,4,1],[1772,8,1772,29,0,0,4,0],[1772,8,1772,29,0,0,4,1],[1774,8,1774,53,0,0,4,1],[1784,8,1784,53,0,0,4,0],[1825,8,1825,23,0,0,4,1],[1832,8,1832,19,0,0,4,1],[1838,8,1838,38,0,0,4,1],[1968,7,1968,19,0,0,4,0],[1972,7,1972,42,0,0,4,1],[1976,7,1976,30,0,0,4,1],[1982,7,1982,24,0,0,4,1],[1998,7,1998,14,0,0,4,0],[1998,7,1998,14,0,0,4,1],[2002,10,2002,12,0,0,4,0],[2002,10,2002,12,0,0,4,1],[72,30,72,40,52,0,4,1],[72,59,72,76,52,0,4,1],[109,22,109,25,53,0,4,1],[72,30,72,40,62,0,4,1],[72,59,72,76,62,0,4,1],[109,22,109,25,66,0,4,1],[72,59,72,76,69,0,4,0],[72,59,72,76,101,0,4,0],[72,59,72,76,123,0,4,0]],"cvt_flip":[[1000,6,1000,15,0,0,4,1],[1003,2,1003,19,0,0,4,1],[1005,2,1005,18,0,0,4,1],[1007,2,1007,18,0,0,4,1],[1009,2,1009,19,0,0,4,1],[1011,2,1011,18,0,0,4,1],[1013,2,1013,19,0,0,4,1],[1015,2,1015,20,0,0,4,1],[1017,2,1017,20,0,0,4,1],[1019,2,1019,19,0,0,4,0],[1019,2,1019,19,0,0,4,1],[1021,2,1021,18,0,0,4,0],[1021,2,1021,18,0,0,4,1],[1023,2,1023,18,0,0,4,1],[1025,2,1025,19,0,0,4,1],[1027,2,1027,18,0,0,4,1],[1029,2,1029,19,0,0,4,1],[1031,2,1031,20,0,0,4,1],[1033,2,1033,20,0,0,4,1],[1035,2,1035,19,0,0,4,1],[1037,2,1037,19,0,0,4,1],[1039,2,1039,20,0,0,4,1],[1041,2,1041,20,0,0,4,1],[1043,2,1043,23,0,0,4,1],[1045,2,1045,23,0,0,4,1],[1047,2,1047,23,0,0,4,1],[1049,2,1049,23,0,0,4,1],[1051,2,1051,9,0,0,4,0],[1051,2,1051,9,0,0,4,1]],"gettag":[[166,7,166,14,0,0,4,0]],"print_sep":[[2558,6,2558,15,0,0,4,1]],"cvt_float":[[1137,6,1137,17,1,0,4,1]],"der_cmp":[[334,6,334,32,0,0,4,1],[343,2,343,11,0,0,4,0],[343,2,343,11,0,0,4,1],[346,2,346,10,0,0,4,1],[349,2,349,9,0,0,4,0],[349,2,349,9,0,0,4,1],[350,7,350,15,0,0,4,1],[359,7,359,37,0,0,4,1],[362,7,362,19,0,0,4,0]],"apprentice_1":[[501,6,501,17,0,0,4,1]],"match":[[226,6,226,23,0,0,4,1],[228,6,228,25,0,0,4,1],[242,10,242,14,0,0,4,0],[242,18,242,56,0,0,4,0],[242,18,242,56,0,0,4,1],[243,10,243,15,0,0,4,1],[343,8,343,34,0,0,4,0],[350,8,350,59,0,0,4,0],[378,9,378,23,0,0,4,1],[383,9,383,33,0,0,4,0],[391,12,391,17,0,0,4,0],[403,9,403,30,0,0,4,0],[405,14,405,44,0,0,4,0],[406,10,406,33,0,0,4,0],[406,10,406,33,0,0,4,1],[411,9,412,13,0,0,4,0],[420,9,420,17,0,0,4,0],[455,5,455,12,0,0,4,0],[459,5,459,12,0,0,4,1]],"doshn":[[1442,6,1442,28,0,0,4,0]],"handle_annotation":[[2540,21,2540,50,0,0,4,1]],"file_is_simh":[[172,6,172,10,0,0,4,0],[173,7,173,58,0,0,4,1]],"mcopy":[[1438,11,1438,27,0,0,4,1]],"msetoffset":[[1544,7,1544,21,0,0,4,0],[1545,8,1545,38,0,0,4,0],[1545,8,1545,38,0,0,4,1],[1555,7,1555,13,0,0,4,0],[1580,8,1580,19,0,0,4,0]],"looks_ucs16":[[464,43,464,56,3,0,4,0]],"apprentice_unmap":[[626,2,626,20,0,0,4,0],[627,7,627,13,0,0,4,0],[627,17,627,37,0,0,4,0]],"file_magicfind":[[3763,15,3763,29,0,0,4,0],[3764,8,3764,31,0,0,4,0],[3764,8,3764,31,0,0,4,1],[3766,8,3766,40,0,0,4,0],[3766,8,3766,40,0,0,4,1],[3769,21,3769,35,0,0,4,0],[3770,13,3770,34,0,0,4,0],[3770,13,3770,34,0,0,4,1]],"file_strncmp":[[2051,8,2051,15,0,0,4,0],[2071,14,2071,20,0,0,4,1]],"varexpand":[[543,16,543,29,0,0,4,1],[545,26,545,29,0,0,4,0],[545,33,545,43,0,0,4,0],[545,33,545,43,0,0,4,1],[547,7,547,17,0,0,4,1],[549,25,549,28,0,0,4,0],[549,32,549,42,0,0,4,0],[549,32,549,42,0,0,4,1],[551,7,551,17,0,0,4,1],[554,3,554,11,0,0,4,0],[555,8,555,23,0,0,4,0],[555,8,555,23,0,0,4,1],[563,3,563,10,0,0,4,1],[566,7,566,15,0,0,4,1]],"file_pstring_get_length":[[3717,2,3717,19,0,0,4,0]],"moffset":[[847,4,847,19,0,0,4,0],[848,4,848,21,0,0,4,0],[860,4,860,20,0,0,4,0],[868,4,868,20,0,0,4,0],[873,4,873,21,0,0,4,0],[882,8,882,27,0,0,4,0],[885,8,885,31,0,0,4,0],[887,9,887,26,0,0,4,1],[896,2,896,18,0,0,4,0],[910,2,910,19,0,0,4,0],[945,2,945,19,0,0,4,0],[948,2,948,15,0,0,4,0],[952,2,952,15,0,0,4,0],[954,7,954,14,0,0,4,1],[954,18,954,42,0,0,4,1]],"cvt_id3":[[992,6,992,36,0,0,4,1]],"file_pstring_length_size":[[3688,2,3688,19,0,0,4,0]],"apprentice_map":[[3336,6,3336,50,0,0,4,1],[3339,6,3339,26,0,0,4,1],[3343,6,3343,20,0,0,4,1],[3343,24,3343,47,0,0,4,1],[3352,6,3353,62,0,0,4,1],[3371,6,3371,40,0,0,4,1],[3375,6,3375,65,0,0,4,1]],"dophn_exec":[[1838,8,1838,30,0,0,4,0],[1866,8,1866,30,0,0,4,0],[1871,6,1871,28,0,0,4,0]],"magiccheck":[[2165,2,2165,18,0,0,4,0],[2196,3,2196,11,0,0,4,0],[2208,3,2208,11,0,0,4,0],[2392,2,2392,15,0,0,4,0]]},"library_calls":["magic_version","magic_open","magic_setflags","magic_close","magic_load_buffers","magic_check","magic_descriptor","magic_file","magic_close"],"critical_calls":["magic_version","magic_open","magic_setflags","magic_load_buffers","magic_check","magic_descriptor","magic_file","magic_close"],"visited":3}
/**/


extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	
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





    // 1. Get the magic version first
    int version = magic_version();
    (void)version; // Use version to avoid unused variable warning
    
    // 2. Create a magic cookie
    magic_t cookie = magic_open(fuzz_int32_t_1);
    if (cookie == nullptr) {
        return 0;
    }
    
    // 3. Set some flags on the cookie
    int flags = MAGIC_MIME | MAGIC_CONTINUE;
    if (magic_setflags(cookie, flags) != 0) {
        magic_close(cookie);
        return 0;
    }
    
    // 4. Load magic database from buffers
    // We'll split the input into two buffers for testing
    if (size > 0) {
        size_t half_size = size / 2;
        void *buffers[2];
        size_t sizes[2];
        
        buffers[0] = (void *)data;
        sizes[0] = half_size;
        buffers[1] = (void *)(data + half_size);
        sizes[1] = size - half_size;
        
        // Try to load from buffers - this may fail if data is not valid magic database
        magic_load_buffers(cookie, buffers, sizes, 2);
    }
    
    // 5. Check the magic database
    // We'll use a dummy filename for checking
    const char *dbfile = "magic.mgc";
    magic_check(cookie, fuzz_str_2);
    
    // 6. Create a temporary file from input data to identify
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file != nullptr) {
        // 7. Get file descriptor for the input file
        int fd = fuzz_fileno(in_file);
        if (fd >= 0) {
            // Try to identify using file descriptor
            const char *result_descriptor = magic_descriptor(cookie, fd);
            (void)result_descriptor; // Use result to avoid unused warning
        }
        
        assert_file_closed(&in_file);;
    }
    
    // 8. Also try to identify using magic_file with a created file
    if (size > 0) {
        // Create a temporary file with the input data
        FILE *tmp_file = fopen("input_file", "wb");
        if (tmp_file != nullptr) {
            fwrite(data, 1, size, tmp_file);
            assert_file_closed(&tmp_file);;
            
            // Use magic_file to identify the file
            const char *result_file = magic_file(cookie, "input_file");
            (void)result_file; // Use result to avoid unused warning
            
            // Clean up the temporary file
            remove("input_file");
        }
    }
    
    // 9. Clean up - release all resources
    magic_close(cookie);
    
    assert_file_closed(&in_file);
	return 0;
}