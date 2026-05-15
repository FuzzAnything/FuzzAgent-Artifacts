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
//<ID> 4
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["gzdopen","gzbuffer","gzopen","gzclose","deflateInit_","gzclose","gzclose","crc32","crc32","gzgets","crc32_z","gzfwrite","deflate","deflate","crc32_z","crc32_combine_gen64","crc32_combine64","deflateEnd","gzclose","gzclose"],"critical_calls":["gzdopen","gzbuffer","gzopen","deflateInit_","crc32","crc32","gzgets","crc32_z","gzfwrite","deflate","deflate","crc32_z","crc32_combine_gen64","crc32_combine64","deflateEnd","gzclose","gzclose"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create input file from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptor and open as gzFile
    int fd_in = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd_in, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Set buffer size for gzFile
    gzbuffer(gz_in, 8192);
    
    // Open output file
    gzFile gz_out = gzopen("output_file", "wb");
    if (!gz_out) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Initialize deflate stream
    z_stream def_strm;
    def_strm.zalloc = Z_NULL;
    def_strm.zfree = Z_NULL;
    def_strm.opaque = Z_NULL;
    
    int ret = deflateInit(&def_strm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        gzclose(gz_out);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Initialize CRC values
    uLong crc1 = crc32(0L, Z_NULL, 0);
    uLong crc2 = crc32(0L, Z_NULL, 0);
    
    // Read compressed data line by line and process
    char buffer[1024];
    char read_buf[1024];
    size_t total_read = 0;
    
    while (gzgets(gz_in, read_buf, sizeof(read_buf)) != NULL) {
        size_t line_len = strlen(read_buf);
        total_read += line_len;
        
        // Update CRC32 of the read data
        crc1 = crc32_z(crc1, (const Bytef *)read_buf, line_len);
        
        // Write data to output using gzfwrite
        z_size_t items_written = gzfwrite(read_buf, 1, line_len, gz_out);
        
        // Prepare data for deflate
        def_strm.avail_in = line_len;
        def_strm.next_in = (Bytef *)read_buf;
        
        // Compress data
        char deflate_out[2048];
        def_strm.avail_out = sizeof(deflate_out);
        def_strm.next_out = (Bytef *)deflate_out;
        
        ret = deflate(&def_strm, Z_NO_FLUSH);
        if (ret != Z_OK) {
            break;
        }
    }
    
    // Finish deflate compression
    def_strm.avail_in = 0;
    char deflate_out[2048];
    def_strm.avail_out = sizeof(deflate_out);
    def_strm.next_out = (Bytef *)deflate_out;
    
    ret = deflate(&def_strm, Z_FINISH);
    
    // Calculate CRC32 of original compressed data
    crc2 = crc32_z(crc2, (const Bytef *)data, size);
    
    // Generate combination operator
    off64_t len2 = (off64_t)size;
    uLong comb_op = crc32_combine_gen64(len2);
    
    // Combine CRC values
    uLong combined_crc = crc32_combine64(crc1, crc2, comb_op);
    
    // Clean up resources
    deflateEnd(&def_strm);
    gzclose(gz_out);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    
    // Use the combined CRC to avoid optimization removal
    volatile uLong dummy = combined_crc;
    (void)dummy;
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_in);
	return 0;
}