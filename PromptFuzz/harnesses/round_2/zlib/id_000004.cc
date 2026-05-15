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
//<ID> 6
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 2
//<Quality> {"density":7,"unique_branches":{"deflateGetDictionary":[[630,9,630,32,0,0,4,0]],"deflateTune":[[823,9,823,32,0,0,4,0]]},"library_calls":["gzdopen","gzopen","gzclose","deflateTune","deflateBound","compress2_z","deflateGetDictionary","gzread","gzclearerr","gzoffset","gzrewind","gzwrite","gzclose","gzclose"],"critical_calls":["gzdopen","gzopen","deflateTune","deflateBound","compress2_z","deflateGetDictionary","gzread","gzclearerr","gzoffset","gzrewind","gzwrite","gzclose","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if input is too small for meaningful processing
    if (size < 10) {
	return 0;
	}
	
    
    // 1. Create a FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Get file descriptor and open with gzdopen
    int fd = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 3. Open another gzFile using gzopen for writing
    gzFile gz_out = gzopen("output_file", "wb");
    if (!gz_out) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
    }
    
    // 4. Initialize z_stream for compression
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // 5. Tune compression parameters using deflateTune
    deflateTune(&strm, 8, 9, 256, 4096);
    
    // 6. Calculate bound for compressed data using deflateBound
    uLong bound = deflateBound(&strm, size);
    
    // 7. Allocate buffers for compression
    Bytef *dest = (Bytef*)malloc(bound);
    z_size_t destLen = bound;
    
    // 8. Compress data using compress2_z
    int level = Z_DEFAULT_COMPRESSION;
    int compress_result = compress2_z(dest, &destLen, data, size, level);
    
    // 9. Get dictionary information using deflateGetDictionary
    Bytef dictionary[32768];
    uInt dictLength = 0;
    deflateGetDictionary(&strm, dictionary, &dictLength);
    
    // 10. Read from gzFile to verify it can be processed
    char read_buf[4096];
    int bytes_read = gzread(gz_in, read_buf, sizeof(read_buf));
    
    // 11. Clear any errors from gzFile
    gzclearerr(gz_in);
    
    // 12. Get current offset using gzoffset
    off_t offset = gzoffset(gz_in);
    
    // 13. Rewind to beginning using gzrewind
    int rewind_result = gzrewind(gz_in);
    
    // 14. Write compressed data to output gzFile
    if (compress_result == Z_OK) {
        gzwrite(gz_out, dest, destLen);
    }
    
    // 15. Release all allocated resources
    free(dest);
    gzclose(gz_in);
    gzclose(gz_out);
    assert_file_closed(&in_file);;
    
    // Clean up output file if created
    remove("output_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}