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
//<ID> 26
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["gzdopen","gzdopen","gzclose","gzclose","gzdirect","gztell64","gzfread","gzfwrite","compressBound_z","compress2_z","inflateInit_","inflateValidate","deflateInit_","deflate","deflateUsed","deflateEnd","inflateEnd","crc32","crc32","crc32","crc32_combine64","gzclose","gzclose"],"critical_calls":["gzdopen","gzdopen","gzdirect","gztell64","gzfread","gzfwrite","compressBound_z","compress2_z","inflateInit_","inflateValidate","deflateInit_","deflate","deflateUsed","deflateEnd","inflateEnd","crc32","crc32","crc32","crc32_combine64","gzclose","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<54) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
	//fuzzer shim end}




    if (size < 10) {
        return 0;
    }

    // Open input file from memory
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Open output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    
    // Create gzFile handles
    gzFile gz_in = gzdopen(fd_in, fuzz_str_1);
    gzFile gz_out = gzdopen(fd_out, fuzz_str_2);
    
    if (!gz_in || !gz_out) {
        if (gz_in) gzclose(gz_in);
        if (gz_out) gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Event: Process compressed data through multiple zlib APIs
    // 1. First check if gzFile is in direct mode (uncompressed)
    int is_direct = gzdirect(gz_in);
    
    // 2. Get current position in gzFile
    off64_t current_pos = gztell64(gz_in);
    
    // 3. Read some data from the compressed input
    const size_t buf_size = 1024;
    Bytef read_buf[buf_size];
    z_size_t read_items = gzfread(read_buf, fuzz_uint64_t_3, buf_size, gz_in);
    
    // 4. Write some data to output gzFile
    z_size_t write_items = gzfwrite(read_buf, 1, read_items, gz_out);
    
    // 5. Use compress2_z to compress data
    z_size_t compressed_len = compressBound_z(read_items);
    Bytef *compressed_buf = (Bytef*)malloc(compressed_len);
    if (compressed_buf) {
        int compress_result = compress2_z(compressed_buf, &compressed_len, 
                                         read_buf, read_items, fuzz_int32_t_4);
        
        // 6. Setup inflate stream for validation
        z_stream inflate_strm;
        memset(&inflate_strm, 0, sizeof(z_stream));
        
        if (inflateInit(&inflate_strm) == Z_OK) {
            inflate_strm.next_in = compressed_buf;
            inflate_strm.avail_in = compressed_len;
            
            // 7. Validate inflate stream
            int validate_result = inflateValidate(&inflate_strm, fuzz_int32_t_5);
            
            // 8. Setup deflate stream for deflateUsed
            z_stream deflate_strm;
            memset(&deflate_strm, 0, sizeof(z_stream));
            
            if (deflateInit(&deflate_strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
                deflate_strm.next_in = read_buf;
                deflate_strm.avail_in = read_items;
                
                Bytef deflate_out[1024];
                deflate_strm.next_out = deflate_out;
                deflate_strm.avail_out = sizeof(deflate_out);
                
                // Perform some deflation
                deflate(&deflate_strm, fuzz_int32_t_6);
                
                // 9. Get used bits from deflate stream
                int used_bits = 0;
                int used_result = deflateUsed(&deflate_strm, &used_bits);
                
                deflateEnd(&deflate_strm);
            }
            
            inflateEnd(&inflate_strm);
        }
        
        free(compressed_buf);
    }
    
    // 10. Combine CRC32 values (using crc32_combine64)
    uLong crc1 = crc32(fuzz_uint64_t_7, Z_NULL, 0);
    uLong crc2 = crc32(fuzz_uint64_t_8, read_buf, read_items > 10 ? 10 : read_items);
    if (read_items > 10) {
        uLong crc3 = crc32(fuzz_uint64_t_9, read_buf + 10, read_items - 10);
        uLong combined_crc = crc32_combine64(crc2, crc3, read_items - 10);
    }
    
    // Clean up
    gzclose(gz_in);  // This also closes fd_in
    gzclose(gz_out); // This also closes fd_out
    
    // Note: gzclose already closed the file descriptors,
    // so we don't need to close the FILE* separately
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}