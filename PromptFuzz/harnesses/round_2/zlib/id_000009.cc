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
//<ID> 12
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["get_crc_table","gzdopen","gztell","inflateInit2_","gzclose","compressBound","inflateEnd","gzclose","inflateValidate","inflate","inflateResetKeep","uncompress_z","adler32","adler32","adler32","adler32","adler32_combine","deflateInit_","deflate","deflatePending","deflateEnd","inflateEnd","gzclose"],"critical_calls":["get_crc_table","gzdopen","gztell","inflateInit2_","compressBound","inflateValidate","inflate","inflateResetKeep","uncompress_z","adler32","adler32","adler32","adler32","adler32_combine","deflateInit_","deflate","deflatePending","deflateEnd","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // Step 1: Get CRC table (API: get_crc_table)
    const z_crc_t *crc_table = get_crc_table();
    
    // Step 2: Create a FILE* for compressed data (as per instruction 4)
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Step 3: Open compressed data as gzFile
    int fd = fuzz_fileno(in_file); // As per instruction 5
    gzFile gz = gzdopen(fd, "rb");
    if (!gz) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Step 4: Get current position (API: gztell)
    off_t pos = gztell(gz);
    
    // Step 5: Prepare for decompression using inflate
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    // Initialize inflate for raw deflate stream (negative windowBits)
    if (inflateInit2(&strm, -MAX_WBITS) != Z_OK) {
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Step 6: Set input data for decompression
    Bytef *compressed = (Bytef *)data;
    strm.avail_in = size;
    strm.next_in = compressed;
    
    // Step 7: Use compressBound to estimate decompressed size (API: compressBound)
    uLong estimated_size = compressBound(size);
    if (estimated_size < size) estimated_size = size * 2;
    
    // Allocate buffer for decompressed data
    Bytef *decompressed = (Bytef *)malloc(estimated_size);
    if (!decompressed) {
        inflateEnd(&strm);
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    strm.avail_out = estimated_size;
    strm.next_out = decompressed;
    
    // Step 8: Enable validation (API: inflateValidate)
    inflateValidate(&strm, 1);
    
    // Step 9: Perform decompression using inflate
    int ret = inflate(&strm, Z_FINISH);
    
    // Step 10: Use inflateResetKeep to preserve dictionary (API: inflateResetKeep)
    if (ret == Z_STREAM_END) {
        inflateResetKeep(&strm);
    }
    
    // Step 11: Try uncompress_z API as alternative decompression
    // (API: uncompress_z - note we're using the same data buffer)
    z_size_t dest_len = estimated_size;
    Bytef *dest = (Bytef *)malloc(dest_len);
    if (dest) {
        int uncompress_ret = uncompress_z(dest, &dest_len, 
                                         (const Bytef *)data, 
                                         size > 1024 ? 1024 : size);
        // Use the result if successful
        if (uncompress_ret == Z_OK) {
            // Calculate Adler-32 checksums for combination
            uLong adler1 = adler32(0L, Z_NULL, 0);
            uLong adler2 = adler32(0L, Z_NULL, 0);
            
            if (dest_len > 0) {
                size_t half = dest_len / 2;
                if (half > 0) {
                    adler1 = adler32(adler1, dest, half);
                }
                if (dest_len - half > 0) {
                    adler2 = adler32(adler2, dest + half, dest_len - half);
                }
                
                // Step 12: Combine Adler-32 checksums (API: adler32_combine)
                if (pos > 0) {
                    uLong combined = adler32_combine(adler1, adler2, pos);
                }
            }
        }
        free(dest);
    }
    
    // Step 13: Create a separate deflate stream to use deflatePending
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    def_strm.zalloc = Z_NULL;
    def_strm.zfree = Z_NULL;
    def_strm.opaque = Z_NULL;
    
    if (deflateInit(&def_strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
        // Compress a small test string
        const char *test = "test";
        def_strm.avail_in = strlen(test);
        def_strm.next_in = (Bytef *)test;
        
        Bytef out[100];
        def_strm.avail_out = sizeof(out);
        def_strm.next_out = out;
        
        deflate(&def_strm, Z_FINISH);
        
        // Step 14: Check pending data (API: deflatePending)
        unsigned int pending;
        int bits;
        deflatePending(&def_strm, &pending, &bits);
        
        deflateEnd(&def_strm);
    }
    
    // Step 15: Clean up inflate stream (API: inflateEnd)
    inflateEnd(&strm);
    
    // Step 16: Close gzFile
    gzclose(gz);
    
    // Step 17: Close FILE
    assert_file_closed(&in_file);;
    
    // Step 18: Free allocated memory
    free(decompressed);
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}