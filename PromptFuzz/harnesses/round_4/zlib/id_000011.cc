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
//<ID> 25
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["gzdopen","gzdirect","gztell64","gzclose","inflateInit_","gzclose","inflateValidate","inflate","inflateEnd","gzclose","deflateInit_","deflateUsed","deflateEnd","gzclose","gzdopen","gzfwrite","gzflush","gzclose","crc32","crc32","crc32","crc32","crc32_combine64","compressBound_z","compress2_z","inflateEnd"],"critical_calls":["gzdopen","gzdirect","gztell64","inflateInit_","inflateValidate","inflate","deflateInit_","deflateUsed","deflateEnd","gzclose","gzdopen","gzfwrite","gzflush","gzclose","crc32","crc32","crc32","crc32","crc32_combine64","compressBound_z","compress2_z","inflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<54) return 0;

	
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
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
	//fuzzer shim end}




    // Initialize variables
    z_stream strm;
    gzFile file = nullptr;
    FILE *in_file = nullptr;
    FILE *out_file = nullptr;
    Bytef *compressed_data = nullptr;
    Bytef *uncompressed_data = nullptr;
    Bytef *recompressed_data = nullptr;
    int ret;
    int bits_used;
    int is_direct;
    off64_t tell_pos;
    int validate_result;
    z_size_t gzfwrite_result;
    uLong combined_crc;
    z_size_t dest_len;
    z_size_t source_len;
    int compress_level;

    // Check minimum size
    if (size < 64) {
	return 0;
	}
	

    // 1. Create input file from fuzzer data
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // 2. Create output file
    out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
	int fuzz_fd_1 = fuzz_fileno(in_file);


    // 3. Open gzFile from file descriptor
    file = gzdopen(fuzz_fd_1, fuzz_str_1);
    if (!file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_1);
	return 0;
    }

    // 4. Check if file is in direct mode using gzdirect()
    is_direct = gzdirect(file);
    // We'll use this value later

    // 5. Get current position using gztell64()
    tell_pos = gztell64(file);
    // We'll use this value later

    // 6. Allocate buffer for reading compressed data
    uncompressed_data = (Bytef *)malloc(size * 2);
    if (!uncompressed_data) {
        gzclose(file);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_1);
	return 0;
    }

    // 7. Initialize z_stream for inflate
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    
    ret = inflateInit(&strm);
    if (ret != Z_OK) {
        free(uncompressed_data);
        gzclose(file);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_1);
	return 0;
    }

    // 8. Set up inflate stream with fuzzer data
    strm.avail_in = size;
    strm.next_in = (Bytef *)data;
    strm.avail_out = size * 2;
    strm.next_out = uncompressed_data;

    // 9. Perform inflate with validation using inflateValidate()
    // First set validation
    validate_result = inflateValidate(&strm, fuzz_int32_t_2);  // Enable validation
    if (validate_result != Z_OK) {
        // Continue anyway
    }
    
    // Perform inflation
    ret = inflate(&strm, fuzz_int32_t_3);
    if (ret != Z_STREAM_END && ret != Z_OK) {
        inflateEnd(&strm);
        free(uncompressed_data);
        gzclose(file);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_1);
	return 0;
    }

    // 10. Check bits used with deflateUsed()
    // We need a deflate stream for this, so initialize one
    z_stream deflate_strm;
    deflate_strm.zalloc = Z_NULL;
    deflate_strm.zfree = Z_NULL;
    deflate_strm.opaque = Z_NULL;
    
    ret = deflateInit(&deflate_strm, Z_DEFAULT_COMPRESSION);
    if (ret == Z_OK) {
        // Set some data in the stream
        deflate_strm.avail_in = strm.total_out;
        deflate_strm.next_in = uncompressed_data;
        deflate_strm.avail_out = 0;
        deflate_strm.next_out = nullptr;
        
        // Call deflateUsed to get bits used
        bits_used = 0;
        int used_result = deflateUsed(&deflate_strm, &bits_used);
        // Use the result
        (void)used_result;
        
        deflateEnd(&deflate_strm);
    }

    // 11. Write data using gzfwrite()
    // First close and reopen file for writing
    gzclose(file);
    assert_file_closed(&in_file);;
	int fuzz_fd_0 = fuzz_fileno(out_file);

    
    // Open output file as gzFile
    file = gzdopen(fuzz_fd_0, fuzz_str_4);
    if (file) {
        // Write the uncompressed data using gzfwrite
        gzfwrite_result = gzfwrite(uncompressed_data, 1, strm.total_out, file);
        (void)gzfwrite_result;
        
        // Flush and close
        gzflush(file, fuzz_int32_t_5);
        gzclose(file);
    }

    // 12. Combine CRC64 values using crc32_combine64()
    // Calculate two CRC32 values from different parts of the data
    uLong crc1 = crc32(fuzz_uint64_t_6, Z_NULL, 0);
    uLong crc2 = crc32(fuzz_uint64_t_7, Z_NULL, 0);
    
    if (size > 32) {
        size_t half = size / 2;
        crc1 = crc32(fuzz_uint64_t_8, data, half);
        crc2 = crc32(fuzz_uint64_t_9, data + half, size - half);
        
        // Combine them using crc32_combine64
        combined_crc = crc32_combine64(crc1, crc2, (off64_t)half);
        (void)combined_crc;
    }

    // 13. Recompress data using compress2_z()
    if (strm.total_out > 0) {
        dest_len = compressBound_z(strm.total_out);
        recompressed_data = (Bytef *)malloc(dest_len);
        if (recompressed_data) {
            source_len = strm.total_out;
            
            // Use a fuzzer-determined compression level (0-9)
            compress_level = data[0] % 10;
            
            ret = compress2_z(recompressed_data, &dest_len, 
                            uncompressed_data, source_len, compress_level);
            (void)ret;
            
            free(recompressed_data);
        }
    }

    // 14. Clean up inflate stream
    inflateEnd(&strm);

    // 15. Clean up all allocated resources
    free(uncompressed_data);
    assert_file_closed(&out_file);;  // Already closed via gzclose, but safe to call

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	assert_fd_closed(fuzz_fd_1);
	return 0;
}