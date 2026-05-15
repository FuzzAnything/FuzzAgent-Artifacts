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
//<ID> 146
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":7,"unique_branches":{"inflate":[[700,9,700,18,0,0,4,0]]},"library_calls":["gzdopen","gzeof","gzdopen","gzprintf","gzclose","inflateInit_","gzclose","inflate","inflateSetDictionary","inflateCopy","inflate","inflateEnd","inflateEnd","deflateInit_","gzclose","deflateTune","deflateCopy","deflate","deflateEnd","deflateEnd","crc32_z","crc32_z","gzclose"],"critical_calls":["gzdopen","gzeof","gzdopen","gzprintf","gzclose","inflateInit_","inflate","inflateSetDictionary","inflateCopy","inflate","inflateEnd","inflateEnd","deflateInit_","deflateTune","deflateCopy","deflate","deflateEnd","deflateEnd","crc32_z","crc32_z","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit for very small inputs
    if (size < 100) {
	return 0;
	}
	
    
    // Create input and output files
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    
    // Part 1: gzFile operations with the input compressed data
    gzFile gz_in = gzdopen(fd_in, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Use gzgetc and check for EOF
    int first_byte = gzgetc(gz_in);
    if (first_byte != -1) {
        // Use gzeof (though we just read a byte, so not at EOF yet)
        int is_eof = gzeof(gz_in);  // Should be 0
        
        // Create a gzFile for output
        gzFile gz_out = gzdopen(fd_out, "wb");
        if (gz_out) {
            // Use gzprintf to write the first byte as hex
            gzprintf(gz_out, "First byte: 0x%02X\n", first_byte);
            gzclose(gz_out);
        }
    }
    
    // Part 2: Inflate operations with the compressed data
    // We'll use the original data buffer directly for inflate
    
    // Create source inflate stream
    z_stream source_inflate;
    memset(&source_inflate, 0, sizeof(source_inflate));
    
    if (inflateInit(&source_inflate) != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Set up source inflate stream with input data
    source_inflate.next_in = (Bytef *)data;
    source_inflate.avail_in = (uInt)(size > 1000 ? 1000 : size);  // Use first part
    
    // Create output buffer for inflation
    Bytef out_buffer[4096];
    source_inflate.next_out = out_buffer;
    source_inflate.avail_out = sizeof(out_buffer);
    
    // Try to inflate a bit
    inflate(&source_inflate, Z_SYNC_FLUSH);
    
    // Try to set a dictionary for inflation (using part of the data as dictionary)
    if (source_inflate.avail_in > 50) {
        inflateSetDictionary(&source_inflate, source_inflate.next_in, 50);
    }
    
    // Create destination inflate stream and copy from source
    z_stream dest_inflate;
    memset(&dest_inflate, 0, sizeof(dest_inflate));
    
    if (inflateCopy(&dest_inflate, &source_inflate) == Z_OK) {
        // Use the copied stream
        dest_inflate.next_out = out_buffer + source_inflate.total_out;
        dest_inflate.avail_out = sizeof(out_buffer) - source_inflate.total_out;
        inflate(&dest_inflate, Z_FINISH);
        inflateEnd(&dest_inflate);
    }
    
    // Clean up source inflate stream
    inflateEnd(&source_inflate);
    
    // Part 3: Deflate operations
    // Create a deflate stream
    z_stream source_deflate;
    memset(&source_deflate, 0, sizeof(source_deflate));
    
    if (deflateInit(&source_deflate, Z_DEFAULT_COMPRESSION) != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Tune the deflate parameters using data-derived values
    // Use modulo to ensure reasonable parameter ranges
    int good_length = data[0] % 32 + 8;
    int max_lazy = data[1] % 32 + 8;
    int nice_length = data[2] % 128 + 32;
    int max_chain = data[3] % 4096 + 1024;
    deflateTune(&source_deflate, good_length, max_lazy, nice_length, max_chain);
    
    // Create destination deflate stream and copy from source
    z_stream dest_deflate;
    memset(&dest_deflate, 0, sizeof(dest_deflate));
    
    if (deflateCopy(&dest_deflate, &source_deflate) == Z_OK) {
        // Use the copied stream with some data
        Bytef deflate_in[100];
        Bytef deflate_out[200];
        
        // Copy a small portion of input data for compression
        size_t copy_size = size > 100 ? 100 : size;
        memcpy(deflate_in, data, copy_size);
        
        dest_deflate.next_in = deflate_in;
        dest_deflate.avail_in = (uInt)copy_size;
        dest_deflate.next_out = deflate_out;
        dest_deflate.avail_out = sizeof(deflate_out);
        
        deflate(&dest_deflate, Z_FINISH);
        deflateEnd(&dest_deflate);
    }
    
    // Clean up source deflate stream
    deflateEnd(&source_deflate);
    
    // Part 4: CRC32 calculation using the input data
    uLong crc = crc32_z(0L, Z_NULL, 0);
    
    // Calculate CRC32 for the entire input data in chunks
    size_t processed = 0;
    while (processed < size) {
        size_t chunk_size = size - processed > 1024 ? 1024 : size - processed;
        crc = crc32_z(crc, data + processed, chunk_size);
        processed += chunk_size;
    }
    
    // Part 5: Clean up
    gzclose(gz_in);  // This also closes the underlying file descriptor
    
    // Note: in_file is already closed by gzclose(gz_in)
    // We only need to close out_file which wasn't used by gzFile operations anymore
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}