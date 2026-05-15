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
//<ID> 41
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":24,"unique_branches":{},"library_calls":["gzdopen","gzclose","gzdopen","gzclose","gzoffset64","inflateInit2_","gzclose","gzclose","inflateReset2","inflateEnd","gzclose","gzclose","inflatePrime","gzread","inflate","inflateSync","inflateCopy","inflate","inflateEnd","deflateInit2_","inflateEnd","gzclose","gzclose","deflate","deflateUsed","deflateCopy","deflate","deflateEnd","gzprintf","gzputs","deflateEnd","inflateEnd","gzflush","gzclose","gzclose"],"critical_calls":["gzdopen","gzdopen","gzoffset64","inflateInit2_","inflateReset2","inflatePrime","gzread","inflate","inflateSync","inflateCopy","inflate","inflateEnd","deflateInit2_","deflate","deflateUsed","deflateCopy","deflate","deflateEnd","gzprintf","gzputs","deflateEnd","inflateEnd","gzflush","gzclose","gzclose"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create FILE* for input using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create gzFile from the FILE descriptor for compressed input reading
    int fd_in = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd_in, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Create output FILE and gzFile for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    int fd_out = fuzz_fileno(out_file);
    gzFile gz_out = gzdopen(fd_out, "wb");
    if (!gz_out) {
        assert_file_closed(&out_file);;
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 1. Use gzoffset64 to get current offset in the gzFile
    off64_t offset = gzoffset64(gz_in);
    
    // 2. Initialize inflation stream for decompression
    z_stream strm_inflate;
    memset(&strm_inflate, 0, sizeof(z_stream));
    
    // Initialize with default windowBits (15)
    if (inflateInit2(&strm_inflate, 15) != Z_OK) {
        gzclose(gz_out);
        gzclose(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 3. Use inflateReset2 to change windowBits and reset stream
    if (inflateReset2(&strm_inflate, 16 + 15) != Z_OK) { // 16 + 15 for gzip format
        inflateEnd(&strm_inflate);
        gzclose(gz_out);
        gzclose(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 4. Use inflatePrime to insert bits into inflation stream
    if (size > 0) {
        int prime_bits = (data[0] & 7) + 1; // 1-8 bits
        int prime_value = (size > 1) ? data[1] : 0;
        inflatePrime(&strm_inflate, prime_bits, prime_value);
    }
    
    // 5. Read some data from gzFile to feed into inflation stream
    unsigned char buffer_in[1024];
    unsigned char buffer_out[1024];
    
    strm_inflate.next_in = buffer_in;
    strm_inflate.avail_in = 0;
    strm_inflate.next_out = buffer_out;
    strm_inflate.avail_out = sizeof(buffer_out);
    
    // Read from gzFile
    int read_len = gzread(gz_in, buffer_in, sizeof(buffer_in));
    if (read_len > 0) {
        strm_inflate.next_in = buffer_in;
        strm_inflate.avail_in = read_len;
    }
    
    // 6. Try to inflate (decompress) a small amount
    int inflate_ret = inflate(&strm_inflate, Z_NO_FLUSH);
    
    // 7. Use inflateSync to try to synchronize if we got an error
    if (inflate_ret == Z_BUF_ERROR || inflate_ret == Z_DATA_ERROR) {
        inflateSync(&strm_inflate);
    }
    
    // 8. Create a copy of inflation stream using inflateCopy
    z_stream strm_inflate_copy;
    memset(&strm_inflate_copy, 0, sizeof(z_stream));
    
    if (inflateCopy(&strm_inflate_copy, &strm_inflate) == Z_OK) {
        // Use the copied stream briefly
        unsigned char copy_buffer[64];
        strm_inflate_copy.next_out = copy_buffer;
        strm_inflate_copy.avail_out = sizeof(copy_buffer);
        
        int copy_ret = inflate(&strm_inflate_copy, Z_SYNC_FLUSH);
        
        // Clean up the copied stream
        inflateEnd(&strm_inflate_copy);
    }
    
    // 9. Initialize deflation stream for compression
    z_stream strm_deflate;
    memset(&strm_deflate, 0, sizeof(z_stream));
    
    if (deflateInit2(&strm_deflate, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                     15, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        inflateEnd(&strm_inflate);
        gzclose(gz_out);
        gzclose(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 10. Feed any inflated output to deflation stream
    if (strm_inflate.total_out > 0) {
        strm_deflate.next_in = buffer_out;
        strm_deflate.avail_in = strm_inflate.total_out;
        
        unsigned char deflate_buffer[1024];
        strm_deflate.next_out = deflate_buffer;
        strm_deflate.avail_out = sizeof(deflate_buffer);
        
        deflate(&strm_deflate, Z_FINISH);
    }
    
    // 11. Use deflateUsed to check used bits
    int used_bits = 0;
    deflateUsed(&strm_deflate, &used_bits);
    
    // 12. Create a copy of deflation stream using deflateCopy
    z_stream strm_deflate_copy;
    memset(&strm_deflate_copy, 0, sizeof(z_stream));
    
    if (deflateCopy(&strm_deflate_copy, &strm_deflate) == Z_OK) {
        // Do a small operation with the copied stream
        unsigned char copy_buffer[64];
        strm_deflate_copy.next_out = copy_buffer;
        strm_deflate_copy.avail_out = sizeof(copy_buffer);
        
        deflate(&strm_deflate_copy, Z_SYNC_FLUSH);
        
        // Clean up the copied stream
        deflateEnd(&strm_deflate_copy);
    }
    
    // 13. Use gzprintf to write some formatted output
    if (offset != -1) {
        gzprintf(gz_out, "Processed offset: %lld, Input size: %zu, Used bits: %d\n", 
                (long long)offset, size, used_bits);
    }
    
    // 14. Write a marker to the output
    gzputs(gz_out, "Fuzzing completed\n");
    
    // 15. Clean up all resources
    deflateEnd(&strm_deflate);
    inflateEnd(&strm_inflate);
    
    gzflush(gz_out, Z_FINISH);
    gzclose(gz_out);
    gzclose(gz_in);
    
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    // Clean up the output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}