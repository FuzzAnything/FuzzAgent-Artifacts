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
//<ID> 2
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["gzdopen","gzdopen","gzclose","gzbuffer","gzclose","gzclose","gzrewind","gzgets","crc32_z","gzfwrite","crc32_z","gzfwrite","gzflush","crc32_combine_gen64","crc32_combine64","deflateInit_","deflateBound","deflate","deflateEnd","gzclose","gzclose"],"critical_calls":["gzdopen","gzdopen","gzbuffer","gzrewind","gzgets","crc32_z","gzfwrite","crc32_z","gzfwrite","gzflush","crc32_combine_gen64","crc32_combine64","deflateInit_","deflateBound","deflate","deflateEnd","gzclose","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Parse zlib compressed data using gzFile API and compute CRC32
    // 2. Combine CRC32 values from different parts of the data
    // 3. Re-compress processed data using deflate API
    
    if (size < 1) {
        return 0;
    }
    
    // Create a memory file from input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file for processed data
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
    
    // Open input as gzFile for reading
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
    
    // Open output as gzFile for writing
    gzFile gz_out = gzdopen(out_fd, "wb");
    if (!gz_out) {
        gzclose(gz_in);  // This will also close in_file
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set buffer size for gz_in (using gzbuffer API)
    gzbuffer(gz_in, 8192);  // 8KB buffer
    
    // Initialize CRC32 values
    uLong crc1 = 0L;
    uLong crc2 = 0L;
    
    // Process data in chunks
    size_t processed = 0;
    size_t chunk_size = 1024;  // 1KB chunks
    size_t first_half = size / 2;
    
    // Read and process first half of data using gzgets
    char* read_buffer = (char*)malloc(chunk_size);
    if (!read_buffer) {
        gzclose(gz_in);
        gzclose(gz_out);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Reset file position to beginning
    gzrewind(gz_in);
    
    // Process first half for CRC32 calculation
    size_t bytes_read = 0;
    while (bytes_read < first_half) {
        int to_read = (first_half - bytes_read < chunk_size - 1) ? 
                      first_half - bytes_read : chunk_size - 1;
        
        // Read chunk using gzgets (reads until newline or buffer full)
        if (gzgets(gz_in, read_buffer, to_read + 1)) {
            size_t len = strlen(read_buffer);
            // Calculate CRC32 for this chunk using crc32_z
            crc1 = crc32_z(crc1, (const Bytef*)read_buffer, len);
            
            // Write to output using gzfwrite
            z_size_t written = gzfwrite(read_buffer, 1, len, gz_out);
            bytes_read += len;
        } else {
            break;
        }
    }
    
    // Process second half for CRC32 calculation
    size_t remaining = size - bytes_read;
    size_t second_half = remaining;
    
    // Read raw data for second half (not using gzgets for binary data)
    // We'll use the original data pointer for the second half
    if (second_half > 0) {
        const Bytef* second_half_data = data + bytes_read;
        crc2 = crc32_z(0L, second_half_data, second_half);
        
        // Write second half to output
        gzfwrite(second_half_data, 1, second_half, gz_out);
    }
    
    // Flush output
    gzflush(gz_out, Z_FINISH);
    
    // Generate combine operation using crc32_combine_gen64
    off64_t len2 = (off64_t)second_half;
    uLong combine_gen = crc32_combine_gen64(len2);
    
    // Combine CRC32 values using crc32_combine64
    uLong combined_crc = crc32_combine64(crc1, crc2, len2);
    
    // Now use deflate API to re-compress the output file
    // Read the output file we just wrote
    fflush(out_file);
    fseek(out_file, 0, SEEK_END);
    long out_size = ftell(out_file);
    fseek(out_file, 0, SEEK_SET);
    
    Bytef* out_data = (Bytef*)malloc(out_size);
    if (out_data) {
        fread(out_data, 1, out_size, out_file);
        
        // Initialize z_stream for deflate
        z_stream strm;
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        
        // Initialize deflate
        if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
            // Set input data
            strm.next_in = out_data;
            strm.avail_in = out_size;
            
            // Allocate output buffer
            uLong bound = deflateBound(&strm, out_size);
            Bytef* compressed = (Bytef*)malloc(bound);
            
            if (compressed) {
                strm.next_out = compressed;
                strm.avail_out = bound;
                
                // Compress using deflate
                int ret = deflate(&strm, Z_FINISH);
                
                // Clean up deflate
                deflateEnd(&strm);
                free(compressed);
            }
        }
        
        free(out_data);
    }
    
    // Clean up
    free(read_buffer);
    
    // Close gz files (this will close the underlying FILE*)
    gzclose(gz_in);  // Closes in_file
    gzclose(gz_out); // Closes out_file
    
    // Remove output file
    remove("output_file");
    
    // Use combined_crc to prevent optimization
    if (combined_crc == 0) {
        // This will never happen, but prevents dead code elimination
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 1;
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}