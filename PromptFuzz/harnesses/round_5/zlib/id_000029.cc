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
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":5,"unique_branches":{},"library_calls":["uncompress2","adler32","adler32","gzdopen","gzwrite","gzclose_w","inflateInit2_","inflateGetDictionary","inflate","inflateSyncPoint","inflateEnd"],"critical_calls":["uncompress2","adler32","gzdopen","gzwrite","gzclose_w","inflateInit2_","inflateGetDictionary","inflate","inflateSyncPoint","inflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. First, try to uncompress the input data (assuming it's zlib compressed data)
    // We'll need to allocate buffers for uncompressed data
    uLongf destLen = size * 10;  // Initial guess - compressed data should be smaller
    if (destLen < 100) destLen = 100;  // Minimum buffer size
    Bytef* dest = (Bytef*)malloc(destLen);
    if (!dest) {
	return 0;
	}
	
    
    uLong sourceLen = size;
    int uncompress_result = uncompress2(dest, &destLen, data, &sourceLen);
    
    // 2. Compute Adler-32 checksum of the uncompressed data (or original data if decompression failed)
    uLong adler = 0;
    if (uncompress_result == Z_OK) {
        // Use the uncompressed data
        adler = adler32(0L, dest, destLen);
    } else {
        // Use the original compressed data
        adler = adler32(0L, data, size);
    }
    
    // 3. Create a FILE* for reading the input data
    FILE* in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        free(dest);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Get file descriptor and create a gzFile for writing
    int fd = fuzz_fileno(in_file);
    gzFile gz_out = gzdopen(fd, "wb");
    if (!gz_out) {
        assert_file_closed(&in_file);;
        free(dest);
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 5. Write some data to the gzFile
    const char* test_data = "test data for gzFile";
    gzwrite(gz_out, test_data, strlen(test_data));
    
    // 6. Close the gzFile for writing (using gzclose_w)
    int close_result = gzclose_w(gz_out);
    
    // Note: assert_file_closed(&in_file); is not needed here because gzdopen took ownership of the fd
    // and gzclose_w should close it
    
    // 7. Set up z_stream for inflation to use inflateSyncPoint and inflateGetDictionary
    z_stream strm;
    Bytef* dictionary = (Bytef*)malloc(32768);  // Max dictionary size
    uInt dictLength = 0;
    
    if (dictionary && size > 0) {
        // Initialize the stream for inflation
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        strm.avail_in = size;
        strm.next_in = (Bytef*)data;
        
        // Use inflateInit2 with windowBits to handle both zlib and gzip formats
        int init_result = inflateInit2(&strm, 15 + 32);  // 15 window bits + 32 for gzip auto detection
        
        if (init_result == Z_OK) {
            // Try to get the dictionary (if any)
            inflateGetDictionary(&strm, dictionary, &dictLength);
            
            // Check for sync points during inflation
            // We'll inflate a small amount to reach a sync point
            Bytef out_buffer[1024];
            strm.avail_out = sizeof(out_buffer);
            strm.next_out = out_buffer;
            
            int inflate_result = inflate(&strm, Z_SYNC_FLUSH);
            
            // Check for sync point
            int sync_point = inflateSyncPoint(&strm);
            
            // Clean up inflation
            inflateEnd(&strm);
        }
    }
    
    // 8. Clean up all allocated resources
    free(dest);
    free(dictionary);
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}