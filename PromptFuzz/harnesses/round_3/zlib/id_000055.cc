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
//<ID> 110
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 3
//<Quality> {"density":12,"unique_branches":{"inflate":[[507,17,507,33,0,0,4,0],[1073,17,1073,28,0,0,4,1],[1098,17,1098,28,0,0,4,1]]},"library_calls":["gzdopen","gzclose","gzdopen","gzclose","gzbuffer","gzfread","gzseek64","gzfread","gzseek64","gzeof","inflateInit2_","gzclose","gzclose","inflateEnd","gzclose","gzclose","inflate","inflateSync","adler32","adler32","adler32","adler32","adler32_combine64","gzwrite","gzflush","gzwrite","inflateEnd","gzclose","gzclose"],"critical_calls":["gzdopen","gzdopen","gzbuffer","gzfread","gzseek64","gzfread","gzseek64","gzeof","inflateInit2_","inflate","inflateSync","adler32","adler32","adler32","adler32","adler32_combine64","gzwrite","gzflush","gzwrite","inflateEnd","gzclose","gzclose"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size) {
	if(f_size<86) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
	//fuzzer shim end}




    if (size < 20) {
        // Too small for meaningful operations
        return 0;
    }

    // 1. Create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
	int fuzz_fd_1 = fuzz_fileno(in_file);


    // 2. Open it as a gzFile for reading
    gzFile gz_in = gzdopen(fuzz_fd_1, fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_1);
	return 0;
    }

    // 3. Create an output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_1);
	return 0;
    }
	int fuzz_fd_0 = fuzz_fileno(out_file);


    // 4. Open output as gzFile for writing
    gzFile gz_out = gzdopen(fuzz_fd_0, fuzz_str_2);
    if (!gz_out) {
        assert_file_closed(&out_file);;
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	assert_fd_closed(fuzz_fd_1);
	return 0;
    }

    // 5. Set buffer size for gz_in
    gzbuffer(gz_in, fuzz_uint32_t_3);

    // 6. Read chunks of data using gzfread
    const size_t CHUNK_SIZE = 1024;
    uint8_t buffer[CHUNK_SIZE];
    z_size_t bytes_read = 0;
    
    // First read
    bytes_read = gzfread(buffer, fuzz_uint64_t_4, CHUNK_SIZE, gz_in);
    
    // 7. Seek to different positions using gzseek64
    // Seek to middle of file
    off64_t mid_pos = size / 2;
    gzseek64(gz_in, mid_pos, fuzz_int32_t_5);
    
    // Read from new position
    bytes_read = gzfread(buffer, fuzz_uint64_t_6, CHUNK_SIZE, gz_in);
    
    // 8. Read single characters using gzgetc
    // Seek to beginning
    gzseek64(gz_in, 0, fuzz_int32_t_7);
    int single_char = gzgetc(gz_in);
    
    // Read a few more characters
    for (int i = 0; i < 10 && !gzeof(gz_in); i++) {
        single_char = gzgetc(gz_in);
    }

    // 9. Create z_stream for inflate operations
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize for raw deflate/inflate (negative windowBits)
    if (inflateInit2(&strm, -15) != Z_OK) {
        gzclose(gz_out);
        gzclose(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	assert_fd_closed(fuzz_fd_1);
	return 0;
    }

    // 10. Set up input data for inflate
    // Use part of the input data as compressed data
    size_t comp_size = size > 100 ? 100 : size;
    Bytef *comp_data = (Bytef *)malloc(comp_size);
    if (!comp_data) {
        inflateEnd(&strm);
        gzclose(gz_out);
        gzclose(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	assert_fd_closed(fuzz_fd_1);
	return 0;
    }
    memcpy(comp_data, data, comp_size);
    
    strm.next_in = comp_data;
    strm.avail_in = comp_size;
    
    // 11. Set up output buffer
    Bytef out_buffer[1024];
    strm.next_out = out_buffer;
    strm.avail_out = sizeof(out_buffer);
    
    // 12. Perform inflate
    int inflate_result = inflate(&strm, fuzz_int32_t_8);
    
    // 13. Try inflateSync to attempt to recover if we have partial data
    if (inflate_result == Z_BUF_ERROR || inflate_result == Z_DATA_ERROR) {
        inflateSync(&strm);
    }
    
    // 14. Calculate Adler-32 checksums for combining
    // Calculate checksum for first half
    uLong adler1 = adler32(fuzz_uint64_t_9, Z_NULL, 0);
    size_t half1_size = size > 50 ? 50 : size;
    adler1 = adler32(adler1, data, half1_size);
    
    // Calculate checksum for second half
    uLong adler2 = adler32(fuzz_uint64_t_10, Z_NULL, 0);
    size_t half2_size = size > 100 ? 50 : (size > 50 ? size - 50 : 0);
    const Bytef *half2_start = data + (size > 50 ? 50 : 0);
    adler2 = adler32(adler2, half2_start, half2_size);
    
    // 15. Combine the two checksums
    off64_t len2 = half2_size;
    uLong combined_adler = adler32_combine64(adler1, adler2, len2);
    
    // 16. Write some data to gz_out and flush
    const char *test_data = "Test data for gzflush";
    gzwrite(gz_out, fuzz_str_11, static_cast<unsigned int>(fuzz_str_sz_11));
    
    // Flush the output
    gzflush(gz_out, fuzz_int32_t_12);
    
    // 17. Write the combined checksum value
    char checksum_str[64];
    int len = snprintf(checksum_str, sizeof(checksum_str), 
                      "Combined Adler-32: %lu\n", combined_adler);
    if (len > 0) {
        gzwrite(gz_out, checksum_str, len);
    }
    
    // 18. Clean up
    free(comp_data);
    inflateEnd(&strm);
    gzclose(gz_out);
    gzclose(gz_in);
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    // 19. Remove the output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fuzz_fd_0);
	assert_fd_closed(fuzz_fd_1);
	return 0;
}