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
//<ID> 53
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["inflateInit2_","inflateEnd","inflateMark","inflate","inflateEnd","inflateResetKeep","compressBound","inflateEnd","compress2","inflateEnd","inflateEnd","gzdopen","inflateEnd","gzwrite","gzclose","inflateEnd","gzprintf","crc32","crc32","crc32","crc32","crc32_combine64","gzprintf","gzflush","gzclose","inflateEnd"],"critical_calls":["inflateInit2_","inflateMark","inflate","inflateResetKeep","compressBound","compress2","gzdopen","gzwrite","gzprintf","crc32","crc32","crc32","crc32","crc32_combine64","gzprintf","gzflush","gzclose","inflateEnd"],"visited":1}
/*Here's a C++ fuzz driver that uses all the specified zlib APIs to achieve the event of decompressing and recompressing data while writing to a gzip file and computing combined checksums:

*/


extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<29) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimum data

    // 1. First, decompress the input data (assumed to be zlib compressed)
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize inflate stream
    if (inflateInit2(&strm, 15 + 16) != Z_OK) {
        return 0;
    }
    
    strm.next_in = (Bytef*)data;
    strm.avail_in = (uInt)size;
    
    // Allocate buffer for decompressed data
    size_t decomp_size = size * 10;  // Conservative estimate
    Bytef* decompressed = (Bytef*)malloc(decomp_size);
    if (!decompressed) {
        inflateEnd(&strm);
        return 0;
    }
    
    strm.next_out = decompressed;
    strm.avail_out = (uInt)decomp_size;
    
    // Call inflateMark to get a marker during decompression
    long mark = inflateMark(&strm);
    
    // Perform decompression
    int inflate_result = inflate(&strm, fuzz_int32_t_1);
    
    if (inflate_result != Z_STREAM_END && inflate_result != Z_OK) {
        free(decompressed);
        inflateEnd(&strm);
        return 0;
    }
    
    size_t decompressed_len = decomp_size - strm.avail_out;
    
    // 2. Use inflateResetKeep to reset the stream without freeing state
    inflateResetKeep(&strm);
    
    // 3. Recompress the decompressed data using compress2
    uLongf compressed_len = compressBound((uLong)decompressed_len);
    Bytef* recompressed = (Bytef*)malloc(compressed_len);
    if (!recompressed) {
        free(decompressed);
        inflateEnd(&strm);
        return 0;
    }
    
    int level = data[0] % 10;  // Use first byte to determine compression level
    if (compress2(recompressed, &compressed_len, decompressed, 
                  (uLong)decompressed_len, level) != Z_OK) {
        free(recompressed);
        free(decompressed);
        inflateEnd(&strm);
        return 0;
    }
    
    // 4. Create a gzip file and write to it
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        free(recompressed);
        free(decompressed);
        inflateEnd(&strm);
        assert_file_closed(&out_file);
	return 0;
    }
    
    int fd = fuzz_fileno(out_file);
    gzFile gz_file = gzdopen(fd, fuzz_str_2);
    if (!gz_file) {
        assert_file_closed(&out_file);;
        free(recompressed);
        free(decompressed);
        inflateEnd(&strm);
        assert_file_closed(&out_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Write the recompressed data to gzip file
    if (gzwrite(gz_file, recompressed, (unsigned int)compressed_len) <= 0) {
        gzclose(gz_file);
        assert_file_closed(&out_file);;
        free(recompressed);
        free(decompressed);
        inflateEnd(&strm);
        assert_file_closed(&out_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Write a formatted string to gzip file
    const char* format_str = "Decompressed: %lu bytes, Recompressed: %lu bytes\n";
    gzprintf(gz_file, format_str, decompressed_len, compressed_len);
    
    // 5. Compute CRC32 checksums and combine them
    uLong crc1 = crc32(fuzz_uint64_t_3, Z_NULL, 0);
    uLong crc2 = crc32(fuzz_uint64_t_4, Z_NULL, 0);
    
    // Split decompressed data into two parts for checksum calculation
    size_t split_point = decompressed_len / 2;
    if (split_point > 0) {
        crc1 = crc32(crc1, decompressed, (uInt)split_point);
        crc2 = crc32(crc2, decompressed + split_point, 
                     (uInt)(decompressed_len - split_point));
        
        // Combine the two CRC32 checksums using combine64
        uLong combined_crc = crc32_combine64(crc1, crc2, (off64_t)(decompressed_len - split_point));
        
        // Write combined CRC to gzip file (as hex string)
        gzprintf(gz_file, "Combined CRC64: %08lX\n", combined_crc);
    }
    
    // 6. Clean up
    gzflush(gz_file, fuzz_int32_t_5);
    gzclose(gz_file);
    assert_file_closed(&out_file);;
    
    free(recompressed);
    free(decompressed);
    inflateEnd(&strm);
    
    assert_file_closed(&out_file);
	assert_fd_closed(fd);
	return 0;
}