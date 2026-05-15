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
//<ID> 236
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 0
//<Quality> {"density":22,"unique_branches":{},"library_calls":["gzdopen","gzgetc_","gzclose","inflateInit2_","inflateEnd","inflate","inflateMark","inflateUndermine","inflateUndermine","crc32","crc32","inflateGetDictionary","inflateSetDictionary","inflateEnd","deflateInit2_","compressBound","deflateBound_z","deflateSetDictionary","deflateEnd","deflate","deflatePending","deflateResetKeep","deflate","deflateResetKeep","deflateEnd"],"critical_calls":["gzdopen","gzgetc_","gzclose","inflateInit2_","inflate","inflateMark","inflateUndermine","inflateUndermine","crc32","crc32","inflateGetDictionary","inflateSetDictionary","inflateEnd","deflateInit2_","compressBound","deflateBound_z","deflateSetDictionary","deflate","deflatePending","deflateResetKeep","deflate","deflateResetKeep","deflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create memory file for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Get file descriptor for gzdopen
    int fd = fuzz_fileno(in_file);
    
    // Use gzFile APIs
    gzFile gz = gzdopen(fd, "rb");
    if (gz) {
        // Try to read one character
        int c = gzgetc_(gz);
        (void)c; // Suppress unused warning
        
        gzclose(gz);
    }
    assert_file_closed(&in_file);;
    
    // Reopen for other operations
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
	}
	
    fd = fuzz_fileno(in_file);
    
    // Decompression setup
    z_stream d_strm;
    memset(&d_strm, 0, sizeof(z_stream));
    
    // Initialize decompression with windowBits that can handle any format
    if (inflateInit2_(&d_strm, MAX_WBITS + 32, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Set up input buffer
    d_strm.next_in = (Bytef *)data;
    d_strm.avail_in = size < UINT_MAX ? (uInt)size : UINT_MAX;
    
    // Allocate output buffer for decompression
    size_t out_size = size * 4 + 1024; // Heuristic for decompressed size
    if (out_size > 10 * 1024 * 1024) out_size = 10 * 1024 * 1024; // Cap at 10MB
    Bytef *decompressed = (Bytef *)malloc(out_size);
    if (!decompressed) {
        inflateEnd(&d_strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	assert_fd_closed(fd);
	return 0;
    }
    
    d_strm.next_out = decompressed;
    d_strm.avail_out = (uInt)(out_size < UINT_MAX ? out_size : UINT_MAX);
    
    // Perform decompression
    int flush = Z_NO_FLUSH;
    if (size < 100) flush = Z_SYNC_FLUSH; // Try different flush modes
    
    int inflate_ret = inflate(&d_strm, flush);
    
    // Try to get a mark
    long mark = inflateMark(&d_strm);
    (void)mark;
    
    // Try undermine if supported (test both success and failure paths)
    if (d_strm.avail_in > 0) {
        inflateUndermine(&d_strm, 0); // Try with valid=0
        inflateUndermine(&d_strm, 1); // Try with valid=1
    }
    
    // Calculate CRC32 of what we decompressed
    uLong crc = crc32(0L, Z_NULL, 0);
    if (d_strm.total_out > 0) {
        crc = crc32(crc, decompressed, (uInt)(d_strm.total_out < UINT_MAX ? d_strm.total_out : UINT_MAX));
    }
    
    // Try to get and set dictionary
    Bytef dict_buffer[32768];
    uInt dict_len = sizeof(dict_buffer);
    int dict_ret = inflateGetDictionary(&d_strm, dict_buffer, &dict_len);
    
    if (dict_ret == Z_OK && dict_len > 0) {
        // Try to set the dictionary back
        inflateSetDictionary(&d_strm, dict_buffer, dict_len);
    }
    
    // End decompression
    inflateEnd(&d_strm);
    
    // Now try compression with the decompressed data (if any)
    if (d_strm.total_out > 0) {
        z_stream c_strm;
        memset(&c_strm, 0, sizeof(z_stream));
        
        // Initialize compression
        int level = Z_DEFAULT_COMPRESSION;
        int method = Z_DEFLATED;
        int windowBits = MAX_WBITS;
        int memLevel = 8;
        int strategy = Z_DEFAULT_STRATEGY;
        
        if (deflateInit2_(&c_strm, level, method, windowBits, memLevel, 
                         strategy, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
            free(decompressed);
            assert_file_closed(&in_file);;
            assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	assert_fd_closed(fd);
	return 0;
        }
        
        // Get compression bounds
        uLong bound = compressBound((uLong)d_strm.total_out);
        z_size_t bound_z = deflateBound_z(&c_strm, (z_size_t)d_strm.total_out);
        (void)bound;
        (void)bound_z;
        
        // Try to set dictionary if we have one
        if (dict_len > 0) {
            deflateSetDictionary(&c_strm, dict_buffer, dict_len);
        }
        
        // Set up compression input
        c_strm.next_in = decompressed;
        c_strm.avail_in = (uInt)(d_strm.total_out < UINT_MAX ? d_strm.total_out : UINT_MAX);
        
        // Allocate compression output buffer
        size_t comp_size = bound < out_size ? bound : out_size;
        Bytef *compressed = (Bytef *)malloc(comp_size);
        if (!compressed) {
            deflateEnd(&c_strm);
            free(decompressed);
            assert_file_closed(&in_file);;
            assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	assert_fd_closed(fd);
	return 0;
        }
        
        c_strm.next_out = compressed;
        c_strm.avail_out = (uInt)(comp_size < UINT_MAX ? comp_size : UINT_MAX);
        
        // Perform compression
        int deflate_ret = deflate(&c_strm, Z_FINISH);
        
        // Try pending bits check
        unsigned int pending;
        int bits;
        deflatePending(&c_strm, &pending, &bits);
        
        // Reset and try again with different flush
        deflateResetKeep(&c_strm);
        
        // Reset input/output pointers
        c_strm.next_in = decompressed;
        c_strm.avail_in = (uInt)(d_strm.total_out < UINT_MAX ? d_strm.total_out : UINT_MAX);
        c_strm.next_out = compressed;
        c_strm.avail_out = (uInt)(comp_size < UINT_MAX ? comp_size : UINT_MAX);
        
        // Try with SYNC_FLUSH
        deflate(&c_strm, Z_SYNC_FLUSH);
        
        // Reset again
        deflateResetKeep(&c_strm);
        
        // Clean up compression
        deflateEnd(&c_strm);
        free(compressed);
    }
    
    // Clean up
    free(decompressed);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	assert_fd_closed(fd);
	return 0;
}