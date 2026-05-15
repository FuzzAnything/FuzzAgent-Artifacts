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
//<ID> 6
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 1
//<Quality> {"density":6,"unique_branches":{"gz_read":[[323,9,323,17,0,0,4,0]]},"library_calls":["uncompress","gzdopen","gzclose","inflateInit_","gzclose","inflateMark","deflateInit_","deflateCopy","deflateEnd","deflateEnd","adler32","adler32","adler32","adler32","adler32_combine64","inflate","inflateEnd","gzread","gzclose"],"critical_calls":["uncompress","gzdopen","inflateInit_","inflateMark","deflateInit_","deflateCopy","deflateEnd","deflateEnd","adler32","adler32","adler32","adler32","adler32_combine64","inflate","inflateEnd","gzread","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // 1. First, try to uncompress the input (assuming it's zlib-compressed data)
    uLongf uncompressed_size = size * 10; // Initial guess
    Bytef *uncompressed_data = (Bytef *)malloc(uncompressed_size);
    if (!uncompressed_data) {
	return 0;
	}
	
    
    int uncompress_result = uncompress(uncompressed_data, &uncompressed_size, 
                                        data, (uLong)size);
    
    // 2. Create a temporary file with the input data for gzdopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        free(uncompressed_data);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Use gzdopen with the file descriptor
    int fd = fuzz_fileno(in_file);
    gzFile gz_file = gzdopen(fd, "rb");
    if (!gz_file) {
        assert_file_closed(&in_file);;
        free(uncompressed_data);
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 4. Set up z_stream for inflate operations
    z_stream strm;
    Bytef *in_buffer = (Bytef *)malloc(size);
    Bytef *out_buffer = (Bytef *)malloc(size * 2);
    
    if (!in_buffer || !out_buffer) {
        free(in_buffer);
        free(out_buffer);
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        free(uncompressed_data);
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Copy input data to buffer
    memcpy(in_buffer, data, size);
    
    // Initialize z_stream for inflation
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = (uInt)size;
    strm.next_in = in_buffer;
    strm.avail_out = size * 2;
    strm.next_out = out_buffer;
    
    if (inflateInit(&strm) != Z_OK) {
        free(in_buffer);
        free(out_buffer);
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        free(uncompressed_data);
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 5. Use inflateMark to mark current position
    long mark_position = inflateMark(&strm);
    
    // 6. Create another z_stream for deflateCopy
    z_stream dest_strm;
    dest_strm.zalloc = Z_NULL;
    dest_strm.zfree = Z_NULL;
    dest_strm.opaque = Z_NULL;
    
    // 7. Use deflateCopy (requires initialized source stream)
    z_stream source_strm;
    source_strm.zalloc = Z_NULL;
    source_strm.zfree = Z_NULL;
    source_strm.opaque = Z_NULL;
    
    if (deflateInit(&source_strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
        // Set some data in source stream
        source_strm.avail_in = (uInt)(uncompressed_size > 100 ? 100 : uncompressed_size);
        source_strm.next_in = uncompressed_data;
        
        // Try deflateCopy
        int copy_result = deflateCopy(&dest_strm, &source_strm);
        
        // Clean up deflate streams
        deflateEnd(&source_strm);
        if (copy_result == Z_OK) {
            deflateEnd(&dest_strm);
        }
    }
    
    // 8. Calculate Adler-32 checksums and combine them
    uLong adler1 = 0;
    uLong adler2 = 0;
    
    if (uncompressed_size > 0 && uncompress_result == Z_OK) {
        // Calculate first half checksum
        size_t half1_size = uncompressed_size / 2;
        if (half1_size > 0) {
            adler1 = adler32(0L, Z_NULL, 0);
            adler1 = adler32(adler1, uncompressed_data, (uInt)half1_size);
        }
        
        // Calculate second half checksum
        size_t half2_size = uncompressed_size - half1_size;
        if (half2_size > 0) {
            adler2 = adler32(0L, Z_NULL, 0);
            adler2 = adler32(adler2, uncompressed_data + half1_size, (uInt)half2_size);
        }
        
        // Combine the checksums using adler32_combine64
        if (adler1 != 0 && adler2 != 0) {
            uLong combined = adler32_combine64(adler1, adler2, (off64_t)uncompressed_size);
            // Use the combined value to avoid unused variable warning
            (void)combined;
        }
    }
    
    // 9. Try to inflate some data (partial inflation)
    if (mark_position != -1) {
        int inflate_result = inflate(&strm, Z_SYNC_FLUSH);
        // We don't care about the result for fuzzing purposes
        (void)inflate_result;
    }
    
    // 10. Clean up all resources
    inflateEnd(&strm);
    free(in_buffer);
    free(out_buffer);
    
    // Read something from gz_file to ensure it's used
    if (gz_file) {
        char buf[1];
        gzread(gz_file, buf, 0); // Just to touch the API
    }
    
    gzclose(gz_file);
    assert_file_closed(&in_file);;
    free(uncompressed_data);
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}