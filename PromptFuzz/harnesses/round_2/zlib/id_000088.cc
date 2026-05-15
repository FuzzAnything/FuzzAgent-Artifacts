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
//<ID> 311
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 6
//<Quality> {"density":7,"unique_branches":{"deflateSetHeader":[[715,36,715,58,0,0,4,1]],"deflateBound_z":[[893,13,893,32,0,0,4,0],[895,17,895,43,0,0,4,1],[898,17,898,30,0,0,4,1],[903,17,903,30,0,0,4,1],[907,17,907,32,0,0,4,1]]},"library_calls":["gzdopen","gzfread","gzseek64","gzerror","deflateInit2_","deflateSetHeader","inflateInit2_","inflateGetHeader","inflateEnd","deflateBound","compressBound","compress","deflatePrime","deflateEnd","gzdopen","gzputc","gzclose","gzclose"],"critical_calls":["gzdopen","gzfread","gzseek64","gzerror","deflateInit2_","deflateSetHeader","inflateInit2_","inflateGetHeader","inflateEnd","deflateBound","compressBound","compress","deflatePrime","deflateEnd","gzdopen","gzputc","gzclose","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_88(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file for compressed data
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
    
    // Variables for compression
    Bytef *src_buf = nullptr;
    Bytef *dst_buf = nullptr;
    uLongf destLen = 0;
    z_stream strm;
    gz_header header;
    int ret = Z_OK;
    
    // 1. Try to read compressed data using gzfread
    // First open the input as a gzFile
    gzFile gz_in = gzdopen(in_fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Try to read some data from the compressed file
    const size_t read_size = 1024;
    Bytef read_buf[read_size];
    z_size_t items_read = gzfread(read_buf, 1, 1, gz_in);
    
    // 2. Get a character from the gz file
    int gz_char = gzgetc(gz_in);
    
    // 3. Seek in the gz file
    off64_t seek_pos = gzseek64(gz_in, 0, SEEK_SET);
    
    // 4. Check for errors
    int errnum = 0;
    const char *error_msg = gzerror(gz_in, &errnum);
    
    // 5. Now try to compress the original data using deflate APIs
    // First initialize zlib stream for deflation
    memset(&strm, 0, sizeof(strm));
    ret = deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                       MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY);
    
    if (ret == Z_OK) {
        // 6. Set header for gzip format
        memset(&header, 0, sizeof(header));
        header.text = 1;
        header.time = time(nullptr);
        header.os = 255;
        ret = deflateSetHeader(&strm, &header);
        
        // 7. Try to get header during inflation
        // First set up inflate stream
        z_stream inf_strm;
        memset(&inf_strm, 0, sizeof(inf_strm));
        ret = inflateInit2(&inf_strm, MAX_WBITS + 16);
        
        if (ret == Z_OK) {
            // Try to get header
            gz_header inf_header;
            memset(&inf_header, 0, sizeof(inf_header));
            ret = inflateGetHeader(&inf_strm, &inf_header);
            
            // Clean up inflate stream
            inflateEnd(&inf_strm);
        }
        
        // 8. Use deflateBound to estimate compressed size
        uLong bound_size = deflateBound(&strm, size);
        
        // 9. Use compress API as well
        destLen = compressBound(size);
        dst_buf = (Bytef *)malloc(destLen);
        if (dst_buf) {
            ret = compress(dst_buf, &destLen, data, size);
            free(dst_buf);
        }
        
        // 10. Use deflatePrime to add some bits
        if (size > 0) {
            // Add 3 bits from the first byte of input
            int bits_to_add = 3;
            int value = (data[0] >> 5) & 0x07; // Get 3 bits
            ret = deflatePrime(&strm, bits_to_add, value);
        }
        
        // Clean up deflate stream
        deflateEnd(&strm);
    }
    
    // 11. Open output file as gzFile and write a character
    gzFile gz_out = gzdopen(out_fd, "wb");
    if (gz_out) {
        // Write the character we read earlier
        if (gz_char != -1) {
            gzputc(gz_out, gz_char);
        }
        
        // Close gz output file
        gzclose(gz_out);
    } else {
        assert_file_closed(&out_file);;
    }
    
    // Clean up
    if (gz_in) {
        gzclose(gz_in);
    } else {
        assert_file_closed(&in_file);;
    }
    
    // Note: out_file is closed by gzclose(gz_out) if gz_out was created
    // If gz_out wasn't created, we already closed out_file above
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}