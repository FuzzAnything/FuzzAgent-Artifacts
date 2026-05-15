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
//<ID> 136
//<Prompt> []
/*<Combination>: [
*/
//<score> 54, nr_unique_branch: 3
//<Quality> {"density":27,"unique_branches":{"inflateSetDictionary":[[1196,9,1196,25,0,0,4,1],[1200,9,1200,28,0,0,4,1],[1210,9,1210,12,0,0,4,1]]},"library_calls":["zlibCompileFlags","inflateInit2_","deflateInit2_","inflateEnd","deflateSetHeader","deflateBound","deflateEnd","inflateEnd","inflate","inflateGetDictionary","inflateSetDictionary","inflateUndermine","inflateReset","deflate","deflatePending","deflatePrime","crc32","crc32","crc32","crc32","crc32_combine","gzdopen","gzgetc_","gztell","gzoffset64","gzclearerr","gzclose","gzdopen","gzwrite","gzclose","compressBound_z","compress_z","deflateEnd","inflateEnd"],"critical_calls":["zlibCompileFlags","inflateInit2_","deflateInit2_","deflateSetHeader","deflateBound","inflate","inflateGetDictionary","inflateSetDictionary","inflateUndermine","inflateReset","deflate","deflatePending","deflatePrime","crc32","crc32","crc32","crc32","crc32_combine","gzdopen","gzgetc_","gztell","gzoffset64","gzclearerr","gzclose","gzdopen","gzwrite","gzclose","compressBound_z","compress_z","deflateEnd","inflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if input is too small
    if (size < 10) {
	return 0;
	}
	
    
    // Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Get file descriptor
    int fd = fuzz_fileno(in_file);
    
    // 1. Check zlib compile flags
    uLong flags = zlibCompileFlags();
    (void)flags; // Mark as used
    
    // 2. Initialize inflation stream for decompression
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    
    // Use negative windowBits for raw deflate stream
    int ret = inflateInit2_(&inflate_strm, -MAX_WBITS, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 3. Initialize deflation stream for compression
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    
    // Initialize deflation with gzip header
    ret = deflateInit2_(&deflate_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                        15 + 16, 8, Z_DEFAULT_STRATEGY,
                        ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        inflateEnd(&inflate_strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 4. Set gzip header for deflation
    gz_header gzip_header;
    memset(&gzip_header, 0, sizeof(gzip_header));
    gzip_header.os = 255; // Unknown OS
    deflateSetHeader(&deflate_strm, &gzip_header);
    
    // 5. Calculate compression bound
    uLong bound = deflateBound(&deflate_strm, size);
    
    // 6. Allocate buffers
    Bytef *decompressed = (Bytef*)malloc(size * 2);
    Bytef *recompressed = (Bytef*)malloc(bound);
    Bytef *dictionary = (Bytef*)malloc(32768);
    uInt dict_len = 0;
    
    if (!decompressed || !recompressed || !dictionary) {
        free(decompressed);
        free(recompressed);
        free(dictionary);
        deflateEnd(&deflate_strm);
        inflateEnd(&inflate_strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 7. Setup input for inflation
    inflate_strm.next_in = (Bytef*)data;
    inflate_strm.avail_in = size;
    inflate_strm.next_out = decompressed;
    inflate_strm.avail_out = size * 2;
    
    // 8. Try to inflate (decompress) the input
    int flush = Z_NO_FLUSH;
    ret = inflate(&inflate_strm, flush);
    
    // 9. Try to get dictionary from inflation stream
    inflateGetDictionary(&inflate_strm, dictionary, &dict_len);
    
    // 10. If we got a dictionary, try to set it back
    if (dict_len > 0) {
        inflateSetDictionary(&inflate_strm, dictionary, dict_len);
    }
    
    // 11. Try undermine operation
    inflateUndermine(&inflate_strm, data[0] & 1);
    
    // 12. Reset inflation stream
    inflateReset(&inflate_strm);
    
    // 13. Setup deflation stream with decompressed data
    deflate_strm.next_in = decompressed;
    deflate_strm.avail_in = inflate_strm.total_out;
    deflate_strm.next_out = recompressed;
    deflate_strm.avail_out = bound;
    
    // 14. Deflate (compress) the decompressed data
    ret = deflate(&deflate_strm, Z_FINISH);
    
    // 15. Check for pending bits
    unsigned int pending;
    int bits;
    deflatePending(&deflate_strm, &pending, &bits);
    
    // 16. Try to prime the deflate stream
    if (size > 2) {
        deflatePrime(&deflate_strm, data[1] & 7, data[2] & 1);
    }
    
    // 17. Calculate CRC32 of decompressed data
    uLong crc1 = crc32(0L, Z_NULL, 0);
    if (inflate_strm.total_out > 0) {
        crc1 = crc32(crc1, decompressed, inflate_strm.total_out);
    }
    
    // 18. Calculate CRC32 of original data
    uLong crc2 = crc32(0L, Z_NULL, 0);
    crc2 = crc32(crc2, data, size > 100 ? 100 : size);
    
    // 19. Combine CRCs
    uLong combined_crc = crc32_combine(crc1, crc2, 100);
    (void)combined_crc; // Mark as used
    
    // 20. Use gzFile operations
    gzFile gz = gzdopen(fd, "rb");
    if (gz) {
        // Try to read one character
        int c = gzgetc_(gz);
        (void)c; // Mark as used
        
        // Get current position
        off_t pos = gztell(gz);
        (void)pos; // Mark as used
        
        // Get offset (64-bit version)
        off64_t offset64 = gzoffset64(gz);
        (void)offset64; // Mark as used
        
        // Clear any errors
        gzclearerr(gz);
        
        // Close the gz file (but don't close the underlying fd)
        gzclose(gz);
    }
    
    // 21. Create output file for gzwrite
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
	int fuzz_fd_0 = fuzz_fileno(out_file);

        gzFile gz_out = gzdopen(fuzz_fd_0, "wb");
        if (gz_out) {
            // Write some data
            if (inflate_strm.total_out > 0) {
                gzwrite(gz_out, decompressed, 
                       inflate_strm.total_out > 100 ? 100 : inflate_strm.total_out);
            }
            gzclose(gz_out);
        }
        assert_file_closed(&out_file);;
    }
    
    // 22. Test compress_z function
    z_size_t dest_len = compressBound_z(size);
    Bytef *compressed_z = (Bytef*)malloc(dest_len);
    if (compressed_z) {
        z_size_t source_len = size > 1000 ? 1000 : size;
        compress_z(compressed_z, &dest_len, data, source_len);
        free(compressed_z);
    }
    
    // 23. Clean up
    free(decompressed);
    free(recompressed);
    free(dictionary);
    
    deflateEnd(&deflate_strm);
    inflateEnd(&inflate_strm);
    
    // Note: in_file was closed by gzdopen when gzclose was called
    // But we need to close it if gzdopen failed
    if (!gz) {
        assert_file_closed(&in_file);;
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}