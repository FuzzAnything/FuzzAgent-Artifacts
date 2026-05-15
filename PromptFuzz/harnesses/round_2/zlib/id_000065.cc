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
//<ID> 188
//<Prompt> []
/*<Combination>: [
*/
//<score> 126, nr_unique_branch: 102
//<Quality> {"density":21,"unique_branches":{"deflate_fast":[[1941,9,1941,26,0,0,4,1],[1945,9,1945,20,0,0,4,0],[1644,8,1644,31,21,0,4,1],[1631,24,1631,44,36,0,4,0],[1631,24,1631,44,36,0,4,1]],"deflate_huff":[[2163,21,2163,40,0,0,4,0],[1644,8,1644,31,3,0,4,0],[1644,8,1644,31,5,0,4,0]],"deflate_rle":[[2094,13,2094,38,0,0,4,1],[2096,17,2096,42,0,0,4,1],[2096,46,2096,65,0,0,4,0],[2099,17,2099,34,0,0,4,1],[2104,13,2104,38,0,0,4,0],[2104,13,2104,38,0,0,4,1],[2104,42,2104,57,0,0,4,0],[2104,42,2104,57,0,0,4,1],[2107,17,2107,32,0,0,4,0],[2107,17,2107,32,0,0,4,1],[2107,36,2107,51,0,0,4,0],[2107,36,2107,51,0,0,4,1],[2107,55,2107,70,0,0,4,0],[2107,55,2107,70,0,0,4,1],[2110,26,2110,41,0,0,4,0],[2110,26,2110,41,0,0,4,1],[2110,45,2110,60,0,0,4,0],[2110,45,2110,60,0,0,4,1],[2111,26,2111,41,0,0,4,0],[2111,26,2111,41,0,0,4,1],[2111,45,2111,60,0,0,4,0],[2111,45,2111,60,0,0,4,1],[2112,26,2112,41,0,0,4,0],[2112,26,2112,41,0,0,4,1],[2112,45,2112,60,0,0,4,0],[2112,45,2112,60,0,0,4,1],[2113,26,2113,41,0,0,4,0],[2113,26,2113,41,0,0,4,1],[2113,45,2113,60,0,0,4,0],[2113,45,2113,60,0,0,4,1],[2114,26,2114,39,0,0,4,0],[2114,26,2114,39,0,0,4,1],[2116,21,2116,51,0,0,4,0],[2116,21,2116,51,0,0,4,1],[2124,13,2124,41,0,0,4,0],[2124,13,2124,41,0,0,4,1],[2139,13,2139,19,0,0,4,0],[2139,13,2139,19,0,0,4,1],[1644,8,1644,31,10,0,4,0],[1644,8,1644,31,12,0,4,0],[321,5,321,17,16,0,4,0],[1631,24,1631,44,19,0,4,0]],"deflate_slow":[[1997,42,1997,67,0,0,4,0]],"deflate_stored":[[1689,13,1689,38,0,0,4,0],[1696,13,1696,23,0,0,4,0],[1704,46,1704,63,0,0,4,0],[1705,33,1705,52,0,0,4,0],[1706,33,1706,64,0,0,4,0],[1712,16,1712,33,0,0,4,1],[1731,13,1731,17,0,0,4,0],[1732,17,1732,27,0,0,4,1],[1790,9,1790,13,0,0,4,1],[1796,9,1796,28,0,0,4,0],[1796,9,1796,28,0,0,4,1],[1796,32,1796,49,0,0,4,0],[1796,32,1796,49,0,0,4,1],[1797,9,1797,31,0,0,4,0],[1797,35,1797,70,0,0,4,0],[1802,9,1802,33,0,0,4,0],[1802,9,1802,33,0,0,4,1],[1802,37,1802,70,0,0,4,0],[1802,37,1802,70,0,0,4,1],[1807,13,1807,27,0,0,4,0],[1807,13,1807,27,0,0,4,1],[1810,13,1810,36,0,0,4,1],[1813,9,1813,33,0,0,4,0],[1813,9,1813,33,0,0,4,1],[1815,9,1815,13,0,0,4,0],[1815,9,1815,13,0,0,4,1],[1820,9,1820,36,0,0,4,0],[1820,9,1820,36,0,0,4,1],[1833,9,1833,26,0,0,4,0],[1833,9,1833,26,0,0,4,1],[1834,11,1834,15,0,0,4,0],[1834,11,1834,15,0,0,4,1],[1834,19,1834,36,0,0,4,0],[1834,41,1834,60,0,0,4,0],[1834,41,1834,60,0,0,4,1],[1835,10,1835,32,0,0,4,0],[1835,36,1835,48,0,0,4,0],[1837,16,1837,33,0,0,4,0],[1837,16,1837,33,0,0,4,1],[1837,37,1837,59,0,0,4,0],[1838,16,1838,27,0,0,4,0],[1845,9,1845,13,0,0,4,0],[1845,9,1845,13,0,0,4,1],[1847,12,1847,16,0,0,4,0],[1847,12,1847,16,0,0,4,1],[1651,20,1651,29,14,0,4,0],[1651,20,1651,29,14,0,4,1],[1651,20,1651,29,15,0,4,1],[1651,20,1651,29,16,0,4,0],[1651,20,1651,29,19,0,4,0],[1651,20,1651,29,19,0,4,1]]},"library_calls":["zlibCompileFlags","deflateInit_","deflateBound","deflateBound_z","deflateEnd","deflate","deflateParams","deflate","deflateResetKeep","deflateReset","inflateInit_","deflateEnd","inflate","inflateMark","inflateReset","adler32","crc32","adler32_combine","crc32_combine","gzdopen","gzsetparams","gzwrite","gzfwrite","gzclose_w","gzdopen","gzread","gztell64","gzclose","compress2","inflateEnd","deflateEnd"],"critical_calls":["zlibCompileFlags","deflateInit_","deflateBound","deflateBound_z","deflate","deflateParams","deflate","deflateResetKeep","deflateReset","inflateInit_","inflate","inflateMark","inflateReset","adler32","crc32","adler32_combine","crc32_combine","gzdopen","gzsetparams","gzwrite","gzfwrite","gzclose_w","gzdopen","gzread","gztell64","gzclose","compress2","inflateEnd","deflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	 // Need minimal data for meaningful testing
    
    // Step 1: Get zlib compile flags
    uLong compile_flags = zlibCompileFlags();
    (void)compile_flags; // Mark as used
    
    // Step 2: Create input/output FILE streams
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 3: Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Step 4: Setup zlib streams
    z_stream comp_stream, decomp_stream;
    memset(&comp_stream, 0, sizeof(comp_stream));
    memset(&decomp_stream, 0, sizeof(decomp_stream));
    
    // Step 5: Initialize compression with deflateInit_
    int level = data[0] % 10;  // Use first byte for level (0-9)
    if (deflateInit_(&comp_stream, level, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 6: Calculate compression bound
    uLong comp_bound = deflateBound(&comp_stream, size);
    z_size_t comp_bound_z = deflateBound_z(&comp_stream, size);
    (void)comp_bound; (void)comp_bound_z; // Mark as used
    
    // Step 7: Allocate compression buffers
    Bytef *comp_buffer = (Bytef *)malloc(comp_bound);
    Bytef *decomp_buffer = (Bytef *)malloc(size);
    if (!comp_buffer || !decomp_buffer) {
        free(comp_buffer);
        free(decomp_buffer);
        deflateEnd(&comp_stream);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 8: Prepare compression stream
    comp_stream.next_in = (Bytef *)data;
    comp_stream.avail_in = size;
    comp_stream.next_out = comp_buffer;
    comp_stream.avail_out = comp_bound;
    
    // Step 9: Perform compression with deflate
    int flush = data[1] % 4; // Use second byte for flush mode
    deflate(&comp_stream, flush);
    
    // Step 10: Change compression parameters
    int strategy = data[2] % 4; // Use third byte for strategy
    deflateParams(&comp_stream, level, strategy);
    
    // Step 11: Continue compression
    deflate(&comp_stream, Z_FINISH);
    uLong comp_size = comp_stream.total_out;
    
    // Step 12: Reset compression stream
    deflateResetKeep(&comp_stream);
    deflateReset(&comp_stream);
    
    // Step 13: Initialize decompression with inflateInit_
    if (inflateInit_(&decomp_stream, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        free(comp_buffer);
        free(decomp_buffer);
        deflateEnd(&comp_stream);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 14: Perform decompression
    decomp_stream.next_in = comp_buffer;
    decomp_stream.avail_in = comp_size;
    decomp_stream.next_out = decomp_buffer;
    decomp_stream.avail_out = size;
    
    inflate(&decomp_stream, Z_FINISH);
    
    // Step 15: Use inflateMark and inflateReset
    long mark = inflateMark(&decomp_stream);
    (void)mark; // Mark as used
    inflateReset(&decomp_stream);
    
    // Step 16: Calculate checksums
    uLong adler = adler32(0L, data, size);
    uLong crc = crc32(0L, data, size);
    
    // Step 17: Combine checksums (use dummy offsets)
    off_t dummy_offset = 100;
    uLong adler_combined = adler32_combine(adler, crc, dummy_offset);
    uLong crc_combined = crc32_combine(crc, adler, dummy_offset);
    (void)adler_combined; (void)crc_combined; // Mark as used
    
    // Step 18: GZIP file operations
    gzFile gz_out = gzdopen(out_fd, "wb");
    if (gz_out) {
        // Set compression parameters
        gzsetparams(gz_out, level, strategy);
        
        // Write compressed data using both gzwrite and gzfwrite
        gzwrite(gz_out, data, size % 1024); // Write partial data
        gzfwrite(data, 1, size, gz_out);    // Write all data
        
        gzclose_w(gz_out); // Close for writing
    }
    
    // Step 19: Reopen for reading
    out_file = fopen("output_file", "rb");
    if (out_file) {
        int read_fd = fuzz_fileno(out_file);
        gzFile gz_in = gzdopen(read_fd, "rb");
        if (gz_in) {
            // Read back data
            unsigned char read_buf[1024];
            gzread(gz_in, read_buf, sizeof(read_buf));
            
            // Get file position
            off64_t pos = gztell64(gz_in);
            (void)pos; // Mark as used
            
            gzclose(gz_in);
        }
        assert_file_closed(&out_file);;
    }
    
    // Step 20: Use compress2 API
    uLongf destLen = comp_bound;
    Bytef *comp2_buffer = (Bytef *)malloc(comp_bound);
    if (comp2_buffer) {
        compress2(comp2_buffer, &destLen, data, size, level);
        free(comp2_buffer);
    }
    
    // Step 21: Cleanup
    inflateEnd(&decomp_stream);
    deflateEnd(&comp_stream);
    free(comp_buffer);
    free(decomp_buffer);
    assert_file_closed(&in_file);;
    
    // Remove temporary output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}