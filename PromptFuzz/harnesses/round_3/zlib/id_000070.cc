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
//<ID> 169
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 1
//<Quality> {"density":18,"unique_branches":{"gz_read":[[323,9,323,17,0,0,4,0]]},"library_calls":["zlibCompileFlags","gzdopen","gzread","gzrewind","gzclose","gzread","inflateInit_","gzclose","compressBound","inflateEnd","gzclose","inflate","inflateMark","deflateInit_","inflateEnd","gzclose","compressBound","deflateEnd","inflateEnd","gzclose","compress2","deflateResetKeep","crc32","crc32","crc32","crc32","crc32_combine64","deflateEnd","inflateEnd","gzclose"],"critical_calls":["zlibCompileFlags","gzdopen","gzread","gzrewind","gzread","inflateInit_","compressBound","inflate","inflateMark","deflateInit_","compressBound","compress2","deflateResetKeep","crc32","crc32","crc32","crc32","crc32_combine64","deflateEnd","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_70(const uint8_t* f_data, size_t f_size) {
	if(f_size<41) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
	//fuzzer shim end}




    // 1. Get zlib compilation flags
    uLong compile_flags = zlibCompileFlags();
    
    // 2. Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 3. Get file descriptor and open as gzFile
    int fd = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 4. Read decompressed data using gzread
    const size_t buf_size = 4096;
    Bytef buffer[buf_size];
    unsigned int bytes_read = 0;
    uLong total_read = 0;
    
    // Create a buffer to store all decompressed data
    Bytef *decompressed_data = nullptr;
    uLong decompressed_size = 0;
    
    // First pass: determine decompressed size
    while ((bytes_read = gzread(gz_in, buffer, buf_size)) > 0) {
        total_read += bytes_read;
    }
    
    // Reset file position
    gzrewind(gz_in);
    
    // Allocate buffer for decompressed data
    decompressed_size = total_read;
    decompressed_data = (Bytef *)malloc(decompressed_size);
    if (!decompressed_data) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Second pass: read all data
    uLong offset = 0;
    while ((bytes_read = gzread(gz_in, decompressed_data + offset, 
                                decompressed_size - offset)) > 0) {
        offset += bytes_read;
    }
    
    // 5. Initialize inflation stream
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(z_stream));
    
    int ret = inflateInit_(&inflate_strm, fuzz_str_1, sizeof(z_stream));
    if (ret != Z_OK) {
        free(decompressed_data);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 6. Set up inflation with the original compressed data
    inflate_strm.next_in = (Bytef *)data;
    inflate_strm.avail_in = size;
    
    // Create output buffer for inflation
    uLong inflate_bound = compressBound(size);
    Bytef *inflate_out = (Bytef *)malloc(inflate_bound);
    if (!inflate_out) {
        inflateEnd(&inflate_strm);
        free(decompressed_data);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    inflate_strm.next_out = inflate_out;
    inflate_strm.avail_out = inflate_bound;
    
    // 7. Perform inflation with mark
    ret = inflate(&inflate_strm, fuzz_int32_t_2);
    long mark_position = inflateMark(&inflate_strm);
    
    // 8. Create compression stream for deflateResetKeep
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(z_stream));
    
    // Initialize deflation
    if (deflateInit(&deflate_strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        free(inflate_out);
        inflateEnd(&inflate_strm);
        free(decompressed_data);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 9. Compress the decompressed data using compress2
    uLongf compressed_size = compressBound(decompressed_size);
    Bytef *compressed_data = (Bytef *)malloc(compressed_size);
    if (!compressed_data) {
        deflateEnd(&deflate_strm);
        free(inflate_out);
        inflateEnd(&inflate_strm);
        free(decompressed_data);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    ret = compress2(compressed_data, &compressed_size, 
                    decompressed_data, decompressed_size, 
                    fuzz_int32_t_3);
    
    // 10. Use deflateResetKeep
    if (deflateResetKeep(&deflate_strm) != Z_OK) {
        // Continue anyway
    }
    
    // 11. Compute CRC32 checksums
    uLong crc1 = crc32(fuzz_uint64_t_4, Z_NULL, 0);
    uLong crc2 = crc32(fuzz_uint64_t_5, Z_NULL, 0);
    
    if (decompressed_size > 0) {
        crc1 = crc32(crc1, decompressed_data, decompressed_size);
    }
    
    if (size > 0) {
        crc2 = crc32(crc2, data, size);
    }
    
    // 12. Combine CRC32 values
    uLong combined_crc = crc32_combine64(crc1, crc2, 0);
    
    // 13. Clean up
    free(compressed_data);
    deflateEnd(&deflate_strm);
    free(inflate_out);
    inflateEnd(&inflate_strm);
    free(decompressed_data);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    
    // Use the computed values to avoid optimization
    volatile uLong dummy = compile_flags + mark_position + combined_crc + compressed_size;
    (void)dummy;
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}