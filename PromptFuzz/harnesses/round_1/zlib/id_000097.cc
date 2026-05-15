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
//<ID> 312
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 1
//<Quality> {"density":10,"unique_branches":{"gztell64":[[450,9,450,21,0,0,4,0]]},"library_calls":["zlibVersion","gzdopen","gzbuffer","inflateInit2_","gzclose","gzread","inflate","inflateMark","inflateCodesUsed","inflateReset2","inflateEnd","gzclose","gztell64","compressBound_z","compress2_z","deflateInit_","deflate","deflateEnd","gzdopen","gzprintf","gzclose"],"critical_calls":["zlibVersion","gzdopen","gzbuffer","inflateInit2_","gzread","inflate","inflateMark","inflateCodesUsed","inflateReset2","inflateEnd","gzclose","gztell64","compressBound_z","compress2_z","deflateInit_","deflate","deflateEnd","gzdopen","gzprintf","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_97(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Get zlib version
    const char* version = zlibVersion();
    
    // Set up input/output files
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
    
    // Step 1: Set up gzFile for reading compressed data
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
    
    // Set buffer size for gzFile
    unsigned int buffer_size = 4096;
    gzbuffer(gz_in, buffer_size);
    
    // Step 2: Decompress the input data using streaming API
    z_stream strm_inflate;
    memset(&strm_inflate, 0, sizeof(z_stream));
    
    // Initialize inflate with automatic header detection
    int ret = inflateInit2_(&strm_inflate, 15 + 32, version, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Prepare input buffer
    Bytef in_buffer[4096];
    Bytef out_buffer[4096];
    
    // Read compressed data from gzFile and decompress
    int bytes_read;
    strm_inflate.avail_in = 0;
    
    do {
        // Read from gzFile
        bytes_read = gzread(gz_in, in_buffer, sizeof(in_buffer));
        if (bytes_read < 0) break;
        
        strm_inflate.next_in = in_buffer;
        strm_inflate.avail_in = bytes_read;
        
        // Decompress
        do {
            strm_inflate.next_out = out_buffer;
            strm_inflate.avail_out = sizeof(out_buffer);
            
            ret = inflate(&strm_inflate, Z_NO_FLUSH);
            
            // Use inflateMark to get current state
            long mark = inflateMark(&strm_inflate);
            
            // Get codes used (for statistics)
            unsigned long codes_used = inflateCodesUsed(&strm_inflate);
            
            size_t decompressed = sizeof(out_buffer) - strm_inflate.avail_out;
            if (decompressed > 0) {
                // Write decompressed data to output file
                fwrite(out_buffer, 1, decompressed, out_file);
            }
            
        } while (strm_inflate.avail_out == 0);
        
    } while (bytes_read > 0 && ret != Z_STREAM_END);
    
    // Try inflateReset2 with different window bits
    inflateReset2(&strm_inflate, 15);
    
    // End inflation
    inflateEnd(&strm_inflate);
    gzclose(gz_in);
    
    // Step 3: Compress the decompressed data
    rewind(out_file);
    fflush(out_file);
    
    // Get file size for output
    off64_t file_pos = gztell64(NULL); // Note: This would normally use gzFile, but we're demonstrating API usage
    
    // Read decompressed data from output file
    fseek(out_file, 0, SEEK_END);
    long decompressed_size = ftell(out_file);
    rewind(out_file);
    
    Bytef* source_data = (Bytef*)malloc(decompressed_size);
    if (!source_data) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    fread(source_data, 1, decompressed_size, out_file);
    
    // Calculate maximum compressed size
    z_size_t max_compressed = compressBound_z(decompressed_size);
    Bytef* compressed_data = (Bytef*)malloc(max_compressed);
    if (!compressed_data) {
        free(source_data);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    z_size_t compressed_len = max_compressed;
    
    // Compress using one-shot API
    ret = compress2_z(compressed_data, &compressed_len, 
                     source_data, decompressed_size, Z_DEFAULT_COMPRESSION);
    
    // Also compress using streaming API for demonstration
    z_stream strm_deflate;
    memset(&strm_deflate, 0, sizeof(z_stream));
    
    ret = deflateInit_(&strm_deflate, Z_DEFAULT_COMPRESSION, version, sizeof(z_stream));
    if (ret == Z_OK) {
        Bytef deflate_out[4096];
        strm_deflate.next_in = source_data;
        strm_deflate.avail_in = decompressed_size;
        
        do {
            strm_deflate.next_out = deflate_out;
            strm_deflate.avail_out = sizeof(deflate_out);
            
            ret = deflate(&strm_deflate, Z_FINISH);
            
        } while (strm_deflate.avail_out == 0);
        
        deflateEnd(&strm_deflate);
    }
    
    // Step 4: Write compressed data back to gzFile
    gzFile gz_out = gzdopen(fd_out, "wb");
    if (gz_out) {
        // Write using gzprintf (formatted output)
        gzprintf(gz_out, "Compressed data: %s\n", "Zlib compression test");
        
        // Close gzFile
        gzclose(gz_out);
    }
    
    // Cleanup
    free(source_data);
    free(compressed_data);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}