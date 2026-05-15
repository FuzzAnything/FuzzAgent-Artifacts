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
//<ID> 119
//<Prompt> []
/*<Combination>: [
*/
//<score> 40, nr_unique_branch: 3
//<Quality> {"density":20,"unique_branches":{"gzeof":[[505,9,505,31,0,0,4,0],[505,35,505,58,0,0,4,1],[509,12,509,34,0,0,4,1]]},"library_calls":["gzdopen","gzopen64","gzclose","gzbuffer","inflateInit_","gzclose","gzclose","inflateEnd","gzclose","gzclose","uncompress2_z","inflateReset","inflate","gzputc","deflateInit_","deflateSetDictionary","deflateEnd","adler32","adler32","adler32_combine","adler32_combine64","gzeof","gzclearerr","gzrewind","gzgets","gzseek","inflateMark","inflateEnd","gzclose","gzclose"],"critical_calls":["gzdopen","gzopen64","gzbuffer","inflateInit_","uncompress2_z","inflateReset","inflate","gzputc","deflateInit_","deflateSetDictionary","deflateEnd","adler32","adler32","adler32_combine","adler32_combine64","gzeof","gzclearerr","gzrewind","gzgets","gzseek","inflateMark","inflateEnd","gzclose","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t* f_data, size_t f_size) {
	if(f_size<68) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Create temporary file with input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
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
    
    // Open input file as gzFile for reading
    gzFile gz_in = gzdopen(in_fd, fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Open output file as gzFile for writing
    gzFile gz_out = gzopen64("output_file", fuzz_str_2);
    if (!gz_out) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set buffer size for gz_out
    gzbuffer(gz_out, fuzz_uint32_t_3);
    
    // Initialize zlib stream for inflation
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    
    int ret = inflateInit_(&strm, fuzz_str_4, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_out);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Allocate buffers for decompression
    Bytef *compressed_buf = (Bytef*)malloc(size);
    Bytef *decompressed_buf = (Bytef*)malloc(size * 2);  // Assume expansion factor of 2
    z_size_t destLen = size * 2;
    z_size_t sourceLen = size;
    
    if (!compressed_buf || !decompressed_buf) {
        free(compressed_buf);
        free(decompressed_buf);
        inflateEnd(&strm);
        gzclose(gz_out);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Read compressed data from gz_in
    memcpy(compressed_buf, data, size);
    
    // Try uncompress2_z first
    z_size_t actualSourceLen = size;
    int uncompress_ret = uncompress2_z(decompressed_buf, &destLen, 
                                       compressed_buf, &actualSourceLen);
    
    // If uncompress2_z fails, try using inflate directly
    if (uncompress_ret != Z_OK) {
        // Reset stream for inflation
        inflateReset(&strm);
        strm.avail_in = (uInt)size;
        strm.next_in = compressed_buf;
        strm.avail_out = (uInt)destLen;
        strm.next_out = decompressed_buf;
        
        ret = inflate(&strm, fuzz_int32_t_5);
        destLen = strm.total_out;
    }
    
    // Write decompressed data to gz_out using gzputc
    for (z_size_t i = 0; i < destLen && i < size * 2; i++) {
        gzputc(gz_out, decompressed_buf[i]);
    }
    
    // Set dictionary for deflation (using first 256 bytes of decompressed data as dictionary)
    uInt dictLength = (destLen < 256) ? (uInt)destLen : 256;
    if (dictLength > 0) {
        // Create a separate stream for deflation to set dictionary
        z_stream deflate_strm;
        deflate_strm.zalloc = Z_NULL;
        deflate_strm.zfree = Z_NULL;
        deflate_strm.opaque = Z_NULL;
        
        if (deflateInit_(&deflate_strm, fuzz_int32_t_7, 
                        fuzz_str_6, sizeof(z_stream)) == Z_OK) {
            deflateSetDictionary(&deflate_strm, decompressed_buf, dictLength);
            deflateEnd(&deflate_strm);
        }
    }
    
    // Compute Adler-32 checksum of decompressed data
    uLong adler1 = adler32(fuzz_uint64_t_8, Z_NULL, 0);
    uLong adler2 = adler32(adler1, decompressed_buf, (uInt)(destLen < 65536 ? destLen : 65536));
    
    // Combine Adler-32 checksums
    off_t offset1 = 100;
    off64_t offset2 = 100;
    uLong combined1 = adler32_combine(adler1, adler2, offset1);
    uLong combined2 = adler32_combine64(adler1, adler2, offset2);
    
    // Test gzeof (should be false since we just wrote)
    int is_eof = gzeof(gz_out);
    
    // Clear any errors on gz_out
    gzclearerr(gz_out);
    
    // Rewind the input gz file
    gzrewind(gz_in);
    
    // Read from gz_in using gzgets
    char read_buf[256];
    if (gzgets(gz_in, read_buf, sizeof(read_buf))) {
        // Write to output using gzseek
        off_t seek_pos = gzseek(gz_out, 0, fuzz_int32_t_9);
    }
    
    // Get inflate mark position
    long mark_pos = inflateMark(&strm);
    
    // Clean up all resources
    inflateEnd(&strm);
    gzclose(gz_out);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    free(compressed_buf);
    free(decompressed_buf);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}