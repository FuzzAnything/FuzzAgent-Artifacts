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
//<ID> 45
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 13
//<Quality> {"density":8,"unique_branches":{"deflateUsed":[[738,9,738,32,0,0,4,1],[739,9,739,23,0,0,4,0]],"gzsetparams":[[635,9,635,21,0,0,4,1],[641,9,641,32,0,0,4,0]],"deflateCopy":[[1327,9,1327,34,0,0,4,1],[1327,38,1327,52,0,0,4,1],[1336,9,1336,21,0,0,4,1],[1347,9,1347,29,0,0,4,1],[1347,33,1347,51,0,0,4,1],[1347,55,1347,73,0,0,4,1],[1348,9,1348,34,0,0,4,1],[1355,14,1355,22,0,0,4,1],[1355,26,1355,64,0,0,4,1]]},"library_calls":["zlibVersion","gzdopen","gzsetparams","gzclose","gzdopen","gzclose","gzgetc_","gzrewind","gzgetc_","gzputs","crc32_z","compressBound_z","compress_z","deflateInit_","deflateUsed","deflateCopy","deflateEnd","inflateInit_","inflateCodesUsed","inflateEnd","deflateEnd","gzclose","gzclose"],"critical_calls":["zlibVersion","gzdopen","gzsetparams","gzdopen","gzgetc_","gzrewind","gzgetc_","gzputs","crc32_z","compressBound_z","compress_z","deflateInit_","deflateUsed","deflateCopy","deflateEnd","inflateInit_","inflateCodesUsed","inflateEnd","deflateEnd","gzclose","gzclose"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<34) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
	//fuzzer shim end}




    // 1. Get zlib version
    const char *version = zlibVersion();
    
    // 2. Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 3. Open the file as gzFile for reading
    int fd = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd, fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 4. Set compression parameters (even though we're reading)
    // This may fail for read mode, but we call it as required
    gzsetparams(gz_in, fuzz_int32_t_2, fuzz_int32_t_3);
    
    // 5. Create output gzFile for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        gzclose(gz_in);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    int out_fd = fuzz_fileno(out_file);
    gzFile gz_out = gzdopen(out_fd, fuzz_str_4);
    if (!gz_out) {
        assert_file_closed(&out_file);;
        gzclose(gz_in);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 6. Read from input gzFile using gzgetc_
    int ch;
    size_t decompressed_size = 0;
    Bytef *decompressed_buf = nullptr;
    
    // First pass to get decompressed size and content
    while ((ch = gzgetc_(gz_in)) != -1) {
        decompressed_size++;
    }
    
    if (decompressed_size > 0) {
        decompressed_buf = (Bytef *)malloc(decompressed_size);
        if (decompressed_buf) {
            // Reset file position
            gzrewind(gz_in);
            
            // Read again into buffer
            for (size_t i = 0; i < decompressed_size; i++) {
                decompressed_buf[i] = (Bytef)gzgetc_(gz_in);
            }
            
            // 7. Write to output gzFile using gzputs
            // Convert first few bytes to string for gzputs
            if (decompressed_size > 0) {
                char *str_buf = (char *)malloc(decompressed_size + 1);
                if (str_buf) {
                    size_t str_len = decompressed_size < 100 ? decompressed_size : 100;
                    memcpy(str_buf, decompressed_buf, str_len);
                    str_buf[str_len] = '\0';
                    gzputs(gz_out, str_buf);
                    free(str_buf);
                }
            }
            
            // 8. Calculate CRC32 using crc32_z
            uLong crc = crc32_z(fuzz_uint64_t_5, decompressed_buf, decompressed_size);
            
            // 9. Compress the decompressed data using compress_z
            z_size_t compressed_len = compressBound_z(fuzz_uint64_t_6);
            Bytef *compressed_buf = (Bytef *)malloc(compressed_len);
            if (compressed_buf) {
                compress_z(compressed_buf, &compressed_len, 
                          decompressed_buf, decompressed_size);
                free(compressed_buf);
            }
            
            free(decompressed_buf);
        }
    }
    
    // 10. Create z_stream structures for deflate operations
    z_stream strm1, strm2;
    memset(&strm1, 0, sizeof(z_stream));
    memset(&strm2, 0, sizeof(z_stream));
    
    // Initialize deflate stream
    if (deflateInit(&strm1, Z_DEFAULT_COMPRESSION) == Z_OK) {
        // 11. Use deflateUsed
        int bits_used;
        deflateUsed(&strm1, &bits_used);
        
        // 12. Use deflateCopy
        if (deflateCopy(&strm2, &strm1) == Z_OK) {
            // Clean up copied stream
            deflateEnd(&strm2);
        }
        
        // 13. Create inflate stream for inflateCodesUsed
        z_stream inflate_strm;
        memset(&inflate_strm, 0, sizeof(z_stream));
        
        if (inflateInit(&inflate_strm) == Z_OK) {
            // 14. Use inflateCodesUsed
            unsigned long codes_used = inflateCodesUsed(&inflate_strm);
            (void)codes_used; // Avoid unused variable warning
            
            inflateEnd(&inflate_strm);
        }
        
        // Clean up deflate stream
        deflateEnd(&strm1);
    }
    
    // 15. Clean up all resources
    gzclose(gz_out);  // This will also close out_file
    gzclose(gz_in);   // This will also close in_file
    
    // Remove temporary output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
}