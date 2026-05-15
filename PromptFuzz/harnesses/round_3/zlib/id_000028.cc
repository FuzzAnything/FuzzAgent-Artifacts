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
//<ID> 50
//<Prompt> []
/*<Combination>: [
*/
//<score> 46, nr_unique_branch: 14
//<Quality> {"density":23,"unique_branches":{"gz_write":[[193,9,193,17,0,0,4,0]],"gz_vacate":[[386,9,386,66,0,0,4,0]],"gzvprintf":[[417,9,417,21,0,0,4,1],[423,9,423,32,0,0,4,1],[423,37,423,55,0,0,4,1],[428,9,428,25,0,0,4,0],[428,29,428,49,0,0,4,1],[432,9,432,20,0,0,4,1],[439,9,439,19,0,0,4,1],[450,9,450,28,0,0,4,0],[472,9,472,17,0,0,4,1],[472,21,472,49,0,0,4,1],[472,53,472,79,0,0,4,1],[481,9,481,19,0,0,4,1]]},"library_calls":["inflateInit_","inflateEnd","inflate","inflateEnd","inflateMark","inflateResetKeep","inflateEnd","compressBound","inflateEnd","compress2","inflateEnd","crc32","crc32","crc32","crc32","crc32_combine64","inflateEnd","gzdopen","inflateEnd","gzwrite","gzprintf","gzclose","inflateEnd"],"critical_calls":["inflateInit_","inflate","inflateMark","inflateResetKeep","compressBound","compress2","crc32","crc32","crc32","crc32","crc32_combine64","gzdopen","gzwrite","gzprintf","gzclose","inflateEnd"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified zlib APIs to achieve a scenario where we decompress input data, process it through multiple zlib operations, and write results to a gzip file:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<25) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // 1. First, decompress the input data (assuming it's zlib-compressed)
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize decompression
    if (inflateInit(&strm) != Z_OK) {
        return 0;
    }
    
    // Set input data
    strm.next_in = (Bytef *)data;
    strm.avail_in = size;
    
    // Create output buffer for decompression
    size_t decompressed_size = size * 4; // Conservative estimate
    Bytef *decompressed = (Bytef *)malloc(decompressed_size);
    if (!decompressed) {
        inflateEnd(&strm);
        return 0;
    }
    
    strm.next_out = decompressed;
    strm.avail_out = decompressed_size;
    
    // Decompress the data
    int ret = inflate(&strm, fuzz_int32_t_1);
    if (ret != Z_STREAM_END && ret != Z_OK) {
        free(decompressed);
        inflateEnd(&strm);
        return 0;
    }
    
    size_t actual_decompressed = strm.total_out;
    
    // 2. Use inflateMark to mark current position
    long mark = inflateMark(&strm);
    
    // 3. Use inflateResetKeep to reset stream while keeping history
    if (inflateResetKeep(&strm) != Z_OK) {
        free(decompressed);
        inflateEnd(&strm);
        return 0;
    }
    
    // 4. Recompress the decompressed data using compress2
    uLongf compressed_size = compressBound(actual_decompressed);
    Bytef *recompressed = (Bytef *)malloc(compressed_size);
    if (!recompressed) {
        free(decompressed);
        inflateEnd(&strm);
        return 0;
    }
    
    int level = data[0] % 10; // Use first byte to determine compression level
    if (compress2(recompressed, &compressed_size, decompressed, 
                  actual_decompressed, level) != Z_OK) {
        free(recompressed);
        free(decompressed);
        inflateEnd(&strm);
        return 0;
    }
    
    // 5. Calculate CRC32 of decompressed data
    uLong crc1 = crc32(fuzz_uint64_t_2, Z_NULL, 0);
    crc1 = crc32(crc1, decompressed, actual_decompressed);
    
    // Calculate CRC32 of recompressed data
    uLong crc2 = crc32(fuzz_uint64_t_3, Z_NULL, 0);
    crc2 = crc32(crc2, recompressed, compressed_size);
    
    // 6. Use crc32_combine64 to combine CRCs
    off64_t offset = (off64_t)actual_decompressed;
    uLong combined_crc = crc32_combine64(crc1, crc2, offset);
    
    // 7. Write results to a gzip file using gzwrite and gzprintf
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        free(recompressed);
        free(decompressed);
        inflateEnd(&strm);
        assert_file_closed(&out_file);
	return 0;
    }
    
    int fd = fuzz_fileno(out_file);
    gzFile gz_out = gzdopen(fd, fuzz_str_4);
    if (!gz_out) {
        assert_file_closed(&out_file);;
        free(recompressed);
        free(decompressed);
        inflateEnd(&strm);
        assert_file_closed(&out_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Write decompressed data using gzwrite
    if (gzwrite(gz_out, decompressed, actual_decompressed) != (int)actual_decompressed) {
        // Handle error
    }
    
    // Write combined CRC using gzprintf
    char crc_buffer[100];
    snprintf(crc_buffer, sizeof(crc_buffer), 
             "Combined CRC: %lu, Mark: %ld, Size: %zu", 
             combined_crc, mark, actual_decompressed);
    gzprintf(gz_out, "%s", crc_buffer);
    
    // 8. Clean up
    gzclose(gz_out);  // This also closes the underlying file descriptor
    
    // Note: assert_file_closed(&out_file); is not needed because gzclose already closed it
    
    free(recompressed);
    free(decompressed);
    inflateEnd(&strm);
    
    assert_file_closed(&out_file);
	assert_fd_closed(fd);
	return 0;
}