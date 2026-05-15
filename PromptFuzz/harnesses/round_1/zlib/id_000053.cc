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
//<ID> 110
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 1
//<Quality> {"density":5,"unique_branches":{"gzflush":[[612,9,612,32,0,0,4,0]]},"library_calls":["gzopen64","gzclose","uncompress2","uncompress2","crc32","crc32","crc32_combine64","gzdopen","gzflush","gzclose","deflateInit_","deflate","deflatePending","deflateReset","deflateEnd","inflateInit2_","inflateGetHeader","inflate","inflateEnd"],"critical_calls":["gzopen64","gzclose","uncompress2","uncompress2","crc32","crc32","crc32_combine64","gzdopen","gzflush","gzclose","deflateInit_","deflate","deflatePending","deflateReset","deflateEnd","inflateInit2_","inflateGetHeader","inflate","inflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 10) {
	return 0;
	}
	 // Need minimum data for meaningful testing
    
    // Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Open the file with gzopen64
    gzFile gz_in = gzopen64("input_file", "rb");
    if (gz_in) {
        // Read some data from gzip file
        int ch = gzgetc(gz_in);
        (void)ch; // Use the result to avoid unused variable warning
        gzclose(gz_in);
    }
    
    // Write input data to a file for gzopen64
    FILE *tmp_out = fopen("input_file", "wb");
    if (tmp_out) {
        fwrite(data, 1, size, tmp_out);
        assert_file_closed(&tmp_out);;
    }
    
    // Try to uncompress the data (assuming it's compressed)
    Bytef *dest = nullptr;
    uLongf destLen = 0;
    uLong sourceLen = size;
    
    // First call to get required buffer size
    int uncompress_result = uncompress2(nullptr, &destLen, data, &sourceLen);
    
    if (uncompress_result == Z_OK || uncompress_result == Z_BUF_ERROR) {
        dest = (Bytef *)malloc(destLen);
        if (dest) {
            // Reset source length for actual decompression
            sourceLen = size;
            uncompress2(dest, &destLen, data, &sourceLen);
            free(dest);
        }
    }
    
    // CRC32 operations - combine two CRC values
    uLong crc1 = crc32(0L, data, size/2);
    uLong crc2 = crc32(0L, data + size/2, size - size/2);
    off64_t combined_len = size/2 + (size - size/2);
    uLong combined_crc = crc32_combine64(crc1, crc2, combined_len);
    (void)combined_crc; // Use the result
    
    // Use gzdopen with file descriptor
    int fd = fuzz_fileno(in_file);
    gzFile gz_fd = gzdopen(fd, "rb");
    if (gz_fd) {
        // Try to flush (though reading)
        gzflush(gz_fd, Z_SYNC_FLUSH);
        
        // Read a byte
        int gz_char = gzgetc(gz_fd);
        (void)gz_char;
        
        gzclose(gz_fd);
    }
    
    // Set up deflate stream
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    
    // Initialize deflate
    if (deflateInit(&deflate_strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
        // Set input data
        deflate_strm.next_in = (Bytef *)data;
        deflate_strm.avail_in = size > 100 ? 100 : size; // Use first 100 bytes
        
        // Create output buffer
        Bytef out_buf[1024];
        deflate_strm.next_out = out_buf;
        deflate_strm.avail_out = sizeof(out_buf);
        
        // Try to deflate
        deflate(&deflate_strm, Z_FINISH);
        
        // Get pending data
        unsigned int pending = 0;
        int bits = 0;
        deflatePending(&deflate_strm, &pending, &bits);
        (void)pending; (void)bits;
        
        // Reset deflate stream
        deflateReset(&deflate_strm);
        
        // Clean up
        deflateEnd(&deflate_strm);
    }
    
    // Set up inflate stream for header reading
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    
    // Initialize inflate for gzip format
    if (inflateInit2(&inflate_strm, 16 + MAX_WBITS) == Z_OK) {
        // Create gzip header structure
        gz_header head;
        memset(&head, 0, sizeof(head));
        
        // Try to get header
        inflateGetHeader(&inflate_strm, &head);
        
        // Set input data
        inflate_strm.next_in = (Bytef *)data;
        inflate_strm.avail_in = size > 50 ? 50 : size; // Use first 50 bytes
        
        // Create output buffer
        Bytef inflate_out[1024];
        inflate_strm.next_out = inflate_out;
        inflate_strm.avail_out = sizeof(inflate_out);
        
        // Try to inflate
        inflate(&inflate_strm, Z_NO_FLUSH);
        
        // Clean up
        inflateEnd(&inflate_strm);
    }
    
    // Clean up
    assert_file_closed(&in_file);;
    
    // Remove temporary files
    remove("input_file");
    
    assert_file_closed(&tmp_out);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}