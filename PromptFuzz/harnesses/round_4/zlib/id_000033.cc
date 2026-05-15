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
#include <zlib.h>
}
//<ID> 67
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 8
//<Quality> {"density":8,"unique_branches":{"inflateReset2":[[145,9,145,23,0,0,4,0],[146,13,146,29,0,0,4,1]],"inflate":[[507,17,507,33,0,0,4,0],[1073,17,1073,28,0,0,4,1],[1098,17,1098,28,0,0,4,1],[1144,9,1144,26,0,0,4,1]],"deflateSetDictionary":[[580,9,580,32,0,0,4,0],[581,13,581,22,0,0,4,1]]},"library_calls":["inflateInit2_","inflate","inflateReset2","get_crc_table","crc32","crc32","inflateResetKeep","deflateInit2_","inflateEnd","deflateSetDictionary","deflateParams","deflateBound","deflate","deflateEnd","inflateBackInit_","inflateBackEnd","inflateEnd","gzdopen","gzerror","gzclose"],"critical_calls":["inflateInit2_","inflate","inflateReset2","get_crc_table","crc32","crc32","inflateResetKeep","deflateInit2_","deflateSetDictionary","deflateParams","deflateBound","deflate","deflateEnd","inflateBackInit_","inflateBackEnd","inflateEnd","gzdopen","gzerror","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<108) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_15, fuzz_str_sz_15, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_16, fuzz_str_sz_16, fdp);
	//fuzzer shim end}




    // Early return if input is too small
    if (size < 100) {
	return 0;
	}
	
    
    // Split input into two parts: dictionary and compressed data
    size_t dict_size = size / 3;
    size_t comp_size = size - dict_size;
    const Bytef* dictionary = data;
    const Bytef* compressed_data = data + dict_size;
    
    // Prepare output buffers
    std::vector<Bytef> decompressed(comp_size * 4);
    std::vector<Bytef> recompressed(comp_size * 2);
    
    // 1. Initialize inflation stream for raw deflate data
    z_stream strm_inflate;
    memset(&strm_inflate, 0, sizeof(z_stream));
    int ret = inflateInit2_(&strm_inflate, fuzz_int32_t_2, fuzz_str_1, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    // Set input for inflation
    strm_inflate.next_in = const_cast<Bytef*>(compressed_data);
    strm_inflate.avail_in = comp_size;
    strm_inflate.next_out = decompressed.data();
    strm_inflate.avail_out = decompressed.size();
    
    // 2. Inflate with Z_SYNC_FLUSH
    ret = inflate(&strm_inflate, fuzz_int32_t_3);
    
    // 3. Reset inflation stream with different window bits
    ret = inflateReset2(&strm_inflate, fuzz_int32_t_4);
    
    // 4. Get CRC table (for potential CRC calculation)
    const z_crc_t* crc_table = get_crc_table();
    uLong crc_value = crc32(fuzz_uint64_t_5, Z_NULL, 0);
    if (crc_table && strm_inflate.avail_out > 0) {
        crc_value = crc32(crc_value, decompressed.data(), 
                         decompressed.size() - strm_inflate.avail_out);
    }
    
    // 5. Reset inflation stream keeping history
    ret = inflateResetKeep(&strm_inflate);
    
    // 6. Initialize deflation stream
    z_stream strm_deflate;
    memset(&strm_deflate, 0, sizeof(z_stream));
    ret = deflateInit2_(&strm_deflate, fuzz_int32_t_7, fuzz_int32_t_8,
                        fuzz_int32_t_9, fuzz_int32_t_10, fuzz_int32_t_11,
                        fuzz_str_6, sizeof(z_stream));
    if (ret != Z_OK) {
        inflateEnd(&strm_inflate);
        return 0;
    }
    
    // 7. Set dictionary for deflation
    ret = deflateSetDictionary(&strm_deflate, dictionary, dict_size);
    
    // 8. Change deflation parameters
    ret = deflateParams(&strm_deflate, fuzz_int32_t_12, fuzz_int32_t_13);
    
    // 9. Get bound for deflated output
    uLong bound = deflateBound(&strm_deflate, decompressed.size() - strm_inflate.avail_out);
    
    // 10. Prepare deflation input
    strm_deflate.next_in = decompressed.data();
    strm_deflate.avail_in = decompressed.size() - strm_inflate.avail_out;
    strm_deflate.next_out = recompressed.data();
    strm_deflate.avail_out = recompressed.size();
    
    // 11. Perform deflation
    ret = deflate(&strm_deflate, fuzz_int32_t_14);
    
    // 12. End deflation
    deflateEnd(&strm_deflate);
    
    // 13. Initialize back-inflation stream
    z_stream strm_back;
    memset(&strm_back, 0, sizeof(z_stream));
    std::vector<unsigned char> back_window(32768);
    ret = inflateBackInit_(&strm_back, 15, back_window.data(),
                          fuzz_str_15, sizeof(z_stream));
    if (ret == Z_OK) {
        // Note: inflateBack requires callbacks which we can't properly implement
        // with fuzzed data, so we just initialize and end it
        inflateBackEnd(&strm_back);
    }
    
    // 14. End inflation
    inflateEnd(&strm_inflate);
    
    // 15. Test gzFile error handling
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        int fd = fuzz_fileno(in_file);
        gzFile gz = gzdopen(fd, fuzz_str_16);
        if (gz) {
            int errnum = 0;
            const char* err_msg = gzerror(gz, &errnum);
            gzclose(gz);
        }
        assert_file_closed(&in_file);;
    }
    
    assert_file_closed(&in_file);
	return 0;
}