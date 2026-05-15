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
//<ID> 62
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"gzseek64":[[391,19,391,30,0,0,4,0]]},"library_calls":["compressBound","compress2","zlibCompileFlags","gzdopen","gzfwrite","gzseek64","gzrewind","gzclose","gzdopen","inflateInit2_","gzclose_r","inflateReset2","crc32","crc32","crc32","crc32","crc32_combine64","crc32_combine_op","inflateEnd","gzclose_r"],"critical_calls":["compressBound","compress2","zlibCompileFlags","gzdopen","gzfwrite","gzseek64","gzrewind","gzclose","gzdopen","inflateInit2_","inflateReset2","crc32","crc32","crc32","crc32","crc32_combine64","crc32_combine_op","inflateEnd","gzclose_r"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Compress the input data using compress2
    uLongf compressed_size = compressBound(size);
    Bytef *compressed_data = (Bytef *)malloc(compressed_size);
    if (!compressed_data) {
	return 0;
	}
	
    
    int compress_result = compress2(compressed_data, &compressed_size, 
                                    data, size, Z_DEFAULT_COMPRESSION);
    if (compress_result != Z_OK) {
        free(compressed_data);
        return 0;
    }
    
    // 2. Get zlib compilation flags
    uLong compile_flags = zlibCompileFlags();
    
    // 3. Create a temporary file and write compressed data using gzfwrite
    FILE *tmp_file = fopen("output_file", "wb");
    if (!tmp_file) {
        free(compressed_data);
        assert_file_closed(&tmp_file);
	return 0;
    }
    
    int fd = fuzz_fileno(tmp_file);
    gzFile gz_file = gzdopen(fd, "wb");
    if (!gz_file) {
        assert_file_closed(&tmp_file);;
        free(compressed_data);
        assert_file_closed(&tmp_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Write compressed data in chunks
    z_size_t items_written = gzfwrite(compressed_data, sizeof(Bytef), 
                                      compressed_size, gz_file);
    
    // 4. Seek and rewind in the gzip file
    off64_t seek_pos = gzseek64(gz_file, 0, SEEK_CUR);
    gzrewind(gz_file);
    
    // 5. Close the gzip file for reading (we opened for writing, so we'll use gzclose)
    // First close the writing handle
    gzclose(gz_file);  // Note: gzclose handles both reading and writing
    
    // 6. Reopen the same file for reading
    FILE *read_file = fopen("output_file", "rb");
    if (!read_file) {
        assert_file_closed(&tmp_file);;
        free(compressed_data);
        assert_file_closed(&read_file);
	assert_file_closed(&tmp_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    int read_fd = fuzz_fileno(read_file);
    gzFile gz_read_file = gzdopen(read_fd, "rb");
    if (!gz_read_file) {
        assert_file_closed(&read_file);;
        assert_file_closed(&tmp_file);;
        free(compressed_data);
        assert_file_closed(&read_file);
	assert_file_closed(&tmp_file);
	assert_fd_closed(read_fd);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 7. Prepare for inflation by setting up a z_stream
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize inflate with windowBits
    int window_bits = MAX_WBITS;
    if (inflateInit2(&strm, window_bits) != Z_OK) {
        gzclose_r(gz_read_file);
        assert_file_closed(&read_file);;
        assert_file_closed(&tmp_file);;
        free(compressed_data);
        assert_file_closed(&read_file);
	assert_file_closed(&tmp_file);
	assert_fd_closed(read_fd);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 8. Reset the inflate stream with different windowBits
    inflateReset2(&strm, window_bits | 16);  // Add 16 for gzip decoding
    
    // 9. Calculate CRC32 values for combining
    uLong crc1 = crc32(0L, Z_NULL, 0);
    uLong crc2 = crc32(0L, Z_NULL, 0);
    
    if (size > 0) {
        // Calculate CRC for first half of data
        size_t half_size = size / 2;
        crc1 = crc32(0L, data, half_size);
        
        // Calculate CRC for second half of data
        if (size - half_size > 0) {
            crc2 = crc32(0L, data + half_size, size - half_size);
        }
    }
    
    // 10. Combine CRC32 values using both functions
    off64_t len2 = size > 0 ? (off64_t)(size / 2) : 0;
    uLong combined_crc64 = crc32_combine64(crc1, crc2, len2);
    
    // For crc32_combine_op, we need an operation value
    // Use the zlibCompileFlags as the operation value
    uLong combined_crc_op = crc32_combine_op(crc1, crc2, compile_flags);
    
    // 11. Clean up all resources
    inflateEnd(&strm);
    gzclose_r(gz_read_file);  // Close the reading gzFile
    assert_file_closed(&read_file);;
    assert_file_closed(&tmp_file);;
    free(compressed_data);
    
    // Remove temporary file
    remove("output_file");
    
    // Use the combined CRC values to prevent optimization
    (void)combined_crc64;
    (void)combined_crc_op;
    (void)items_written;
    (void)seek_pos;
    
    assert_file_closed(&read_file);
	assert_file_closed(&tmp_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(read_fd);
	assert_fd_closed(fd);
	return 0;
}