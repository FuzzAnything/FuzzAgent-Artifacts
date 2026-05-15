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
//<score> 32, nr_unique_branch: 8
//<Quality> {"density":24,"unique_branches":{"deflate":[[1110,17,1110,32,0,0,4,0],[1188,13,1188,28,0,0,4,0],[1189,17,1189,53,0,0,4,1],[975,13,975,28,64,0,4,0],[975,32,975,50,64,0,4,0]],"inflateGetDictionary":[[1176,9,1176,21,0,0,4,0],[1176,25,1176,45,0,0,4,0]],"deflateBound_z":[[907,17,907,32,0,0,4,0]]},"library_calls":["inflateInit2_","zError","deflateInit2_","zError","inflateEnd","deflateSetHeader","zError","deflateTune","zError","inflate","zError","inflateGetDictionary","zError","deflateBound","deflate","zError","crc32","crc32","crc32","crc32","crc32_combine","crc32_combine64","deflateEnd","inflateEnd","gzdopen","gzwrite","gzclose_w","zError"],"critical_calls":["inflateInit2_","deflateInit2_","deflateSetHeader","zError","deflateTune","zError","inflate","zError","inflateGetDictionary","zError","deflateBound","deflate","zError","crc32","crc32","crc32","crc32","crc32_combine","crc32_combine64","deflateEnd","inflateEnd","gzdopen","gzwrite","gzclose_w","zError"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<47) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_9, fuzz_str_sz_9, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Create a FILE* for reading input data
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

    // Get file descriptors
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);

    // Initialize zlib structures
    z_stream strm_inflate, strm_deflate;
    gz_header header;
    Bytef dictionary[32768];
    uInt dictLength = 0;
    int ret;
    
    // Setup inflation stream
    strm_inflate.zalloc = Z_NULL;
    strm_inflate.zfree = Z_NULL;
    strm_inflate.opaque = Z_NULL;
    strm_inflate.avail_in = 0;
    strm_inflate.next_in = Z_NULL;
    
    ret = inflateInit2(&strm_inflate, 15 + 32); // 15 window bits + 32 for gzip header
    if (ret != Z_OK) {
        const char *err_msg = zError(ret);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Setup deflation stream
    strm_deflate.zalloc = Z_NULL;
    strm_deflate.zfree = Z_NULL;
    strm_deflate.opaque = Z_NULL;
    
    ret = deflateInit2(&strm_deflate, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                       15 + 16, 8, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK) {
        const char *err_msg = zError(ret);
        inflateEnd(&strm_inflate);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Configure gzip header for deflation
    memset(&header, 0, sizeof(header));
    header.text = 1;
    header.time = time(NULL);
    header.os = 3; // Unix
    header.extra = Z_NULL;
    header.name = (Bytef*)"input_file";
    header.comment = Z_NULL;
    header.hcrc = 1;
    
    ret = deflateSetHeader(&strm_deflate, &header);
    if (ret != Z_OK) {
        const char *err_msg = zError(ret);
    }

    // Tune deflation parameters
    ret = deflateTune(&strm_deflate, fuzz_int32_t_1, fuzz_int32_t_2, fuzz_int32_t_3, fuzz_int32_t_4);
    if (ret != Z_OK) {
        const char *err_msg = zError(ret);
    }

    // Read and process input data
    Bytef in_buffer[4096];
    Bytef out_buffer[8192];
    Bytef decompressed[8192];
    
    // First, try to inflate the input data
    strm_inflate.avail_in = fread(in_buffer, 1, sizeof(in_buffer), in_file);
    strm_inflate.next_in = in_buffer;
    
    strm_inflate.avail_out = sizeof(decompressed);
    strm_inflate.next_out = decompressed;
    
    ret = inflate(&strm_inflate, fuzz_int32_t_5);
    if (ret != Z_OK && ret != Z_STREAM_END) {
        const char *err_msg = zError(ret);
    }

    // Try to get dictionary from inflation stream
    ret = inflateGetDictionary(&strm_inflate, dictionary, &dictLength);
    if (ret != Z_OK) {
        const char *err_msg = zError(ret);
    }

    // Calculate deflate bound for the decompressed data
    uLong bound = deflateBound(&strm_deflate, strm_inflate.total_out);
    
    // Setup deflation with decompressed data
    strm_deflate.avail_in = strm_inflate.total_out;
    strm_deflate.next_in = decompressed;
    strm_deflate.avail_out = sizeof(out_buffer);
    strm_deflate.next_out = out_buffer;
    
    // Compress with Z_FINISH to ensure all data is processed
    ret = deflate(&strm_deflate, fuzz_int32_t_6);
    if (ret != Z_STREAM_END && ret != Z_OK) {
        const char *err_msg = zError(ret);
    }

    // Write compressed data to output file
    size_t compressed_size = sizeof(out_buffer) - strm_deflate.avail_out;
    if (compressed_size > 0) {
        fwrite(out_buffer, 1, compressed_size, out_file);
    }

    // Calculate CRC32 values and combine them
    uLong crc1 = crc32(fuzz_uint64_t_7, Z_NULL, 0);
    uLong crc2 = crc32(fuzz_uint64_t_8, Z_NULL, 0);
    
    // Calculate CRC for first half of decompressed data
    if (strm_inflate.total_out > 0) {
        size_t half = strm_inflate.total_out / 2;
        crc1 = crc32(crc1, decompressed, half);
        crc2 = crc32(crc2, decompressed + half, strm_inflate.total_out - half);
        
        // Combine CRCs using 32-bit offset
        uLong combined32 = crc32_combine(crc1, crc2, strm_inflate.total_out - half);
        
        // Combine CRCs using 64-bit offset
        uLong combined64 = crc32_combine64(crc1, crc2, (off64_t)(strm_inflate.total_out - half));
    }

    // Clean up
    deflateEnd(&strm_deflate);
    inflateEnd(&strm_inflate);
    
    // Close gzip file for writing
    gzFile gz_out = gzdopen(fd_out, "wb");
    if (gz_out) {
        // Write some data to ensure file is properly initialized
        const char *test_data = "test";
        gzwrite(gz_out, fuzz_str_9, static_cast<unsigned int>(fuzz_str_sz_9));
        
        // Close with gzclose_w
        ret = gzclose_w(gz_out);
        if (ret != Z_OK) {
            const char *err_msg = zError(ret);
        }
    } else {
        assert_file_closed(&out_file);;
    }
    
    assert_file_closed(&in_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}