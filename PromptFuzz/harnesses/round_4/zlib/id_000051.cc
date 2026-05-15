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
//<ID> 117
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 3
//<Quality> {"density":8,"unique_branches":{"inflateCopy":[[1344,9,1344,32,0,0,4,0],[1347,13,1347,29,0,0,4,1],[1363,9,1363,25,0,0,4,0]]},"library_calls":["zlibCompileFlags","gzdopen","inflateInit2_","gzclose","inflateEnd","gzclose","inflateEnd","gzclose","inflate","inflateCopy","deflateInit2_","deflateSetHeader","gzwrite","gzputc","gzoffset","deflateEnd","inflateBackInit_","inflateBackEnd","inflateEnd","inflateEnd","gzclose"],"critical_calls":["zlibCompileFlags","gzdopen","inflateInit2_","inflate","inflateCopy","deflateInit2_","deflateSetHeader","gzwrite","gzputc","gzoffset","deflateEnd","inflateBackInit_","inflateBackEnd","inflateEnd","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* f_data, size_t f_size) {
	if(f_size<9) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // 1. Get zlib compilation flags
    uLong flags = zlibCompileFlags();
    (void)flags; // Suppress unused variable warning
    
    // 2. Create a FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Create a FILE* for writing output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; // Suppress unused variable warning
    (void)out_fd; // Suppress unused variable warning
    
    // 5. Open gzFile for writing using the output file descriptor
    gzFile gz_out = gzdopen(dup(out_fd), fuzz_str_1);
    if (!gz_out) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 6. Set up z_stream for inflation (decompression)
    z_stream strm1, strm2;
    memset(&strm1, 0, sizeof(strm1));
    memset(&strm2, 0, sizeof(strm2));
    
    // 7. Initialize inflation stream
    if (inflateInit2(&strm1, 15 + 32) != Z_OK) { // 15+32 for gzip/zlib auto detection
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 8. Set up input buffer for inflation
    Bytef *in_buf = (Bytef *)malloc(size);
    if (!in_buf) {
        inflateEnd(&strm1);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    memcpy(in_buf, data, size);
    
    strm1.next_in = in_buf;
    strm1.avail_in = (uInt)size;
    
    // 9. Set up output buffer for inflation
    size_t out_buf_size = size * 2; // Reasonable guess for decompressed size
    if (out_buf_size < 1024) out_buf_size = 1024;
    Bytef *out_buf = (Bytef *)malloc(out_buf_size);
    if (!out_buf) {
        free(in_buf);
        inflateEnd(&strm1);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    strm1.next_out = out_buf;
    strm1.avail_out = (uInt)out_buf_size;
    
    // 10. Perform inflation (decompression)
    int inflate_ret = inflate(&strm1, fuzz_int32_t_2);
    
    // 11. Copy the inflation stream state
    if (inflateCopy(&strm2, &strm1) != Z_OK) {
        // Continue even if copy fails
    }
    
    // 12. Set up gzip header for deflation
    gz_header header;
    memset(&header, 0, sizeof(header));
    header.text = 1;
    header.time = time(NULL);
    header.os = 255; // Unknown OS
    
    // 13. Set up z_stream for deflation (compression)
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    
    if (deflateInit2(&def_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                     15 + 16, 8, Z_DEFAULT_STRATEGY) == Z_OK) {
        // 14. Set gzip header for deflation
        deflateSetHeader(&def_strm, &header);
        
        // 15. Write decompressed data to gzFile
        size_t decompressed_size = out_buf_size - strm1.avail_out;
        if (decompressed_size > 0) {
            // Write using gzwrite
            gzwrite(gz_out, out_buf, (unsigned int)decompressed_size);
            
            // Write a single character using gzputc
            gzputc(gz_out, '\n');
            
            // Get current offset in gzFile
            off_t offset = gzoffset(gz_out);
            (void)offset; // Suppress unused variable warning
        }
        
        // 16. Clean up deflation stream
        deflateEnd(&def_strm);
    }
    
    // 17. Set up for inflateBack (though we won't fully use it)
    z_stream back_strm;
    memset(&back_strm, 0, sizeof(back_strm));
    
    // Initialize for inflateBack
    unsigned char *window = (unsigned char *)malloc(1 << 15);
    if (window && inflateBackInit(&back_strm, 15, window) == Z_OK) {
        // We initialize it but don't actually use it for callbacks
        // This satisfies the requirement to call inflateBackEnd
        inflateBackEnd(&back_strm);
    }
    free(window);
    
    // 18. Clean up resources
    free(in_buf);
    free(out_buf);
    inflateEnd(&strm1);
    inflateEnd(&strm2);
    gzclose(gz_out);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}