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
//<ID> 161
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 3
//<Quality> {"density":24,"unique_branches":{"deflateCopy":[[1355,14,1355,22,0,0,4,0],[1355,26,1355,64,0,0,4,0]],"deflateGetDictionary":[[634,9,634,24,0,0,4,0]]},"library_calls":["inflateInit2_","zError","inflateEnd","inflate","inflateMark","inflateUndermine","crc32","crc32","inflateGetDictionary","inflateSetDictionary","inflateSetDictionary","deflateInit2_","compressBound","deflateBound_z","deflateSetDictionary","deflate","deflatePending","deflateGetDictionary","deflateCopy","deflateResetKeep","deflateEnd","deflateResetKeep","deflateEnd","gzdopen","gzgetc_","gzclose","inflateEnd","deflateEnd"],"critical_calls":["inflateInit2_","inflate","inflateMark","inflateUndermine","crc32","crc32","inflateGetDictionary","inflateSetDictionary","deflateInit2_","compressBound","deflateBound_z","deflateSetDictionary","deflate","deflatePending","deflateGetDictionary","deflateCopy","deflateResetKeep","deflateEnd","deflateResetKeep","deflateEnd","gzdopen","gzgetc_","gzclose","inflateEnd","deflateEnd"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Use the input data as compressed zlib data
    if (size < 1) {
	return 0;
	}
	
    
    // Variables for decompression (inflation)
    z_stream inflate_stream;
    Bytef *inflate_out = nullptr;
    uLong inflate_out_size = 0;
    uLong crc_val = 0;
    int inflate_result = Z_OK;
    
    // Variables for compression (deflation)
    z_stream deflate_stream;
    Bytef *deflate_out = nullptr;
    uLong deflate_out_size = 0;
    int deflate_result = Z_OK;
    
    // Variables for dictionary operations
    Bytef dictionary[32768];
    uInt dict_length = 0;
    unsigned int pending;
    int bits;
    
    // Variables for gz operations
    gzFile gz_fp = nullptr;
    int fd = -1;
    int gz_result = 0;
    
    // FILE pointer for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Get file descriptor for gzdopen
    fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 1. Initialize inflation stream with specific windowBits
    memset(&inflate_stream, 0, sizeof(z_stream));
    inflate_result = inflateInit2_(&inflate_stream, MAX_WBITS + 16, 
                                  ZLIB_VERSION, sizeof(z_stream));
    if (inflate_result != Z_OK) {
        // Use zError to get error message
        const char *err_msg = zError(inflate_result);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Set up inflation input
    inflate_stream.next_in = (Bytef *)data;
    inflate_stream.avail_in = (uInt)size;
    
    // Allocate output buffer for inflation
    inflate_out_size = size * 4; // Reasonable estimate
    inflate_out = (Bytef *)malloc(inflate_out_size);
    if (!inflate_out) {
        inflateEnd(&inflate_stream);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    inflate_stream.next_out = inflate_out;
    inflate_stream.avail_out = inflate_out_size;
    
    // 2. Perform inflation in a loop
    while (inflate_stream.avail_in > 0 && inflate_result == Z_OK) {
        inflate_result = inflate(&inflate_stream, Z_NO_FLUSH);
        
        // Use inflateMark to save current position
        long mark_pos = inflateMark(&inflate_stream);
        (void)mark_pos; // Mark position for potential rollback
        
        // Try to undermine the stream (testing robustness)
        if (inflate_stream.avail_in > 100) {
            inflateUndermine(&inflate_stream, 1);
        }
        
        // If we need more output space
        if (inflate_result == Z_BUF_ERROR && inflate_stream.avail_out == 0) {
            uLong old_size = inflate_out_size;
            inflate_out_size *= 2;
            Bytef *new_out = (Bytef *)realloc(inflate_out, inflate_out_size);
            if (!new_out) break;
            inflate_out = new_out;
            
            inflate_stream.next_out = inflate_out + old_size;
            inflate_stream.avail_out = inflate_out_size - old_size;
            inflate_result = Z_OK;
        }
    }
    
    // Calculate CRC of inflated data
    if (inflate_stream.total_out > 0) {
        crc_val = crc32(0L, Z_NULL, 0);
        crc_val = crc32(crc_val, inflate_out, (uInt)inflate_stream.total_out);
    }
    
    // Try to get inflation dictionary
    if (inflate_result == Z_NEED_DICT || inflate_result == Z_OK) {
        inflateGetDictionary(&inflate_stream, dictionary, &dict_length);
        
        // Try to set a dictionary for inflation
        if (dict_length > 0) {
            inflateSetDictionary(&inflate_stream, dictionary, dict_length);
        } else {
            // Use a small dummy dictionary
            Bytef dummy_dict[] = {0x01, 0x02, 0x03};
            inflateSetDictionary(&inflate_stream, dummy_dict, 3);
        }
    }
    
    // 3. Initialize deflation stream
    memset(&deflate_stream, 0, sizeof(z_stream));
    deflate_result = deflateInit2_(&deflate_stream, Z_DEFAULT_COMPRESSION,
                                   Z_DEFLATED, MAX_WBITS + 16, 8,
                                   Z_DEFAULT_STRATEGY,
                                   ZLIB_VERSION, sizeof(z_stream));
    
    if (deflate_result == Z_OK) {
        // Calculate deflate bound using both functions
        uLong source_len = inflate_stream.total_out;
        z_size_t zsize = (z_size_t)source_len;
        
        uLong bound1 = compressBound(source_len);
        z_size_t bound2 = deflateBound_z(&deflate_stream, zsize);
        
        // Use the larger bound
        deflate_out_size = (uLong)(bound2 > bound1 ? bound2 : bound1);
        if (deflate_out_size < 64) deflate_out_size = 64;
        
        deflate_out = (Bytef *)malloc(deflate_out_size);
        if (deflate_out) {
            // Set up deflation
            deflate_stream.next_in = inflate_out;
            deflate_stream.avail_in = (uInt)inflate_stream.total_out;
            deflate_stream.next_out = deflate_out;
            deflate_stream.avail_out = deflate_out_size;
            
            // Set dictionary for deflation if we have one
            if (dict_length > 0) {
                deflateSetDictionary(&deflate_stream, dictionary, dict_length);
            }
            
            // Perform deflation
            deflate_result = deflate(&deflate_stream, Z_FINISH);
            
            // Check pending bits
            deflatePending(&deflate_stream, &pending, &bits);
            
            // Try to get deflate dictionary
            deflateGetDictionary(&deflate_stream, dictionary, &dict_length);
            
            // Create a copy of deflate stream for testing
            z_stream deflate_copy_stream;
            memset(&deflate_copy_stream, 0, sizeof(z_stream));
            int copy_result = deflateCopy(&deflate_copy_stream, &deflate_stream);
            if (copy_result == Z_OK) {
                deflateResetKeep(&deflate_copy_stream);
                deflateEnd(&deflate_copy_stream);
            }
            
            // Reset and end the original deflate stream
            deflateResetKeep(&deflate_stream);
            deflateEnd(&deflate_stream);
        }
    }
    
    // 4. Test gz operations
    // Use gzdopen with our file descriptor
    gz_fp = gzdopen(fd, "rb");
    if (gz_fp) {
        // Try to read one character
        int gz_char = gzgetc_(gz_fp);
        (void)gz_char;
        
        // Close the gz file
        gzclose(gz_fp);
    } else {
        // If gzdopen failed, we still need to close the file descriptor
        // by closing the FILE pointer
        assert_file_closed(&in_file);;
    }
    
    // 5. Clean up all resources
    if (inflate_out) free(inflate_out);
    if (deflate_out) free(deflate_out);
    
    inflateEnd(&inflate_stream);
    if (deflate_result == Z_OK || deflate_result == Z_STREAM_END) {
        // Already ended above
    } else if (deflate_stream.state) {
        deflateEnd(&deflate_stream);
    }
    
    // If gzdopen didn't close the file (because it failed), close it now
    if (in_file) assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}