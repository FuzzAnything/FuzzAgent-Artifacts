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
//<ID> 48
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["zlibCompileFlags","gzopen64","gzoffset","gzclose","gzread","gzclose","inflateInit2_","inflateReset2","inflateEnd","inflateReset","inflateEnd","inflate","inflateEnd","compressBound","compressBound","compress"],"critical_calls":["zlibCompileFlags","gzopen64","gzoffset","gzread","gzclose","inflateInit2_","inflateReset2","inflateReset","inflate","inflateEnd","compressBound","compressBound","compress"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Early exit if not enough data for meaningful processing
    if (size < 10) {
	return 0;
	}
	
    
    // 1. Get zlib compile flags (API call #1)
    uLong compile_flags = zlibCompileFlags();
    (void)compile_flags; // Suppress unused warning
    
    // 2. Create a temporary file with the input data for gzopen64
    FILE *tmp_file = fopen("input_file", "wb");
    if (!tmp_file) {
	assert_file_closed(&tmp_file);
	return 0;
	}
	
    fwrite(data, 1, size, tmp_file);
    assert_file_closed(&tmp_file);;
    
    // 3. Open the file with gzopen64 (API call #2)
    gzFile gz = gzopen64("input_file", "rb");
    if (!gz) {
        remove("input_file");
        assert_file_closed(&tmp_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 4. Get file offset (API call #3)
    off_t offset = gzoffset(gz);
    (void)offset; // Suppress unused warning
    
    // 5. Prepare for decompression
    z_stream strm;
    Bytef *in_buf = (Bytef*)malloc(size);
    Bytef *out_buf = (Bytef*)malloc(size * 2); // Decompressed may be larger
    uLongf out_len = size * 2;
    
    if (!in_buf || !out_buf) {
        free(in_buf);
        free(out_buf);
        gzclose(gz);
        remove("input_file");
        assert_file_closed(&tmp_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 6. Read some data from gz file
    int bytes_read = gzread(gz, in_buf, size < 1024 ? size : 1024);
    gzclose(gz);
    
    // 7. Initialize z_stream for decompression
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = in_buf;
    strm.avail_in = bytes_read;
    strm.next_out = out_buf;
    strm.avail_out = out_len;
    
    // Use raw inflate since input is raw compressed data
    if (inflateInit2(&strm, -15) != Z_OK) {
        free(in_buf);
        free(out_buf);
        remove("input_file");
        assert_file_closed(&tmp_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 8. Reset inflate stream with different window bits (API call #4)
    if (inflateReset2(&strm, 15) != Z_OK) {
        inflateEnd(&strm);
        free(in_buf);
        free(out_buf);
        remove("input_file");
        assert_file_closed(&tmp_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 9. Reset inflate stream (API call #5)
    if (inflateReset(&strm) != Z_OK) {
        inflateEnd(&strm);
        free(in_buf);
        free(out_buf);
        remove("input_file");
        assert_file_closed(&tmp_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 10. Try to decompress
    inflate(&strm, Z_FINISH);
    inflateEnd(&strm);
    
    // 11. Compress the decompressed data (or original if decompression failed)
    Bytef *compress_buf = (Bytef*)malloc(compressBound(size * 2));
    uLongf compress_len = compressBound(size * 2);
    
    if (compress_buf) {
        // API call #6: compress
        int compress_ret = compress(compress_buf, &compress_len, 
                                  out_buf, strm.total_out > 0 ? strm.total_out : bytes_read);
        (void)compress_ret; // Suppress unused warning
        free(compress_buf);
    }
    
    // 12. Cleanup
    free(in_buf);
    free(out_buf);
    remove("input_file");
    
    assert_file_closed(&tmp_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}