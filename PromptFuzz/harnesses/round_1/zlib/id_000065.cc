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
//<ID> 160
//<Prompt> []
/*<Combination>: [
*/
//<score> 46, nr_unique_branch: 1
//<Quality> {"density":23,"unique_branches":{"inflate":[[494,36,494,60,0,0,4,0]]},"library_calls":["gzdopen","gzgetc_","inflateInit2_","gzclose","inflate","inflateMark","inflateSetDictionary","inflateGetDictionary","inflateUndermine","zError","crc32","deflateInit2_","inflateEnd","gzclose","deflateSetDictionary","deflateGetDictionary","deflateBound_z","compressBound","deflateEnd","inflateEnd","gzclose","deflate","deflatePending","deflateCopy","deflateResetKeep","deflateEnd","deflateEnd","inflateEnd","gzclose"],"critical_calls":["gzdopen","gzgetc_","inflateInit2_","inflate","inflateMark","inflateSetDictionary","inflateGetDictionary","inflateUndermine","zError","crc32","deflateInit2_","deflateSetDictionary","deflateGetDictionary","deflateBound_z","compressBound","deflate","deflatePending","deflateCopy","deflateResetKeep","deflateEnd","deflateEnd","inflateEnd","gzclose"],"visited":0}
/**/


// Define missing types that might be needed
typedef void *(*alloc_func)(void *, uInt, uInt);
typedef void (*free_func)(void *, void *);
typedef void *voidpf;
typedef void *voidp;
typedef const void *voidpc;

extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // If data is too small, return early
    if (size < 10) {
	return 0;
	}
	
    
    // Setup variables
    z_stream strm_inflate, strm_deflate, strm_copy;
    gzFile gz_file = NULL;
    FILE *in_file = NULL;
    FILE *out_file = NULL;
    int ret, fd;
    unsigned int pending;
    int bits;
    uLong crc_val = 0;
    long mark_pos;
    
    // 1. Create a FILE* from input data using fmemopen
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Get file descriptor and open as gzFile
    fd = fuzz_fileno(in_file);
    gz_file = gzdopen(fd, "rb");
    if (!gz_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 3. Use gzgetc_ to read a character
    int c = gzgetc_(gz_file);
    
    // 4. Initialize inflation stream for decompression
    // Using windowBits = 15 + 32 to enable zlib and gzip decoding
    ret = inflateInit2_(&strm_inflate, 15 + 32, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 5. Set up input buffer from the remaining data
    // We'll use the original data pointer since we already read some via gzgetc_
    // Reset the position for the z_stream
    strm_inflate.next_in = (Bytef *)data;
    strm_inflate.avail_in = size;
    strm_inflate.next_out = NULL;
    strm_inflate.avail_out = 0;
    
    // 6. Try to inflate a bit
    ret = inflate(&strm_inflate, Z_NO_FLUSH);
    
    // 7. Get current mark position
    mark_pos = inflateMark(&strm_inflate);
    
    // 8. Try to set a dictionary (using part of input as dictionary)
    uInt dict_len = size > 256 ? 256 : (uInt)size;
    if (dict_len > 0) {
        inflateSetDictionary(&strm_inflate, data, dict_len);
    }
    
    // 9. Try to get the dictionary back
    Bytef dict_buf[256];
    uInt dict_buf_len = 256;
    inflateGetDictionary(&strm_inflate, dict_buf, &dict_buf_len);
    
    // 10. Try to undermine the stream (for testing error conditions)
    inflateUndermine(&strm_inflate, 0);
    
    // 11. Get error string for current return code
    const char *err_str = zError(ret);
    
    // 12. Calculate CRC32 of part of the input
    uInt crc_len = size > 100 ? 100 : (uInt)size;
    if (crc_len > 0) {
        crc_val = crc32(0L, data, crc_len);
    }
    
    // 13. Initialize deflation stream for compression
    // Using windowBits = 15, memLevel = 8, strategy = Z_DEFAULT_STRATEGY
    ret = deflateInit2_(&strm_deflate, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                        15, 8, Z_DEFAULT_STRATEGY, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        inflateEnd(&strm_inflate);
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 14. Set dictionary for deflation (using same dictionary data)
    if (dict_len > 0) {
        deflateSetDictionary(&strm_deflate, data, dict_len);
    }
    
    // 15. Get dictionary from deflation stream
    uInt deflate_dict_len = 256;
    deflateGetDictionary(&strm_deflate, dict_buf, &deflate_dict_len);
    
    // 16. Calculate bound for compressed size
    z_size_t source_len = size > 1024 ? 1024 : (z_size_t)size;
    z_size_t bound = deflateBound_z(&strm_deflate, source_len);
    
    // 17. Also use compressBound (traditional version)
    uLong bound2 = compressBound((uLong)source_len);
    
    // 18. Create output buffer for compressed data
    Bytef *out_buf = (Bytef *)malloc(bound);
    if (!out_buf) {
        deflateEnd(&strm_deflate);
        inflateEnd(&strm_inflate);
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 19. Prepare deflation stream with input data
    strm_deflate.next_in = (Bytef *)data;
    strm_deflate.avail_in = size > 1024 ? 1024 : (uInt)size;
    strm_deflate.next_out = out_buf;
    strm_deflate.avail_out = (uInt)bound;
    
    // 20. Perform deflation
    ret = deflate(&strm_deflate, Z_FINISH);
    
    // 21. Check for pending bits
    deflatePending(&strm_deflate, &pending, &bits);
    
    // 22. Copy deflation stream to another stream
    memset(&strm_copy, 0, sizeof(z_stream));
    deflateCopy(&strm_copy, &strm_deflate);
    
    // 23. Reset deflation stream while keeping internal state
    deflateResetKeep(&strm_deflate);
    
    // 24. Open output file for writing
    out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write compressed data to file
        size_t compressed_size = bound - strm_deflate.avail_out;
        if (compressed_size > 0) {
            fwrite(out_buf, 1, compressed_size, out_file);
        }
        assert_file_closed(&out_file);;
    }
    
    // 25. Clean up copied stream
    deflateEnd(&strm_copy);
    
    // 26. Clean up deflation stream
    deflateEnd(&strm_deflate);
    
    // 27. Clean up inflation stream
    inflateEnd(&strm_inflate);
    
    // 28. Close gzFile
    gzclose(gz_file);
    
    // 29. Close input file
    assert_file_closed(&in_file);;
    
    // 30. Free allocated memory
    free(out_buf);
    
    // Remove output file if created
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}