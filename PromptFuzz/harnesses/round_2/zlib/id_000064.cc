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
//<ID> 187
//<Prompt> []
/*<Combination>: [
*/
//<score> 29, nr_unique_branch: 1
//<Quality> {"density":29,"unique_branches":{"gzfwrite":[[303,12,303,15,0,0,4,1]]},"library_calls":["zlibCompileFlags","compressBound","compress2","deflateInit_","deflateBound","deflateBound_z","deflateEnd","deflate","deflateParams","deflate","inflateInit_","deflateEnd","inflateEnd","deflateEnd","inflateMark","inflate","adler32","adler32","crc32","crc32","adler32_combine","crc32_combine","inflateEnd","deflateEnd","gzdopen","inflateEnd","deflateEnd","gzread","gztell64","gzclose","inflateEnd","deflateEnd","gzdopen","inflateEnd","deflateEnd","gzsetparams","gzwrite","gzfwrite","gzclose_w","deflateReset","inflateReset","deflateResetKeep","inflateEnd","deflateEnd"],"critical_calls":["zlibCompileFlags","compressBound","compress2","deflateInit_","deflateBound","deflateBound_z","deflate","deflateParams","deflate","inflateInit_","inflateMark","inflate","adler32","adler32","crc32","crc32","adler32_combine","crc32_combine","gzdopen","gzread","gztell64","gzclose","gzdopen","gzsetparams","gzwrite","gzfwrite","gzclose_w","deflateReset","inflateReset","deflateResetKeep","inflateEnd","deflateEnd"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need enough data for meaningful operations
    
    // 1. Get zlib compile flags
    uLong flags = zlibCompileFlags();
    
    // 2. Use compress2 API to compress input data
    uLongf comprLen = compressBound(size);
    Bytef* compr = (Bytef*)malloc(comprLen);
    if (!compr) {
	return 0;
	}
	
    
    int comprRet = compress2(compr, &comprLen, data, size, Z_DEFAULT_COMPRESSION);
    if (comprRet != Z_OK) {
        free(compr);
        return 0;
    }
    
    // 3. Set up deflate stream
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(z_stream));
    
    int deflateInitRet = deflateInit_(&deflate_strm, Z_DEFAULT_COMPRESSION, 
                                      ZLIB_VERSION, sizeof(z_stream));
    if (deflateInitRet != Z_OK) {
        free(compr);
        return 0;
    }
    
    // 4. Use deflate APIs
    deflate_strm.next_in = (Bytef*)data;
    deflate_strm.avail_in = size;
    
    uLong deflate_bound = deflateBound(&deflate_strm, size);
    z_size_t deflate_bound_z = deflateBound_z(&deflate_strm, size);
    
    // Create output buffer for deflate
    uLongf deflate_out_size = deflate_bound;
    Bytef* deflate_out = (Bytef*)malloc(deflate_out_size);
    if (!deflate_out) {
        deflateEnd(&deflate_strm);
        free(compr);
        return 0;
    }
    
    deflate_strm.next_out = deflate_out;
    deflate_strm.avail_out = deflate_out_size;
    
    // First deflate call
    int deflateRet = deflate(&deflate_strm, Z_NO_FLUSH);
    
    // Change parameters mid-stream
    int paramsRet = deflateParams(&deflate_strm, Z_BEST_COMPRESSION, Z_DEFAULT_STRATEGY);
    
    // Complete deflation
    deflateRet = deflate(&deflate_strm, Z_FINISH);
    
    uLongf deflate_actual_size = deflate_out_size - deflate_strm.avail_out;
    
    // 5. Set up inflate stream for the compressed data
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(z_stream));
    
    int inflateInitRet = inflateInit_(&inflate_strm, ZLIB_VERSION, sizeof(z_stream));
    if (inflateInitRet != Z_OK) {
        free(deflate_out);
        deflateEnd(&deflate_strm);
        free(compr);
        return 0;
    }
    
    // 6. Use inflate APIs on compressed data
    inflate_strm.next_in = compr;
    inflate_strm.avail_in = comprLen;
    
    uLongf inflate_out_size = size * 2;  // Enough space for decompressed data
    Bytef* inflate_out = (Bytef*)malloc(inflate_out_size);
    if (!inflate_out) {
        inflateEnd(&inflate_strm);
        free(deflate_out);
        deflateEnd(&deflate_strm);
        free(compr);
        return 0;
    }
    
    inflate_strm.next_out = inflate_out;
    inflate_strm.avail_out = inflate_out_size;
    
    // Get mark before inflation
    long mark = inflateMark(&inflate_strm);
    
    // Inflate the data
    int inflateRet = inflate(&inflate_strm, Z_FINISH);
    
    // 7. Compute checksums
    uLong adler = adler32(0L, Z_NULL, 0);
    adler = adler32(adler, data, size);
    
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, data, size);
    
    // Combine checksums (use arbitrary values)
    uLong adler_combined = adler32_combine(adler, crc, size);
    uLong crc_combined = crc32_combine(crc, adler, size);
    
    // 8. Use gzFile operations with file descriptors
    // Create input FILE* from memory
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        free(inflate_out);
        inflateEnd(&inflate_strm);
        free(deflate_out);
        deflateEnd(&deflate_strm);
        free(compr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptor
    int fd = fuzz_fileno(in_file);
    
    // Open gzFile for reading
    gzFile gz_in = gzdopen(fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        free(inflate_out);
        inflateEnd(&inflate_strm);
        free(deflate_out);
        deflateEnd(&deflate_strm);
        free(compr);
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Read from gzFile
    unsigned char gz_buffer[1024];
    int gzreadRet = gzread(gz_in, gz_buffer, sizeof(gz_buffer));
    
    // Get current position
    off64_t gz_pos = gztell64(gz_in);
    
    // Close gzFile for reading
    int gzcloseRet = gzclose(gz_in);  // This will also close the underlying fd
    
    // Note: assert_file_closed(&in_file); is not needed because gzdopen took ownership
    
    // 9. Create output FILE* and gzFile for writing
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        free(inflate_out);
        inflateEnd(&inflate_strm);
        free(deflate_out);
        deflateEnd(&deflate_strm);
        free(compr);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    int out_fd = fuzz_fileno(out_file);
    gzFile gz_out = gzdopen(out_fd, "wb");
    if (!gz_out) {
        assert_file_closed(&out_file);;
        free(inflate_out);
        inflateEnd(&inflate_strm);
        free(deflate_out);
        deflateEnd(&deflate_strm);
        free(compr);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Set gzFile parameters
    int gzparamsRet = gzsetparams(gz_out, Z_BEST_SPEED, Z_DEFAULT_STRATEGY);
    
    // Write to gzFile using both gzwrite and gzfwrite
    int gzwriteRet = gzwrite(gz_out, data, size > 100 ? 100 : size);
    z_size_t gzfwriteRet = gzfwrite(data + 100, 1, size > 200 ? (size - 100) : 0, gz_out);
    
    // Close gzFile for writing
    int gzclose_wRet = gzclose_w(gz_out);  // This closes the underlying file
    
    // 10. Reset streams
    int deflateResetRet = deflateReset(&deflate_strm);
    int inflateResetRet = inflateReset(&inflate_strm);
    
    // 11. Use deflateResetKeep
    int deflateResetKeepRet = deflateResetKeep(&deflate_strm);
    
    // Cleanup all resources
    free(inflate_out);
    inflateEnd(&inflate_strm);
    
    free(deflate_out);
    deflateEnd(&deflate_strm);
    
    free(compr);
    
    // Remove output file if created
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
}