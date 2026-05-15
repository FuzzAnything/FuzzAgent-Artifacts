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
//<ID> 30
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 0
//<Quality> {"density":21,"unique_branches":{},"library_calls":["gzdopen","inflateInit2_","zError","gzclose","gzfread","deflateInit2_","zError","inflateEnd","gzclose","deflateTune","deflateSetHeader","deflateBound","deflateEnd","inflateEnd","gzclose","deflate","zError","inflateGetDictionary","inflate","zError","inflateReset2","crc32","crc32","crc32","crc32","crc32_combine","crc32_combine64","gzopen","gzfread","gzclose","deflateEnd","inflateEnd","gzclose"],"critical_calls":["gzdopen","inflateInit2_","gzfread","deflateInit2_","deflateTune","deflateSetHeader","deflateBound","deflate","zError","inflateGetDictionary","inflate","zError","inflateReset2","crc32","crc32","crc32","crc32","crc32_combine","crc32_combine64","gzopen","gzfread","gzclose","deflateEnd","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<120) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_16, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_17, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_18, fuzz_str_sz_18, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_19, fdp);
	//fuzzer shim end}




    // 1. Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Get file descriptor and open as gzFile
    int fd = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd, fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 3. Initialize inflate stream for decompression
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    inflate_strm.zalloc = Z_NULL;
    inflate_strm.zfree = Z_NULL;
    inflate_strm.opaque = Z_NULL;
    
    int ret = inflateInit2_(&inflate_strm, 15 + 16, fuzz_str_2, sizeof(z_stream));
    if (ret != Z_OK) {
        const char *err_msg = zError(ret);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 4. Read decompressed data using gzfread
    const size_t BUF_SIZE = 4096;
    Bytef read_buf[BUF_SIZE];
    z_size_t read_items = gzfread(read_buf, fuzz_uint64_t_3, BUF_SIZE, gz_in);
    
    // 5. Initialize deflate stream for recompression
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    deflate_strm.zalloc = Z_NULL;
    deflate_strm.zfree = Z_NULL;
    deflate_strm.opaque = Z_NULL;
    
    // 6. Set deflate parameters
    ret = deflateInit2_(&deflate_strm, fuzz_int32_t_5, fuzz_int32_t_6,
                        15 + 16, fuzz_int32_t_7, fuzz_int32_t_8,
                        fuzz_str_4, sizeof(z_stream));
    if (ret != Z_OK) {
        const char *err_msg = zError(ret);
        inflateEnd(&inflate_strm);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 7. Tune deflate parameters
    deflateTune(&deflate_strm, fuzz_int32_t_9, fuzz_int32_t_10, fuzz_int32_t_11, fuzz_int32_t_12);
    
    // 8. Set gzip header
    gz_header gzheader;
    memset(&gzheader, 0, sizeof(gzheader));
    gzheader.os = 255; // Unknown OS
    deflateSetHeader(&deflate_strm, &gzheader);
    
    // 9. Calculate bound for compressed data
    uLong compressed_bound = deflateBound(&deflate_strm, read_items);
    Bytef *compressed_buf = (Bytef*)malloc(compressed_bound);
    if (!compressed_buf) {
        deflateEnd(&deflate_strm);
        inflateEnd(&inflate_strm);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 10. Perform deflation (compression)
    deflate_strm.next_in = read_buf;
    deflate_strm.avail_in = read_items;
    deflate_strm.next_out = compressed_buf;
    deflate_strm.avail_out = compressed_bound;
    
    ret = deflate(&deflate_strm, fuzz_int32_t_13);
    if (ret != Z_STREAM_END && ret != Z_OK) {
        const char *err_msg = zError(ret);
    }
    
    // 11. Get dictionary from inflate stream
    Bytef dict_buf[256];
    uInt dict_len = 256;
    inflateGetDictionary(&inflate_strm, dict_buf, &dict_len);
    
    // 12. Perform inflation (decompression) of compressed data
    Bytef decomp_buf[BUF_SIZE];
    inflate_strm.next_in = compressed_buf;
    inflate_strm.avail_in = deflate_strm.total_out;
    inflate_strm.next_out = decomp_buf;
    inflate_strm.avail_out = BUF_SIZE;
    
    ret = inflate(&inflate_strm, fuzz_int32_t_14);
    if (ret != Z_STREAM_END && ret != Z_OK && ret != Z_BUF_ERROR) {
        const char *err_msg = zError(ret);
    }
    
    // 13. Reset inflate stream with different windowBits
    inflateReset2(&inflate_strm, fuzz_int32_t_15);
    
    // 14. Calculate CRC32 values
    uLong crc1 = crc32(fuzz_uint64_t_16, Z_NULL, 0);
    crc1 = crc32(crc1, read_buf, read_items);
    
    uLong crc2 = crc32(fuzz_uint64_t_17, Z_NULL, 0);
    crc2 = crc32(crc2, compressed_buf, deflate_strm.total_out);
    
    // 15. Combine CRC values (using both 32-bit and 64-bit versions)
    uLong combined_crc = crc32_combine(crc1, crc2, read_items);
    uLong combined_crc64 = crc32_combine64(crc1, crc2, read_items);
    
    // 16. Open another gzFile using gzopen
    FILE *tmp_file = fopen("output_file", "wb");
    if (tmp_file) {
        fwrite(compressed_buf, 1, deflate_strm.total_out, tmp_file);
        assert_file_closed(&tmp_file);;
        
        gzFile gz_out = gzopen("output_file", fuzz_str_18);
        if (gz_out) {
            // Read a small amount to verify file
            Bytef verify_buf[16];
            gzfread(verify_buf, fuzz_uint64_t_19, 16, gz_out);
            gzclose(gz_out);
        }
    }
    
    // 17. Clean up
    free(compressed_buf);
    deflateEnd(&deflate_strm);
    inflateEnd(&inflate_strm);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&tmp_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}