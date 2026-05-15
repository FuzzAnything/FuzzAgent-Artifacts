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
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":5,"unique_branches":{},"library_calls":["inflateInit2_","inflateEnd","inflate","inflateCodesUsed","inflateEnd","compressBound_z","compress_z","compressBound_z","compress2_z","gzdopen","gzungetc","gzdopen","gzprintf","gzclose","gzclose","deflateInit_","deflateSetDictionary","deflateEnd"],"critical_calls":["inflateInit2_","inflate","inflateCodesUsed","inflateEnd","compressBound_z","compress_z","compressBound_z","compress2_z","gzdopen","gzungetc","gzdopen","gzprintf","gzclose","gzclose","deflateInit_","deflateSetDictionary","deflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Check if we have enough data for basic operations
    if (size < 100) {
        return 0;
    }

    // 1. First, we'll try to decompress the input data (assuming it's compressed)
    // Initialize inflation stream using inflateInit2_
    z_stream inflate_strm;
    inflate_strm.zalloc = Z_NULL;
    inflate_strm.zfree = Z_NULL;
    inflate_strm.opaque = Z_NULL;
    inflate_strm.avail_in = 0;
    inflate_strm.next_in = Z_NULL;
    
    int windowBits = 15 + 32; // Auto-detect gzip or zlib
    int ret = inflateInit2_(&inflate_strm, windowBits, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }

    // Setup input for inflation
    inflate_strm.avail_in = size;
    inflate_strm.next_in = (Bytef*)data;
    
    // Create output buffer for decompressed data
    size_t decompressed_size = size * 4; // Initial guess
    if (decompressed_size > 10 * 1024 * 1024) {
        decompressed_size = 10 * 1024 * 1024; // Cap at 10MB
    }
    Bytef* decompressed = (Bytef*)malloc(decompressed_size);
    if (!decompressed) {
        inflateEnd(&inflate_strm);
        return 0;
    }
    
    inflate_strm.avail_out = decompressed_size;
    inflate_strm.next_out = decompressed;
    
    // Try to decompress
    ret = inflate(&inflate_strm, Z_FINISH);
    size_t actual_decompressed = decompressed_size - inflate_strm.avail_out;
    
    // Check code usage after inflation
    unsigned long codes_used = inflateCodesUsed(&inflate_strm);
    
    // Clean up inflation stream
    inflateEnd(&inflate_strm);

    // 2. Now use the decompressed data (or original if decompression failed) 
    // for compression operations
    Bytef* source_data;
    size_t source_len;
    
    if (ret == Z_STREAM_END || ret == Z_OK) {
        // Use decompressed data
        source_data = decompressed;
        source_len = actual_decompressed;
    } else {
        // Use original data
        source_data = (Bytef*)data;
        source_len = size;
    }

    // 3. Try compress_z with the data
    z_size_t dest_len_z = compressBound_z(source_len);
    Bytef* compressed_z = nullptr;
    if (dest_len_z > 0 && dest_len_z < 100 * 1024 * 1024) { // Reasonable size limit
        compressed_z = (Bytef*)malloc(dest_len_z);
        if (compressed_z) {
            z_size_t actual_dest_len_z = dest_len_z;
            int compress_ret = compress_z(compressed_z, &actual_dest_len_z, 
                                         source_data, source_len);
            // Ignore result for fuzzing
        }
    }

    // 4. Try compress2_z with different compression level
    z_size_t dest_len2_z = compressBound_z(source_len);
    Bytef* compressed2_z = nullptr;
    if (dest_len2_z > 0 && dest_len2_z < 100 * 1024 * 1024) {
        compressed2_z = (Bytef*)malloc(dest_len2_z);
        if (compressed2_z) {
            z_size_t actual_dest_len2_z = dest_len2_z;
            // Use compression level from input data (mod 10 to get valid level)
            int level = data[0] % 10;
            if (level < 0) level = -level;
            int compress2_ret = compress2_z(compressed2_z, &actual_dest_len2_z,
                                           source_data, source_len, level);
        }
    }

    // 5. Work with gzFile operations
    // Create a temporary file with our data
    FILE* tmp_file = fmemopen((void*)source_data, source_len, "rb");
    if (tmp_file) {
        // Get file descriptor and create gzFile
        int fd = fuzz_fileno(tmp_file);
        gzFile gz = gzdopen(fd, "rb");
        if (gz) {
            // Try gzgetc
            int c = gzgetc(gz);
            
            // Try gzungetc if we read something
            if (c != -1) {
                gzungetc(c, gz);
            }
            
            // Try gzprintf (this requires a gzFile opened for writing)
            // So create another file for writing
            FILE* out_file = fopen("output_file", "wb");
            if (out_file) {
                int out_fd = fuzz_fileno(out_file);
                gzFile gz_out = gzdopen(out_fd, "wb");
                if (gz_out) {
                    // Format string based on input data
                    char format[100];
                    snprintf(format, sizeof(format), "Data size: %zu, Code used: %lu", 
                            source_len, codes_used);
                    gzprintf(gz_out, "%s\n", format);
                    
                    gzclose(gz_out);
                }
                assert_file_closed(&out_file);;
            }
            
            gzclose(gz);
        }
        assert_file_closed(&tmp_file);;
    }

    // 6. Try deflateSetDictionary with a dictionary from input data
    z_stream deflate_strm;
    deflate_strm.zalloc = Z_NULL;
    deflate_strm.zfree = Z_NULL;
    deflate_strm.opaque = Z_NULL;
    
    ret = deflateInit(&deflate_strm, Z_DEFAULT_COMPRESSION);
    if (ret == Z_OK) {
        // Create dictionary from input data (first 256 bytes or less)
        uInt dict_length = (source_len > 256) ? 256 : (uInt)source_len;
        if (dict_length > 0) {
            Bytef* dictionary = (Bytef*)malloc(dict_length);
            if (dictionary) {
                memcpy(dictionary, source_data, dict_length);
                deflateSetDictionary(&deflate_strm, dictionary, dict_length);
                free(dictionary);
            }
        }
        
        // Clean up deflate stream
        deflateEnd(&deflate_strm);
    }

    // 7. Clean up allocated resources
    if (decompressed != (Bytef*)data && decompressed != nullptr) {
        free(decompressed);
    }
    
    if (compressed_z) {
        free(compressed_z);
    }
    
    if (compressed2_z) {
        free(compressed2_z);
    }

    assert_file_closed(&tmp_file);
	return 0;
}