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
//<ID> 117
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["inflateInit2_","inflateEnd","inflateEnd","inflate","inflateGetDictionary","inflateCodesUsed","inflateEnd","deflateInit_","deflateSetDictionary","compressBound_z","deflateEnd","compress_z","compress2_z","deflateEnd","gzdopen","gzdopen","gzfwrite","gzseek64","gzrewind","gzungetc","gzclose","gzclose"],"critical_calls":["inflateInit2_","inflate","inflateGetDictionary","inflateCodesUsed","inflateEnd","deflateInit_","deflateSetDictionary","compressBound_z","compress_z","compress2_z","deflateEnd","gzdopen","gzdopen","gzfwrite","gzseek64","gzrewind","gzungetc","gzclose","gzclose"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified zlib APIs to achieve a comprehensive decompression and recompression event:

*/


extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Variables for decompression
    z_stream inflate_stream;
    memset(&inflate_stream, 0, sizeof(inflate_stream));
    
    // Initialize inflation for zlib format
    int ret = inflateInit2_(&inflate_stream, MAX_WBITS + 32, 
                           ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    // Set input for inflation
    inflate_stream.next_in = (Bytef*)data;
    inflate_stream.avail_in = (uInt)size;
    
    // Allocate output buffer for decompressed data
    size_t decompressed_size = size * 4; // Initial guess
    if (decompressed_size < 1024) decompressed_size = 1024;
    Bytef* decompressed = (Bytef*)malloc(decompressed_size);
    if (!decompressed) {
        inflateEnd(&inflate_stream);
        return 0;
    }
    
    // Inflate in chunks
    size_t total_out = 0;
    do {
        inflate_stream.next_out = decompressed + total_out;
        inflate_stream.avail_out = (uInt)(decompressed_size - total_out);
        
        if (inflate_stream.avail_out == 0) {
            // Resize buffer
            decompressed_size *= 2;
            Bytef* new_decompressed = (Bytef*)realloc(decompressed, decompressed_size);
            if (!new_decompressed) {
                free(decompressed);
                inflateEnd(&inflate_stream);
                return 0;
            }
            decompressed = new_decompressed;
            inflate_stream.next_out = decompressed + total_out;
            inflate_stream.avail_out = (uInt)(decompressed_size - total_out);
        }
        
        ret = inflate(&inflate_stream, Z_SYNC_FLUSH);
        total_out = (size_t)(inflate_stream.next_out - decompressed);
        
    } while (ret == Z_OK);
    
    // Get dictionary if available
    Bytef dictionary[32768];
    uInt dict_length = sizeof(dictionary);
    inflateGetDictionary(&inflate_stream, dictionary, &dict_length);
    
    // Get codes used
    unsigned long codes_used = inflateCodesUsed(&inflate_stream);
    
    // End inflation
    inflateEnd(&inflate_stream);
    
    if (ret != Z_STREAM_END && ret != Z_OK) {
        free(decompressed);
        return 0;
    }
    
    // Now test compression APIs
    z_stream deflate_stream;
    memset(&deflate_stream, 0, sizeof(deflate_stream));
    
    // Initialize deflation
    ret = deflateInit_(&deflate_stream, Z_DEFAULT_COMPRESSION, 
                      ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        free(decompressed);
        return 0;
    }
    
    // Set dictionary for deflation if we got one
    if (dict_length > 0) {
        deflateSetDictionary(&deflate_stream, dictionary, dict_length);
    }
    
    // Calculate compressed bound
    z_size_t bound = compressBound_z((z_size_t)total_out);
    
    // Allocate compression buffers
    Bytef* compressed1 = (Bytef*)malloc(bound);
    Bytef* compressed2 = (Bytef*)malloc(bound);
    if (!compressed1 || !compressed2) {
        free(compressed1);
        free(compressed2);
        deflateEnd(&deflate_stream);
        free(decompressed);
        return 0;
    }
    
    // Test compress_z
    z_size_t comp_len1 = bound;
    ret = compress_z(compressed1, &comp_len1, decompressed, (z_size_t)total_out);
    
    // Test compress2_z with different compression levels
    z_size_t comp_len2 = bound;
    for (int level = 0; level <= 9; level += 3) {
        comp_len2 = bound;
        compress2_z(compressed2, &comp_len2, decompressed, (z_size_t)total_out, level);
    }
    
    // End deflation
    deflateEnd(&deflate_stream);
    
    // Test gzFile operations
    FILE* in_file = fmemopen((void *)decompressed, total_out, "rb");
    if (!in_file) {
        free(compressed1);
        free(compressed2);
        free(decompressed);
        assert_file_closed(&in_file);
	return 0;
    }
    
    int fd = fuzz_fileno(in_file);
    gzFile gz = gzdopen(fd, "rb");
    if (gz) {
        // Write to a temporary file using gzfwrite
        FILE* out_file = fopen("output_file", "wb");
        if (out_file) {
            int out_fd = fuzz_fileno(out_file);
            gzFile gz_out = gzdopen(out_fd, "wb");
            if (gz_out) {
                // Write in chunks
                Bytef buffer[4096];
                size_t bytes_read;
                while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
                    gzfwrite(buffer, 1, bytes_read, gz_out);
                }
                
                // Seek operations
                gzseek64(gz_out, 0, SEEK_SET);
                gzrewind(gz_out);
                
                // Unget a character
                int c = gzgetc(gz_out);
                if (c != EOF) {
                    gzungetc(c, gz_out);
                }
                
                gzclose(gz_out);
            }
            assert_file_closed(&out_file);;
        }
        
        // Close gz file
        gzclose(gz);
    }
    
    // Clean up
    assert_file_closed(&in_file);;
    free(compressed1);
    free(compressed2);
    free(decompressed);
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}