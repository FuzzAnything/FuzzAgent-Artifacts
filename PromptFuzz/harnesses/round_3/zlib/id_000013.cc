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
//<ID> 29
//<Prompt> []
/*<Combination>: [
*/
//<score> 53.333332, nr_unique_branch: 9
//<Quality> {"density":32,"unique_branches":{"deflate":[[1145,13,1145,38,0,0,4,1]],"deflateBound_z":[[898,17,898,30,0,0,4,1]],"deflateStateCheck":[[553,44,553,69,0,0,4,0]],"gzfwrite":[[286,9,286,21,0,0,4,1],[291,9,291,32,0,0,4,1],[291,37,291,55,0,0,4,1],[297,9,297,13,0,0,4,0],[297,17,297,37,0,0,4,1],[303,12,303,15,0,0,4,0]]},"library_calls":["gzdopen","inflateInit2_","gzclose","inflateEnd","gzclose","gzfread","deflateBound","inflateEnd","gzclose","inflate","inflateGetDictionary","inflateGetDictionary","zError","deflateInit2_","inflateEnd","gzclose","deflateSetHeader","deflateTune","deflateBound","deflate","crc32","crc32","crc32","crc32","crc32_combine","crc32_combine64","gzopen","gzfwrite","gzclose_w","deflateEnd","inflateEnd","gzclose"],"critical_calls":["gzdopen","inflateInit2_","gzfread","deflateBound","inflate","inflateGetDictionary","inflateGetDictionary","zError","deflateInit2_","deflateSetHeader","deflateTune","deflateBound","deflate","crc32","crc32","crc32","crc32","crc32_combine","crc32_combine64","gzopen","gzfwrite","gzclose_w","deflateEnd","inflateEnd","gzclose"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<74) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_12, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_13, fuzz_str_sz_13, fdp);
	//fuzzer shim end}




    // 1. Create input FILE* from the fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Open gzFile for reading
    int fd_in = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd_in, fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 3. Prepare for decompression
    z_stream strm_in;
    memset(&strm_in, 0, sizeof(z_stream));
    
    // 4. Initialize inflation with automatic header detection
    if (inflateInit2(&strm_in, 15 + 32) != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 5. Read compressed data from gzFile into buffer
    unsigned char *compressed_buf = (unsigned char *)malloc(size);
    if (!compressed_buf) {
        inflateEnd(&strm_in);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    size_t compressed_len = gzfread(compressed_buf, fuzz_uint64_t_2, size, gz_in);
    
    // 6. Set up inflation stream
    strm_in.next_in = compressed_buf;
    strm_in.avail_in = compressed_len;
    
    // 7. Allocate output buffer for decompressed data
    uLong decompressed_bound = deflateBound(&strm_in, size);
    if (decompressed_bound == 0) decompressed_bound = size * 2;
    
    unsigned char *decompressed_buf = (unsigned char *)malloc(decompressed_bound);
    if (!decompressed_buf) {
        free(compressed_buf);
        inflateEnd(&strm_in);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    strm_in.next_out = decompressed_buf;
    strm_in.avail_out = decompressed_bound;
    
    // 8. Perform inflation
    int inflate_result = inflate(&strm_in, fuzz_int32_t_3);
    
    // 9. Get dictionary from inflation stream
    uInt dict_length = 0;
    unsigned char *dictionary = NULL;
    inflateGetDictionary(&strm_in, NULL, &dict_length);
    
    if (dict_length > 0) {
        dictionary = (unsigned char *)malloc(dict_length);
        if (dictionary) {
            inflateGetDictionary(&strm_in, dictionary, &dict_length);
        }
    }
    
    // 10. Get error message if inflation failed
    const char *error_msg = NULL;
    if (inflate_result != Z_STREAM_END && inflate_result != Z_OK) {
        error_msg = zError(inflate_result);
    }
    
    // 11. Prepare for recompression
    z_stream strm_out;
    memset(&strm_out, 0, sizeof(z_stream));
    
    // 12. Initialize deflation with gzip header
    if (deflateInit2(&strm_out, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                    15 + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        if (dictionary) free(dictionary);
        free(decompressed_buf);
        free(compressed_buf);
        inflateEnd(&strm_in);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 13. Set gzip header
    gz_header header;
    memset(&header, 0, sizeof(gz_header));
    header.text = 1;
    header.time = 1234567890;
    header.os = 255;
    deflateSetHeader(&strm_out, &header);
    
    // 14. Tune compression parameters
    deflateTune(&strm_out, fuzz_int32_t_4, fuzz_int32_t_5, fuzz_int32_t_6, fuzz_int32_t_7);
    
    // 15. Calculate bound for recompressed data
    uLong recompressed_bound = deflateBound(&strm_out, strm_in.total_out);
    
    // 16. Recompress the decompressed data
    unsigned char *recompressed_buf = (unsigned char *)malloc(recompressed_bound);
    if (recompressed_buf) {
        strm_out.next_in = decompressed_buf;
        strm_out.avail_in = strm_in.total_out;
        strm_out.next_out = recompressed_buf;
        strm_out.avail_out = recompressed_bound;
        
        deflate(&strm_out, fuzz_int32_t_8);
    }
    
    // 17. Calculate CRC32 values
    uLong crc1 = crc32(fuzz_uint64_t_9, Z_NULL, 0);
    uLong crc2 = crc32(fuzz_uint64_t_10, Z_NULL, 0);
    
    if (decompressed_buf && strm_in.total_out > 0) {
        crc1 = crc32(fuzz_uint64_t_11, decompressed_buf, strm_in.total_out);
    }
    
    if (recompressed_buf && strm_out.total_out > 0) {
        crc2 = crc32(fuzz_uint64_t_12, recompressed_buf, strm_out.total_out);
    }
    
    // 18. Combine CRC32 values using both 32-bit and 64-bit versions
    uLong combined_crc32 = crc32_combine(crc1, crc2, (off_t)strm_out.total_out);
    uLong combined_crc64 = crc32_combine64(crc1, crc2, (off64_t)strm_out.total_out);
    
    // 19. Open gzFile for writing and close it
    gzFile gz_out = gzopen("output_file", fuzz_str_13);
    if (gz_out) {
        if (recompressed_buf && strm_out.total_out > 0) {
            gzfwrite(recompressed_buf, 1, strm_out.total_out, gz_out);
        }
        gzclose_w(gz_out);
    }
    
    // 20. Clean up all resources
    if (dictionary) free(dictionary);
    if (recompressed_buf) free(recompressed_buf);
    free(decompressed_buf);
    free(compressed_buf);
    deflateEnd(&strm_out);
    inflateEnd(&strm_in);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_in);
	return 0;
}