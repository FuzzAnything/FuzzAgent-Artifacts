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
//<ID> 162
//<Prompt> []
/*<Combination>: [
*/
//<score> 56.666668, nr_unique_branch: 50
//<Quality> {"density":17,"unique_branches":{"deflateParams":[[784,9,784,39,0,0,4,0],[786,9,786,18,0,0,4,0],[786,35,786,47,0,0,4,0],[801,13,801,26,0,0,4,0],[801,30,801,45,0,0,4,1]],"deflate_fast":[[1644,8,1644,31,16,0,4,0],[1644,8,1644,31,20,0,4,0]],"flush_pending":[[957,9,957,17,0,0,4,0]],"compress2_z":[[61,14,61,25,0,0,4,0],[65,12,65,31,0,0,4,1]],"deflateInit2_":[[434,9,434,21,0,0,4,0],[435,9,435,23,0,0,4,0],[435,46,435,55,0,0,4,0],[436,9,436,21,0,0,4,0],[436,48,436,63,0,0,4,0],[436,67,436,76,0,0,4,1],[439,9,439,24,0,0,4,0]],"deflateBound_z":[[917,9,917,24,0,0,4,0],[917,28,917,49,0,0,4,0],[918,17,918,42,0,0,4,0],[918,17,918,42,0,0,4,1],[918,46,918,54,0,0,4,0],[918,46,918,54,0,0,4,1],[920,16,920,39,0,0,4,1]],"crc32_combine_gen64":[[955,9,955,17,0,0,4,0]],"inflate":[[542,29,542,47,0,0,4,0]],"deflate_stored":[[1696,13,1696,23,0,0,4,0],[1706,33,1706,64,0,0,4,0],[1790,9,1790,13,0,0,4,1],[1796,9,1796,28,0,0,4,0],[1796,32,1796,49,0,0,4,1],[1802,9,1802,33,0,0,4,1],[1813,9,1813,33,0,0,4,0],[1815,9,1815,13,0,0,4,0],[1820,9,1820,36,0,0,4,0],[1833,9,1833,26,0,0,4,0],[1833,9,1833,26,0,0,4,1],[1834,11,1834,15,0,0,4,0],[1834,41,1834,60,0,0,4,0],[1835,10,1835,32,0,0,4,0],[1835,36,1835,48,0,0,4,0],[1837,16,1837,33,0,0,4,0],[1837,37,1837,59,0,0,4,0],[1838,16,1838,27,0,0,4,0],[1845,9,1845,13,0,0,4,0],[1847,12,1847,16,0,0,4,0],[1651,20,1651,29,14,0,4,1],[1651,20,1651,29,15,0,4,1],[1651,20,1651,29,16,0,4,0],[1651,20,1651,29,19,0,4,1]]},"library_calls":["inflateInit2_","deflateInit2_","inflateEnd","deflateSetHeader","crc32_combine_gen64","deflateParams","deflateBound","deflateBound_z","inflateGetDictionary","inflateSetDictionary","crc32","crc32_z","inflate","deflate","gzdopen","gzwrite","gzclose_w","crc32_combine","crc32_combine64","compress","compress2_z","inflateEnd","deflateEnd"],"critical_calls":["inflateInit2_","deflateInit2_","deflateSetHeader","crc32_combine_gen64","deflateParams","deflateBound","deflateBound_z","inflateGetDictionary","inflateSetDictionary","crc32","crc32_z","inflate","deflate","gzdopen","gzwrite","gzclose_w","crc32_combine","crc32_combine64","compress","compress2_z","inflateEnd","deflateEnd"],"visited":2}
/**/


// Helper function to read integers from fuzzing input
template<typename T>
T readInt(const uint8_t*& data, size_t& size) {
    if (size < sizeof(T)) {
        return 0;
    }
    T value = 0;
    memcpy(&value, data, sizeof(T));
    data += sizeof(T);
    size -= sizeof(T);
    return value;
}

extern "C" int LLVMFuzzerTestOneInput_68(const uint8_t* f_data, size_t f_size) {
	if(f_size<67) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
	//fuzzer shim end}




    // Early exit for insufficient data
    if (!data || size < 100) {
	return 0;
	}
	
    
    // Create input and output files using provided methods
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Initialize variables for decompression
    z_stream strm_inflate;
    memset(&strm_inflate, 0, sizeof(z_stream));
    
    // Read parameters from fuzz input
    const uint8_t* dynamic_data = data;
    size_t dynamic_size = size;
    
    int windowBits = readInt<int>(dynamic_data, dynamic_size) % 16 + 8;
    int level = readInt<int>(dynamic_data, dynamic_size) % 10;
    int method = Z_DEFLATED;
    int memLevel = readInt<int>(dynamic_data, dynamic_size) % 9 + 1;
    int strategy = readInt<int>(dynamic_data, dynamic_size) % 4;
    
    // 1. Initialize decompression for zlib/gzip format detection
    if (inflateInit2_(&strm_inflate, windowBits, fuzz_str_1, sizeof(z_stream)) != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set up input buffer for decompression
    size_t inflate_input_size = dynamic_size > 1024 ? 1024 : dynamic_size;
    Bytef* inflate_input = (Bytef*)malloc(inflate_input_size);
    memcpy(inflate_input, dynamic_data, inflate_input_size);
    strm_inflate.next_in = inflate_input;
    strm_inflate.avail_in = inflate_input_size;
    
    // 2. Initialize compression stream
    z_stream strm_deflate;
    memset(&strm_deflate, 0, sizeof(z_stream));
    
    if (deflateInit2_(&strm_deflate, level, fuzz_int32_t_3, windowBits, memLevel, 
                      strategy, fuzz_str_2, sizeof(z_stream)) != Z_OK) {
        inflateEnd(&strm_inflate);
        free(inflate_input);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 3. Set gzip header if windowBits indicates gzip format
    if (windowBits > 15) {
        gz_header header;
        memset(&header, 0, sizeof(gz_header));
        header.text = readInt<int>(dynamic_data, dynamic_size) & 1;
        header.time = readInt<uLong>(dynamic_data, dynamic_size);
        header.os = readInt<int>(dynamic_data, dynamic_size) % 255;
        deflateSetHeader(&strm_deflate, &header);
    }
    
    // 4. Generate CRC combine value
    off64_t len1 = readInt<off64_t>(dynamic_data, dynamic_size);
    uLong crc_combine_gen = crc32_combine_gen64(len1);
    
    // 5. Change compression parameters during operation
    if (dynamic_size > 10) {
        int new_level = readInt<int>(dynamic_data, dynamic_size) % 10;
        int new_strategy = readInt<int>(dynamic_data, dynamic_size) % 4;
        deflateParams(&strm_deflate, new_level, new_strategy);
    }
    
    // 6. Calculate bound for compressed size
    uLong source_len = readInt<uLong>(dynamic_data, dynamic_size) % 65536;
    uLong deflate_bound = deflateBound(&strm_deflate, source_len);
    z_size_t deflate_bound_z = deflateBound_z(&strm_deflate, source_len);
    
    // 7. Get dictionary from inflate stream (if any)
    Bytef dict_buffer[256];
    uInt dict_length = 256;
    inflateGetDictionary(&strm_inflate, dict_buffer, &dict_length);
    
    // 8. Set dictionary for inflate stream
    if (dict_length > 0 && dynamic_size > dict_length) {
        inflateSetDictionary(&strm_inflate, dynamic_data, dict_length);
        dynamic_data += dict_length;
        dynamic_size -= dict_length;
    }
    
    // 9. Calculate CRC32 using different functions
    uLong crc = 0;
    if (dynamic_size > 100) {
        // Use crc32 for first part
        crc = crc32(fuzz_uint64_t_4, dynamic_data, 100);
        
        // Use crc32_z for second part if available
        if (dynamic_size > 200) {
            crc = crc32_z(fuzz_uint64_t_5, dynamic_data + 100, 100);
        }
    }
    
    // 10. Perform decompression
    Bytef inflate_output[4096];
    strm_inflate.next_out = inflate_output;
    strm_inflate.avail_out = sizeof(inflate_output);
    
    inflate(&strm_inflate, fuzz_int32_t_6);
    
    // 11. Perform compression of decompressed data
    if (strm_inflate.total_out > 0) {
        strm_deflate.next_in = inflate_output;
        strm_deflate.avail_in = strm_inflate.total_out;
        
        Bytef deflate_output[8192];
        strm_deflate.next_out = deflate_output;
        strm_deflate.avail_out = sizeof(deflate_output);
        
        deflate(&strm_deflate, fuzz_int32_t_7);
        
        // 12. Write compressed data to gzFile
        gzFile gz_out = gzdopen(out_fd, fuzz_str_8);
        if (gz_out) {
            gzwrite(gz_out, deflate_output, sizeof(deflate_output) - strm_deflate.avail_out);
            gzclose_w(gz_out);
        }
    }
    
    // 13. Combine CRC values
    uLong crc1 = readInt<uLong>(dynamic_data, dynamic_size);
    uLong crc2 = readInt<uLong>(dynamic_data, dynamic_size);
    off_t len2 = readInt<off_t>(dynamic_data, dynamic_size);
    off64_t len3 = readInt<off64_t>(dynamic_data, dynamic_size);
    
    uLong combined_crc = crc32_combine(crc1, crc2, len2);
    uLong combined_crc64 = crc32_combine64(crc1, crc2, len3);
    
    // 14. Use simple compression APIs
    if (dynamic_size > 100) {
        uLongf destLen = deflate_bound;
        Bytef* compress_dest = (Bytef*)malloc(deflate_bound);
        compress(compress_dest, &destLen, dynamic_data, 
                dynamic_size > 65536 ? 65536 : dynamic_size);
        free(compress_dest);
    }
    
    // 15. Use compress2_z
    if (dynamic_size > 50) {
        z_size_t destLen_z = deflate_bound_z;
        Bytef* compress_dest_z = (Bytef*)malloc(deflate_bound_z);
        int comp_level = readInt<int>(dynamic_data, dynamic_size) % 10;
        compress2_z(compress_dest_z, &destLen_z, dynamic_data, 
                   dynamic_size > 32768 ? 32768 : dynamic_size, comp_level);
        free(compress_dest_z);
    }
    
    // Cleanup
    inflateEnd(&strm_inflate);
    deflateEnd(&strm_deflate);
    free(inflate_input);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}