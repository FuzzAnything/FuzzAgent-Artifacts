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
//<ID> 15
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["zlibCompileFlags","inflateInit2_","inflateValidate","zError","inflateEnd","inflateEnd","inflate","zError","inflateCopy","inflateEnd","inflateEnd","deflateInit_","deflateTune","deflateBound","deflateEnd","deflate","deflateUsed","gzdopen","gzbuffer","gzwrite","gzclose","deflateEnd"],"critical_calls":["zlibCompileFlags","inflateInit2_","inflateValidate","inflate","zError","inflateCopy","inflateEnd","inflateEnd","deflateInit_","deflateTune","deflateBound","deflate","deflateUsed","gzdopen","gzbuffer","gzwrite","gzclose","deflateEnd"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified zlib APIs to achieve decompression and recompression of zlib data:

*/


extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if input is too small
    if (size < 2) {
	return 0;
	}
	
    
    // 7. Get zlib compilation flags (for information)
    uLong compile_flags = zlibCompileFlags();
    (void)compile_flags; // Use variable to avoid unused warning
    
    // Initialize decompression stream
    z_stream inflate_stream;
    memset(&inflate_stream, 0, sizeof(inflate_stream));
    
    // Use inflateInit2 with windowBits for zlib/gzip detection
    int windowBits = 15 + 32; // 32 enables gzip/zlib auto-detection
    if (inflateInit2(&inflate_stream, windowBits) != Z_OK) {
        return 0;
    }
    
    // 7. Enable validation during inflation
    if (inflateValidate(&inflate_stream, 1) != Z_OK) {
        const char* err = zError(inflate_stream.msg ? Z_DATA_ERROR : Z_OK);
        (void)err; // Use variable
        inflateEnd(&inflate_stream);
        return 0;
    }
    
    // Set input data for decompression
    inflate_stream.next_in = (Bytef*)data;
    inflate_stream.avail_in = (uInt)size;
    
    // Allocate output buffer for decompressed data
    size_t decompressed_size = size * 4; // Conservative estimate
    Bytef* decompressed = (Bytef*)malloc(decompressed_size);
    if (!decompressed) {
        inflateEnd(&inflate_stream);
        return 0;
    }
    
    inflate_stream.next_out = decompressed;
    inflate_stream.avail_out = (uInt)decompressed_size;
    
    // Decompress the data
    int inflate_result = inflate(&inflate_stream, Z_FINISH);
    
    // 7. Get error message if decompression failed
    if (inflate_result != Z_STREAM_END && inflate_result != Z_OK) {
        const char* err_msg = zError(inflate_result);
        (void)err_msg; // Use variable
    }
    
    size_t actual_decompressed_size = decompressed_size - inflate_stream.avail_out;
    
    // 7. Copy the inflate stream state
    z_stream inflate_copy_stream;
    memset(&inflate_copy_stream, 0, sizeof(inflate_copy_stream));
    if (inflateCopy(&inflate_copy_stream, &inflate_stream) == Z_OK) {
        // Clean up the copy
        inflateEnd(&inflate_copy_stream);
    }
    
    // End decompression stream
    inflateEnd(&inflate_stream);
    
    // If decompression failed, clean up and return
    if (inflate_result != Z_STREAM_END && inflate_result != Z_OK) {
        free(decompressed);
        return 0;
    }
    
    // Now recompress the decompressed data
    z_stream deflate_stream;
    memset(&deflate_stream, 0, sizeof(deflate_stream));
    
    // Initialize compression stream with default level
    if (deflateInit(&deflate_stream, Z_DEFAULT_COMPRESSION) != Z_OK) {
        free(decompressed);
        return 0;
    }
    
    // 7. Tune the deflate parameters
    deflateTune(&deflate_stream, 8, 9, 4, 1);
    
    // Set input for compression (the decompressed data)
    deflate_stream.next_in = decompressed;
    deflate_stream.avail_in = (uInt)actual_decompressed_size;
    
    // Allocate output buffer for recompressed data
    size_t recompressed_size = deflateBound(&deflate_stream, (uLong)actual_decompressed_size);
    Bytef* recompressed = (Bytef*)malloc(recompressed_size);
    if (!recompressed) {
        deflateEnd(&deflate_stream);
        free(decompressed);
        return 0;
    }
    
    deflate_stream.next_out = recompressed;
    deflate_stream.avail_out = (uInt)recompressed_size;
    
    // Compress the data
    int deflate_result = deflate(&deflate_stream, Z_FINISH);
    
    // 7. Check how many bits were used
    int bits_used = 0;
    deflateUsed(&deflate_stream, &bits_used);
    (void)bits_used; // Use variable
    
    size_t actual_recompressed_size = recompressed_size - deflate_stream.avail_out;
    
    // 7. Create a gzFile and set buffer size
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
	int fuzz_fd_0 = fuzz_fileno(out_file);

        gzFile gz_out = gzdopen(fuzz_fd_0, "wb");
        if (gz_out) {
            // Set buffer size
            gzbuffer(gz_out, 8192);
            
            // Write the recompressed data to the gzFile
            if (actual_recompressed_size > 0) {
                gzwrite(gz_out, recompressed, (unsigned)actual_recompressed_size);
            }
            
            gzclose(gz_out);
        }
        assert_file_closed(&out_file);;
    }
    
    // 7. Clean up all allocated resources
    deflateEnd(&deflate_stream);
    free(decompressed);
    free(recompressed);
    
    assert_file_closed(&out_file);
	return 0;
}