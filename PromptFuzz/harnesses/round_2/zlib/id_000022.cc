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
//<ID> 42
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":20,"unique_branches":{},"library_calls":["gzdopen","gzdopen","gzclose","gzoffset64","gzprintf","deflateInit_","gzclose","gzclose","deflateCopy","deflateEnd","gzclose","gzclose","deflateUsed","inflateInit_","deflateEnd","deflateEnd","gzclose","gzclose","inflateCopy","inflateEnd","deflateEnd","deflateEnd","gzclose","gzclose","inflateReset2","inflatePrime","inflateSync","gzread","gzwrite","gzprintf","gzoffset64","gzprintf","inflateEnd","inflateEnd","deflateEnd","deflateEnd","gzclose","gzclose"],"critical_calls":["gzdopen","gzdopen","gzoffset64","gzprintf","deflateInit_","deflateCopy","deflateUsed","inflateInit_","inflateCopy","inflateReset2","inflatePrime","inflateSync","gzread","gzwrite","gzprintf","gzoffset64","gzprintf","inflateEnd","inflateEnd","deflateEnd","deflateEnd","gzclose","gzclose"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if not enough data
    if (size < 100) {
	return 0;
	}
	
    
    // Create input FILE* from the fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file for gz operations
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
    
    // Event: Process compressed data through gzFile with offset tracking,
    // copy/debug deflate streams, and manipulate inflate streams
    
    // 1. Create gzFile for input compressed data
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
    
    // 2. Create gzFile for output
    gzFile gz_out = gzdopen(out_fd, "wb");
    if (!gz_out) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 3. Get current offset in gz_in using gzoffset64
    off64_t offset = gzoffset64(gz_in);
    
    // 4. Use gzprintf to write metadata
    gzprintf(gz_out, "Starting offset: %lld\n", (long long)offset);
    
    // 5. Prepare deflate streams
    z_stream deflate_strm1, deflate_strm2;
    memset(&deflate_strm1, 0, sizeof(z_stream));
    memset(&deflate_strm2, 0, sizeof(z_stream));
    
    deflate_strm1.zalloc = Z_NULL;
    deflate_strm1.zfree = Z_NULL;
    deflate_strm1.opaque = Z_NULL;
    
    // Initialize deflate stream
    if (deflateInit(&deflate_strm1, Z_DEFAULT_COMPRESSION) != Z_OK) {
        gzclose(gz_out);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 6. Copy deflate stream using deflateCopy
    if (deflateCopy(&deflate_strm2, &deflate_strm1) != Z_OK) {
        deflateEnd(&deflate_strm1);
        gzclose(gz_out);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 7. Get bits used in deflate stream
    int bits_used = 0;
    deflateUsed(&deflate_strm1, &bits_used);
    
    // 8. Prepare inflate streams for processing compressed data
    z_stream inflate_strm1, inflate_strm2;
    memset(&inflate_strm1, 0, sizeof(z_stream));
    memset(&inflate_strm2, 0, sizeof(z_stream));
    
    inflate_strm1.zalloc = Z_NULL;
    inflate_strm1.zfree = Z_NULL;
    inflate_strm1.opaque = Z_NULL;
    inflate_strm1.avail_in = 0;
    inflate_strm1.next_in = Z_NULL;
    
    // Initialize inflate stream with default windowBits
    if (inflateInit(&inflate_strm1) != Z_OK) {
        deflateEnd(&deflate_strm2);
        deflateEnd(&deflate_strm1);
        gzclose(gz_out);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 9. Copy inflate stream using inflateCopy
    if (inflateCopy(&inflate_strm2, &inflate_strm1) != Z_OK) {
        inflateEnd(&inflate_strm1);
        deflateEnd(&deflate_strm2);
        deflateEnd(&deflate_strm1);
        gzclose(gz_out);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 10. Reset inflate stream with different windowBits using inflateReset2
    inflateReset2(&inflate_strm2, MAX_WBITS + 16); // +16 for gzip header
    
    // 11. Prime inflate stream with some bits using inflatePrime
    // Using first 2 bytes of data as prime bits
    unsigned int prime_bits = (size >= 2) ? ((data[0] << 8) | data[1]) : 0xFFFF;
    inflatePrime(&inflate_strm2, 16, prime_bits);
    
    // 12. Try to sync inflate stream using inflateSync
    // This is for demonstration - actual sync would need proper zlib data
    inflateSync(&inflate_strm2);
    
    // 13. Read some data through gzFile and write it back
    unsigned char buffer[1024];
    int bytes_read;
    
    // Read a chunk from input gzFile
    bytes_read = gzread(gz_in, buffer, sizeof(buffer));
    if (bytes_read > 0) {
        // Write to output gzFile
        gzwrite(gz_out, buffer, bytes_read);
        
        // Use gzprintf again
        gzprintf(gz_out, "\nProcessed %d bytes\n", bytes_read);
    }
    
    // 14. Get final offset
    off64_t final_offset = gzoffset64(gz_out);
    gzprintf(gz_out, "Final offset: %lld\n", (long long)final_offset);
    
    // Clean up all resources
    inflateEnd(&inflate_strm2);
    inflateEnd(&inflate_strm1);
    deflateEnd(&deflate_strm2);
    deflateEnd(&deflate_strm1);
    gzclose(gz_out);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}