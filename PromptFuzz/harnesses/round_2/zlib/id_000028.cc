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
//<ID> 56
//<Prompt> []
/*<Combination>: [
*/
//<score> 9.5, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["inflateInit_","inflate","inflateMark","inflateReset","inflateEnd","inflate","compressBound_z","compress_z","compressBound","compress2","gzdopen","gzwrite","gztell64","gzsetparams","gzwrite","gzclose","deflateInit_","deflateBound_z","deflate","deflateParams","deflate","deflateResetKeep","deflate","deflateEnd","inflateEnd"],"critical_calls":["inflateInit_","inflate","inflateMark","inflateReset","inflate","compressBound_z","compress_z","compressBound","compress2","gzdopen","gzwrite","gztell64","gzsetparams","gzwrite","gzclose","deflateInit_","deflateBound_z","deflate","deflateParams","deflate","deflateResetKeep","deflate","deflateEnd","inflateEnd"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. First, try to decompress the input data (which should be zlib-compressed)
    // using inflate to get the original data
    z_stream decompress_strm;
    Bytef *decompressed_buf = nullptr;
    uLongf decompressed_len = 0;
    int decompress_ret = Z_OK;
    
    // Allocate inflate state
    decompress_strm.zalloc = Z_NULL;
    decompress_strm.zfree = Z_NULL;
    decompress_strm.opaque = Z_NULL;
    decompress_strm.avail_in = 0;
    decompress_strm.next_in = Z_NULL;
    
    if (inflateInit(&decompress_strm) != Z_OK) {
        // Initialization failed
        return 0;
    }
    
    // Set input data
    decompress_strm.avail_in = size;
    decompress_strm.next_in = (Bytef *)data;
    
    // Try to decompress with inflation
    // First, attempt to determine decompressed size by inflating once
    Bytef temp_buf[1024];
    do {
        decompress_strm.avail_out = sizeof(temp_buf);
        decompress_strm.next_out = temp_buf;
        decompress_ret = inflate(&decompress_strm, Z_NO_FLUSH);
    } while (decompress_ret == Z_OK && decompress_strm.avail_out == 0);
    
    // Mark position in inflate stream
    long mark_pos = inflateMark(&decompress_strm);
    
    // Reset inflate stream to beginning
    inflateReset(&decompress_strm);
    
    // Now decompress completely to get original data
    decompress_strm.avail_in = size;
    decompress_strm.next_in = (Bytef *)data;
    
    // Estimate decompressed size (for simplicity, use 4x compressed size)
    decompressed_len = size * 4;
    decompressed_buf = (Bytef *)malloc(decompressed_len);
    if (!decompressed_buf) {
        inflateEnd(&decompress_strm);
        return 0;
    }
    
    decompress_strm.avail_out = decompressed_len;
    decompress_strm.next_out = decompressed_buf;
    
    decompress_ret = inflate(&decompress_strm, Z_FINISH);
    
    if (decompress_ret == Z_STREAM_END || decompress_ret == Z_OK) {
        // Successfully decompressed, actual decompressed size
        decompressed_len = decompress_strm.total_out;
        
        // 2. Now compress the decompressed data using compress_z
        z_size_t compressed_len_z = compressBound_z(decompressed_len);
        Bytef *compressed_buf_z = (Bytef *)malloc(compressed_len_z);
        if (compressed_buf_z) {
            compress_z(compressed_buf_z, &compressed_len_z, 
                       decompressed_buf, decompressed_len);
            free(compressed_buf_z);
        }
        
        // 3. Compress using compress2 with different levels
        uLongf compressed_len2 = compressBound(decompressed_len);
        Bytef *compressed_buf2 = (Bytef *)malloc(compressed_len2);
        if (compressed_buf2) {
            // Try different compression levels
            for (int level = 0; level <= 9; level += 3) {
                uLongf current_len = compressed_len2;
                compress2(compressed_buf2, &current_len, 
                         decompressed_buf, decompressed_len, level);
            }
            free(compressed_buf2);
        }
        
        // 4. Create a gzFile and use gzsetparams
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            int fd = fuzz_fileno(out_file);
            gzFile gz_out = gzdopen(fd, "wb");
            if (gz_out) {
                // Write some data
                gzwrite(gz_out, decompressed_buf, 
                        decompressed_len > 100 ? 100 : (unsigned)decompressed_len);
                
                // Get position before setting params
                off64_t pos_before = gztell64(gz_out);
                
                // Set compression parameters
                gzsetparams(gz_out, 6, Z_DEFAULT_STRATEGY);
                
                // Write more data
                if (decompressed_len > 100) {
                    gzwrite(gz_out, decompressed_buf + 100, 
                            decompressed_len - 100 > 100 ? 100 : 
                            (unsigned)(decompressed_len - 100));
                }
                
                gzclose(gz_out);
            }
            assert_file_closed(&out_file);;
        }
        
        // 5. Use deflate functions with deflateBound_z, deflateResetKeep, deflateParams
        z_stream compress_strm;
        compress_strm.zalloc = Z_NULL;
        compress_strm.zfree = Z_NULL;
        compress_strm.opaque = Z_NULL;
        
        if (deflateInit(&compress_strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
            // Calculate bound for compression
            z_size_t bound_size = deflateBound_z(&compress_strm, decompressed_len);
            
            // Allocate buffer for compression
            Bytef *deflate_buf = (Bytef *)malloc(bound_size);
            if (deflate_buf) {
                compress_strm.next_in = decompressed_buf;
                compress_strm.avail_in = decompressed_len;
                compress_strm.next_out = deflate_buf;
                compress_strm.avail_out = bound_size;
                
                // Compress some data
                deflate(&compress_strm, Z_NO_FLUSH);
                
                // Change compression parameters
                deflateParams(&compress_strm, 9, Z_HUFFMAN_ONLY);
                
                // Continue compression
                deflate(&compress_strm, Z_FINISH);
                
                // Reset stream but keep allocated memory
                deflateResetKeep(&compress_strm);
                
                // Try compression again with different data
                if (decompressed_len > 50) {
                    compress_strm.next_in = decompressed_buf;
                    compress_strm.avail_in = 50;
                    compress_strm.next_out = deflate_buf;
                    compress_strm.avail_out = bound_size;
                    deflate(&compress_strm, Z_FINISH);
                }
                
                free(deflate_buf);
            }
            
            deflateEnd(&compress_strm);
        }
    }
    
    // Cleanup
    if (decompressed_buf) {
        free(decompressed_buf);
    }
    inflateEnd(&decompress_strm);
    
    // Remove temporary output file
    remove("output_file");
    
    assert_file_name_closed("output_file");
	return 0;
}