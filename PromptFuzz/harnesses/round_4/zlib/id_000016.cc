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
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["zlibVersion","gzdopen","compressBound","deflateInit2_","gzclose","deflateSetHeader","gzfread","gzrewind","gzgets","deflateEnd","gzclose"],"critical_calls":["zlibVersion","gzdopen","compressBound","deflateInit2_","deflateSetHeader","gzfread","gzrewind","gzgets","deflateEnd","gzclose"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified zlib APIs to achieve the event of decompressing and processing zlib-compressed data:

*/


extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<29) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
	//fuzzer shim end}




    // 1. Get zlib version
    const char *version = zlibVersion();
    if (!version) {
	return 0;
	}
	
    
    // 2. Create a temporary file with compressed data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 3. Get file descriptor and open as gzFile
    int fd = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd, fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 4. Calculate compression bound for potential recompression
    uLong bound = compressBound(size);
    
    // 5. Initialize compression stream for potential recompression
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize with gzip format
    int ret = deflateInit2_(&strm, fuzz_int32_t_2, fuzz_int32_t_3,
                           MAX_WBITS + 16, fuzz_int32_t_4, fuzz_int32_t_5,
                           version, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 6. Set gzip header
    gz_header header;
    memset(&header, 0, sizeof(header));
    header.os = 255; // Unknown OS
    deflateSetHeader(&strm, &header);
    
    // 7. Read from gzFile using gzfread
    char buffer[4096];
    z_size_t items_read = 0;
    size_t total_read = 0;
    
    do {
        items_read = gzfread(buffer, fuzz_uint64_t_6, sizeof(buffer), gz_in);
        total_read += items_read;
    } while (items_read > 0 && total_read < 65536); // Limit total read
    
    // 8. Rewind and read line by line using gzgets
    gzrewind(gz_in);
    
    char line[256];
    int line_count = 0;
    while (gzgets(gz_in, line, sizeof(line)) != nullptr && line_count < 10) {
        line_count++;
    }
    
    // 9. Clean up compression stream
    deflateEnd(&strm);
    
    // 10. Close gzFile
    gzclose(gz_in);
    
    // 11. Close the FILE pointer
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}