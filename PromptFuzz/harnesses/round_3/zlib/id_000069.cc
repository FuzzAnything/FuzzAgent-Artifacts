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
//<ID> 168
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 3
//<Quality> {"density":9,"unique_branches":{"deflateStateCheck":[[544,9,544,20,0,0,4,0]],"deflateEnd":[[1296,9,1296,32,0,0,4,0]],"deflateResetKeep":[[647,9,647,32,0,0,4,0]]},"library_calls":["zlibCompileFlags","gzdopen","gzclose","gzread","inflateInit_","gzclose","inflateMark","inflateEnd","gzclose","inflate","crc32","inflateEnd","gzclose","inflateEnd","gzclose","compressBound","inflateEnd","gzclose","compress2","crc32","crc32_combine64","deflateInit_","deflateResetKeep","inflateReset","deflateEnd","inflateEnd","gzclose"],"critical_calls":["zlibCompileFlags","gzdopen","gzread","inflateInit_","inflateMark","inflate","crc32","compressBound","compress2","crc32","crc32_combine64","deflateInit_","deflateResetKeep","inflateReset","deflateEnd","inflateEnd","gzclose"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified zlib APIs to process zlib-compressed data:

*/


extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t* f_data, size_t f_size) {
	if(f_size<42) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimum data for meaningful fuzzing
    
    // 1. Get zlib compile flags (API usage)
    uLong compile_flags = zlibCompileFlags();
    
    // 2. Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 3. Open gzFile using gzdopen (API usage)
    int fd_in = fuzz_fileno(in_file);
    gzFile gzfile = gzdopen(fd_in, fuzz_str_1);
    if (!gzfile) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 4. Read compressed data using gzread (API usage)
    unsigned int read_len = size > 1000 ? 1000 : (unsigned int)size;
    Bytef *read_buf = (Bytef *)malloc(read_len);
    if (!read_buf) {
        gzclose(gzfile);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    int bytes_read = gzread(gzfile, read_buf, read_len);
    
    // 5. Initialize inflation stream (API usage)
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(z_stream));
    inflate_strm.zalloc = Z_NULL;
    inflate_strm.zfree = Z_NULL;
    inflate_strm.opaque = Z_NULL;
    inflate_strm.avail_in = bytes_read > 0 ? bytes_read : 0;
    inflate_strm.next_in = read_buf;
    
    int ret = inflateInit_(&inflate_strm, fuzz_str_2, sizeof(z_stream));
    if (ret != Z_OK) {
        free(read_buf);
        gzclose(gzfile);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 6. Use inflateMark (API usage)
    long mark_position = inflateMark(&inflate_strm);
    
    // 7. Allocate output buffer for inflation
    Bytef *inflate_out = (Bytef *)malloc(1024);
    if (!inflate_out) {
        inflateEnd(&inflate_strm);
        free(read_buf);
        gzclose(gzfile);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    inflate_strm.avail_out = 1024;
    inflate_strm.next_out = inflate_out;
    
    // 8. Perform inflation (API usage)
    ret = inflate(&inflate_strm, fuzz_int32_t_3);
    
    // 9. Get CRC32 of compressed data (API usage)
    uLong compressed_crc = crc32(fuzz_uint64_t_4, (const Bytef *)data, 
                                 size > 100 ? 100 : (uInt)size);
    
    // 10. Create output file for compression
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        free(inflate_out);
        inflateEnd(&inflate_strm);
        free(read_buf);
        gzclose(gzfile);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    int fd_out = fuzz_fileno(out_file);
    
    // 11. Prepare data for compression
    uLong source_len = inflate_strm.total_out > 0 ? inflate_strm.total_out : 100;
    Bytef *source_data = (Bytef *)malloc(source_len);
    if (!source_data) {
        assert_file_closed(&out_file);;
        free(inflate_out);
        inflateEnd(&inflate_strm);
        free(read_buf);
        gzclose(gzfile);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Use inflated data if available, otherwise use some test data
    if (inflate_strm.total_out > 0) {
        memcpy(source_data, inflate_out, inflate_strm.total_out);
    } else {
        memset(source_data, 'A', source_len);
    }
    
    // 12. Get compression bound (API usage)
    uLong bound_size = compressBound(source_len);
    Bytef *compressed_data = (Bytef *)malloc(bound_size);
    if (!compressed_data) {
        free(source_data);
        assert_file_closed(&out_file);;
        free(inflate_out);
        inflateEnd(&inflate_strm);
        free(read_buf);
        gzclose(gzfile);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 13. Perform compression (API usage)
    uLongf dest_len = bound_size;
    int level = (data[0] % 10) + 1;  // Use fuzzer data to determine level
    ret = compress2(compressed_data, &dest_len, source_data, 
                    source_len, level);
    
    // 14. Write compressed data to output file
    if (ret == Z_OK && dest_len > 0) {
        fwrite(compressed_data, 1, dest_len, out_file);
    }
    
    // 15. Get another CRC32 for combination (API usage)
    uLong crc2 = crc32(fuzz_uint64_t_5, compressed_data, 
                       dest_len > 50 ? 50 : (uInt)dest_len);
    
    // 16. Combine CRC32 values (API usage)
    uLong combined_crc = crc32_combine64(compressed_crc, crc2, 100);
    
    // 17. Create a deflate stream for deflateResetKeep
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(z_stream));
    deflate_strm.zalloc = Z_NULL;
    deflate_strm.zfree = Z_NULL;
    deflate_strm.opaque = Z_NULL;
    
    // Initialize deflate
    deflateInit(&deflate_strm, level);
    deflate_strm.avail_in = source_len;
    deflate_strm.next_in = source_data;
    
    // 18. Use deflateResetKeep (API usage)
    ret = deflateResetKeep(&deflate_strm);
    
    // 19. Inflate reset
    ret = inflateReset(&inflate_strm);
    
    // 20. Clean up all resources
    deflateEnd(&deflate_strm);
    inflateEnd(&inflate_strm);
    free(compressed_data);
    free(source_data);
    free(inflate_out);
    free(read_buf);
    gzclose(gzfile);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // 21. Clean up output file
    remove("output_file");
    
    // Use compile_flags and combined_crc to avoid optimization
    if (compile_flags > 0 && combined_crc > 0) {
        // Do nothing, just prevent optimization
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}