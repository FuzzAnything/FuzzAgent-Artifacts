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
//<ID> 87
//<Prompt> []
/*<Combination>: [
*/
//<score> 45, nr_unique_branch: 3
//<Quality> {"density":15,"unique_branches":{"fill_window":[[330,21,330,57,0,0,4,0]],"deflate_slow":[[1997,42,1997,67,0,0,4,0],[1644,8,1644,31,27,0,4,0]]},"library_calls":["inflateInit2_","inflateGetHeader","inflateEnd","deflateInit2_","inflateEnd","deflateParams","deflateEnd","inflateEnd","deflateSetHeader","gzdopen","deflateEnd","inflateEnd","crc32","crc32","inflate","crc32","crc32","deflate","gzfwrite","crc32_combine_gen","gztell64","deflate","gzfwrite","gztell64","gzclose","deflateEnd","inflateEnd"],"critical_calls":["inflateInit2_","inflateGetHeader","deflateInit2_","deflateParams","deflateSetHeader","gzdopen","crc32","crc32","inflate","crc32","deflate","gzfwrite","crc32_combine_gen","gztell64","deflate","gzfwrite","gztell64","gzclose","deflateEnd","inflateEnd"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified zlib APIs to achieve a complete decompression/recompression cycle with header manipulation and CRC calculation:

*/


extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* f_data, size_t f_size) {
	if(f_size<41) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
	//fuzzer shim end}




    // Early return if input is too small for meaningful processing
    if (size < 10) {
	return 0;
	}
	
    
    // Create FILE* variables for input and output
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int infd = fuzz_fileno(in_file);
    int outfd = fuzz_fileno(out_file);
    
    // Initialize variables
    z_stream inflate_strm, deflate_strm;
    gz_header header;
    int ret;
    Bytef buffer_in[4096];
    Bytef buffer_out[4096];
    
    // 1. Setup inflation with gz header support
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    memset(&header, 0, sizeof(header));
    
    // Initialize header fields
    header.extra_max = 0;
    header.name_max = 0;
    header.comm_max = 0;
    
    // Initialize inflation stream
    ret = inflateInit2(&inflate_strm, 15 + 32); // 32 enables gzip decoding
    if (ret != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(outfd);
	assert_fd_closed(infd);
	return 0;
    }
    
    // 2. Get header from input data using inflateGetHeader
    ret = inflateGetHeader(&inflate_strm, &header);
    if (ret != Z_OK) {
        inflateEnd(&inflate_strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(outfd);
	assert_fd_closed(infd);
	return 0;
    }
    
    // Setup deflation stream
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    ret = deflateInit2(&deflate_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                       15 + 16, 8, Z_DEFAULT_STRATEGY); // 16 enables gzip encoding
    if (ret != Z_OK) {
        inflateEnd(&inflate_strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(outfd);
	assert_fd_closed(infd);
	return 0;
    }
    
    // 3. Use deflateParams to change compression parameters
    ret = deflateParams(&deflate_strm, fuzz_int32_t_1, fuzz_int32_t_2);
    if (ret != Z_OK) {
        deflateEnd(&deflate_strm);
        inflateEnd(&inflate_strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(outfd);
	assert_fd_closed(infd);
	return 0;
    }
    
    // 4. Set header for deflation if we got one from inflation
    if (header.done) {
        deflateSetHeader(&deflate_strm, &header);
    }
    
    // Open gzFile for writing using file descriptor
    gzFile gz_out = gzdopen(outfd, fuzz_str_3);
    if (!gz_out) {
        deflateEnd(&deflate_strm);
        inflateEnd(&inflate_strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(outfd);
	assert_fd_closed(infd);
	return 0;
    }
    
    // Process data in chunks
    size_t bytes_read;
    uLong crc1 = crc32(fuzz_uint64_t_4, Z_NULL, 0);
    uLong crc2 = crc32(fuzz_uint64_t_5, Z_NULL, 0);
    off_t split_point = size / 2;
    
    // Read and process input data
    while ((bytes_read = fread(buffer_in, 1, sizeof(buffer_in), in_file)) > 0) {
        // Set input for inflation
        inflate_strm.avail_in = bytes_read;
        inflate_strm.next_in = buffer_in;
        
        do {
            // Set output for inflation
            inflate_strm.avail_out = sizeof(buffer_out);
            inflate_strm.next_out = buffer_out;
            
            ret = inflate(&inflate_strm, fuzz_int32_t_6);
            if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR) {
                break;
            }
            
            size_t have = sizeof(buffer_out) - inflate_strm.avail_out;
            
            if (have > 0) {
                // Calculate CRC for first half of data
                if (inflate_strm.total_in <= split_point) {
                    crc1 = crc32(crc1, buffer_out, have);
                } else {
                    crc2 = crc32(crc2, buffer_out, have);
                }
                
                // Set input for deflation
                deflate_strm.avail_in = have;
                deflate_strm.next_in = buffer_out;
                
                do {
                    Bytef deflate_out[4096];
                    deflate_strm.avail_out = sizeof(deflate_out);
                    deflate_strm.next_out = deflate_out;
                    
                    ret = deflate(&deflate_strm, fuzz_int32_t_7);
                    if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR) {
                        break;
                    }
                    
                    size_t deflate_have = sizeof(deflate_out) - deflate_strm.avail_out;
                    
                    // 5. Write compressed data using gzfwrite
                    if (deflate_have > 0) {
                        z_size_t items_written = gzfwrite(deflate_out, deflate_have, 1, gz_out);
                        if (items_written != 1) {
                            // Handle write error
                            break;
                        }
                    }
                } while (deflate_strm.avail_out == 0);
            }
        } while (inflate_strm.avail_out == 0);
        
        if (ret == Z_STREAM_END) break;
    }
    
    // 6. Generate CRC combination value using crc32_combine_gen
    uLong comb_gen = crc32_combine_gen(split_point);
    
    // 7. Get current position in gzFile using gztell64
    off64_t current_pos = gztell64(gz_out);
    
    // Finalize deflation
    deflate_strm.avail_in = 0;
    deflate_strm.next_in = Z_NULL;
    
    do {
        Bytef deflate_out[4096];
        deflate_strm.avail_out = sizeof(deflate_out);
        deflate_strm.next_out = deflate_out;
        
        ret = deflate(&deflate_strm, fuzz_int32_t_8);
        size_t deflate_have = sizeof(deflate_out) - deflate_strm.avail_out;
        
        if (deflate_have > 0) {
            z_size_t items_written = gzfwrite(deflate_out, deflate_have, 1, gz_out);
            (void)items_written; // Suppress unused variable warning
        }
    } while (ret == Z_OK);
    
    // Update position after final writes
    current_pos = gztell64(gz_out);
    
    // Clean up
    gzclose(gz_out);
    deflateEnd(&deflate_strm);
    inflateEnd(&inflate_strm);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    // Use the calculated values to prevent optimization
    (void)comb_gen;
    (void)current_pos;
    (void)crc1;
    (void)crc2;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(outfd);
	assert_fd_closed(infd);
	return 0;
}