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
//<ID> 121
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 1
//<Quality> {"density":6,"unique_branches":{"gzvprintf":[[423,9,423,32,0,0,4,0]]},"library_calls":["gzopen","gzerror","gzoffset","gzoffset64","gzprintf","compressBound_z","uncompress","inflateInit_","inflatePrime","inflateCopy","inflate","inflateEnd","inflateEnd","gzclose"],"critical_calls":["gzopen","gzerror","gzoffset","gzoffset64","gzprintf","compressBound_z","uncompress","inflateInit_","inflatePrime","inflateCopy","inflate","inflateEnd","inflateEnd","gzclose"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t* f_data, size_t f_size) {
	if(f_size<17) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // 1. Create a temporary file with the input data for gzopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Open the file with gzopen (simulating reading zlib compressed data)
    gzFile gz_in = gzopen("input_file", fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // 3. Check for errors using gzerror
    int errnum = 0;
    const char *error_msg = gzerror(gz_in, &errnum);
    (void)error_msg; // Suppress unused variable warning

    // 4. Get file offsets using both gzoffset and gzoffset64
    off_t offset32 = gzoffset(gz_in);
    off64_t offset64 = gzoffset64(gz_in);
    (void)offset32;
    (void)offset64;

    // 5. Write to the gz file using gzprintf
    gzprintf(gz_in, "Test format string: %d", 42);

    // 6. Prepare for decompression using uncompress
    // First, calculate maximum possible decompressed size using compressBound_z
    z_size_t max_decompressed = compressBound_z(size);
    Bytef *decompressed = nullptr;
    uLongf destLen = 0;
    
    if (max_decompressed > 0 && max_decompressed < 10 * 1024 * 1024) { // Limit to 10MB
        decompressed = (Bytef *)malloc(max_decompressed);
        if (decompressed) {
            destLen = max_decompressed;
            
            // Try to uncompress the input data
            int uncompress_result = uncompress(decompressed, &destLen, data, size);
            (void)uncompress_result;
        }
    }

    // 7. Set up z_stream for inflate operations
    z_stream strm1, strm2;
    memset(&strm1, 0, sizeof(z_stream));
    memset(&strm2, 0, sizeof(z_stream));
    
    // Initialize inflate stream
    if (inflateInit(&strm1) == Z_OK) {
        // 8. Use inflatePrime to insert bits
        inflatePrime(&strm1, fuzz_int32_t_2, fuzz_int32_t_3); // Insert 5 bits with value 31
        
        // 9. Copy the inflate stream using inflateCopy
        if (inflateCopy(&strm2, &strm1) == Z_OK) {
            // Set up input/output buffers for inflation
            Bytef *inflate_input = (Bytef *)malloc(size);
            Bytef *inflate_output = (Bytef *)malloc(size * 2);
            
            if (inflate_input && inflate_output) {
                memcpy(inflate_input, data, size);
                
                strm2.next_in = inflate_input;
                strm2.avail_in = size;
                strm2.next_out = inflate_output;
                strm2.avail_out = size * 2;
                
                // Perform inflation
                inflate(&strm2, fuzz_int32_t_4);
                
                free(inflate_input);
                free(inflate_output);
            }
            
            // Clean up copied stream
            inflateEnd(&strm2);
        }
        
        // Clean up original stream
        inflateEnd(&strm1);
    }

    // 10. Clean up resources
    if (decompressed) {
        free(decompressed);
    }
    
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}