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
//<ID> 10
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["deflateInit_","inflateInit_","deflateEnd","deflateSetHeader","gzdopen","inflateEnd","deflateEnd","gzdopen","gzclose","inflateEnd","deflateEnd","gzeof","gzgets","deflateBound","deflate","deflatePending","inflateUndermine","inflate","gzwrite","gztell","gzflush","gzoffset64","crc32","crc32","crc32","crc32","crc32_combine","gzclose","gzclose","inflateEnd","deflateEnd"],"critical_calls":["deflateInit_","inflateInit_","deflateSetHeader","gzdopen","gzdopen","gzeof","gzgets","deflateBound","deflate","deflatePending","inflateUndermine","inflate","gzwrite","gztell","gzflush","gzoffset64","crc32","crc32","crc32","crc32","crc32_combine","gzclose","gzclose","inflateEnd","deflateEnd"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified zlib APIs with the given input data:

*/


// Helper function to read data from input
static size_t  id_000008_read_data(const uint8_t **src, size_t *src_len, void *dst, size_t dst_len) {
    size_t to_copy = (*src_len < dst_len) ? *src_len : dst_len;
    if (to_copy > 0) {
        memcpy(dst, *src, to_copy);
        *src += to_copy;
        *src_len -= to_copy;
    }
    return to_copy;
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Track original data pointer for cleanup
    const uint8_t *original_data = data;
    size_t original_size = size;
    
    // Create temporary files for gzFile operations
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
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Initialize zlib streams
    z_stream deflate_strm, inflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    
    // Initialize deflate stream
    if (deflateInit(&deflate_strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Initialize inflate stream
    if (inflateInit(&inflate_strm) != Z_OK) {
        deflateEnd(&deflate_strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set up gz_header for deflate
    gz_header header;
    memset(&header, 0, sizeof(header));
    header.text = 1;
    header.time = 0;
    header.os = 3; // Unix
    
    // Use deflateSetHeader
    deflateSetHeader(&deflate_strm, &header);
    
    // Create a gzFile for reading compressed data
    gzFile gz_in = gzdopen(in_fd, "rb");
    if (!gz_in) {
        inflateEnd(&inflate_strm);
        deflateEnd(&deflate_strm);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Create output gzFile
    gzFile gz_out = gzdopen(out_fd, "wb");
    if (!gz_out) {
        gzclose(gz_in);
        inflateEnd(&inflate_strm);
        deflateEnd(&deflate_strm);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Reset data pointer to beginning for stream processing
    rewind(in_file);
    
    // Buffer for operations
    char buffer[1024];
    uLong compressed_size = 0;
    uLong decompressed_size = 0;
    
    // Process the input data in chunks
    while (!gzeof(gz_in) && original_size > 0) {
        // Read using gzgets
        if (gzgets(gz_in, buffer, sizeof(buffer))) {
            size_t len = strlen(buffer);
            
            // Use deflateBound to estimate compressed size
            uLong bound = deflateBound(&deflate_strm, len);
            
            // Prepare input for deflate
            deflate_strm.next_in = (Bytef*)buffer;
            deflate_strm.avail_in = len;
            
            Bytef out_buffer[2048];
            deflate_strm.next_out = out_buffer;
            deflate_strm.avail_out = sizeof(out_buffer);
            
            // Perform deflate
            int ret = deflate(&deflate_strm, Z_SYNC_FLUSH);
            if (ret == Z_OK || ret == Z_STREAM_END) {
                compressed_size = sizeof(out_buffer) - deflate_strm.avail_out;
                
                // Use deflatePending
                unsigned int pending;
                int bits;
                deflatePending(&deflate_strm, &pending, &bits);
                
                // Prepare for inflate
                inflate_strm.next_in = out_buffer;
                inflate_strm.avail_in = compressed_size;
                
                Bytef inf_buffer[1024];
                inflate_strm.next_out = inf_buffer;
                inflate_strm.avail_out = sizeof(inf_buffer);
                
                // Perform inflate with undermine
                inflateUndermine(&inflate_strm, 0); // Try with undermine=0
                int inf_ret = inflate(&inflate_strm, Z_SYNC_FLUSH);
                
                if (inf_ret == Z_OK || inf_ret == Z_STREAM_END) {
                    decompressed_size = sizeof(inf_buffer) - inflate_strm.avail_out;
                    
                    // Write to output gzFile
                    gzwrite(gz_out, inf_buffer, decompressed_size);
                    
                    // Use gztell and gzoffset64
                    gztell(gz_in); // Current position
                    
                    // For gzoffset64, we need to flush first
                    gzflush(gz_out, Z_SYNC_FLUSH);
                    gzoffset64(gz_out);
                }
                
                // Calculate CRC32 for combined data
                uLong crc1 = crc32(0L, Z_NULL, 0);
                uLong crc2 = crc32(0L, Z_NULL, 0);
                
                if (compressed_size > 0) {
                    crc1 = crc32(crc1, out_buffer, compressed_size);
                }
                
                if (decompressed_size > 0) {
                    crc2 = crc32(crc2, inf_buffer, decompressed_size);
                }
                
                // Use crc32_combine
                off_t combined_len = compressed_size + decompressed_size;
                if (combined_len > 0) {
                    uLong combined_crc = crc32_combine(crc1, crc2, combined_len);
                    // Use combined_crc to avoid unused variable warning
                    (void)combined_crc;
                }
            }
        }
        
        // Update remaining data size
        original_size -= (original_size > 1024) ? 1024 : original_size;
    }
    
    // Cleanup
    gzclose(gz_in);
    gzclose(gz_out);
    inflateEnd(&inflate_strm);
    deflateEnd(&deflate_strm);
    
    // Close file handles
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}