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
//<ID> 20
//<Prompt> []
/*<Combination>: [
*/
//<score> 37.5, nr_unique_branch: 80
//<Quality> {"density":5,"unique_branches":{"inflate_fast":[[208,30,208,40,0,0,4,0],[211,29,211,37,0,0,4,0],[211,29,211,37,0,0,4,1],[215,38,215,42,0,0,4,0],[215,38,215,42,0,0,4,1],[217,33,217,44,0,0,4,1]],"updatewindow":[[281,13,281,24,0,0,4,1],[291,17,291,45,0,0,4,0]],"deflateResetKeep":[[659,9,659,20,0,0,4,0]],"deflate_slow":[[2052,17,2052,40,0,0,4,0],[1644,8,1644,31,19,0,4,0]],"gztell64":[[450,9,450,21,0,0,4,1],[453,9,453,31,0,0,4,1],[457,28,457,39,0,0,4,0],[457,28,457,39,0,0,4,1]],"gz_write":[[193,9,193,17,0,0,4,1],[197,9,197,25,0,0,4,0],[197,9,197,25,0,0,4,1],[197,29,197,49,0,0,4,1],[201,9,201,20,0,0,4,1],[205,9,205,26,0,0,4,0],[210,17,210,42,0,0,4,0],[210,17,210,42,0,0,4,1],[215,17,215,27,0,0,4,0],[215,17,215,27,0,0,4,1],[222,17,222,25,0,0,4,0],[222,17,222,25,0,0,4,1],[224,17,224,49,0,0,4,1]],"gzgets":[[574,9,574,21,0,0,4,1],[574,25,574,36,0,0,4,1],[574,40,574,47,0,0,4,1],[577,9,577,31,0,0,4,1],[581,9,581,27,0,0,4,0],[581,9,581,27,0,0,4,1],[581,31,581,56,0,0,4,0],[581,31,581,56,0,0,4,1],[581,60,581,73,0,0,4,0],[586,9,586,20,0,0,4,1],[593,9,593,13,0,0,4,0],[595,13,595,31,0,0,4,0],[595,13,595,31,0,0,4,1],[595,35,595,56,0,0,4,0],[595,35,595,56,0,0,4,1],[597,13,597,31,0,0,4,0],[597,13,597,31,0,0,4,1],[603,13,603,33,0,0,4,0],[603,13,603,33,0,0,4,1],[605,13,605,24,0,0,4,0],[605,13,605,24,0,0,4,1],[615,14,615,18,0,0,4,0],[615,14,615,18,0,0,4,1],[615,22,615,33,0,0,4,0],[615,22,615,33,0,0,4,1],[620,9,620,19,0,0,4,0],[620,9,620,19,0,0,4,1]],"gz_fetch":[[259,9,259,18,0,0,4,0],[260,17,261,26,0,0,4,1]],"inflateReset2":[[162,9,162,32,0,0,4,0],[162,36,162,72,0,0,4,1]],"inflate":[[1046,21,1046,40,0,0,4,0],[360,13,360,22,113,0,4,0]],"gz_comp":[[94,9,94,21,0,0,4,0],[97,13,97,32,0,0,4,0],[97,36,97,55,0,0,4,1],[108,13,108,33,0,0,4,0],[108,38,108,57,0,0,4,1],[124,17,124,37,0,0,4,0]],"deflate":[[1003,13,1003,33,0,0,4,0],[1213,32,1213,49,0,0,4,0]],"gzputc":[[314,9,314,21,0,0,4,1],[320,9,320,32,0,0,4,1],[320,37,320,55,0,0,4,1],[325,9,325,20,0,0,4,1],[330,9,330,20,0,0,4,0],[330,9,330,20,0,0,4,1],[331,13,331,32,0,0,4,1],[334,13,334,31,0,0,4,0],[334,13,334,31,0,0,4,1],[344,9,344,37,0,0,4,1]],"gztell":[[465,12,465,31,0,0,4,0]]},"library_calls":["uncompress_z","gzdopen","gzputc","gzflush","gzclose","gzdopen","gzgets","gztell","inflateInit2_","gzclose","inflate","inflateReset2","inflate","inflateEnd","gzclose"],"critical_calls":["uncompress_z","gzdopen","gzputc","gzflush","gzclose","gzdopen","gzgets","gztell","inflateInit2_","inflate","inflateReset2","inflate","inflateEnd","gzclose"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified zlib APIs to achieve the event of reading compressed data, decompressing it, and writing it back:

*/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<22) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create input file from the fuzzer data
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
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Variables for decompression
    Bytef *decompressed_data = nullptr;
    z_size_t decompressed_len = 0;
    int ret = Z_OK;
    
    // First, try to decompress the input data using uncompress_z
    // We'll use a reasonable maximum size for decompressed data
    decompressed_len = size * 10;  // Initial guess
    if (decompressed_len > 1024 * 1024) {  // Cap at 1MB for safety
        decompressed_len = 1024 * 1024;
    }
    
    decompressed_data = (Bytef *)malloc(decompressed_len);
    if (!decompressed_data) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Try to decompress the data
    ret = uncompress_z(decompressed_data, &decompressed_len, data, size);
    
    // If decompression failed, use original data
    if (ret != Z_OK) {
        free(decompressed_data);
        decompressed_data = (Bytef *)malloc(size);
        if (!decompressed_data) {
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
        }
        memcpy(decompressed_data, data, size);
        decompressed_len = size;
    }
    
    // Create a gzip file for writing
    gzFile gz_out = gzdopen(out_fd, fuzz_str_1);
    if (!gz_out) {
        free(decompressed_data);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Write decompressed data to gzip file using gzputc
    for (z_size_t i = 0; i < decompressed_len; i++) {
        if (gzputc(gz_out, decompressed_data[i]) == EOF) {
            break;
        }
    }
    
    // Flush and close the gzip output
    gzflush(gz_out, fuzz_int32_t_2);
    gzclose(gz_out);
    
    // Reopen the output file for reading
    assert_file_closed(&out_file);;
    out_file = fopen("output_file", "rb");
    if (!out_file) {
        free(decompressed_data);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    out_fd = fuzz_fileno(out_file);
    gzFile gz_in = gzdopen(out_fd, fuzz_str_3);
    if (!gz_in) {
        free(decompressed_data);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Read from the gzip file using gzgets
    char buffer[256];
    while (gzgets(gz_in, buffer, sizeof(buffer)) != Z_NULL) {
        // We could process the buffer here if needed
    }
    
    // Get current position using gztell
    off_t position = gztell(gz_in);
    
    // Now test inflateReset2 with the decompressed data
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize inflate with default window bits
    if (inflateInit2(&strm, MAX_WBITS) != Z_OK) {
        free(decompressed_data);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set up input for inflation
    strm.next_in = decompressed_data;
    strm.avail_in = decompressed_len;
    
    // Create output buffer
    Bytef out_buffer[4096];
    strm.next_out = out_buffer;
    strm.avail_out = sizeof(out_buffer);
    
    // Try to inflate a little
    ret = inflate(&strm, fuzz_int32_t_4);
    
    // Reset the inflate stream with different window bits
    if (inflateReset2(&strm, MAX_WBITS + 16) != Z_OK) {
        // Clean up and continue
    }
    
    // Try inflating again
    strm.next_in = decompressed_data;
    strm.avail_in = decompressed_len;
    strm.next_out = out_buffer;
    strm.avail_out = sizeof(out_buffer);
    
    ret = inflate(&strm, fuzz_int32_t_5);
    
    // Clean up inflate stream
    inflateEnd(&strm);
    
    // Clean up resources
    free(decompressed_data);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove the output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}