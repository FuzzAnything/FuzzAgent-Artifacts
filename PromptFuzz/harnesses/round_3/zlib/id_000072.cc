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
//<ID> 180
//<Prompt> []
/*<Combination>: [
*/
//<score> 52, nr_unique_branch: 1
//<Quality> {"density":26,"unique_branches":{"deflateSetDictionary":[[571,9,571,18,0,0,4,0]]},"library_calls":["inflateInit2_","inflateEnd","inflate","inflateGetDictionary","inflateEnd","deflateInit2_","deflateSetHeader","deflateSetDictionary","deflateBound","deflateEnd","deflate","deflateEnd","gzdopen","gzwrite","gzclose_w","crc32","crc32","crc32","crc32","crc32_combine","crc32_combine64","crc32_combine_gen64","compressBound","compress","compressBound","compress2","compressBound_z","compress_z","compressBound_z","compress2_z","gzdopen","gzread","gzclose_r","inflateInit2_","inflateSetDictionary","inflateEnd"],"critical_calls":["inflateInit2_","inflate","inflateGetDictionary","inflateEnd","deflateInit2_","deflateSetHeader","deflateSetDictionary","deflateBound","deflate","deflateEnd","gzdopen","gzwrite","gzclose_w","crc32","crc32","crc32","crc32","crc32_combine","crc32_combine64","crc32_combine_gen64","compressBound","compress","compressBound","compress2","compressBound_z","compress_z","compressBound_z","compress2_z","gzdopen","gzread","gzclose_r","inflateInit2_","inflateSetDictionary","inflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t* f_data, size_t f_size) {
	if(f_size<155) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_12, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_13, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_14, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_15, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_16, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_17, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_18, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_19, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_20, fuzz_str_sz_20, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_21, fdp);
	//fuzzer shim end}




    // Early exit for insufficient data
    if (size < 10) {
	return 0;
	}
	
    
    // Create a FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create a FILE* for output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    
    // Variables for various operations
    int ret;
    uLong crc_val = 0;
    uLong crc_val2 = 0;
    uLong combined_crc;
    uLongf destLen;
    z_size_t destLen_z;
    Bytef *dest = nullptr;
    Bytef *source = nullptr;
    uInt dictLength = 0;
    Bytef dictionary[32768];
    gz_header header;
    memset(&header, 0, sizeof(header));
    
    // 1. Decompress the input zlib data using inflate APIs
    z_stream strm_inflate;
    memset(&strm_inflate, 0, sizeof(strm_inflate));
    
    // Initialize inflate with automatic header detection
    ret = inflateInit2_(&strm_inflate, 15 + 32, fuzz_str_1, sizeof(z_stream));
    if (ret != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Set up input buffer
    strm_inflate.next_in = (Bytef *)data;
    strm_inflate.avail_in = (uInt)size;
    
    // Allocate output buffer for decompressed data
    size_t decompressed_size = size * 4; // Reasonable guess
    Bytef *decompressed = (Bytef *)malloc(decompressed_size);
    if (!decompressed) {
        inflateEnd(&strm_inflate);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    strm_inflate.next_out = decompressed;
    strm_inflate.avail_out = (uInt)decompressed_size;
    
    // Perform decompression
    ret = inflate(&strm_inflate, fuzz_int32_t_2);
    size_t actual_decompressed = decompressed_size - strm_inflate.avail_out;
    
    // Try to get dictionary if available
    inflateGetDictionary(&strm_inflate, dictionary, &dictLength);
    
    // Clean up inflate
    inflateEnd(&strm_inflate);
    
    // 2. Recompress the decompressed data using deflate APIs
    z_stream strm_deflate;
    memset(&strm_deflate, 0, sizeof(strm_deflate));
    
    // Initialize deflate with gzip header (windowBits = 31 for gzip)
    ret = deflateInit2_(&strm_deflate, fuzz_int32_t_4, fuzz_int32_t_5,
                        fuzz_int32_t_6, fuzz_int32_t_7, fuzz_int32_t_8,
                        fuzz_str_3, sizeof(z_stream));
    if (ret != Z_OK) {
        free(decompressed);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Set gzip header
    header.text = 1;
    header.time = 0;
    header.os = 255;
    deflateSetHeader(&strm_deflate, &header);
    
    // Set dictionary if we got one
    if (dictLength > 0) {
        deflateSetDictionary(&strm_deflate, dictionary, dictLength);
    }
    
    // Allocate buffer for recompressed data
    uLong bound_size = deflateBound(&strm_deflate, (uLong)actual_decompressed);
    Bytef *recompressed = (Bytef *)malloc(bound_size);
    if (!recompressed) {
        deflateEnd(&strm_deflate);
        free(decompressed);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    strm_deflate.next_in = decompressed;
    strm_deflate.avail_in = (uInt)actual_decompressed;
    strm_deflate.next_out = recompressed;
    strm_deflate.avail_out = (uInt)bound_size;
    
    // Perform compression
    ret = deflate(&strm_deflate, fuzz_int32_t_9);
    size_t actual_recompressed = bound_size - strm_deflate.avail_out;
    
    // Clean up deflate
    deflateEnd(&strm_deflate);
    
    // 3. Use gzFile APIs to write the recompressed data
    gzFile gz_out = gzdopen(fd_out, "wb");
    if (gz_out) {
        gzwrite(gz_out, recompressed, (unsigned int)actual_recompressed);
        gzclose_w(gz_out);
    } else {
        // If gzdopen fails, we still need to close the file
        assert_file_closed(&out_file);;
    }
    
    // 4. Compute CRC32 checksums
    crc_val = crc32(fuzz_uint64_t_10, Z_NULL, 0);
    crc_val = crc32(fuzz_uint64_t_11, data, (uInt)(size > 1000 ? 1000 : size));
    
    crc_val2 = crc32(fuzz_uint64_t_12, Z_NULL, 0);
    crc_val2 = crc32(fuzz_uint64_t_13, decompressed, 
                     (uInt)(actual_decompressed > 1000 ? 1000 : actual_decompressed));
    
    // Combine CRC32 values using different methods
    combined_crc = crc32_combine(fuzz_uint64_t_14, fuzz_uint64_t_15, 1000);
    uLong combined_crc64 = crc32_combine64(fuzz_uint64_t_16, fuzz_uint64_t_17, 1000);
    uLong op = crc32_combine_gen64(1000);
    (void)combined_crc64; // Mark as used
    (void)op; // Mark as used
    
    // 5. Use various compression APIs on the decompressed data
    // compress
    destLen = compressBound((uLong)actual_decompressed);
    dest = (Bytef *)malloc(destLen);
    if (dest && actual_decompressed > 0) {
        compress(dest, &destLen, decompressed, (uLong)actual_decompressed);
    }
    free(dest);
    dest = nullptr;
    
    // compress2
    destLen = compressBound((uLong)actual_decompressed);
    dest = (Bytef *)malloc(destLen);
    if (dest && actual_decompressed > 0) {
        compress2(dest, &destLen, decompressed, (uLong)actual_decompressed, fuzz_int32_t_18);
    }
    free(dest);
    dest = nullptr;
    
    // compress_z
    destLen_z = compressBound_z((z_size_t)actual_decompressed);
    dest = (Bytef *)malloc(destLen_z);
    if (dest && actual_decompressed > 0) {
        compress_z(dest, &destLen_z, decompressed, (z_size_t)actual_decompressed);
    }
    free(dest);
    dest = nullptr;
    
    // compress2_z
    destLen_z = compressBound_z((z_size_t)actual_decompressed);
    dest = (Bytef *)malloc(destLen_z);
    if (dest && actual_decompressed > 0) {
        compress2_z(dest, &destLen_z, decompressed, 
                   (z_size_t)actual_decompressed, fuzz_int32_t_19);
    }
    free(dest);
    dest = nullptr;
    
    // 6. Use gzFile APIs for reading (though we already consumed the data)
    // We'll create another FILE* to use gzdopen
    FILE *in_file2 = fmemopen((void *)data, size > 100 ? 100 : size, "rb");
    if (in_file2) {
        int fd_in2 = fuzz_fileno(in_file2);
        gzFile gz_in = gzdopen(fd_in2, "rb");
        if (gz_in) {
            char buf[100];
            gzread(gz_in, buf, sizeof(buf));
            gzclose_r(gz_in);
        } else {
            assert_file_closed(&in_file2);;
        }
    }
    
    // 7. Set dictionary for inflate (though we already decompressed)
    // We'll create a new stream just to use inflateSetDictionary
    z_stream strm_test;
    memset(&strm_test, 0, sizeof(strm_test));
    inflateInit2_(&strm_test, fuzz_int32_t_21, fuzz_str_20, sizeof(z_stream));
    if (dictLength > 0) {
        inflateSetDictionary(&strm_test, dictionary, dictLength);
    }
    inflateEnd(&strm_test);
    
    // Clean up resources
    free(decompressed);
    free(recompressed);
    assert_file_closed(&in_file);;
    
    // output_file was already closed by gzclose_w or fclose
    
    assert_file_closed(&in_file2);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}