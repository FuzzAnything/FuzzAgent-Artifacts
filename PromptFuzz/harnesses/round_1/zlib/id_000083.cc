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
//<ID> 226
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":7,"unique_branches":{"inflatePrime":[[223,9,223,18,0,0,4,0]]},"library_calls":["gzopen64","gzbuffer","inflateInit_","gzclose","uncompress2","inflatePrime","inflate","gzread","gzoffset","gztell64","inflateEnd","gzclose"],"critical_calls":["gzopen64","gzbuffer","inflateInit_","uncompress2","inflatePrime","inflate","gzread","gzoffset","gztell64","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_83(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 10) {
        return 0;
    }

    // Step 1: Create a FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 2: Get file descriptor and open as gzip file
    int fd = fuzz_fileno(in_file);
    gzFile gz = gzopen64("input_file", "rb");  // Using filename directly per instruction
    if (!gz) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }

    // Step 3: Set buffer size for gzip file
    gzbuffer(gz, 8192);

    // Step 4: Prepare decompression stream
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    
    if (inflateInit(&strm) != Z_OK) {
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }

    // Step 5: Use uncompress2 with part of the input data
    // Split input data: first half for uncompress2, second half for inflate operations
    size_t split = size / 2;
    uLongf destLen = 1024 * 1024;  // 1MB buffer
    Bytef *dest = (Bytef *)malloc(destLen);
    uLong sourceLen = split;
    
    // Note: uncompress2 expects valid zlib-compressed data
    // We'll attempt decompression but handle errors gracefully
    int uncompress_ret = uncompress2(dest, &destLen, 
                                     data, &sourceLen);

    // Step 6: Use inflatePrime with remaining data
    if (size - split > 0) {
        // Prime the stream with some bits from remaining data
        int bits = data[split] & 0x07;  // Use 0-7 bits
        int value = data[split + 1] & 0xFF;
        inflatePrime(&strm, bits, value);
        
        // Feed remaining data to inflate
        strm.avail_in = size - split - 2;
        strm.next_in = (Bytef *)(data + split + 2);
        
        Bytef out_buffer[4096];
        strm.avail_out = sizeof(out_buffer);
        strm.next_out = out_buffer;
        
        inflate(&strm, Z_SYNC_FLUSH);
    }

    // Step 7: Read from gzip file and get offsets
    char read_buffer[1024];
    gzread(gz, read_buffer, sizeof(read_buffer));
    
    // Get offsets using both functions
    off_t offset1 = gzoffset(gz);
    off64_t offset2 = gztell64(gz);
    
    // Step 8: Clean up
    inflateEnd(&strm);
    free(dest);
    gzclose(gz);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}