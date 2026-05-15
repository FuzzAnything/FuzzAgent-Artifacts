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
//<ID> 61
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 2
//<Quality> {"density":8,"unique_branches":{"inflateReset2":[[162,9,162,32,0,0,4,0],[162,36,162,72,0,0,4,1]]},"library_calls":["zlibCompileFlags","gzdopen","gzrewind","gzclose_r","inflateInit2_","gzclose_r","inflate","inflateReset2","inflateEnd","gzseek64","gzclose_r","crc32","crc32","crc32","crc32","crc32_combine64","crc32_combine_gen64","crc32_combine_op","gzdopen","gzfwrite","gzclose","compressBound","compress2"],"critical_calls":["zlibCompileFlags","gzdopen","gzrewind","inflateInit2_","inflate","inflateReset2","inflateEnd","gzseek64","gzclose_r","crc32","crc32","crc32","crc32","crc32_combine64","crc32_combine_gen64","crc32_combine_op","gzdopen","gzfwrite","gzclose","compressBound","compress2"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. First, get the zlib compilation flags
    uLong compile_flags = zlibCompileFlags();
    (void)compile_flags; // Use variable to avoid unused warning
    
    // 2. Create a temporary file to write the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Get file descriptor and open as gzFile for reading
    int fd = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 4. Try to rewind the gzFile
    gzrewind(gz_in);
    
    // 5. Allocate buffer for decompressed data
    // Estimate decompressed size (worst case: assume 1000x expansion, but limit to reasonable size)
    const size_t max_decompressed_size = size * 10 + 10000;
    Bytef *decompressed = (Bytef*)malloc(max_decompressed_size);
    if (!decompressed) {
        gzclose_r(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 6. Create inflation stream for decompression
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    
    int ret = inflateInit2(&strm, 15 + 32); // 32 for automatic header detection
    if (ret != Z_OK) {
        free(decompressed);
        gzclose_r(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 7. Read from gzFile and decompress
    strm.avail_in = (uInt)size;
    strm.next_in = (Bytef*)data; // Use original input data
    strm.avail_out = (uInt)max_decompressed_size;
    strm.next_out = decompressed;
    
    ret = inflate(&strm, Z_FINISH);
    
    // 8. Reset the inflation stream with different window bits
    inflateReset2(&strm, 15); // Reset with different window bits
    
    // 9. Close the inflation stream
    inflateEnd(&strm);
    
    // 10. Try seeking in the gzFile
    off64_t seek_pos = gzseek64(gz_in, size/2, SEEK_SET);
    (void)seek_pos; // Use variable to avoid unused warning
    
    // 11. Close the gzFile for reading
    gzclose_r(gz_in);
    
    // 12. Calculate CRC32 values for combining
    uLong crc1 = crc32(0L, Z_NULL, 0);
    uLong crc2 = crc32(0L, Z_NULL, 0);
    
    if (size > 0) {
        // Calculate CRC for first half
        size_t half1 = size / 2;
        if (half1 > 0) {
            crc1 = crc32(0L, data, (uInt)half1);
        }
        
        // Calculate CRC for second half
        size_t half2 = size - half1;
        if (half2 > 0) {
            crc2 = crc32(0L, data + half1, (uInt)half2);
        }
        
        // Combine the two CRC values using crc32_combine64
        off64_t len2 = (off64_t)half2;
        uLong combined_crc = crc32_combine64(crc1, crc2, len2);
        
        // Also use crc32_combine_op
        // Generate the combine operation
        uLong op = crc32_combine_gen64(len2);
        uLong combined_crc_op = crc32_combine_op(crc1, crc2, op);
        
        (void)combined_crc; // Use variables
        (void)combined_crc_op;
    }
    
    // 13. Create output file for writing compressed data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        free(decompressed);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 14. Get file descriptor and open as gzFile for writing
    int out_fd = fuzz_fileno(out_file);
    gzFile gz_out = gzdopen(out_fd, "wb");
    if (!gz_out) {
        free(decompressed);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 15. Write decompressed data to gzFile using gzfwrite
    if (strm.total_out > 0) {
        size_t items = 1;
        size_t write_size = (size_t)strm.total_out;
        z_size_t written = gzfwrite(decompressed, write_size, items, gz_out);
        (void)written; // Use variable
    }
    
    // 16. Close the gzFile for writing
    gzclose(gz_out); // Note: gzclose_w is not in our list, using gzclose
    
    // 17. Now compress the original data using compress2
    uLongf destLen = compressBound((uLong)size);
    Bytef *compressed = (Bytef*)malloc(destLen);
    if (compressed) {
        // Try different compression levels
        for (int level = 0; level <= 9; level += 3) {
            uLongf currentDestLen = destLen;
            int compress_ret = compress2(compressed, &currentDestLen, data, (uLong)size, level);
            (void)compress_ret; // Use variable
        }
        free(compressed);
    }
    
    // 18. Clean up
    free(decompressed);
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    // Remove temporary output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
}