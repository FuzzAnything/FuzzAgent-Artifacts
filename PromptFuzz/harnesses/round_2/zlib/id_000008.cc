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
//<ID> 11
//<Prompt> []
/*<Combination>: [
*/
//<score> 4, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["get_crc_table","uncompress_z","uncompress_z","adler32","adler32","adler32","adler32","adler32_combine","gzdopen","gzread","gztell","gzclose","inflateInit2_","inflateValidate","inflateResetKeep","inflateEnd","compressBound","deflateInit_","deflate","deflatePending","deflateEnd","inflateInit_","inflate","inflateEnd"],"critical_calls":["get_crc_table","uncompress_z","uncompress_z","adler32","adler32","adler32","adler32","adler32_combine","gzdopen","gzread","gztell","gzclose","inflateInit2_","inflateValidate","inflateResetKeep","inflateEnd","compressBound","deflateInit_","deflate","deflatePending","deflateEnd","inflateInit_","inflate","inflateEnd"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if input is too small to be meaningful
    if (size < 2) {
        return 0;
    }

    // Step 1: Initialize variables for uncompressed data
    z_size_t destLen = 0;
    const Bytef* source = reinterpret_cast<const Bytef*>(data);
    z_size_t sourceLen = size;
    
    // Get CRC table (API 1)
    const z_crc_t* crc_table = get_crc_table();
    if (!crc_table) {
        // Not fatal, but we should note it
    }
    
    // Step 2: First pass with uncompress_z to get uncompressed size
    // We'll use a two-pass approach: first get size, then allocate
    int uncompress_result = uncompress_z(nullptr, &destLen, source, sourceLen);
    
    if (uncompress_result == Z_OK && destLen > 0) {
        // Step 3: Allocate buffer for uncompressed data
        Bytef* dest = static_cast<Bytef*>(malloc(destLen));
        if (!dest) {
            return 0;
        }
        
        // Step 4: Actually uncompress the data
        z_size_t actualDestLen = destLen;
        uncompress_result = uncompress_z(dest, &actualDestLen, source, sourceLen);
        
        if (uncompress_result == Z_OK) {
            // Step 5: Calculate Adler-32 checksums for different segments
            if (actualDestLen > 0) {
                // Split data into two parts for adler32_combine
                size_t split_point = actualDestLen / 2;
                uLong adler1 = adler32(0L, Z_NULL, 0);
                adler1 = adler32(adler1, dest, static_cast<uInt>(split_point));
                
                uLong adler2 = adler32(0L, Z_NULL, 0);
                adler2 = adler32(adler2, dest + split_point, 
                               static_cast<uInt>(actualDestLen - split_point));
                
                // Combine the two Adler-32 checksums (API 2)
                uLong combined = adler32_combine(adler1, adler2, 
                                               static_cast<off_t>(actualDestLen - split_point));
                (void)combined;  // Use result to avoid unused variable warning
            }
            
            // Step 6: Create a gzFile to read the compressed data again
            FILE* in_file = fmemopen((void*)data, size, "rb");
            if (in_file) {
                int fd = fuzz_fileno(in_file);
                gzFile gzfile = gzdopen(fd, "rb");
                if (gzfile) {
                    // Read some data to advance position
                    char buffer[256];
                    int bytes_read = gzread(gzfile, buffer, sizeof(buffer));
                    
                    // Get current position (API 3)
                    off_t position = gztell(gzfile);
                    (void)position;  // Use result
                    
                    // Close gzFile
                    gzclose(gzfile);
                }
                assert_file_closed(&in_file);;
            }
            
            // Step 7: Create inflate stream for validation
            z_stream strm;
            strm.zalloc = Z_NULL;
            strm.zfree = Z_NULL;
            strm.opaque = Z_NULL;
            strm.next_in = const_cast<Bytef*>(source);
            strm.avail_in = static_cast<uInt>(sourceLen);
            
            // Initialize inflate for gzip decoding
            int init_result = inflateInit2_(&strm, MAX_WBITS + 16, 
                                          ZLIB_VERSION, sizeof(z_stream));
            
            if (init_result == Z_OK) {
                // Validate the inflate stream (API 4)
                int validate_result = inflateValidate(&strm, 1);
                (void)validate_result;
                
                // Reset the stream while keeping internal state (API 5)
                int reset_result = inflateResetKeep(&strm);
                (void)reset_result;
                
                // End the inflate stream (API 6)
                inflateEnd(&strm);
            }
            
            // Step 8: Create deflate stream to test deflatePending
            z_stream deflate_strm;
            deflate_strm.zalloc = Z_NULL;
            deflate_strm.zfree = Z_NULL;
            deflate_strm.opaque = Z_NULL;
            deflate_strm.next_in = dest;
            deflate_strm.avail_in = static_cast<uInt>(actualDestLen);
            
            // Calculate compressed bound (API 7)
            uLong bound = compressBound(static_cast<uLong>(actualDestLen));
            (void)bound;
            
            // Initialize deflate
            if (deflateInit(&deflate_strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
                // Set up output buffer
                Bytef out_buffer[1024];
                deflate_strm.next_out = out_buffer;
                deflate_strm.avail_out = sizeof(out_buffer);
                
                // Compress a bit
                deflate(&deflate_strm, Z_NO_FLUSH);
                
                // Check for pending data (API 8)
                unsigned int pending = 0;
                int bits = 0;
                int pending_result = deflatePending(&deflate_strm, &pending, &bits);
                (void)pending_result;
                
                // Clean up deflate
                deflateEnd(&deflate_strm);
            }
        }
        
        // Clean up
        free(dest);
    }
    
    // Step 9: Additional inflate stream for more testing
    z_stream strm2;
    strm2.zalloc = Z_NULL;
    strm2.zfree = Z_NULL;
    strm2.opaque = Z_NULL;
    strm2.next_in = const_cast<Bytef*>(source);
    strm2.avail_in = static_cast<uInt>(sourceLen > 100 ? 100 : sourceLen);
    
    if (inflateInit(&strm2) == Z_OK) {
        Bytef out_buffer[256];
        strm2.next_out = out_buffer;
        strm2.avail_out = sizeof(out_buffer);
        
        // Inflate a small amount
        inflate(&strm2, Z_NO_FLUSH);
        
        // End this stream too
        inflateEnd(&strm2);
    }
    
    return 0;
}