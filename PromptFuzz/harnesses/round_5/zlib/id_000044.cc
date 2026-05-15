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
//<ID> 63
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 2
//<Quality> {"density":11,"unique_branches":{"gzseek64":[[412,9,412,19,0,0,4,0],[413,13,413,35,0,0,4,0]]},"library_calls":["zlibCompileFlags","gzdopen","compressBound","compress2","gzclose_r","gzdopen","gzclose_r","gzfwrite","gzseek64","gzrewind","inflateInit2_","inflateReset2","inflateEnd","crc32","crc32","crc32_combine64","crc32_combine_gen64","crc32_combine_op","gzclose_r","gzclose"],"critical_calls":["zlibCompileFlags","gzdopen","compressBound","compress2","gzdopen","gzfwrite","gzseek64","gzrewind","inflateInit2_","inflateReset2","inflateEnd","crc32","crc32","crc32_combine64","crc32_combine_gen64","crc32_combine_op","gzclose_r","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. First, get the zlib compilation flags (API 1)
    uLong flags = zlibCompileFlags();
    
    if (size < 2) {
        return 0;
    }
    
    // Create a FILE* for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptor for reading
    int fd_in = fuzz_fileno(in_file);
    
    // 2. Open gzFile for reading the compressed data
    gzFile gz_in = gzdopen(fd_in, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 3. Compress the data using compress2 (API 2)
    // First, determine the bound for compressed data
    uLongf compressed_bound = compressBound(size);
    Bytef *compressed_data = new Bytef[compressed_bound];
    Bytef *source_data = new Bytef[size];
    
    // Read the source data from input
    memcpy(source_data, data, size);
    
    uLongf destLen = compressed_bound;
    int level = data[0] % 10;  // Use first byte to determine compression level (0-9)
    int compress_result = compress2(compressed_data, &destLen, source_data, size, level);
    
    // 4. Create output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        delete[] compressed_data;
        delete[] source_data;
        gzclose_r(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Get file descriptor for writing
    int fd_out = fuzz_fileno(out_file);
    
    // 5. Open gzFile for writing
    gzFile gz_out = gzdopen(fd_out, "wb");
    if (!gz_out) {
        delete[] compressed_data;
        delete[] source_data;
        assert_file_closed(&out_file);;
        gzclose_r(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 6. Write compressed data to gzFile using gzfwrite (API 3)
    z_size_t items_written = gzfwrite(compressed_data, 1, destLen, gz_out);
    
    // 7. Use gzseek64 (API 4) - seek back to beginning
    off64_t seek_result = gzseek64(gz_out, 0, SEEK_SET);
    
    // 8. Rewind the input gzFile (API 7)
    int rewind_result = gzrewind(gz_in);
    
    // 9. Set up inflation stream for inflateReset2
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    
    // Initialize inflation with default windowBits
    int init_result = inflateInit2(&strm, 15 + 16);  // 15+16 for gzip decoding
    
    if (init_result == Z_OK) {
        // 10. Reset inflation with different windowBits (API 8)
        int windowBits = (data[1] % 15) + 8;  // Use second byte for windowBits (8-22)
        int reset_result = inflateReset2(&strm, windowBits);
        
        // Clean up inflation stream
        inflateEnd(&strm);
    }
    
    // 11. Calculate CRC32 values for combining
    uLong crc1 = crc32(0L, data, size / 2);
    uLong crc2 = crc32(0L, data + size / 2, size - (size / 2));
    off64_t len2 = size - (size / 2);
    
    // 12. Combine CRC32 values using crc32_combine64 (API 6)
    uLong combined_crc = crc32_combine64(crc1, crc2, len2);
    
    // 13. Also use crc32_combine_op (API 9)
    // First generate the combine operation
    uLong op = crc32_combine_gen64(len2);
    uLong combined_crc_op = crc32_combine_op(crc1, crc2, op);
    
    // 14. Close the reading gzFile (API 5)
    int close_read_result = gzclose_r(gz_in);
    
    // Close the writing gzFile
    gzclose(gz_out);
    
    // Clean up resources
    delete[] compressed_data;
    delete[] source_data;
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Clean up the output file
    remove("output_file");
    
    // Use the computed values to avoid optimization
    volatile uLong result = flags + compress_result + items_written + 
                           seek_result + rewind_result + combined_crc + 
                           combined_crc_op + close_read_result;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}