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
//<ID> 43
//<Prompt> []
/*<Combination>: [
*/
//<score> 38, nr_unique_branch: 1
//<Quality> {"density":19,"unique_branches":{"deflate":[[990,9,990,33,0,0,4,0]]},"library_calls":["gzdopen","gzdopen","gzclose","deflateInit2_","gzclose","gzclose","inflateInit2_","deflateEnd","gzclose","gzclose","deflateSetHeader","deflateBound","inflateEnd","deflateEnd","gzclose","gzclose","gzread","crc32","gzwrite","gztell","gzoffset64","gzclearerr","inflateGetDictionary","compress_z","deflate","deflatePending","inflate","inflateUndermine","crc32_combine","deflate","inflateReset","inflateEnd","deflateEnd","gzclose","gzclose"],"critical_calls":["gzdopen","gzdopen","deflateInit2_","inflateInit2_","deflateSetHeader","deflateBound","gzread","crc32","gzwrite","gztell","gzoffset64","gzclearerr","inflateGetDictionary","compress_z","deflate","deflatePending","inflate","inflateUndermine","crc32_combine","deflate","inflateReset","inflateEnd","deflateEnd","gzclose","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create input file from the fuzzer data
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
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    
    // 1. Open gzFile for reading compressed data
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
    
    // 2. Open gzFile for writing compressed data
    gzFile gz_out = gzdopen(fd_out, "wb");
    if (!gz_out) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 3. Initialize deflate stream for compression
    z_stream deflate_stream;
    memset(&deflate_stream, 0, sizeof(z_stream));
    
    // Use deflateInit2_ with specific parameters
    int windowBits = 15 + 16; // gzip encoding
    int level = Z_DEFAULT_COMPRESSION;
    int method = Z_DEFLATED;
    int memLevel = 8;
    int strategy = Z_DEFAULT_STRATEGY;
    
    int ret = deflateInit2_(&deflate_stream, level, method, windowBits, 
                           memLevel, strategy, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_in);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 4. Initialize inflate stream for decompression
    z_stream inflate_stream;
    memset(&inflate_stream, 0, sizeof(z_stream));
    
    ret = inflateInit2_(&inflate_stream, windowBits, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        deflateEnd(&deflate_stream);
        gzclose(gz_in);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 5. Set gzip header for deflate stream
    gz_header header;
    memset(&header, 0, sizeof(gz_header));
    header.text = 1;
    header.time = 0;
    header.os = 255; // unknown OS
    
    deflateSetHeader(&deflate_stream, &header);
    
    // 6. Read input data through gzFile
    const size_t buffer_size = 4096;
    Bytef *read_buffer = (Bytef *)malloc(buffer_size);
    Bytef *compress_buffer = (Bytef *)malloc(deflateBound(&deflate_stream, buffer_size));
    Bytef *decompress_buffer = (Bytef *)malloc(buffer_size);
    Bytef *dictionary = (Bytef *)malloc(32768);
    uInt dictLength = 0;
    
    if (!read_buffer || !compress_buffer || !decompress_buffer || !dictionary) {
        free(read_buffer);
        free(compress_buffer);
        free(decompress_buffer);
        free(dictionary);
        inflateEnd(&inflate_stream);
        deflateEnd(&deflate_stream);
        gzclose(gz_in);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    uLong crc1 = 0;
    uLong crc2 = 0;
    off_t total_read = 0;
    
    // 7. Process data through multiple paths
    while (true) {
        // Read from gzFile
        int bytes_read = gzread(gz_in, read_buffer, buffer_size);
        if (bytes_read <= 0) {
            break;
        }
        
        // Update CRC for the read data
        crc1 = crc32(crc1, read_buffer, bytes_read);
        
        // Write to output gzFile
        gzwrite(gz_out, read_buffer, bytes_read);
        
        // Get current position in gzFile
        off_t pos = gztell(gz_in);
        off64_t offset64 = gzoffset64(gz_in);
        
        // Clear any errors on gzFile
        gzclearerr(gz_in);
        
        // Try to get inflate dictionary
        inflateGetDictionary(&inflate_stream, dictionary, &dictLength);
        
        // Use compress_z for one-step compression
        z_size_t destLen = buffer_size * 2;
        compress_z(compress_buffer, &destLen, read_buffer, bytes_read);
        
        // Process with deflate stream
        deflate_stream.next_in = read_buffer;
        deflate_stream.avail_in = bytes_read;
        deflate_stream.next_out = compress_buffer;
        deflate_stream.avail_out = buffer_size * 2;
        
        deflate(&deflate_stream, Z_SYNC_FLUSH);
        
        // Check pending data in deflate stream
        unsigned int pending;
        int bits;
        deflatePending(&deflate_stream, &pending, &bits);
        
        // Process with inflate stream (using compressed output from deflate)
        inflate_stream.next_in = compress_buffer;
        inflate_stream.avail_in = deflate_stream.total_out;
        inflate_stream.next_out = decompress_buffer;
        inflate_stream.avail_out = buffer_size;
        
        inflate(&inflate_stream, Z_SYNC_FLUSH);
        
        // Try to undermine inflate stream (for testing error paths)
        inflateUndermine(&inflate_stream, 0);
        
        total_read += bytes_read;
    }
    
    // 8. Combine CRC values
    uLong combined_crc = crc32_combine(crc1, crc2, total_read);
    
    // 9. Finalize deflate stream
    deflate_stream.next_in = nullptr;
    deflate_stream.avail_in = 0;
    deflate(&deflate_stream, Z_FINISH);
    
    // 10. Reset inflate stream
    inflateReset(&inflate_stream);
    
    // 11. Clean up
    free(read_buffer);
    free(compress_buffer);
    free(decompress_buffer);
    free(dictionary);
    
    inflateEnd(&inflate_stream);
    deflateEnd(&deflate_stream);
    
    gzclose(gz_in);
    gzclose(gz_out);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}