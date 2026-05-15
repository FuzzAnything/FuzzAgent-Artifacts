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
//<ID> 102
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 2
//<Quality> {"density":11,"unique_branches":{"deflateSetDictionary":[[580,9,580,32,0,0,4,0],[581,13,581,22,0,0,4,1]]},"library_calls":["inflateInit2_","inflateEnd","inflate","inflateGetDictionary","inflateCodesUsed","inflateValidate","inflateSync","inflateEnd","deflateInit_","deflateSetDictionary","deflateEnd","compressBound_z","compress_z","compressBound_z","compress2_z","gzdopen","gzwrite","gzungetc","gzclose","crc32_z","crc32_z"],"critical_calls":["inflateInit2_","inflate","inflateGetDictionary","inflateCodesUsed","inflateValidate","inflateSync","inflateEnd","deflateInit_","deflateSetDictionary","deflateEnd","compressBound_z","compress_z","compressBound_z","compress2_z","gzdopen","gzwrite","gzungetc","gzclose","crc32_z","crc32_z"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // Create input FILE* from the fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output FILE* for compressed data
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
    
    // Initialize inflation stream for decompression
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    
    // Use inflateInit2_ with windowBits = 15+32 for automatic header detection
    if (inflateInit2_(&inflate_strm, 15 + 32, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set up input buffer
    inflate_strm.next_in = (Bytef *)data;
    inflate_strm.avail_in = (uInt)size;
    
    // Allocate output buffer for decompressed data
    size_t decomp_buf_size = size * 4; // Reasonable guess
    Bytef *decomp_buf = (Bytef *)malloc(decomp_buf_size);
    if (!decomp_buf) {
        inflateEnd(&inflate_strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Decompress the data
    int inflate_result = Z_OK;
    size_t total_decompressed = 0;
    
    do {
        inflate_strm.next_out = decomp_buf + total_decompressed;
        inflate_strm.avail_out = (uInt)(decomp_buf_size - total_decompressed);
        
        inflate_result = inflate(&inflate_strm, Z_NO_FLUSH);
        
        if (inflate_result == Z_OK || inflate_result == Z_STREAM_END) {
            total_decompressed = decomp_buf_size - inflate_strm.avail_out;
            
            // Resize buffer if needed
            if (inflate_result == Z_OK && inflate_strm.avail_out == 0) {
                decomp_buf_size *= 2;
                Bytef *new_buf = (Bytef *)realloc(decomp_buf, decomp_buf_size);
                if (!new_buf) break;
                decomp_buf = new_buf;
            }
        }
    } while (inflate_result == Z_OK);
    
    // Try to get dictionary from inflation stream
    Bytef inflate_dict[32768];
    uInt dict_len = 32768;
    int dict_result = inflateGetDictionary(&inflate_strm, inflate_dict, &dict_len);
    
    // Get codes used
    unsigned long codes_used = inflateCodesUsed(&inflate_strm);
    
    // Validate inflation
    int validate_result = inflateValidate(&inflate_strm, 1);
    
    // Try sync if inflation failed
    if (inflate_result != Z_STREAM_END && inflate_result != Z_OK) {
        inflateSync(&inflate_strm);
    }
    
    // End inflation
    inflateEnd(&inflate_strm);
    
    // Initialize deflation stream for recompression
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    
    // Use deflateInit_ with default compression level
    if (deflateInit_(&deflate_strm, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, sizeof(z_stream)) == Z_OK) {
        // Set dictionary if we got one
        if (dict_result == Z_OK && dict_len > 0) {
            deflateSetDictionary(&deflate_strm, inflate_dict, dict_len);
        }
        
        // End deflation stream (we won't actually use it for compression)
        deflateEnd(&deflate_strm);
    }
    
    // Now compress the decompressed data using compress_z and compress2_z
    if (total_decompressed > 0) {
        // First with compress_z (default compression)
        z_size_t dest_len1 = compressBound_z((z_size_t)total_decompressed);
        Bytef *dest1 = (Bytef *)malloc(dest_len1);
        if (dest1) {
            int comp_result1 = compress_z(dest1, &dest_len1, decomp_buf, (z_size_t)total_decompressed);
            free(dest1);
        }
        
        // Then with compress2_z (specified compression level)
        z_size_t dest_len2 = compressBound_z((z_size_t)total_decompressed);
        Bytef *dest2 = (Bytef *)malloc(dest_len2);
        if (dest2) {
            int level = Z_DEFAULT_COMPRESSION;
            if (size > 0) level = data[0] % 10; // Use first byte to determine level
            int comp_result2 = compress2_z(dest2, &dest_len2, decomp_buf, (z_size_t)total_decompressed, level);
            free(dest2);
        }
    }
    
    // Open gzFile for writing
    gzFile gz_out = gzdopen(out_fd, "wb");
    if (gz_out) {
        // Write decompressed data to gzFile
        if (total_decompressed > 0) {
            gzwrite(gz_out, decomp_buf, (unsigned int)total_decompressed);
        }
        
        // Try to unget a character (using first byte of data if available)
        if (size > 0) {
            gzungetc(data[0], gz_out);
        }
        
        // Close gzFile
        gzclose(gz_out);
    } else {
        // If gzdopen failed, close the file descriptor manually
        assert_file_closed(&out_file);;
    }
    
    // Compute CRC32 of decompressed data
    if (total_decompressed > 0) {
        uLong crc = crc32_z(0L, Z_NULL, 0);
        crc = crc32_z(crc, decomp_buf, (z_size_t)total_decompressed);
    }
    
    // Cleanup
    free(decomp_buf);
    assert_file_closed(&in_file);;
    
    // Note: out_file was closed by gzclose or fclose above
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}