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
//<ID> 221
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["zlibCompileFlags","compressBound","uncompress2","compressBound","compressBound","compress","inflateInit2_","inflateGetHeader","inflateMark","inflate","inflateCodesUsed","deflateInit_","deflateTune","deflateUsed","deflateEnd","gzdopen","gzclose","inflateEnd"],"critical_calls":["zlibCompileFlags","compressBound","uncompress2","compressBound","compressBound","compress","inflateInit2_","inflateGetHeader","inflateMark","inflate","inflateCodesUsed","deflateInit_","deflateTune","deflateUsed","deflateEnd","gzdopen","gzclose","inflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_81(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Ensure minimum size for basic operations
    if (size < 2) {
	return 0;
	}
	
    
    // 1. First check zlib compilation flags
    uLong compile_flags = zlibCompileFlags();
    
    // 2. Try to decompress the input data using uncompress2
    // Allocate destination buffer - use compressBound to estimate max size
    uLongf destLen = compressBound((uLong)size);
    Bytef* dest = (Bytef*)malloc(destLen);
    if (!dest) {
	return 0;
	}
	
    
    // Use uncompress2 to decompress
    uLong sourceLen = size;
    int uncompress_result = uncompress2(dest, &destLen, data, &sourceLen);
    
    // 3. If decompression succeeded, recompress the data
    if (uncompress_result == Z_OK && destLen > 0) {
        Bytef* recompressed = (Bytef*)malloc(compressBound(destLen));
        if (recompressed) {
            uLongf recompressedLen = compressBound(destLen);
            int compress_result = compress(recompressed, &recompressedLen, dest, destLen);
            free(recompressed);
        }
    }
    
    // 4. Create inflate stream to parse zlib header and check compression details
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    
    // Initialize inflate stream
    if (inflateInit2(&inflate_strm, MAX_WBITS) != Z_OK) {
        free(dest);
        return 0;
    }
    
    // Set up input data for inflate
    inflate_strm.next_in = (Bytef*)data;
    inflate_strm.avail_in = (uInt)size;
    
    // Create header structure for inflateGetHeader
    gz_header header;
    memset(&header, 0, sizeof(header));
    inflateGetHeader(&inflate_strm, &header);
    
    // Get initial mark position
    long mark_pos = inflateMark(&inflate_strm);
    
    // Inflate a small amount to move forward
    Bytef out_buf[256];
    inflate_strm.next_out = out_buf;
    inflate_strm.avail_out = sizeof(out_buf);
    
    int inflate_result = inflate(&inflate_strm, Z_SYNC_FLUSH);
    
    // Get inflate codes used
    unsigned long codes_used = inflateCodesUsed(&inflate_strm);
    
    // 5. Create deflate stream to test deflateTune and deflateUsed
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    
    if (deflateInit(&deflate_strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
        // Tune deflate parameters using some values from input data
        int good_length = data[0] % 32 + 8;       // 8-39
        int max_lazy = data[1 % size] % 32 + 8;   // 8-39
        int nice_length = data[2 % size] % 32 + 8; // 8-39
        int max_chain = data[3 % size] % 256 + 1; // 1-256
        
        deflateTune(&deflate_strm, good_length, max_lazy, nice_length, max_chain);
        
        // Check deflate used bits
        int bits_used;
        deflateUsed(&deflate_strm, &bits_used);
        
        // Clean up deflate stream
        deflateEnd(&deflate_strm);
    }
    
    // 6. Read input data through gzFile interface using fmemopen
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        int fd = fuzz_fileno(in_file);
        gzFile gz_file = gzdopen(fd, "rb");
        
        if (gz_file) {
            // Read at least one character if possible
            if (size > 0) {
                int ch = gzgetc(gz_file);
            }
            gzclose(gz_file);
        } else {
            assert_file_closed(&in_file);;
        }
    }
    
    // 7. Clean up inflate stream
    inflateEnd(&inflate_strm);
    
    // 8. Free allocated memory
    free(dest);
    
    assert_file_closed(&in_file);
	return 0;
}