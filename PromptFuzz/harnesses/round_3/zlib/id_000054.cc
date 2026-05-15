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
//<ID> 108
//<Prompt> []
/*<Combination>: [
*/
//<score> 51, nr_unique_branch: 11
//<Quality> {"density":17,"unique_branches":{"gz_comp":[[116,21,116,29,0,0,4,0],[117,25,117,40,0,0,4,1],[117,44,117,64,0,0,4,1]],"gz_write":[[245,17,245,26,0,0,4,0],[246,24,246,36,0,0,4,1]],"gzclose_w":[[685,9,685,39,0,0,4,0],[696,9,696,31,0,0,4,0]],"deflateBound_z":[[883,13,883,24,0,0,4,0]],"inflateSetDictionary":[[1196,29,1196,48,0,0,4,1],[1200,9,1200,28,0,0,4,0],[1203,13,1203,35,0,0,4,0]]},"library_calls":["inflateInit2_","zError","deflateInit2_","zError","inflateEnd","deflateSetHeader","zError","deflateTune","zError","inflate","inflateGetDictionary","inflateSetDictionary","crc32","crc32_z","deflate","gzdopen","gzwrite","gzclose_w","deflate","deflateBound","crc32_combine","crc32_combine64","compress","compress2_z","inflateEnd","deflateEnd"],"critical_calls":["inflateInit2_","deflateInit2_","deflateSetHeader","zError","deflateTune","zError","inflate","inflateGetDictionary","inflateSetDictionary","crc32","crc32_z","deflate","gzdopen","gzwrite","gzclose_w","deflate","deflateBound","crc32_combine","crc32_combine64","compress","compress2_z","inflateEnd","deflateEnd"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t* f_data, size_t f_size) {
	if(f_size<126) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_12, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_15, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_16, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_17, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_18, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_19, fdp);
	//fuzzer shim end}




    // Early exit if input is too small
    if (size < 10) {
	return 0;
	}
	
    
    // Create input and output files using fmemopen/fopen
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
    
    // Variables for zlib operations
    int ret;
    z_stream strm_inflate, strm_deflate;
    gz_header header;
    Bytef *dict = nullptr;
    uInt dictLength = 0;
    uLong crc1 = 0, crc2 = 0, crc_combined = 0;
    
    // Initialize inflate stream for decompression
    memset(&strm_inflate, 0, sizeof(strm_inflate));
    ret = inflateInit2_(&strm_inflate, 15 + 32, fuzz_str_1, sizeof(z_stream));
    if (ret != Z_OK) {
        const char *err_msg = zError(ret);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Initialize deflate stream for recompression
    memset(&strm_deflate, 0, sizeof(strm_deflate));
    ret = deflateInit2_(&strm_deflate, fuzz_int32_t_3, fuzz_int32_t_4,
                        15 + 16, fuzz_int32_t_5, fuzz_int32_t_6,
                        fuzz_str_2, sizeof(z_stream));
    if (ret != Z_OK) {
        const char *err_msg = zError(ret);
        inflateEnd(&strm_inflate);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set gzip header for deflate
    memset(&header, 0, sizeof(header));
    header.os = 255; // Unknown OS
    ret = deflateSetHeader(&strm_deflate, &header);
    if (ret != Z_OK) {
        const char *err_msg = zError(ret);
    }
    
    // Tune deflate parameters
    ret = deflateTune(&strm_deflate, fuzz_int32_t_7, fuzz_int32_t_8, fuzz_int32_t_9, fuzz_int32_t_10);
    if (ret != Z_OK) {
        const char *err_msg = zError(ret);
    }
    
    // Allocate buffers
    const size_t CHUNK = 64 * 1024;
    Bytef in_buf[CHUNK];
    Bytef out_buf[CHUNK];
    Bytef decompressed_buf[CHUNK];
    
    // Read and decompress input data
    strm_inflate.avail_in = 0;
    strm_inflate.next_in = in_buf;
    
    do {
        // Read from input file
        size_t bytes_read = fread(in_buf, 1, CHUNK, in_file);
        if (ferror(in_file)) break;
        
        strm_inflate.avail_in = bytes_read;
        strm_inflate.next_in = in_buf;
        
        do {
            strm_inflate.avail_out = CHUNK;
            strm_inflate.next_out = decompressed_buf;
            
            ret = inflate(&strm_inflate, fuzz_int32_t_11);
            
            if (ret == Z_NEED_DICT) {
                // Try to get dictionary if needed
                dict = (Bytef*)malloc(32768);
                if (dict) {
                    ret = inflateGetDictionary(&strm_inflate, dict, &dictLength);
                    if (ret == Z_OK) {
                        ret = inflateSetDictionary(&strm_inflate, dict, dictLength);
                    }
                    free(dict);
                }
            }
            
            size_t decompressed_len = CHUNK - strm_inflate.avail_out;
            
            if (decompressed_len > 0) {
                // Compute CRC32 of decompressed data
                crc1 = crc32(fuzz_uint64_t_12, decompressed_buf, decompressed_len);
                crc2 = crc32_z(fuzz_uint64_t_13, decompressed_buf, decompressed_len);
                
                // Recompress the decompressed data
                strm_deflate.avail_in = decompressed_len;
                strm_deflate.next_in = decompressed_buf;
                
                do {
                    strm_deflate.avail_out = CHUNK;
                    strm_deflate.next_out = out_buf;
                    
                    ret = deflate(&strm_deflate, fuzz_int32_t_14);
                    if (ret == Z_STREAM_ERROR) break;
                    
                    size_t compressed_len = CHUNK - strm_deflate.avail_out;
                    
                    if (compressed_len > 0) {
                        // Write to output file
                        fwrite(out_buf, 1, compressed_len, out_file);
                        
                        // Also use gzwrite for gzip file
                        gzFile gz_out = gzdopen(out_fd, "wb");
                        if (gz_out) {
                            gzwrite(gz_out, out_buf, compressed_len);
                            gzclose_w(gz_out);
                        }
                    }
                } while (strm_deflate.avail_out == 0);
            }
        } while (strm_inflate.avail_out == 0);
    } while (ret != Z_STREAM_END && !feof(in_file));
    
    // Finish deflation
    do {
        strm_deflate.avail_out = CHUNK;
        strm_deflate.next_out = out_buf;
        
        ret = deflate(&strm_deflate, Z_FINISH);
        if (ret == Z_STREAM_ERROR) break;
        
        size_t compressed_len = CHUNK - strm_deflate.avail_out;
        if (compressed_len > 0) {
            fwrite(out_buf, 1, compressed_len, out_file);
        }
    } while (ret != Z_STREAM_END);
    
    // Get deflate bound for the decompressed data
    uLong bound = deflateBound(&strm_deflate, strm_inflate.total_out);
    
    // Combine CRC32 values
    crc_combined = crc32_combine(fuzz_uint64_t_15, fuzz_uint64_t_16, 100);
    crc_combined = crc32_combine64(fuzz_uint64_t_17, fuzz_uint64_t_18, 100);
    
    // Test compress and compress2_z with small buffer
    if (strm_inflate.total_out > 0 && strm_inflate.total_out < 1024) {
        Bytef comp_buf[2048];
        uLongf comp_len = sizeof(comp_buf);
        Bytef source_buf[1024];
        
        // Copy some decompressed data
        size_t copy_len = strm_inflate.total_out < 1024 ? 
                         strm_inflate.total_out : 1024;
        memcpy(source_buf, decompressed_buf, copy_len);
        
        // Use compress
        ret = compress(comp_buf, &comp_len, source_buf, copy_len);
        
        // Use compress2_z
        z_size_t comp_len_z = sizeof(comp_buf);
        ret = compress2_z(comp_buf, &comp_len_z, source_buf, copy_len, fuzz_int32_t_19);
    }
    
    // Clean up
    inflateEnd(&strm_inflate);
    deflateEnd(&strm_deflate);
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