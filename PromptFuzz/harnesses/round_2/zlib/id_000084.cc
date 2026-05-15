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
//<ID> 288
//<Prompt> []
/*<Combination>: [
*/
//<score> 40, nr_unique_branch: 3
//<Quality> {"density":20,"unique_branches":{"deflateInit2_":[[435,9,435,23,0,0,4,0],[436,48,436,63,0,0,4,0],[436,67,436,76,0,0,4,0]]},"library_calls":["zlibVersion","gzdopen","gzdopen","gzclose","gzdirect","gzoffset64","gzbuffer","deflateInit2_","gzclose","gzclose","deflateCopy","deflateEnd","inflateInit2_","deflateEnd","gzclose","gzclose","inflateBackInit_","inflateCopy","inflateEnd","gzeof","gzread","deflate","inflate","inflateSync","inflatePrime","deflatePrime","deflateUsed","deflate","inflateReset2","deflateParams","gzprintf","gzputs","gzflush","deflateEnd","inflateEnd","inflateBackEnd","gzclose","gzclose"],"critical_calls":["zlibVersion","gzdopen","gzdopen","gzdirect","gzoffset64","gzbuffer","deflateInit2_","deflateCopy","deflateEnd","inflateInit2_","inflateBackInit_","inflateCopy","inflateEnd","gzeof","gzread","deflate","inflate","inflateSync","inflatePrime","deflatePrime","deflateUsed","deflate","inflateReset2","deflateParams","gzprintf","gzputs","gzflush","deflateEnd","inflateEnd","inflateBackEnd","gzclose","gzclose"],"visited":0}
/*Here's a comprehensive fuzz driver that utilizes the specified zlib APIs to achieve a complete compression/decompression workflow with various operations:

*/


extern "C" int LLVMFuzzerTestOneInput_84(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful fuzzing
    
    // Create temporary files for input/output
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
    
    // Version check
    const char *version = zlibVersion();
    
    // ====== GZFILE OPERATIONS ======
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
    
    // Test gzdirect
    int is_direct = gzdirect(gz_in);
    
    // Test gzoffset64
    off64_t offset = gzoffset64(gz_in);
    
    // Set buffer size
    gzbuffer(gz_out, 8192);
    
    // ====== DEFLATE (COMPRESSION) STREAM ======
    z_stream deflate_stream;
    memset(&deflate_stream, 0, sizeof(deflate_stream));
    
    // Initialize deflate stream with various parameters
    int windowBits = 15 + (data[0] % 16); // 15-30
    int memLevel = 1 + (data[1] % 9);    // 1-9
    int strategy = data[2] % 4;          // 0-3
    
    int ret = deflateInit2_(&deflate_stream, 
                           Z_DEFAULT_COMPRESSION, 
                           Z_DEFLATED, 
                           windowBits, 
                           memLevel, 
                           strategy,
                           ZLIB_VERSION, 
                           sizeof(z_stream));
    
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
    
    // Create another deflate stream for copying
    z_stream deflate_stream_copy;
    memset(&deflate_stream_copy, 0, sizeof(deflate_stream_copy));
    
    // Test deflateCopy
    if (deflateCopy(&deflate_stream_copy, &deflate_stream) == Z_OK) {
        deflateEnd(&deflate_stream_copy);
    }
    
    // ====== INFLATE (DECOMPRESSION) STREAM ======
    z_stream inflate_stream;
    memset(&inflate_stream, 0, sizeof(inflate_stream));
    
    // Initialize inflate stream
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
    
    // Create another inflate stream for back decompression
    z_stream inflate_stream_back;
    memset(&inflate_stream_back, 0, sizeof(inflate_stream_back));
    
    unsigned char window[32768]; // 32K window
    ret = inflateBackInit_(&inflate_stream_back, windowBits, window, 
                          ZLIB_VERSION, sizeof(z_stream));
    if (ret == Z_OK) {
        // Note: inflateBack requires callbacks which we're not implementing here
        // Just initialize and clean up
    }
    
    // Create copy of inflate stream
    z_stream inflate_stream_copy;
    memset(&inflate_stream_copy, 0, sizeof(inflate_stream_copy));
    
    // Test inflateCopy
    if (inflateCopy(&inflate_stream_copy, &inflate_stream) == Z_OK) {
        inflateEnd(&inflate_stream_copy);
    }
    
    // ====== PROCESS DATA ======
    size_t processed = 0;
    Bytef in_buffer[4096];
    Bytef out_buffer[8192];
    
    // Read from gzfile and process
    while (processed < size && !gzeof(gz_in)) {
        // Read some data
        int bytes_read = gzread(gz_in, in_buffer, 
                               (size - processed) > sizeof(in_buffer) ? 
                               sizeof(in_buffer) : size - processed);
        if (bytes_read <= 0) break;
        
        processed += bytes_read;
        
        // Set input for deflate
        deflate_stream.next_in = in_buffer;
        deflate_stream.avail_in = bytes_read;
        
        // Compress the data
        do {
            deflate_stream.next_out = out_buffer;
            deflate_stream.avail_out = sizeof(out_buffer);
            
            ret = deflate(&deflate_stream, Z_NO_FLUSH);
            if (ret == Z_STREAM_ERROR) break;
            
            size_t compressed_size = sizeof(out_buffer) - deflate_stream.avail_out;
            
            // Set compressed data as input for inflate
            inflate_stream.next_in = out_buffer;
            inflate_stream.avail_in = compressed_size;
            
            // Decompress the data
            Bytef decompressed[8192];
            inflate_stream.next_out = decompressed;
            inflate_stream.avail_out = sizeof(decompressed);
            
            ret = inflate(&inflate_stream, Z_NO_FLUSH);
            if (ret < 0) {
                // Try sync if error
                inflateSync(&inflate_stream);
            }
            
            // Test inflatePrime with random bits
            if (processed % 100 == 0) {
                inflatePrime(&inflate_stream, 4, data[processed % size] & 0x0F);
            }
            
            // Test deflatePrime with random bits
            if (processed % 150 == 0) {
                deflatePrime(&deflate_stream, 3, data[processed % size] & 0x07);
            }
            
            // Test deflateUsed
            int bits_used;
            deflateUsed(&deflate_stream, &bits_used);
            
        } while (deflate_stream.avail_out == 0);
    }
    
    // Finish deflation
    deflate_stream.next_in = Z_NULL;
    deflate_stream.avail_in = 0;
    
    int deflate_done = 0;
    while (!deflate_done) {
        deflate_stream.next_out = out_buffer;
        deflate_stream.avail_out = sizeof(out_buffer);
        
        ret = deflate(&deflate_stream, Z_FINISH);
        
        if (ret == Z_STREAM_END) {
            deflate_done = 1;
        } else if (ret != Z_OK) {
            break;
        }
    }
    
    // Reset streams with different parameters
    inflateReset2(&inflate_stream, windowBits - 1);
    
    // Set parameters on deflate stream
    deflateParams(&deflate_stream, 
                  data[3] % 10,  // level 0-9
                  data[4] % 4);  // strategy 0-3
    
    // ====== GZFILE WRITING OPERATIONS ======
    // Write some data to gzfile
    const char *test_string = "Test output\n";
    gzprintf(gz_out, "Processed %lu bytes\n", (unsigned long)processed);
    gzputs(gz_out, test_string);
    
    // Flush the output
    gzflush(gz_out, Z_SYNC_FLUSH);
    
    // ====== CLEANUP ======
    deflateEnd(&deflate_stream);
    inflateEnd(&inflate_stream);
    
    if (inflate_stream_back.zalloc) {
        inflateBackEnd(&inflate_stream_back);
    }
    
    // Close gzfiles
    gzclose(gz_in);
    gzclose(gz_out);
    
    // Close regular files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}