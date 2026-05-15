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
//<ID> 130
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 1
//<Quality> {"density":6,"unique_branches":{"deflateBound_z":[[888,24,888,35,0,0,4,0]]},"library_calls":["gzdopen","gztell64","gzrewind","gztell","inflateBackInit_","inflateBackEnd","compressBound","compress","deflateInit_","deflateSetDictionary","deflateBound","deflateEnd","inflateInit_","inflateUndermine","inflateEnd","crc32","crc32","crc32_combine_gen","gzread","gzclose"],"critical_calls":["gzdopen","gztell64","gzrewind","gztell","inflateBackInit_","inflateBackEnd","compressBound","compress","deflateInit_","deflateSetDictionary","deflateBound","deflateEnd","inflateInit_","inflateUndermine","inflateEnd","crc32","crc32","crc32_combine_gen","gzread","gzclose"],"visited":0}
/*To create a fuzz driver that uses the specified zlib APIs to process compressed data, I'll implement a scenario that involves:

1. Decompressing input data using gzFile operations
2. Using inflate back-end for decompression
3. Computing CRC32 checksums
4. Using dictionary operations
5. Measuring file positions

Here's the complete implementation:

*/


extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t* f_data, size_t f_size) {
	if(f_size<42) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimum data for meaningful testing

    // Create input file from data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 1. Use gzdopen to open compressed file
    int fd = fuzz_fileno(in_file);
    gzFile gz_file = gzdopen(fd, fuzz_str_1);
    if (!gz_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 2. Use gzgetc to read first byte
    int first_byte = gzgetc(gz_file);
    (void)first_byte; // Suppress unused warning

    // 3. Get current position using gztell64 and gztell
    off64_t pos64 = gztell64(gz_file);
    (void)pos64; // Suppress unused warning
    
    // Rewind to use gztell (regular version)
    gzrewind(gz_file);
    off_t pos = gztell(gz_file);
    (void)pos; // Suppress unused warning

    // 4. Prepare for inflate back-end
    z_stream strm;
    unsigned char window[32768]; // 32K window
    memset(&strm, 0, sizeof(strm));
    
    // Initialize inflate back-end
    int ret = inflateBackInit_(&strm, 15, window, fuzz_str_2, sizeof(z_stream));
    if (ret == Z_OK) {
        // We would need callback functions for inflateBack, but for fuzzing
        // we'll just test initialization and cleanup
        inflateBackEnd(&strm);
    }

    // 5. Test compress API
    uLongf destLen = compressBound(size);
    Bytef *compressed = (Bytef *)malloc(destLen);
    if (compressed) {
        // Use a subset of input data for compression
        size_t compress_size = size > 100 ? 100 : size;
        int compress_ret = compress(compressed, &destLen, data, compress_size);
        (void)compress_ret; // Suppress unused warning
        free(compressed);
    }

    // 6. Test deflate operations
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    deflateInit(&deflate_strm, Z_DEFAULT_COMPRESSION);
    
    // Use part of input as dictionary
    if (size > 50) {
        uInt dict_len = size > 100 ? 100 : (uInt)size;
        deflateSetDictionary(&deflate_strm, data, dict_len);
    }
    
    // Get deflate bound
    uLong bound = deflateBound(&deflate_strm, size);
    (void)bound; // Suppress unused warning
    
    deflateEnd(&deflate_strm);

    // 7. Test inflate undermine (requires valid inflate stream)
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    inflateInit(&inflate_strm);
    
    // Try undermine (this may fail, but we call it)
    inflateUndermine(&inflate_strm, fuzz_int32_t_3);
    
    inflateEnd(&inflate_strm);

    // 8. Test CRC32 combine generation
    // Generate CRC32 for first half and second half
    if (size > 20) {
        size_t half1 = size / 2;
        size_t half2 = size - half1;
        
        uLong crc1 = crc32(fuzz_uint64_t_4, data, (uInt)half1);
        uLong crc2 = crc32(fuzz_uint64_t_5, data + half1, (uInt)half2);
        
        // Generate combine value
        uLong combine_gen = crc32_combine_gen((off_t)half2);
        (void)combine_gen; // Suppress unused warning
    }

    // 9. Read more data from gz file
    char buffer[1024];
    int bytes_read;
    while ((bytes_read = gzread(gz_file, buffer, sizeof(buffer))) > 0) {
        // Write to output file
        fwrite(buffer, 1, bytes_read, out_file);
    }

    // Cleanup
    gzclose(gz_file);  // This will also close the underlying file descriptor
    // Note: gzdopen duplicates the fd, so we shouldn't close in_file before gzclose
    assert_file_closed(&out_file);;
    
    // in_file was closed by gzclose through the duplicated fd
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}