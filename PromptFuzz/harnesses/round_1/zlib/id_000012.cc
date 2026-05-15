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
//<ID> 24
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 0
//<Quality> {"density":21,"unique_branches":{},"library_calls":["zlibVersion","uncompress2_z","uncompress2_z","deflateInit_","deflateBound","deflateEnd","deflate","deflateParams","deflateUsed","inflateInit_","deflateEnd","inflateEnd","deflateEnd","inflateCopy","inflateMark","inflateCodesUsed","inflate","inflateReset2","inflateEnd","inflateEnd","deflateEnd"],"critical_calls":["zlibVersion","uncompress2_z","uncompress2_z","deflateInit_","deflateBound","deflate","deflateParams","deflateUsed","inflateInit_","inflateCopy","inflateMark","inflateCodesUsed","inflate","inflateReset2","inflateEnd","inflateEnd","deflateEnd"],"visited":0}
/**/



extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Get zlib version first
    const char *version = zlibVersion();
    if (version == nullptr) {
        return 0;
    }

    // 2. Prepare variables for uncompress2_z
    z_size_t destLen = 0;
    z_size_t sourceLen = size;
    Bytef *dest = nullptr;
    
    // First call to get required buffer size (dest can be nullptr for size query)
    int uncompress_ret = uncompress2_z(nullptr, &destLen, data, &sourceLen);
    if (uncompress_ret != Z_BUF_ERROR && destLen == 0) {
        // Need to handle this case - try with reasonable buffer
        destLen = size * 10; // Heuristic expansion factor
    }
    
    // Allocate decompression buffer
    dest = static_cast<Bytef*>(malloc(destLen));
    if (dest == nullptr) {
        return 0;
    }
    
    // Reset sourceLen for actual decompression
    sourceLen = size;
    
    // 3. Actually decompress the data
    uncompress_ret = uncompress2_z(dest, &destLen, data, &sourceLen);
    
    // 4. Initialize deflate stream for re-compression
    z_stream deflate_stream;
    memset(&deflate_stream, 0, sizeof(z_stream));
    
    int init_ret = deflateInit_(&deflate_stream, Z_DEFAULT_COMPRESSION, version, sizeof(z_stream));
    if (init_ret != Z_OK) {
        free(dest);
        return 0;
    }
    
    // 5. Set up deflate stream with decompressed data
    deflate_stream.next_in = dest;
    deflate_stream.avail_in = static_cast<uInt>(destLen > UINT_MAX ? UINT_MAX : destLen);
    
    // Allocate output buffer for deflate
    size_t compressed_size = deflateBound(&deflate_stream, destLen);
    Bytef *compressed = static_cast<Bytef*>(malloc(compressed_size));
    if (compressed == nullptr) {
        deflateEnd(&deflate_stream);
        free(dest);
        return 0;
    }
    
    deflate_stream.next_out = compressed;
    deflate_stream.avail_out = static_cast<uInt>(compressed_size > UINT_MAX ? UINT_MAX : compressed_size);
    
    // 6. Compress some data first
    int deflate_ret = deflate(&deflate_stream, Z_SYNC_FLUSH);
    
    // 7. Change compression parameters using deflateParams
    int params_ret = deflateParams(&deflate_stream, Z_BEST_COMPRESSION, Z_DEFAULT_STRATEGY);
    
    // 8. Check bits used by deflate
    int bits_used = 0;
    int used_ret = deflateUsed(&deflate_stream, &bits_used);
    
    // 9. Initialize inflate stream from the compressed data
    z_stream inflate_stream;
    memset(&inflate_stream, 0, sizeof(z_stream));
    
    int inflate_init_ret = inflateInit_(&inflate_stream, version, sizeof(z_stream));
    if (inflate_init_ret != Z_OK) {
        deflateEnd(&deflate_stream);
        free(compressed);
        free(dest);
        return 0;
    }
    
    // Set up inflate stream with compressed data
    inflate_stream.next_in = compressed;
    inflate_stream.avail_in = static_cast<uInt>(deflate_stream.total_out > UINT_MAX ? UINT_MAX : deflate_stream.total_out);
    
    // Allocate output buffer for inflate
    Bytef *decompressed_again = static_cast<Bytef*>(malloc(destLen));
    if (decompressed_again == nullptr) {
        inflateEnd(&inflate_stream);
        deflateEnd(&deflate_stream);
        free(compressed);
        free(dest);
        return 0;
    }
    
    inflate_stream.next_out = decompressed_again;
    inflate_stream.avail_out = static_cast<uInt>(destLen > UINT_MAX ? UINT_MAX : destLen);
    
    // 10. Create a copy of the inflate stream
    z_stream inflate_copy_stream;
    memset(&inflate_copy_stream, 0, sizeof(z_stream));
    
    int copy_ret = inflateCopy(&inflate_copy_stream, &inflate_stream);
    
    // 11. Mark current position in the original inflate stream
    long mark_position = inflateMark(&inflate_stream);
    
    // 12. Get codes used by inflate
    unsigned long codes_used = inflateCodesUsed(&inflate_stream);
    
    // 13. Try to inflate some data
    int inflate_ret = inflate(&inflate_stream, Z_SYNC_FLUSH);
    
    // 14. Reset inflate stream with different window bits
    int reset_ret = inflateReset2(&inflate_stream, MAX_WBITS + 16); // Add 16 for gzip decoding
    
    // Clean up all resources
    if (copy_ret == Z_OK) {
        inflateEnd(&inflate_copy_stream);
    }
    
    inflateEnd(&inflate_stream);
    deflateEnd(&deflate_stream);
    
    free(decompressed_again);
    free(compressed);
    free(dest);
    
    return 0;
}