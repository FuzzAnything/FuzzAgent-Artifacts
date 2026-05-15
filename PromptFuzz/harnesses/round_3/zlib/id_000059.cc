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
//<ID> 123
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 3
//<Quality> {"density":16,"unique_branches":{"gzoffset64":[[477,9,477,31,0,0,4,0],[477,35,477,58,0,0,4,1],[484,9,484,31,0,0,4,1]]},"library_calls":["gzopen","gzerror","compressBound_z","uncompress","inflateInit_","inflatePrime","inflateCopy","inflate","inflateEnd","inflateEnd","gzopen","gzprintf","gzprintf","gzprintf","gzprintf","gzoffset","gzoffset64","gzprintf","gzprintf","gzwrite","gzclose","gzclose"],"critical_calls":["gzopen","gzerror","compressBound_z","uncompress","inflateInit_","inflatePrime","inflateCopy","inflate","inflateEnd","inflateEnd","gzopen","gzprintf","gzprintf","gzprintf","gzprintf","gzoffset","gzoffset64","gzprintf","gzprintf","gzwrite","gzclose","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t* f_data, size_t f_size) {
	if(f_size<22) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Event: Decompress zlib data from a file while manipulating the inflate stream,
    // then write decompressed info to another file using gzip APIs
    
    if (size < 10) {
	return 0;
	}
	  // Need enough data for meaningful operations
    
    // 1. Write input data to a temporary file (simulating zlib output file)
    FILE* tmp_in = fopen("input_file", "wb");
    if (!tmp_in) {
	assert_file_closed(&tmp_in);
	return 0;
	}
	
    fwrite(data, 1, size, tmp_in);
    assert_file_closed(&tmp_in);;
    
    // 2. Open the file with gzopen for reading compressed data
    gzFile gz_in = gzopen("input_file", fuzz_str_1);
    if (!gz_in) {
        remove("input_file");
        assert_file_closed(&tmp_in);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 3. Get error status using gzerror
    int errnum = 0;
    const char* err_msg = gzerror(gz_in, &errnum);
    
    // 4. Use compressBound_z to estimate maximum compressed size
    z_size_t bound = compressBound_z(size);
    
    // 5. Prepare for uncompress operation
    Bytef* uncompressed_buf = nullptr;
    uLongf uncompressed_len = bound * 2;  // Conservative estimate
    if (uncompressed_len > 0) {
        uncompressed_buf = (Bytef*)malloc(uncompressed_len);
    }
    
    // 6. Also try to uncompress the original data directly
    int uncompress_result = Z_DATA_ERROR;
    if (uncompressed_buf) {
        uncompress_result = uncompress(uncompressed_buf, &uncompressed_len, 
                                      data, (uLong)size);
    }
    
    // 7. Create and manipulate inflate streams
    z_stream strm1, strm2;
    memset(&strm1, 0, sizeof(z_stream));
    memset(&strm2, 0, sizeof(z_stream));
    
    // Initialize inflate stream
    if (inflateInit(&strm1) == Z_OK) {
        // Set up input buffer
        strm1.next_in = (Bytef*)data;
        strm1.avail_in = (uInt)(size > 100 ? 100 : size);
        
        // 8. Use inflatePrime to insert bits into the stream
        inflatePrime(&strm1, fuzz_int32_t_2, fuzz_int32_t_3);  // Insert 5 bits with value 0x1F
        
        // 9. Copy the inflate stream state
        if (inflateCopy(&strm2, &strm1) == Z_OK) {
            // Try to inflate a little with copied stream
            Bytef out_buf[100];
            strm2.next_out = out_buf;
            strm2.avail_out = sizeof(out_buf);
            inflate(&strm2, fuzz_int32_t_4);
            
            inflateEnd(&strm2);
        }
        
        inflateEnd(&strm1);
    }
    
    // 10. Open output file with gzopen for writing
    gzFile gz_out = gzopen("output_file", fuzz_str_5);
    if (gz_out) {
        // 11. Use gzprintf to write information about the decompression
        gzprintf(gz_out, "Original size: %zu\n", size);
        gzprintf(gz_out, "CompressBound: %zu\n", (size_t)bound);
        gzprintf(gz_out, "Uncompress result: %d\n", uncompress_result);
        if (err_msg) {
            gzprintf(gz_out, "Gzip error: %s\n", err_msg);
        }
        
        // 12. Get file offsets using both gzoffset and gzoffset64
        off_t offset = gzoffset(gz_out);
        off64_t offset64 = gzoffset64(gz_out);
        
        gzprintf(gz_out, "Current offset: %ld\n", (long)offset);
        gzprintf(gz_out, "Current offset64: %lld\n", (long long)offset64);
        
        // Write some of the uncompressed data if available
        if (uncompressed_buf && uncompress_result == Z_OK && uncompressed_len > 0) {
            size_t write_len = uncompressed_len > 100 ? 100 : uncompressed_len;
            gzwrite(gz_out, uncompressed_buf, (unsigned int)write_len);
        }
        
        gzclose(gz_out);
    }
    
    // 13. Clean up
    if (uncompressed_buf) {
        free(uncompressed_buf);
    }
    
    if (gz_in) {
        gzclose(gz_in);
    }
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&tmp_in);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}