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
//<ID> 214
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 2
//<Quality> {"density":10,"unique_branches":{"deflateParams":[[797,13,797,27,0,0,4,0],[797,31,797,76,0,0,4,0]]},"library_calls":["zlibVersion","uncompress2_z","compressBound_z","compress2_z","inflateInit2_","inflateEnd","inflate","inflateMark","inflateCodesUsed","inflateCopy","inflateReset2","inflateEnd","deflateInit_","inflateEnd","deflate","deflateParams","deflateEnd","inflateEnd"],"critical_calls":["zlibVersion","uncompress2_z","compressBound_z","compress2_z","inflateInit2_","inflate","inflateMark","inflateCodesUsed","inflateCopy","inflateReset2","inflateEnd","deflateInit_","deflate","deflateParams","deflateEnd","inflateEnd"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_79(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
        return 0;
    }

    // 1. Get zlib version
    const char *version = zlibVersion();
    (void)version; // Avoid unused variable warning

    // 2. First decompression pass using uncompress2_z
    // Prepare destination buffer for decompression
    z_size_t destLen = size * 10; // Initial guess
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) {
	return 0;
	}
	
    
    z_size_t sourceLen = size;
    const Bytef *source = data;
    
    // First decompression
    int uncompress_result = uncompress2_z(dest, &destLen, source, &sourceLen);
    
    // 3. If decompression succeeded, recompress with compress2_z
    if (uncompress_result == Z_OK && destLen > 0) {
        // Prepare for recompression
        z_size_t compLen = compressBound_z(destLen);
        Bytef *compressed = (Bytef *)malloc(compLen);
        if (compressed) {
            // Choose compression level based on input data
            int level = data[0] % 10; // 0-9
            if (level < 0) level = 0;
            if (level > 9) level = 9;
            
            int compress_result = compress2_z(compressed, &compLen, dest, destLen, level);
            (void)compress_result; // Avoid unused warning
            
            free(compressed);
        }
    }
    
    // 4. Setup stream-based decompression
    z_stream strm_inflate;
    memset(&strm_inflate, 0, sizeof(z_stream));
    
    // Initialize inflation
    int windowBits = 15; // Default window size
    if (inflateInit2_(&strm_inflate, windowBits, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        free(dest);
        return 0;
    }
    
    // 5. Setup input for stream inflation
    strm_inflate.next_in = (Bytef *)data;
    strm_inflate.avail_in = size > UINT_MAX ? UINT_MAX : (uInt)size;
    
    // Create output buffer for inflation
    Bytef *out_buf = (Bytef *)malloc(1024);
    if (!out_buf) {
        inflateEnd(&strm_inflate);
        free(dest);
        return 0;
    }
    
    strm_inflate.next_out = out_buf;
    strm_inflate.avail_out = 1024;
    
    // 6. Perform inflation in steps
    int flush = Z_NO_FLUSH;
    int inflate_result = Z_OK;
    
    while (strm_inflate.avail_in > 0 && inflate_result == Z_OK) {
        inflate_result = inflate(&strm_inflate, flush);
        
        // Use inflateMark to get current position
        long mark = inflateMark(&strm_inflate);
        (void)mark; // Avoid unused warning
        
        // Get codes used
        unsigned long codes_used = inflateCodesUsed(&strm_inflate);
        (void)codes_used; // Avoid unused warning
        
        // Reset output buffer
        strm_inflate.next_out = out_buf;
        strm_inflate.avail_out = 1024;
    }
    
    // 7. Create a copy of inflation stream
    z_stream strm_copy;
    memset(&strm_copy, 0, sizeof(z_stream));
    if (inflateCopy(&strm_copy, &strm_inflate) == Z_OK) {
        // Reset the copied stream with different window bits
        int new_windowBits = 15 + 16; // Add 16 for gzip decoding
        inflateReset2(&strm_copy, new_windowBits);
        inflateEnd(&strm_copy);
    }
    
    // 8. Setup stream-based compression
    z_stream strm_deflate;
    memset(&strm_deflate, 0, sizeof(z_stream));
    
    // Initialize deflation with same level as before
    int level = data[0] % 10;
    if (deflateInit_(&strm_deflate, level, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        inflateEnd(&strm_inflate);
        free(out_buf);
        free(dest);
        return 0;
    }
    
    // 9. Use the decompressed data as input for deflation
    if (destLen > 0) {
        strm_deflate.next_in = dest;
        strm_deflate.avail_in = destLen > UINT_MAX ? UINT_MAX : (uInt)destLen;
        
        // Create output buffer for deflation
        Bytef *deflate_out = (Bytef *)malloc(1024);
        if (deflate_out) {
            strm_deflate.next_out = deflate_out;
            strm_deflate.avail_out = 1024;
            
            // Perform deflation
            int deflate_result = deflate(&strm_deflate, Z_FINISH);
            (void)deflate_result; // Avoid unused warning
            
            // Change compression parameters during deflation
            int new_level = (level + 1) % 10;
            int strategy = data[1] % 4; // 0-3 for different strategies
            deflateParams(&strm_deflate, new_level, strategy);
            
            free(deflate_out);
        }
    }
    
    // 10. Create a FILE* for gz operations
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Get file descriptor
        int fd = fuzz_fileno(in_file);
        (void)fd; // Avoid unused warning
        
        // Note: gzdopen requires fd, but we'll use it conceptually
        // Since we can't use gzFile without actual file operations,
        // we'll demonstrate gzbuffer with a dummy approach
        
        // Create output file for gz operations
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            int out_fd = fuzz_fileno(out_file);
            // In a real scenario, we would use:
            // gzFile gz = gzdopen(out_fd, "wb");
            // gzbuffer(gz, 8192); // Set buffer size
            
            assert_file_closed(&out_file);;
            remove("output_file"); // Clean up
        }
        
        assert_file_closed(&in_file);;
    }
    
    // 11. Clean up all resources
    deflateEnd(&strm_deflate);
    inflateEnd(&strm_inflate);
    free(out_buf);
    free(dest);
    
    assert_file_closed(&in_file);
	return 0;
}