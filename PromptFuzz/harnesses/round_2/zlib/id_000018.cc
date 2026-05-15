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
//<ID> 26
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["inflateInit2_","inflate","inflateEnd","crc32","crc32_z","crc32","crc32_z","crc32_combine64","crc32_combine_gen64","deflateInit_","deflateSetHeader","deflateEnd","deflateBound_z","deflateEnd","deflatePrime","deflateEnd","deflate","gzopen","gzbuffer","gzfwrite","gzfwrite","gzdopen","gzgets","gzclose","gzclose","deflateEnd"],"critical_calls":["inflateInit2_","inflate","inflateEnd","crc32","crc32_z","crc32","crc32_z","crc32_combine64","crc32_combine_gen64","deflateInit_","deflateSetHeader","deflateBound_z","deflatePrime","deflate","gzopen","gzbuffer","gzfwrite","gzfwrite","gzdopen","gzgets","gzclose","gzclose","deflateEnd"],"visited":0}
/*Here's a fuzz driver that uses the specified zlib APIs to achieve a common event: **decompressing zlib data, then re-compressing it with custom headers and prime bits, while also calculating CRC32 checksums**.

*/


extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 50) {
	return 0;
	}
	 // Need minimal data for meaningful operations

    // Step 1: Create a FILE* from input data for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Step 2: Create output file for compressed data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 3: Get file descriptors
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);

    // Step 4: Initialize zlib stream for decompression
    z_stream decomp_stream;
    memset(&decomp_stream, 0, sizeof(decomp_stream));
    
    // Use inflateInit2 with windowBits=47 (automatic header detection)
    if (inflateInit2(&decomp_stream, 47) != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Step 5: Prepare buffers for decompression
    Bytef decomp_in[1024];
    Bytef decomp_out[2048];
    
    // Read and decompress in chunks
    size_t bytes_read;
    int decomp_result = Z_OK;
    
    while ((bytes_read = fread(decomp_in, 1, sizeof(decomp_in), in_file)) > 0) {
        decomp_stream.next_in = decomp_in;
        decomp_stream.avail_in = bytes_read;
        
        do {
            decomp_stream.next_out = decomp_out;
            decomp_stream.avail_out = sizeof(decomp_out);
            
            decomp_result = inflate(&decomp_stream, Z_NO_FLUSH);
            
            // Process decompressed output if any
            if (decomp_result == Z_OK || decomp_result == Z_STREAM_END) {
                size_t decompressed_bytes = sizeof(decomp_out) - decomp_stream.avail_out;
                // Could store or process decompressed data here
            }
        } while (decomp_stream.avail_out == 0 && decomp_result == Z_OK);
        
        if (decomp_result == Z_STREAM_END) break;
        if (decomp_result != Z_OK) break;
    }
    
    inflateEnd(&decomp_stream);
    assert_file_closed(&in_file);;

    // Step 6: Re-open input for CRC calculation
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Step 7: Calculate CRC32 checksums using both functions
    uLong crc1 = crc32(0L, Z_NULL, 0);
    uLong crc2 = crc32_z(0L, Z_NULL, 0);
    
    Bytef buffer[512];
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
        crc1 = crc32(crc1, buffer, bytes_read);
        crc2 = crc32_z(crc2, buffer, bytes_read);
    }
    
    // Step 8: Combine CRC values (demonstrating CRC64 combine functions)
    uLong crc_combined = crc32_combine64(crc1, crc2, size);
    uLong crc_gen = crc32_combine_gen64(size);
    
    assert_file_closed(&in_file);;

    // Step 9: Prepare for compression with deflateInit_
    z_stream comp_stream;
    memset(&comp_stream, 0, sizeof(comp_stream));
    
    if (deflateInit_(&comp_stream, Z_DEFAULT_COMPRESSION, 
                     ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Step 10: Set gzip header
    gz_header header;
    memset(&header, 0, sizeof(header));
    header.text = 1;
    header.time = 0;
    header.os = 255; // Unknown OS
    
    if (deflateSetHeader(&comp_stream, &header) != Z_OK) {
        deflateEnd(&comp_stream);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Step 11: Use deflateBound_z to allocate output buffer
    z_size_t source_len = size > 1024 ? 1024 : size; // Use portion of data
    z_size_t max_bound = deflateBound_z(&comp_stream, source_len);
    
    Bytef *compressed_buf = (Bytef*)malloc(max_bound);
    if (!compressed_buf) {
        deflateEnd(&comp_stream);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Step 12: Insert prime bits (demonstrating deflatePrime)
    if (deflatePrime(&comp_stream, 3, 5) != Z_OK) { // Insert 3 bits with value 5
        free(compressed_buf);
        deflateEnd(&comp_stream);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Step 13: Compress a portion of the original data
    comp_stream.next_in = (Bytef*)data;
    comp_stream.avail_in = source_len;
    comp_stream.next_out = compressed_buf;
    comp_stream.avail_out = max_bound;
    
    int flush = Z_FINISH;
    int deflate_result = deflate(&comp_stream, flush);
    
    if (deflate_result == Z_STREAM_END || deflate_result == Z_OK) {
        z_size_t compressed_size = max_bound - comp_stream.avail_out;
        
        // Step 14: Write compressed data to gzip file using gzopen/gzfwrite
        const char* mode = "wb6"; // Write, binary, compression level 6
        gzFile gz_out = gzopen("output_file", mode);
        if (gz_out) {
            // Set buffer size
            gzbuffer(gz_out, 8192);
            
            // Write compressed data
            z_size_t items_written = gzfwrite(compressed_buf, compressed_size, 1, gz_out);
            
            // Also write the CRC values for demonstration
            char crc_str[100];
            snprintf(crc_str, sizeof(crc_str), 
                    "CRC1: %lu, CRC2: %lu, Combined: %lu, Gen: %lu\n",
                    crc1, crc2, crc_combined, crc_gen);
            gzfwrite(crc_str, strlen(crc_str), 1, gz_out);
            
            // Step 15: Try to read back with gzgets (demonstrating the API)
            gzFile gz_in = gzdopen(fd_out, "rb");
            if (gz_in) {
                char read_buf[256];
                gzgets(gz_in, read_buf, sizeof(read_buf));
                gzclose(gz_in); // Close read handle
            }
            
            // Step 16: Close the gzip file
            gzclose(gz_out);
        }
    }

    // Step 17: Clean up deflate
    deflateEnd(&comp_stream);
    free(compressed_buf);

    // Step 18: Clean up other resources
    assert_file_closed(&out_file);;

    // Remove temporary output file
    remove("output_file");

    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}