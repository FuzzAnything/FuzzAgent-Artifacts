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
//<ID> 82
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 3
//<Quality> {"density":7,"unique_branches":{"gzread":[[407,31,407,56,0,0,4,0],[407,60,407,73,0,0,4,0]],"gzbuffer":[[333,9,333,25,0,0,4,0]]},"library_calls":["uncompress2","uncompress2","crc32_z","crc32_z","inflateInit_","inflateReset2","inflate","inflateSetDictionary","inflate","uncompress","inflateEnd","gzopen","gzgetc_","gzbuffer","gzread","gzrewind","gzgets","gzclose","deflateInit_","deflateBound","deflate","deflateCopy","deflateEnd","deflateEnd"],"critical_calls":["uncompress2","uncompress2","crc32_z","crc32_z","inflateInit_","inflateReset2","inflate","inflateSetDictionary","inflate","uncompress","inflateEnd","gzopen","gzgetc_","gzbuffer","gzread","gzrewind","gzgets","gzclose","deflateInit_","deflateBound","deflate","deflateCopy","deflateEnd","deflateEnd"],"visited":0}
/*Looking at the provided zlib APIs and the requirements, I'll create a fuzz driver that tests decompression functionality with dictionary support and CRC validation. The event I'll implement is: "Decompressing zlib-compressed data with dictionary support and verifying CRC integrity."

*/


extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return for insufficient data
    if (size < 100) {
	return 0;
	}
	
    
    // Split input into different parts for different purposes
    size_t dict_size = size / 4;
    size_t comp_size = size / 2;
    size_t remaining = size - dict_size - comp_size;
    
    if (dict_size == 0 || comp_size == 0 || remaining == 0) {
	return 0;
	}
	
    
    // Part 1: Create a dictionary from first part of input
    Bytef *dictionary = (Bytef *)data;
    uInt dictLength = dict_size;
    
    // Part 2: Use second part as compressed data
    const Bytef *compressed_data = data + dict_size;
    uLong sourceLen = comp_size;
    
    // Part 3: Use remaining part for other operations
    const Bytef *extra_data = data + dict_size + comp_size;
    size_t extra_len = remaining;
    
    // Variables for decompression
    Bytef *dest = nullptr;
    uLongf destLen = 0;
    int ret = 0;
    
    // 1. First, try to uncompress the data using uncompress2 to get decompressed size
    destLen = sourceLen * 10; // Initial guess for decompressed size
    dest = (Bytef *)malloc(destLen);
    if (!dest) {
	return 0;
	}
	
    
    uLong sourceLenCopy = sourceLen;
    ret = uncompress2(dest, &destLen, compressed_data, &sourceLenCopy);
    
    // If uncompress2 failed with Z_BUF_ERROR, try with a larger buffer
    if (ret == Z_BUF_ERROR) {
        free(dest);
        destLen = sourceLen * 100; // Try larger buffer
        dest = (Bytef *)malloc(destLen);
        if (!dest) {
	return 0;
	}
	
        ret = uncompress2(dest, &destLen, compressed_data, &sourceLenCopy);
    }
    
    // If we have decompressed data, use crc32_z to compute CRC
    uLong crc = 0;
    if (ret == Z_OK) {
        // 3. Compute CRC of decompressed data using crc32_z
        crc = crc32_z(0L, Z_NULL, 0);
        crc = crc32_z(crc, dest, (z_size_t)destLen);
    }
    
    // Now test with inflate stream API using dictionary
    z_stream strm;
    Bytef *out_buffer = nullptr;
    uLong out_len = 0;
    
    // 2. Setup inflate stream with dictionary
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    
    ret = inflateInit(&strm);
    if (ret == Z_OK) {
        // 7. Reset inflate stream with different window bits
        ret = inflateReset2(&strm, 15); // MAX_WBITS
        
        // Setup input
        strm.avail_in = (uInt)sourceLen;
        strm.next_in = (Bytef *)compressed_data;
        
        // Allocate output buffer
        out_len = sourceLen * 10;
        out_buffer = (Bytef *)malloc(out_len);
        if (out_buffer) {
            strm.avail_out = (uInt)out_len;
            strm.next_out = out_buffer;
            
            // Try to inflate
            ret = inflate(&strm, Z_FINISH);
            
            // 2. Set dictionary for inflate
            if (ret == Z_NEED_DICT) {
                ret = inflateSetDictionary(&strm, dictionary, dictLength);
                if (ret == Z_OK) {
                    // Continue inflation
                    ret = inflate(&strm, Z_FINISH);
                }
            }
            
            // If we got some output, try using uncompress API as well
            if (strm.total_out > 0) {
                // 1. Try regular uncompress API
                uLongf destLen2 = 0;
                Bytef *dest2 = nullptr;
                
                // First get size
                uLong sourceLen2 = sourceLen;
                destLen2 = sourceLen * 10;
                dest2 = (Bytef *)malloc(destLen2);
                if (dest2) {
                    ret = uncompress(dest2, &destLen2, compressed_data, sourceLen2);
                    free(dest2);
                }
            }
            
            free(out_buffer);
        }
        
        inflateEnd(&strm);
    }
    
    // 5. Test gzgetc_ with file operations
    // Create a temporary file with compressed data
    FILE *tmp_file = fopen("temp_compressed.gz", "wb");
    if (tmp_file) {
        fwrite(compressed_data, 1, sourceLen, tmp_file);
        assert_file_closed(&tmp_file);;
        
        // Open as gzFile
        gzFile gzfile = gzopen("temp_compressed.gz", "rb");
        if (gzfile) {
            // 5. Use gzgetc_ to read first character
            int gz_char = gzgetc_(gzfile);
            
            // Try to read more if we have a valid file
            if (gz_char != -1) {
                // Set buffer size
                gzbuffer(gzfile, 8192);
                
                // Try to read a small buffer
                char buf[100];
                int bytes_read = gzread(gzfile, buf, sizeof(buf));
                
                // Try gzgets
                if (bytes_read > 0) {
                    gzrewind(gzfile);
                    char line[100];
                    gzgets(gzfile, line, sizeof(line));
                }
            }
            
            gzclose(gzfile);
        }
        
        // Clean up temp file
        remove("temp_compressed.gz");
    }
    
    // 6. Test deflateCopy
    z_stream deflate_strm1, deflate_strm2;
    
    // Initialize first deflate stream
    deflate_strm1.zalloc = Z_NULL;
    deflate_strm1.zfree = Z_NULL;
    deflate_strm1.opaque = Z_NULL;
    
    ret = deflateInit(&deflate_strm1, Z_DEFAULT_COMPRESSION);
    if (ret == Z_OK) {
        // Set input for compression
        if (dest && destLen > 0) {
            deflate_strm1.avail_in = (uInt)destLen;
            deflate_strm1.next_in = dest;
            
            // Allocate output buffer
            uLong comp_bound = deflateBound(&deflate_strm1, destLen);
            Bytef *comp_out = (Bytef *)malloc(comp_bound);
            if (comp_out) {
                deflate_strm1.avail_out = (uInt)comp_bound;
                deflate_strm1.next_out = comp_out;
                
                // Compress
                ret = deflate(&deflate_strm1, Z_FINISH);
                
                // 6. Copy the deflate stream
                ret = deflateCopy(&deflate_strm2, &deflate_strm1);
                if (ret == Z_OK) {
                    // Can use the copied stream for more operations
                    deflateEnd(&deflate_strm2);
                }
                
                free(comp_out);
            }
        }
        
        deflateEnd(&deflate_strm1);
    }
    
    // Clean up
    if (dest) free(dest);
    
    // Always return 0
    assert_file_closed(&tmp_file);
	return 0;
}