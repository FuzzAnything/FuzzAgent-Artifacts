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
//<ID> 35
//<Prompt> []
/*<Combination>: [
*/
//<score> 76.666664, nr_unique_branch: 11
//<Quality> {"density":23,"unique_branches":{"gz_look":[[141,9,141,30,0,0,4,0]],"gz_read":[[359,42,359,60,0,0,4,0]],"gz_open":[[277,13,277,31,0,0,4,0]],"gz_avail":[[75,13,76,62,0,0,4,0]],"gzread":[[423,35,423,60,0,0,4,0]],"deflate":[[1086,17,1086,32,0,0,4,0]],"gzclose_r":[[667,12,667,15,0,0,4,0]],"gz_fetch":[[254,17,254,37,0,0,4,0]],"gz_load":[[35,9,35,16,0,0,4,0],[36,13,36,28,0,0,4,1],[36,32,36,52,0,0,4,1]]},"library_calls":["gzdopen","inflateInit2_","zError","inflateEnd","gzread","inflate","inflateUndermine","inflateSetDictionary","inflate","zError","inflateGetDictionary","inflateEnd","gzclose","gzdopen","deflateInit2_","gzread","deflateBound_z","compressBound","deflateSetDictionary","deflate","deflatePending","deflateResetKeep","deflate","deflateEnd","crc32","crc32","crc32","gzclose"],"critical_calls":["gzdopen","inflateInit2_","gzread","inflate","inflateUndermine","inflateSetDictionary","inflate","zError","inflateGetDictionary","inflateEnd","gzclose","gzdopen","deflateInit2_","gzread","deflateBound_z","compressBound","deflateSetDictionary","deflate","deflatePending","deflateResetKeep","deflate","deflateEnd","crc32","crc32","crc32","gzclose"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Check if we have enough data for basic operations
    if (size < 4) {
	return 0;
	}
	
    
    // 1. Create a FILE* from input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Get file descriptor for gzdopen
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 3. Open gzFile using gzdopen (compressed data reading)
    gzFile gz_in = gzdopen(fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 4. Initialize z_stream structures for compression and decompression
    z_stream strm_inflate, strm_deflate;
    Bytef *dictionary = nullptr;
    uInt dictLength = 0;
    int ret;
    
    // 5. Setup inflation (decompression) stream
    memset(&strm_inflate, 0, sizeof(z_stream));
    ret = inflateInit2_(&strm_inflate, MAX_WBITS + 16, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        // Get error message
        const char *err_msg = zError(ret);
        (void)err_msg; // Use error message to avoid unused variable warning
        // Cleanup and continue with compression part
        inflateEnd(&strm_inflate);
    } else {
        // Read and decompress data
        size_t compressed_size = size > 1024 ? 1024 : size;
        Bytef *compressed_buf = (Bytef *)malloc(compressed_size);
        if (compressed_buf) {
            // Read some data from gzFile
            gzread(gz_in, compressed_buf, compressed_size > UINT_MAX ? UINT_MAX : (uInt)compressed_size);
            
            // Set input for inflation
            strm_inflate.next_in = compressed_buf;
            strm_inflate.avail_in = compressed_size > UINT_MAX ? UINT_MAX : (uInt)compressed_size;
            
            // Allocate output buffer for inflation
            size_t out_buf_size = 2048;
            Bytef *out_buf = (Bytef *)malloc(out_buf_size);
            if (out_buf) {
                strm_inflate.next_out = out_buf;
                strm_inflate.avail_out = out_buf_size > UINT_MAX ? UINT_MAX : (uInt)out_buf_size;
                
                // Perform inflation
                ret = inflate(&strm_inflate, Z_NO_FLUSH);
                
                // Try to undermine inflation (for fuzzing)
                if (ret == Z_OK || ret == Z_BUF_ERROR) {
                    // Try to undermine with value from input data
                    int undermine_val = (size > 0) ? (data[0] & 1) : 0;
                    inflateUndermine(&strm_inflate, undermine_val);
                    
                    // Try to set dictionary if we have enough data
                    if (size > 10) {
                        dictLength = (size > UINT_MAX) ? UINT_MAX : (uInt)(size % 100);
                        if (dictLength > 0) {
                            dictionary = (Bytef *)malloc(dictLength);
                            if (dictionary) {
                                // Use part of input data as dictionary
                                size_t dict_offset = size > 100 ? 50 : 0;
                                memcpy(dictionary, data + dict_offset, dictLength);
                                inflateSetDictionary(&strm_inflate, dictionary, dictLength);
                            }
                        }
                    }
                    
                    // Continue inflation
                    ret = inflate(&strm_inflate, Z_FINISH);
                    
                    // Get error message if failed
                    if (ret != Z_STREAM_END && ret != Z_OK) {
                        const char *inflate_err = zError(ret);
                        (void)inflate_err;
                    }
                }
                
                free(out_buf);
            }
            
            // Try to get dictionary back
            if (dictionary && dictLength > 0) {
                uInt got_dict_length = dictLength;
                Bytef *got_dict = (Bytef *)malloc(got_dict_length);
                if (got_dict) {
                    inflateGetDictionary(&strm_inflate, got_dict, &got_dict_length);
                    free(got_dict);
                }
            }
            
            free(compressed_buf);
        }
        
        // End inflation stream
        inflateEnd(&strm_inflate);
    }
    
    // 6. Reset file position for compression operations
    fseek(in_file, 0, SEEK_SET);
    gzclose(gz_in);
    gz_in = gzdopen(fd, "rb"); // Reopen
    
    // 7. Setup deflation (compression) stream
    memset(&strm_deflate, 0, sizeof(z_stream));
    ret = deflateInit2_(&strm_deflate, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                       MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY,
                       ZLIB_VERSION, sizeof(z_stream));
    
    if (ret == Z_OK) {
        // Read data from gzFile for compression
        size_t read_size = size > 512 ? 512 : size;
        Bytef *read_buf = (Bytef *)malloc(read_size);
        if (read_buf) {
            gzread(gz_in, read_buf, read_size > UINT_MAX ? UINT_MAX : (uInt)read_size);
            
            // Set input for deflation
            strm_deflate.next_in = read_buf;
            strm_deflate.avail_in = read_size > UINT_MAX ? UINT_MAX : (uInt)read_size;
            
            // Calculate bounds using different methods
            z_size_t zsource_len = read_size;
            z_size_t deflate_bound_z = deflateBound_z(&strm_deflate, zsource_len);
            uLong source_len = read_size > ULONG_MAX ? ULONG_MAX : (uLong)read_size;
            uLong compress_bound = compressBound(source_len);
            
            // Allocate output buffer for deflation
            size_t deflate_out_size = (size_t)(deflate_bound_z > compress_bound ? 
                                             deflate_bound_z : compress_bound);
            if (deflate_out_size > 4096) deflate_out_size = 4096;
            
            Bytef *deflate_out = (Bytef *)malloc(deflate_out_size);
            if (deflate_out) {
                strm_deflate.next_out = deflate_out;
                strm_deflate.avail_out = deflate_out_size > UINT_MAX ? UINT_MAX : (uInt)deflate_out_size;
                
                // Set dictionary for deflation if available
                if (dictionary && dictLength > 0) {
                    deflateSetDictionary(&strm_deflate, dictionary, dictLength);
                }
                
                // Perform deflation
                ret = deflate(&strm_deflate, Z_FULL_FLUSH);
                
                if (ret == Z_OK) {
                    // Check pending data
                    unsigned int pending;
                    int bits;
                    deflatePending(&strm_deflate, &pending, &bits);
                    
                    // Try to reset while keeping internal state
                    deflateResetKeep(&strm_deflate);
                    
                    // Continue deflation
                    ret = deflate(&strm_deflate, Z_FINISH);
                }
                
                free(deflate_out);
            }
            
            free(read_buf);
        }
        
        // End deflation stream
        deflateEnd(&strm_deflate);
    }
    
    // 8. Calculate CRC32 of input data
    if (size > 0) {
        uInt crc_len = size > UINT_MAX ? UINT_MAX : (uInt)size;
        uLong crc = crc32(0L, Z_NULL, 0);
        crc = crc32(crc, data, crc_len);
        
        // Calculate another CRC with different starting value
        if (size > 10) {
            uLong crc2 = crc32(0xFFFFFFFFL, data + 5, crc_len > 5 ? crc_len - 5 : 0);
            (void)crc2; // Use to avoid unused variable warning
        }
    }
    
    // 9. Cleanup all resources
    if (dictionary) free(dictionary);
    if (gz_in) gzclose(gz_in);
    if (in_file) assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}