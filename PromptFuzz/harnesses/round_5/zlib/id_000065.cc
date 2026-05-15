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
//<ID> 166
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":7,"unique_branches":{"gzbuffer":[[339,9,339,17,0,0,4,0]]},"library_calls":["gzdopen","gzbuffer","gzclose","gzread","gzerror","gzclose","deflateInit_","deflateTune","deflateBound_z","compress","deflatePrime","uncompress_z","deflateEnd"],"critical_calls":["gzdopen","gzbuffer","gzread","gzerror","gzclose","deflateInit_","deflateTune","deflateBound_z","compress","deflatePrime","uncompress_z","deflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Open input file with fmemopen for reading compressed data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Get file descriptor for gzopen
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 3. Open gzip file for reading using the file descriptor
    gzFile gz_in = gzdopen(fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 4. Set buffer size for gzip file
    unsigned int buffer_size = 8192; // Default buffer size
    if (size > 0) {
        buffer_size = (data[0] % 65536) + 1; // Random buffer size based on input
    }
    gzbuffer(gz_in, buffer_size);
    
    // 5. Read uncompressed data from gzip file
    size_t uncompressed_size = size * 10; // Estimate max size
    if (uncompressed_size < 1024) uncompressed_size = 1024;
    
    Bytef *uncompressed_data = (Bytef*)malloc(uncompressed_size);
    if (!uncompressed_data) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Read using gzread (we'll read in chunks)
    size_t total_read = 0;
    int bytes_read;
    while ((bytes_read = gzread(gz_in, uncompressed_data + total_read, 
                                 uncompressed_size - total_read)) > 0) {
        total_read += bytes_read;
        if (total_read >= uncompressed_size) {
            // Resize buffer if needed
            uncompressed_size *= 2;
            Bytef *new_data = (Bytef*)realloc(uncompressed_data, uncompressed_size);
            if (!new_data) break;
            uncompressed_data = new_data;
        }
    }
    
    // 6. Check for gzip errors
    int gz_errnum;
    const char *gz_error = gzerror(gz_in, &gz_errnum);
    
    // 7. Close gzip file and input file
    gzclose(gz_in);
    assert_file_closed(&in_file);; // gzdopen duplicates the fd, so we need to close the original FILE*
    
    // 8. If we read some data, process it further
    if (total_read > 0) {
        // 9. Create z_stream for compression
        z_stream strm;
        memset(&strm, 0, sizeof(strm));
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        
        // 10. Initialize deflate with default compression
        if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
            
            // 11. Tune deflate parameters
            int good_length = (data[0] % 64) + 1;
            int max_lazy = (data[1] % 128) + 1;
            int nice_length = (data[2] % 258) + 1;
            int max_chain = (data[3] % 4096) + 1;
            deflateTune(&strm, good_length, max_lazy, nice_length, max_chain);
            
            // 12. Get bound for compressed size
            z_size_t bound = deflateBound_z(&strm, total_read);
            
            // 13. Allocate destination buffer for compression
            Bytef *compressed_data = (Bytef*)malloc(bound);
            if (compressed_data) {
                // 14. Use compress API (simpler alternative to deflate)
                uLongf compressed_len = bound;
                compress(compressed_data, &compressed_len, uncompressed_data, total_read);
                
                // 15. Prime deflate with some bits
                if (size > 4) {
                    int bits = data[4] % 16;
                    int value = data[5] % 256;
                    deflatePrime(&strm, bits, value);
                }
                
                // 16. Allocate buffer for uncompress_z
                z_size_t uncompressed_len2 = total_read * 2;
                Bytef *uncompressed_data2 = (Bytef*)malloc(uncompressed_len2);
                if (uncompressed_data2) {
                    // 17. Use uncompress_z to decompress
                    z_size_t destLen = uncompressed_len2;
                    uncompress_z(uncompressed_data2, &destLen, compressed_data, compressed_len);
                    
                    free(uncompressed_data2);
                }
                
                free(compressed_data);
            }
            
            // 18. Clean up deflate stream
            deflateEnd(&strm);
        }
    }
    
    // 19. Clean up
    if (uncompressed_data) {
        free(uncompressed_data);
    }
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}