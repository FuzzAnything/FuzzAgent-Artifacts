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
//<ID> 9
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["gzdopen","gzread","gzclose","uncompress","adler32","adler32","adler32","adler32","adler32_combine64","inflateInit_","inflate","inflateMark","inflateEnd","deflateInit_","deflateCopy","deflateEnd","deflateEnd"],"critical_calls":["gzdopen","gzread","gzclose","uncompress","adler32","adler32","adler32","adler32","adler32_combine64","inflateInit_","inflate","inflateMark","inflateEnd","deflateInit_","deflateCopy","deflateEnd","deflateEnd"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified zlib APIs to achieve a scenario where compressed data is processed through multiple zlib operations:

*/


extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return for insufficient data
    if (size < 100) {
	return 0;
	}
	
    
    // Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Get file descriptor for gzdopen
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Open compressed data stream with gzdopen
    gzFile gz = gzdopen(fd, "rb");
    if (!gz) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Read some compressed data from gz stream
    unsigned char gz_buffer[1024];
    int bytes_read = gzread(gz, gz_buffer, sizeof(gz_buffer));
    
    // Close gz stream (but keep file descriptor open)
    gzclose(gz);
    
    // Reset file position for uncompress
    fseek(in_file, 0, SEEK_SET);
    
    // Allocate buffers for uncompress operation
    const size_t compressed_size = size;
    const size_t uncompressed_size = compressed_size * 4; // Conservative estimate
    Bytef *compressed_data = (Bytef *)malloc(compressed_size);
    Bytef *uncompressed_data = (Bytef *)malloc(uncompressed_size);
    
    if (!compressed_data || !uncompressed_data) {
        free(compressed_data);
        free(uncompressed_data);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Read compressed data from file
    size_t read_size = fread(compressed_data, 1, compressed_size, in_file);
    assert_file_closed(&in_file);;
    
    if (read_size == 0) {
        free(compressed_data);
        free(uncompressed_data);
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Use uncompress API
    uLongf dest_len = uncompressed_size;
    int uncompress_result = uncompress(uncompressed_data, &dest_len, 
                                       compressed_data, (uLong)read_size);
    
    // Only proceed if uncompress was successful
    if (uncompress_result == Z_OK && dest_len > 100) {
        // Split uncompressed data into two parts for adler32_combine64
        size_t split_point = dest_len / 2;
        
        // Calculate Adler-32 checksums for two parts
        uLong adler1 = adler32(0L, Z_NULL, 0);
        uLong adler2 = adler32(0L, Z_NULL, 0);
        
        adler1 = adler32(adler1, uncompressed_data, (uInt)split_point);
        adler2 = adler32(adler2, uncompressed_data + split_point, 
                        (uInt)(dest_len - split_point));
        
        // Use adler32_combine64 to combine checksums
        uLong combined_adler = adler32_combine64(adler1, adler2, (off64_t)split_point);
        
        // Set up z_stream for inflation to use inflateMark
        z_stream strm;
        memset(&strm, 0, sizeof(strm));
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        strm.avail_in = 0;
        strm.next_in = Z_NULL;
        
        int init_result = inflateInit(&strm);
        if (init_result == Z_OK) {
            // Provide some data to the stream
            strm.avail_in = (uInt)(read_size > 100 ? 100 : read_size);
            strm.next_in = compressed_data;
            
            // Try to inflate a bit
            unsigned char out_buffer[200];
            strm.avail_out = sizeof(out_buffer);
            strm.next_out = out_buffer;
            
            inflate(&strm, Z_NO_FLUSH);
            
            // Use inflateMark to get current position
            long mark_position = inflateMark(&strm);
            
            // Clean up inflation stream
            inflateEnd(&strm);
        }
        
        // Set up deflate streams for deflateCopy
        z_stream deflate_strm1, deflate_strm2;
        memset(&deflate_strm1, 0, sizeof(deflate_strm1));
        memset(&deflate_strm2, 0, sizeof(deflate_strm2));
        
        deflate_strm1.zalloc = Z_NULL;
        deflate_strm1.zfree = Z_NULL;
        deflate_strm1.opaque = Z_NULL;
        
        int deflate_init_result = deflateInit(&deflate_strm1, Z_DEFAULT_COMPRESSION);
        if (deflate_init_result == Z_OK) {
            // Use deflateCopy to copy the stream state
            int copy_result = deflateCopy(&deflate_strm2, &deflate_strm1);
            
            // Clean up deflate streams
            deflateEnd(&deflate_strm1);
            deflateEnd(&deflate_strm2);
        }
    }
    
    // Clean up allocated resources
    free(compressed_data);
    free(uncompressed_data);
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}