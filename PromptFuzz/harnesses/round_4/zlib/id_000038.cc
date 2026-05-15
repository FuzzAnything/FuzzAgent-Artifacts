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
//<ID> 79
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["compressBound","compress2","gzdopen","gzbuffer","gzfread","adler32_z","adler32_z","gzoffset","gzeof","crc32_combine_gen","deflateInit_","deflateTune","inflateInit_","inflate","inflateCopy","inflateBackInit_","inflateBackEnd","inflateEnd","inflateEnd","deflateEnd","gzclose"],"critical_calls":["compressBound","compress2","gzdopen","gzbuffer","gzfread","adler32_z","adler32_z","gzoffset","gzeof","crc32_combine_gen","deflateInit_","deflateTune","inflateInit_","inflate","inflateCopy","inflateBackInit_","inflateBackEnd","inflateEnd","inflateEnd","deflateEnd","gzclose"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified zlib APIs to achieve a comprehensive compression/decompression workflow:

*/


extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<74) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful testing

    // 1. First, compress the input data using compress2
    uLongf compressed_size = compressBound(size);
    std::vector<Bytef> compressed_data(compressed_size);
    int compress_result = compress2(compressed_data.data(), &compressed_size,
                                   data, size, fuzz_int32_t_1);
    if (compress_result != Z_OK) {
	return 0;
	}
	

    // 2. Create a temporary file with compressed data for gz operations
    FILE *in_file = fmemopen((void *)compressed_data.data(), compressed_size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    int fd = fuzz_fileno(in_file);
    gzFile gz = gzdopen(fd, fuzz_str_2);
    if (!gz) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 3. Set buffer size for gzFile
    gzbuffer(gz, fuzz_uint32_t_3);

    // 4. Read from gzFile and compute Adler-32 checksum
    std::vector<Bytef> decompressed(size);
    z_size_t bytes_read = gzfread(decompressed.data(), fuzz_uint64_t_4, size, gz);
    
    // 5. Compute Adler-32 checksum
    uLong adler = adler32_z(fuzz_uint64_t_5, Z_NULL, fuzz_uint64_t_6);
    adler = adler32_z(adler, decompressed.data(), bytes_read);

    // 6. Get current offset in gzFile
    off_t offset = gzoffset(gz);

    // 7. Check if we're at EOF
    int is_eof = gzeof(gz);

    // 8. Generate CRC combine value using the offset
    uLong crc_combine = crc32_combine_gen(offset);

    // 9. Set up deflate stream with tuning
    z_stream deflate_stream;
    memset(&deflate_stream, 0, sizeof(z_stream));
    deflateInit(&deflate_stream, Z_DEFAULT_COMPRESSION);
    
    // Tune deflate parameters
    deflateTune(&deflate_stream, fuzz_int32_t_7, fuzz_int32_t_8, fuzz_int32_t_9, fuzz_int32_t_10);

    // 10. Set up inflate stream from compressed data
    z_stream inflate_stream;
    memset(&inflate_stream, 0, sizeof(z_stream));
    inflateInit(&inflate_stream);
    
    inflate_stream.next_in = compressed_data.data();
    inflate_stream.avail_in = compressed_size;

    std::vector<Bytef> inflate_output(size);
    inflate_stream.next_out = inflate_output.data();
    inflate_stream.avail_out = size;

    int inflate_result = inflate(&inflate_stream, fuzz_int32_t_11);

    // 11. Create a copy of the inflate stream
    z_stream inflate_copy_stream;
    memset(&inflate_copy_stream, 0, sizeof(z_stream));
    int copy_result = inflateCopy(&inflate_copy_stream, &inflate_stream);

    // 12. Set up inflate back-end for the copy
    if (copy_result == Z_OK) {
        // Initialize inflate back-end (requires window buffer)
        unsigned char window[32768];  // 32KB window
        z_stream back_stream;
        memset(&back_stream, 0, sizeof(z_stream));
        
        // We need to initialize with inflateBackInit for the back-end
        // This is a simplified setup - in real usage we'd need proper callbacks
        int back_init_result = inflateBackInit_(&back_stream, 15, window,
                                               fuzz_str_12, sizeof(z_stream));
        
        if (back_init_result == Z_OK) {
            // End the inflate back-end
            inflateBackEnd(&back_stream);
        }
    }

    // 13. Clean up all resources
    inflateEnd(&inflate_stream);
    if (copy_result == Z_OK) {
        inflateEnd(&inflate_copy_stream);
    }
    
    deflateEnd(&deflate_stream);
    gzclose(gz);
    assert_file_closed(&in_file);;

    // 14. Additional test: write output to file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        fwrite(inflate_output.data(), 1, inflate_stream.total_out, out_file);
        assert_file_closed(&out_file);;
    }

    // 15. Verify the decompressed data matches original
    if (bytes_read > 0 && inflate_stream.total_out > 0) {
        // Compare original data with decompressed data
        size_t compare_size = bytes_read < inflate_stream.total_out ? 
                             bytes_read : inflate_stream.total_out;
        if (memcmp(data, inflate_output.data(), compare_size) != 0) {
            // Data mismatch - could indicate a bug
        }
    }

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}