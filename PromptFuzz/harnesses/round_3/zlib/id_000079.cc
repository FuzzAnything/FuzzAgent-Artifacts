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
//<ID> 204
//<Prompt> []
/*<Combination>: [
*/
//<score> 80, nr_unique_branch: 9
//<Quality> {"density":20,"unique_branches":{"deflate_slow":[[1631,24,1631,44,44,0,4,1]],"gzseek64":[[380,9,380,27,0,0,4,0],[380,31,380,56,0,0,4,0],[380,31,380,56,0,0,4,1],[412,9,412,19,0,0,4,0],[413,13,413,35,0,0,4,1],[416,13,416,23,0,0,4,1],[418,13,418,33,0,0,4,1]],"deflate":[[1256,17,1256,37,0,0,4,0]]},"library_calls":["gzopen","gzopen","gzclose","inflateInit2_","gzclose","gzclose","deflateInit2_","inflateEnd","gzclose","gzclose","deflateTune","gzread","inflateSetDictionary","gzseek64","gzread","inflateSyncPoint","gzwrite","gzflush","gzflush","deflateEnd","inflateEnd","gzclose","gzclose"],"critical_calls":["gzopen","gzopen","inflateInit2_","deflateInit2_","deflateTune","gzread","inflateSetDictionary","gzseek64","gzread","inflateSyncPoint","gzwrite","gzflush","gzflush","deflateEnd","inflateEnd","gzclose","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_79(const uint8_t* f_data, size_t f_size) {
	if(f_size<22) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file.gz", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 10) {
	return 0;
	}
	  // Need minimum data for meaningful operations

    // 1. Create temporary file with compressed data
    FILE *tmp_file = fopen("input_file.gz", "wb");
    if (!tmp_file) {
	assert_file_closed(&tmp_file);
	return 0;
	}
	
    fwrite(data, 1, size, tmp_file);
    assert_file_closed(&tmp_file);;

    // 2. Open the compressed file with gzopen (read mode)
    gzFile gz_in = gzopen("input_file.gz", fuzz_str_1);
    if (!gz_in) {
        remove("input_file.gz");
        assert_file_closed(&tmp_file);
	assert_file_name_closed("input_file.gz");
	assert_file_name_closed("input_file.gz");
	return 0;
    }

    // 3. Create output file for writing
    gzFile gz_out = gzopen("output_file.gz", fuzz_str_2);
    if (!gz_out) {
        gzclose(gz_in);
        remove("input_file.gz");
        assert_file_closed(&tmp_file);
	assert_file_name_closed("input_file.gz");
	assert_file_name_closed("output_file.gz");
	assert_file_name_closed("input_file.gz");
	return 0;
    }

    // 4. Create z_stream for inflation (decompression)
    z_stream strm_inflate;
    memset(&strm_inflate, 0, sizeof(strm_inflate));
    
    // Initialize inflation stream with gzip header detection
    if (inflateInit2(&strm_inflate, 15 + 16) != Z_OK) {
        gzclose(gz_out);
        gzclose(gz_in);
        remove("input_file.gz");
        remove("output_file.gz");
        assert_file_closed(&tmp_file);
	assert_file_name_closed("output_file.gz");
	assert_file_name_closed("input_file.gz");
	assert_file_name_closed("output_file.gz");
	assert_file_name_closed("input_file.gz");
	return 0;
    }

    // 5. Create z_stream for deflation (compression)
    z_stream strm_deflate;
    memset(&strm_deflate, 0, sizeof(strm_deflate));
    
    // Initialize deflation stream
    if (deflateInit2(&strm_deflate, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                     15 + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        inflateEnd(&strm_inflate);
        gzclose(gz_out);
        gzclose(gz_in);
        remove("input_file.gz");
        remove("output_file.gz");
        assert_file_closed(&tmp_file);
	assert_file_name_closed("output_file.gz");
	assert_file_name_closed("input_file.gz");
	assert_file_name_closed("output_file.gz");
	assert_file_name_closed("input_file.gz");
	return 0;
    }

    // 6. Use deflateTune to adjust compression parameters
    // Use some data bytes to parameterize the tuning
    int good_length = data[0] % 64 + 8;      // 8-71
    int max_lazy = data[1] % 128 + 8;        // 8-135
    int nice_length = data[2] % 256 + 8;     // 8-263
    int max_chain = data[3] % 4096 + 8;      // 8-4103
    
    deflateTune(&strm_deflate, good_length, max_lazy, nice_length, max_chain);

    // 7. Read from input gzFile and process
    unsigned char buffer[4096];
    unsigned char dict_buffer[1024];
    unsigned int dict_len = 0;
    
    // Try to read some data to use as dictionary
    int bytes_read = gzread(gz_in, dict_buffer, sizeof(dict_buffer));
    if (bytes_read > 0) {
        dict_len = bytes_read;
        
        // 8. Use inflateSetDictionary with read data
        inflateSetDictionary(&strm_inflate, dict_buffer, dict_len);
        
        // Reset file position to beginning
        gzseek64(gz_in, 0, fuzz_int32_t_3);
    }

    // 9. Process the compressed file
    while (true) {
        int read_result = gzread(gz_in, buffer, sizeof(buffer));
        if (read_result <= 0) break;
        
        // 10. Check sync points during inflation
        inflateSyncPoint(&strm_inflate);
        
        // Write to output file
        gzwrite(gz_out, buffer, read_result);
        
        // 11. Periodically flush output
        if (rand() % 10 == 0) {  // Random flush for testing
            gzflush(gz_out, fuzz_int32_t_4);
        }
    }

    // 12. Final flush
    gzflush(gz_out, fuzz_int32_t_5);

    // 13. Clean up
    deflateEnd(&strm_deflate);
    inflateEnd(&strm_inflate);
    gzclose(gz_out);
    gzclose(gz_in);
    
    // 14. Remove temporary files
    remove("input_file.gz");
    remove("output_file.gz");
    
    assert_file_closed(&tmp_file);
	assert_file_name_closed("output_file.gz");
	assert_file_name_closed("input_file.gz");
	assert_file_name_closed("output_file.gz");
	assert_file_name_closed("input_file.gz");
	return 0;
}