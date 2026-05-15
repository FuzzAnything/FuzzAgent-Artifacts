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
//<ID> 27
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["gzdopen","gzdirect","gztell64","inflateInit2_","gzclose","inflateEnd","gzclose","inflateValidate","inflate","inflateEnd","gzclose","gzdopen","inflateEnd","gzclose","gzfwrite","compressBound_z","compress2_z","crc32","crc32","crc32","crc32","crc32_combine64","deflateInit_","deflate","deflateUsed","deflateEnd","gzclose","inflateEnd","gzclose"],"critical_calls":["gzdopen","gzdirect","gztell64","inflateInit2_","inflateValidate","inflate","gzdopen","gzfwrite","compressBound_z","compress2_z","crc32","crc32","crc32","crc32","crc32_combine64","deflateInit_","deflate","deflateUsed","deflateEnd","gzclose","inflateEnd","gzclose"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified zlib APIs to achieve processing of compressed data:

*/


extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<42) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
	//fuzzer shim end}




    if (size < 10) {
        return 0; // Too small to be meaningful
    }

    // 1. Create a gzFile from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    int fd = fuzz_fileno(in_file);
    gzFile gz_input = gzdopen(fd, fuzz_str_1);
    if (!gz_input) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 2. Check if gzFile is in direct mode (using gzdirect)
    int is_direct = gzdirect(gz_input);
    
    // 3. Get current position (using gztell64)
    off64_t current_pos = gztell64(gz_input);
    
    // 4. Set up inflation stream to decompress the data
    z_stream inflate_stream;
    memset(&inflate_stream, 0, sizeof(z_stream));
    
    // Initialize inflation with window bits appropriate for zlib format
    int window_bits = is_direct ? 15 + 16 : 15;
    if (inflateInit2(&inflate_stream, window_bits) != Z_OK) {
        gzclose(gz_input);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 5. Set up input and output buffers for inflation
    Bytef *inflate_input = (Bytef *)data;
    uInt inflate_input_size = (uInt)(size > UINT_MAX ? UINT_MAX : size);
    
    // Create output buffer for decompressed data
    size_t decompressed_size = size * 4; // Conservative estimate
    if (decompressed_size > 10 * 1024 * 1024) {
        decompressed_size = 10 * 1024 * 1024; // Cap at 10MB
    }
    Bytef *decompressed = (Bytef *)malloc(decompressed_size);
    if (!decompressed) {
        inflateEnd(&inflate_stream);
        gzclose(gz_input);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    inflate_stream.next_in = inflate_input;
    inflate_stream.avail_in = inflate_input_size;
    inflate_stream.next_out = decompressed;
    inflate_stream.avail_out = (uInt)decompressed_size;
    
    // 6. Enable validation (using inflateValidate)
    inflateValidate(&inflate_stream, fuzz_int32_t_2);
    
    // 7. Perform inflation
    int inflate_result = inflate(&inflate_stream, fuzz_int32_t_3);
    
    // 8. Create a gzFile for writing output
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        free(decompressed);
        inflateEnd(&inflate_stream);
        gzclose(gz_input);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    int out_fd = fuzz_fileno(out_file);
    gzFile gz_output = gzdopen(out_fd, fuzz_str_4);
    if (!gz_output) {
        assert_file_closed(&out_file);;
        free(decompressed);
        inflateEnd(&inflate_stream);
        gzclose(gz_input);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 9. Write decompressed data to gzFile (using gzfwrite)
    size_t decompressed_len = decompressed_size - inflate_stream.avail_out;
    if (decompressed_len > 0) {
        z_size_t items_written = gzfwrite(decompressed, decompressed_len, 1, gz_output);
        (void)items_written; // Suppress unused variable warning
    }
    
    // 10. Recompress the data using compress2_z
    z_size_t compressed_len = compressBound_z(decompressed_len);
    Bytef *recompressed = (Bytef *)malloc(compressed_len);
    if (recompressed && decompressed_len > 0) {
        int compress_result = compress2_z(recompressed, &compressed_len, 
                                         decompressed, decompressed_len, fuzz_int32_t_5);
        (void)compress_result; // Suppress unused variable warning
    }
    
    // 11. Calculate CRC32 checksums and combine them (using crc32_combine64)
    uLong crc1 = crc32(fuzz_uint64_t_6, Z_NULL, 0);
    uLong crc2 = crc32(fuzz_uint64_t_7, Z_NULL, 0);
    
    if (decompressed_len > 0) {
        size_t half_len = decompressed_len / 2;
        if (half_len > 0) {
            crc1 = crc32(crc1, decompressed, (uInt)half_len);
            crc2 = crc32(crc2, decompressed + half_len, (uInt)(decompressed_len - half_len));
            
            // Combine the two CRC32 checksums
            uLong combined_crc = crc32_combine64(crc1, crc2, (off64_t)decompressed_len);
            (void)combined_crc; // Suppress unused variable warning
        }
    }
    
    // 12. Set up deflation stream to use deflateUsed
    z_stream deflate_stream;
    memset(&deflate_stream, 0, sizeof(z_stream));
    
    if (deflateInit(&deflate_stream, Z_DEFAULT_COMPRESSION) == Z_OK) {
        // Feed some data to the deflate stream
        Bytef test_data[] = "test";
        deflate_stream.next_in = test_data;
        deflate_stream.avail_in = sizeof(test_data) - 1;
        
        Bytef deflate_output[100];
        deflate_stream.next_out = deflate_output;
        deflate_stream.avail_out = sizeof(deflate_output);
        
        // Perform deflation
        deflate(&deflate_stream, fuzz_int32_t_8);
        
        // Get bits used (using deflateUsed)
        int bits_used = 0;
        deflateUsed(&deflate_stream, &bits_used);
        (void)bits_used; // Suppress unused variable warning
        
        deflateEnd(&deflate_stream);
    }
    
    // 13. Clean up all resources
    if (recompressed) {
        free(recompressed);
    }
    free(decompressed);
    
    // Close gzFile handles and regular FILE handles
    gzclose(gz_output);
    assert_file_closed(&out_file);;
    
    inflateEnd(&inflate_stream);
    gzclose(gz_input);
    assert_file_closed(&in_file);;
    
    // Remove temporary output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
}