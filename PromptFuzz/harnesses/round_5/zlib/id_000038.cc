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
//<ID> 52
//<Prompt> []
/*<Combination>: [
*/
//<score> 4, nr_unique_branch: 0
//<Quality> {"density":4,"unique_branches":{},"library_calls":["get_crc_table","gzdopen","gzread","gzerror","gzclose","compressBound","compress","deflateInit_","deflateReset","deflateEnd","uncompress2_z"],"critical_calls":["get_crc_table","gzdopen","gzread","gzerror","gzclose","compressBound","compress","deflateInit_","deflateReset","deflateEnd","uncompress2_z"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. First, get CRC table (API 6)
    const z_crc_t *crc_table = get_crc_table();
    (void)crc_table; // Use variable to avoid unused warning

    // 2. Create a FILE* for input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Get file descriptor for gzopen
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 4. Open gzFile for reading compressed data
    gzFile gz_file = gzdopen(fd, "rb");
    if (!gz_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 5. Read some data from gzFile to trigger potential errors
    char buffer[1024];
    int bytes_read = gzread(gz_file, buffer, sizeof(buffer));
    
    // 6. Check for gzFile errors (API 5)
    int gz_errnum = 0;
    const char *gz_error_msg = gzerror(gz_file, &gz_errnum);
    (void)gz_error_msg; // Use variable to avoid unused warning

    // 7. Close gzFile
    gzclose(gz_file);
    // Note: gzclose also closes the underlying fd, so we don't need to close in_file separately

    // 8. Now process the original input data with compress/uncompress2_z
    if (size == 0) {
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 9. First compress the input data (API 4)
    uLongf compressed_size = compressBound(size);
    if (compressed_size == 0) {
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    Bytef *compressed_data = (Bytef *)malloc(compressed_size);
    if (!compressed_data) {
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    int compress_result = compress(compressed_data, &compressed_size, 
                                   (const Bytef *)data, (uLong)size);
    
    // 10. Initialize z_stream for deflate operations (API 2)
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    int init_result = deflateInit_(&strm, Z_DEFAULT_COMPRESSION, 
                                   ZLIB_VERSION, sizeof(z_stream));
    
    // 11. Reset the deflate stream (API 1)
    if (init_result == Z_OK) {
        int reset_result = deflateReset(&strm);
        (void)reset_result; // Use variable to avoid unused warning
        
        // Clean up deflate stream
        deflateEnd(&strm);
    }

    // 12. Now uncompress the compressed data using uncompress2_z (API 3)
    if (compress_result == Z_OK && compressed_size > 0) {
        z_size_t uncompressed_size = size * 2; // Initial guess
        Bytef *uncompressed_data = (Bytef *)malloc(uncompressed_size);
        if (uncompressed_data) {
            z_size_t source_len = compressed_size;
            int uncompress_result = uncompress2_z(uncompressed_data, &uncompressed_size,
                                                  compressed_data, &source_len);
            (void)uncompress_result; // Use variable to avoid unused warning
            
            free(uncompressed_data);
        }
    }

    // 13. Clean up all allocated resources
    free(compressed_data);
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}