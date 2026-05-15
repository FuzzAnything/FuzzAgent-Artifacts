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
//<ID> 48
//<Prompt> []
/*<Combination>: [
*/
//<score> 45, nr_unique_branch: 16
//<Quality> {"density":9,"unique_branches":{"gzsetparams":[[647,9,647,30,0,0,4,1],[651,9,651,20,0,0,4,1],[655,9,655,20,0,0,4,1]],"deflate_rle":[[2094,13,2094,38,0,0,4,0],[2096,17,2096,42,0,0,4,0],[2096,46,2096,65,0,0,4,1],[2099,17,2099,34,0,0,4,0],[2142,9,2142,26,0,0,4,0],[1644,8,1644,31,12,0,4,1],[1631,24,1631,44,20,0,4,0]],"deflate":[[1219,18,1219,38,0,0,4,0]],"deflate_stored":[[1704,34,1704,42,0,0,4,0],[1704,46,1704,63,0,0,4,1],[1745,13,1745,16,0,0,4,1],[1760,9,1760,13,0,0,4,1],[1786,9,1786,36,0,0,4,1]]},"library_calls":["zlibVersion","gzdopen","gzclose","gzdopen","gzclose","gzsetparams","gzgetc_","gzputs","compressBound_z","compress_z","crc32_z","crc32_z","deflateInit_","deflateCopy","deflateUsed","inflateInit_","inflate","inflateCodesUsed","inflateEnd","deflateEnd","deflateEnd","gzclose","gzclose"],"critical_calls":["zlibVersion","gzdopen","gzdopen","gzsetparams","gzgetc_","gzputs","compressBound_z","compress_z","crc32_z","crc32_z","deflateInit_","deflateCopy","deflateUsed","inflateInit_","inflate","inflateCodesUsed","inflateEnd","deflateEnd","deflateEnd","gzclose","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<30) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}




    // 1. Get zlib version
    const char *version = zlibVersion();
    if (!version) {
	return 0;
	}
	
    
    // 2. Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 3. Create gzFile from file descriptor
    int fd = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd, fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 4. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 5. Create output gzFile
    int out_fd = fuzz_fileno(out_file);
    gzFile gz_out = gzdopen(out_fd, fuzz_str_2);
    if (!gz_out) {
        assert_file_closed(&out_file);;
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 6. Set compression parameters on output stream
    int level = Z_DEFAULT_COMPRESSION;
    int strategy = Z_DEFAULT_STRATEGY;
    if (size > 0) {
        level = data[0] % 10;  // 0-9 compression levels
        strategy = data[0] % 4; // 0-3 strategies
    }
    gzsetparams(gz_out, fuzz_int32_t_3, fuzz_int32_t_4);
    
    // 7. Read from input gzFile and write to output gzFile
    // Use gzgetc_ to read character by character
    int ch;
    while ((ch = gzgetc_(gz_in)) != -1) {
        // Convert char to string for gzputs
        char buf[2] = {(char)ch, '\0'};
        gzputs(gz_out, buf);
    }
    
    // 8. Create z_stream for compression
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Allocate buffers
    size_t compressed_size = compressBound_z(size);
    Bytef *compressed_buf = (Bytef *)malloc(compressed_size);
    Bytef *uncompressed_buf = (Bytef *)malloc(size);
    
    if (compressed_buf && uncompressed_buf && size > 0) {
        // 9. Use compress_z to compress data
        z_size_t destLen = compressed_size;
        int ret = compress_z(compressed_buf, &destLen, data, size);
        
        if (ret == Z_OK) {
            // 10. Calculate CRC32 of original data
            uLong crc = crc32_z(fuzz_uint64_t_5, Z_NULL, 0);
            crc = crc32_z(crc, data, size);
            
            // 11. Create another z_stream for deflate operations
            z_stream strm2;
            memset(&strm2, 0, sizeof(strm2));
            deflateInit(&strm2, level);
            
            // 12. Use deflateCopy (though not typical usage, we'll copy strm2 to a new stream)
            z_stream strm_copy;
            memset(&strm_copy, 0, sizeof(strm_copy));
            deflateCopy(&strm_copy, &strm2);
            
            // 13. Use deflateUsed to get bits used
            int bits_used = 0;
            deflateUsed(&strm2, &bits_used);
            
            // 14. Create inflate stream to check codes used
            z_stream inflate_strm;
            memset(&inflate_strm, 0, sizeof(inflate_strm));
            inflateInit(&inflate_strm);
            
            // Set input for inflate stream
            inflate_strm.next_in = compressed_buf;
            inflate_strm.avail_in = destLen;
            
            // Inflate a bit to populate codes
            Bytef out_buf[1024];
            inflate_strm.next_out = out_buf;
            inflate_strm.avail_out = sizeof(out_buf);
            inflate(&inflate_strm, fuzz_int32_t_6);
            
            // 15. Get inflate codes used
            unsigned long codes_used = inflateCodesUsed(&inflate_strm);
            
            // Clean up inflate stream
            inflateEnd(&inflate_strm);
            
            // Clean up deflate streams
            deflateEnd(&strm2);
            deflateEnd(&strm_copy);
        }
    }
    
    // 16. Clean up
    if (compressed_buf) free(compressed_buf);
    if (uncompressed_buf) free(uncompressed_buf);
    
    gzclose(gz_out);
    gzclose(gz_in);
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
}