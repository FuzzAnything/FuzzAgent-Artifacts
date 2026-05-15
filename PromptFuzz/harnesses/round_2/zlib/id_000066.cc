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
//<ID> 192
//<Prompt> []
/*<Combination>: [
*/
//<score> 14.666667, nr_unique_branch: 3
//<Quality> {"density":11,"unique_branches":{"compress2_z":[[43,9,43,20,0,0,4,0]],"deflateInit2_":[[435,59,435,68,0,0,4,0]],"uncompress2_z":[[36,31,36,45,0,0,4,1]]},"library_calls":["compressBound_z","compress2_z","gzdopen","gzungetc","inflateBackInit_","inflatePrime","inflateBack","inflateUndermine","deflateBound_z","gzeof","inflateCodesUsed","inflateBackEnd","deflateInit_","deflateReset","uncompress2_z","crc32","crc32","crc32_combine_gen","crc32","crc32_combine_op","deflateEnd","gzclose"],"critical_calls":["compressBound_z","compress2_z","gzdopen","gzungetc","inflateBackInit_","inflatePrime","inflateBack","inflateUndermine","deflateBound_z","gzeof","inflateCodesUsed","inflateBackEnd","deflateInit_","deflateReset","uncompress2_z","crc32","crc32","crc32_combine_gen","crc32","crc32_combine_op","deflateEnd","gzclose"],"visited":2}
/**/


// Callback functions for inflateBack
static unsigned int  id_000066_in_callback(void* desc, unsigned char** buf) {
    // Simple implementation: just return 0 bytes to terminate early
    *buf = nullptr;
    return 0;
}

static int  id_000066_out_callback(void* desc, unsigned char* buf, unsigned int size) {
    // Consume output but don't do anything with it
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimum data for testing
    
    // Create FILE* for reading input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create FILE* for writing output
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Allocate buffers
    z_size_t source_len = size < 100 ? size : 100;
    z_size_t dest_len = compressBound_z(source_len);
    Bytef* source_buf = (Bytef*)malloc(source_len);
    Bytef* dest_buf = (Bytef*)malloc(dest_len);
    Bytef* decomp_buf = (Bytef*)malloc(source_len);
    
    if (!source_buf || !dest_buf || !decomp_buf) {
        free(source_buf);
        free(dest_buf);
        free(decomp_buf);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Read some data from input file
    fread(source_buf, 1, source_len, in_file);
    
    // 1. compressBound_z - already used above
    // 2. compress2_z - compress data
    int level = (data[0] % 10) + 1;  // Level 1-10
    int compress_result = compress2_z(dest_buf, &dest_len, source_buf, source_len, level);
    
    // 3. gzdopen - open gzFile from file descriptor
    gzFile gz = gzdopen(out_fd, "wb");
    
    // 4. gzungetc - push back a character
    if (gz) {
        gzungetc('A', gz);
    }
    
    // 5. inflateBackInit_ - initialize for inflateBack
    z_stream strm_back;
    unsigned char window[32768];
    int windowBits = (data[1] % 8) + 8;  // 8-15
    int back_init_result = inflateBackInit_(&strm_back, windowBits, window,
                                          ZLIB_VERSION, sizeof(z_stream));
    
    // 6. inflatePrime - provide bits to inflate
    if (back_init_result == Z_OK) {
        int bits = data[2] % 8;
        int value = data[3];
        inflatePrime(&strm_back, bits, value);
    }
    
    // 7. inflateBack - decompress using callbacks
    if (back_init_result == Z_OK) {
        void* in_desc = nullptr;
        void* out_desc = nullptr;
        inflateBack(&strm_back,  id_000066_in_callback, in_desc,  id_000066_out_callback, out_desc);
    }
    
    // 8. inflateUndermine - undermine integrity check
    if (back_init_result == Z_OK) {
        int subvert = data[4] & 1;
        inflateUndermine(&strm_back, subvert);
    }
    
    // 9. deflateBound_z - get bound for deflated size
    z_stream strm_deflate;
    z_size_t deflate_bound = 0;
    if (dest_len > 0) {
        deflate_bound = deflateBound_z(&strm_deflate, dest_len);
    }
    
    // 10. gzeof - check if at end of gz file
    int at_eof = 0;
    if (gz) {
        at_eof = gzeof(gz);
    }
    
    // 11. inflateCodesUsed - get codes used (even if inflateBack didn't work)
    unsigned long codes_used = 0;
    if (back_init_result == Z_OK) {
        codes_used = inflateCodesUsed(&strm_back);
    }
    
    // 12. inflateBackEnd - clean up inflateBack
    if (back_init_result == Z_OK) {
        inflateBackEnd(&strm_back);
    }
    
    // 13. deflateInit_ - initialize deflate stream
    int deflate_init_result = deflateInit_(&strm_deflate, level,
                                         ZLIB_VERSION, sizeof(z_stream));
    
    // 14. deflateReset - reset deflate stream
    if (deflate_init_result == Z_OK) {
        deflateReset(&strm_deflate);
    }
    
    // 15. uncompress2_z - decompress the compressed data
    z_size_t decomp_len = source_len;
    z_size_t source_len_for_uncompress = dest_len;
    int uncompress_result = uncompress2_z(decomp_buf, &decomp_len,
                                        dest_buf, &source_len_for_uncompress);
    
    // 16. crc32 - compute CRC
    uLong crc = crc32(0L, Z_NULL, 0);
    if (source_len > 0) {
        crc = crc32(crc, source_buf, source_len < 100 ? source_len : 100);
    }
    
    // 17. crc32_combine_gen - generate combine operation
    off_t len2 = source_len;
    uLong combine_gen = crc32_combine_gen(len2);
    
    // 18. crc32_combine_op - combine two CRCs
    uLong crc2 = crc32(0L, dest_buf, dest_len < 50 ? dest_len : 50);
    uLong combined = crc32_combine_op(crc, crc2, combine_gen);
    
    // 19. deflateEnd - end deflate
    if (deflate_init_result == Z_OK) {
        deflateEnd(&strm_deflate);
    }
    
    // 20. gzclose - close gz file
    if (gz) {
        gzclose(gz);
    }
    
    // Clean up
    free(source_buf);
    free(dest_buf);
    free(decomp_buf);
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