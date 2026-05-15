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
//<ID> 31
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["gzdopen","gzdirect","gzopen","gzclose_r","gzsetparams","gzfread","gzputc","gzputs","gzoffset","inflateInit2_","gzclose","gzclose_r","inflatePrime","inflateSync","inflateEnd","gzclose","gzclose_r"],"critical_calls":["gzdopen","gzdirect","gzopen","gzsetparams","gzfread","gzputc","gzputs","gzoffset","inflateInit2_","inflatePrime","inflateSync","inflateEnd","gzclose","gzclose_r"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Get file descriptor and open as gzFile for reading
    int fd = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 3. Check if file is in direct mode (uncompressed)
    int is_direct = gzdirect(gz_in);
    
    // 4. Create output gzFile for writing
    gzFile gz_out = gzopen("output_file", "wb");
    if (!gz_out) {
        gzclose_r(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
    }
    
    // 5. Set compression parameters for output file
    gzsetparams(gz_out, Z_DEFAULT_COMPRESSION, Z_DEFAULT_STRATEGY);
    
    // 6. Read from input gzFile using gzfread
    char buffer[4096];
    size_t bytes_read = 0;
    while (bytes_read < size) {
        size_t to_read = (size - bytes_read < sizeof(buffer)) ? 
                        size - bytes_read : sizeof(buffer);
        z_size_t read_items = gzfread(buffer, 1, to_read, gz_in);
        if (read_items == 0) break;
        bytes_read += read_items;
        
        // 7. Write to output gzFile using gzputc and gzputs
        for (size_t i = 0; i < read_items; i++) {
            gzputc(gz_out, buffer[i]);
        }
        // Write a marker string
        gzputs(gz_out, "[MARKER]");
    }
    
    // 8. Get current offset in input file
    off_t offset = gzoffset(gz_in);
    
    // 9. Prepare inflate stream for decompression
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize inflate with automatic header detection
    if (inflateInit2(&strm, MAX_WBITS + 32) != Z_OK) {
        gzclose(gz_out);
        gzclose_r(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
    }
    
    // 10. Prime the inflate stream with bits from input
    if (size > 0) {
        // Use first byte as priming value, limited to 16 bits
        int bits = (size > 1) ? 16 : 8;
        int value = (size > 0) ? data[0] : 0;
        if (size > 1) {
            value |= (data[1] << 8);
        }
        inflatePrime(&strm, bits, value);
    }
    
    // 11. Attempt to synchronize inflate stream
    inflateSync(&strm);
    
    // 12. Clean up inflate stream
    inflateEnd(&strm);
    
    // 13. Close all files and release resources
    gzclose(gz_out);
    gzclose_r(gz_in);
    assert_file_closed(&in_file);;
    
    // 14. Remove output file
    remove("output_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}