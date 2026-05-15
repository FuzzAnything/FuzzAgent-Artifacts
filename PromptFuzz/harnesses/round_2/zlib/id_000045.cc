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
//<ID> 103
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"inflatePrime":[[223,9,223,18,0,0,4,0]]},"library_calls":["gzdopen","gzdopen","gzclose","gzclose","inflateInit_","gzclose","gzclose","deflateInit_","inflateEnd","gzclose","gzclose","deflateSetHeader","inflateCopy","inflateReset2","deflatePrime","inflatePrime","gzread","gzwrite","gzprintf","inflate","deflate","deflateUsed","gzflush","inflateEnd","inflateEnd","deflateEnd","gzclose","gzclose"],"critical_calls":["gzdopen","gzdopen","inflateInit_","deflateInit_","deflateSetHeader","inflateCopy","inflateReset2","deflatePrime","inflatePrime","gzread","gzwrite","gzprintf","inflate","deflate","deflateUsed","gzflush","inflateEnd","inflateEnd","deflateEnd","gzclose","gzclose"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 1: Create temporary file for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Step 2: Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Step 3: Open gzFile for reading and writing
    gzFile gz_in = gzdopen(in_fd, "rb");
    gzFile gz_out = gzdopen(out_fd, "wb");
    
    if (!gz_in || !gz_out) {
        if (gz_in) gzclose(gz_in);
        if (gz_out) gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 4: Initialize inflate stream for decompression
    z_stream strm_inflate;
    memset(&strm_inflate, 0, sizeof(z_stream));
    
    if (inflateInit_(&strm_inflate, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        gzclose(gz_in);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 5: Initialize deflate stream for compression
    z_stream strm_deflate;
    memset(&strm_deflate, 0, sizeof(z_stream));
    
    if (deflateInit_(&strm_deflate, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        inflateEnd(&strm_inflate);
        gzclose(gz_in);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 6: Set gzip header for deflate stream
    gz_header header;
    memset(&header, 0, sizeof(gz_header));
    header.os = 255; // Unknown OS
    deflateSetHeader(&strm_deflate, &header);
    
    // Step 7: Create copy of inflate stream
    z_stream strm_inflate_copy;
    memset(&strm_inflate_copy, 0, sizeof(z_stream));
    inflateCopy(&strm_inflate_copy, &strm_inflate);
    
    // Step 8: Reset inflate stream with different window bits
    inflateReset2(&strm_inflate, MAX_WBITS + 16); // gzip decoding
    
    // Step 9: Use prime functions (minimal effect)
    deflatePrime(&strm_deflate, 0, 0);
    inflatePrime(&strm_inflate, 0, 0);
    
    // Step 10: Read from gzFile
    const size_t buf_size = 4096;
    uint8_t read_buf[buf_size];
    uint8_t write_buf[buf_size];
    
    // Read some data from gzFile
    int read_len = gzread(gz_in, read_buf, buf_size);
    
    if (read_len > 0) {
        // Step 11: Write to gzFile
        gzwrite(gz_out, read_buf, read_len);
        
        // Step 12: Write formatted output
        gzprintf(gz_out, "Processed %d bytes\n", read_len);
        
        // Step 13: Prepare inflate input (using original data)
        strm_inflate.avail_in = size > buf_size ? buf_size : size;
        strm_inflate.next_in = (Bytef*)data;
        
        // Step 14: Prepare inflate output
        strm_inflate.avail_out = buf_size;
        strm_inflate.next_out = write_buf;
        
        // Perform inflation
        int ret_inflate = inflate(&strm_inflate, Z_NO_FLUSH);
        
        if (ret_inflate == Z_OK || ret_inflate == Z_STREAM_END) {
            // Step 15: Prepare deflate input (using inflated data)
            size_t inflated_size = buf_size - strm_inflate.avail_out;
            if (inflated_size > 0) {
                strm_deflate.avail_in = inflated_size;
                strm_deflate.next_in = write_buf;
                
                // Step 16: Prepare deflate output
                strm_deflate.avail_out = buf_size;
                strm_deflate.next_out = read_buf; // reuse buffer
                
                // Perform deflation
                int ret_deflate = deflate(&strm_deflate, Z_FINISH);
                
                if (ret_deflate == Z_STREAM_END) {
                    // Write deflated data
                    size_t deflated_size = buf_size - strm_deflate.avail_out;
                    if (deflated_size > 0) {
                        fwrite(read_buf, 1, deflated_size, out_file);
                    }
                }
                
                // Step 17: Check deflate used bits
                int bits_used = 0;
                deflateUsed(&strm_deflate, &bits_used);
            }
        }
        
        // Step 18: Flush gzFile
        gzflush(gz_out, Z_SYNC_FLUSH);
    }
    
    // Step 19: Cleanup
    inflateEnd(&strm_inflate);
    inflateEnd(&strm_inflate_copy);
    deflateEnd(&strm_deflate);
    gzclose(gz_in);
    gzclose(gz_out);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove temporary output file
    unlink("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}