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
//<ID> 37
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["inflateInit2_","inflateEnd","inflateSetDictionary","inflate","zError","inflateUndermine","crc32","crc32","inflateEnd","deflateInit2_","compressBound","deflateBound_z","deflateEnd","deflateSetDictionary","deflate","deflatePending","deflateResetKeep","deflateEnd","gzdopen","gzread","gzclose"],"critical_calls":["inflateInit2_","inflateSetDictionary","inflate","zError","inflateUndermine","crc32","crc32","inflateEnd","deflateInit2_","compressBound","deflateBound_z","deflateSetDictionary","deflate","deflatePending","deflateResetKeep","deflateEnd","gzdopen","gzread","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if input is too small for meaningful operations
    if (size < 10) {
	return 0;
	}
	

    // 1. Decompression setup
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    
    // Initialize inflate for gzip/zlib format auto-detection
    int windowBits = 15 + 32; // Auto-detect gzip/zlib
    if (inflateInit2_(&inflate_strm, windowBits, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        return 0;
    }

    // Create output buffer for decompression
    size_t decompressed_size = size * 10; // Conservative estimate
    Bytef* decompressed_buf = (Bytef*)malloc(decompressed_size);
    if (!decompressed_buf) {
        inflateEnd(&inflate_strm);
        return 0;
    }

    // Set up input/output for inflation
    inflate_strm.next_in = (Bytef*)data;
    inflate_strm.avail_in = (uInt)size;
    inflate_strm.next_out = decompressed_buf;
    inflate_strm.avail_out = (uInt)decompressed_size;

    // 2. Try to set dictionary (if data is long enough)
    if (size > 512) {
        Bytef dict_buf[256];
        uInt dict_len = 256;
        memcpy(dict_buf, data + 256, 256);
        inflateSetDictionary(&inflate_strm, dict_buf, dict_len);
    }

    // 3. Attempt inflation
    int inflate_result = inflate(&inflate_strm, Z_SYNC_FLUSH);
    
    // Get error message if inflation failed
    if (inflate_result < 0) {
        const char* err_msg = zError(inflate_result);
        // Error message available but not used further
    }

    // 4. Try undermine operation (for testing only)
    inflateUndermine(&inflate_strm, (size % 2 == 0) ? 0 : 1);

    // 5. Calculate CRC32 of whatever was decompressed
    uLong crc = crc32(0L, Z_NULL, 0);
    uLong decompressed_bytes = decompressed_size - inflate_strm.avail_out;
    if (decompressed_bytes > 0) {
        crc = crc32(crc, decompressed_buf, (uInt)decompressed_bytes);
    }

    // 6. End inflation
    inflateEnd(&inflate_strm);

    // 7. Setup for re-compression if we got some decompressed data
    if (decompressed_bytes > 0) {
        // Compression setup
        z_stream deflate_strm;
        memset(&deflate_strm, 0, sizeof(deflate_strm));
        
        int level = Z_DEFAULT_COMPRESSION;
        int method = Z_DEFLATED;
        int memLevel = 8;
        int strategy = Z_DEFAULT_STRATEGY;
        
        // Initialize deflate
        if (deflateInit2_(&deflate_strm, level, method, windowBits, memLevel, 
                         strategy, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
            free(decompressed_buf);
            return 0;
        }

        // Calculate compression bounds
        uLong comp_bound = compressBound((uLong)decompressed_bytes);
        z_size_t comp_bound_z = deflateBound_z(&deflate_strm, (z_size_t)decompressed_bytes);

        // Allocate compression buffer (use the larger bound)
        size_t comp_buf_size = (comp_bound > comp_bound_z) ? comp_bound : comp_bound_z;
        Bytef* compressed_buf = (Bytef*)malloc(comp_buf_size);
        if (!compressed_buf) {
            deflateEnd(&deflate_strm);
            free(decompressed_buf);
            return 0;
        }

        // Set dictionary for compression (different from inflation dictionary)
        if (decompressed_bytes > 256) {
            deflateSetDictionary(&deflate_strm, decompressed_buf, 256);
        }

        // Setup compression
        deflate_strm.next_in = decompressed_buf;
        deflate_strm.avail_in = (uInt)decompressed_bytes;
        deflate_strm.next_out = compressed_buf;
        deflate_strm.avail_out = (uInt)comp_buf_size;

        // 8. Perform compression
        int deflate_result = deflate(&deflate_strm, Z_FINISH);
        
        // Check for pending bits
        unsigned int pending;
        int bits;
        deflatePending(&deflate_strm, &pending, &bits);

        // 9. Reset but keep dictionary
        deflateResetKeep(&deflate_strm);

        // 10. End deflation
        deflateEnd(&deflate_strm);
        
        free(compressed_buf);
    }

    free(decompressed_buf);

    // 11. File operations using gzdopen
    // Create a temporary file with input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Get file descriptor for gzdopen
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Open with gzdopen
    gzFile gz = gzdopen(fd, "rb");
    if (gz) {
        // Read a small amount to test
        Bytef read_buf[128];
        gzread(gz, read_buf, 128);
        
        // Close gz file
        gzclose(gz);
    }
    
    // Close the underlying FILE
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}