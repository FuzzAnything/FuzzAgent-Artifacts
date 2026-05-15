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
//<ID> 304
//<Prompt> []
/*<Combination>: [
*/
//<score> 60, nr_unique_branch: 2
//<Quality> {"density":20,"unique_branches":{"inflateStateCheck":[[94,9,94,24,0,0,4,0]],"inflateEnd":[[1157,9,1157,32,0,0,4,0]]},"library_calls":["zlibCompileFlags","gzdopen","gzclose","gzclose","gzdopen","gzclose","gzsetparams","gzread","gzwrite","gzputs","gztell64","gzclose","gzclose_w","deflateInit_","deflateInit2_","deflateEnd","deflateCopy","deflateEnd","inflateInit_","deflateEnd","deflateEnd","deflateResetKeep","inflateReset","deflateParams","deflateBound","deflateBound_z","compressBound","compressBound_z","compressBound","compress2","compressBound_z","compress_z","deflateBound","deflate","inflate","inflateMark","inflateSyncPoint","crc32","crc32","crc32","crc32","crc32_combine","gzdopen","gzclose","inflateBackEnd","inflateEnd","deflateEnd","deflateEnd"],"critical_calls":["zlibCompileFlags","gzdopen","gzdopen","gzsetparams","gzread","gzwrite","gzputs","gztell64","gzclose","gzclose_w","deflateInit_","deflateInit2_","deflateCopy","deflateEnd","inflateInit_","deflateResetKeep","inflateReset","deflateParams","deflateBound","deflateBound_z","compressBound","compressBound_z","compressBound","compress2","compressBound_z","compress_z","deflateBound","deflate","inflate","inflateMark","inflateSyncPoint","crc32","crc32","crc32","crc32","crc32_combine","gzdopen","gzclose","inflateBackEnd","inflateEnd","deflateEnd","deflateEnd"],"visited":0}
/*Here's a comprehensive fuzz driver that uses all the specified zlib APIs to handle a byte stream of zlib-compressed data:

*/


extern "C" int LLVMFuzzerTestOneInput_86(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // 1. Get zlib compile flags
    uLong compile_flags = zlibCompileFlags();
    (void)compile_flags; // Use the variable to avoid compiler warnings

    // 2. Create input file from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    int fd_in = fuzz_fileno(in_file);
    if (fd_in < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 3. Open gzFile for reading
    gzFile gz_in = gzdopen(fd_in, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 4. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        gzclose(gz_in);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    int fd_out = fuzz_fileno(out_file);
    if (fd_out < 0) {
        assert_file_closed(&out_file);;
        gzclose(gz_in);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 5. Open gzFile for writing
    gzFile gz_out = gzdopen(fd_out, "wb");
    if (!gz_out) {
        assert_file_closed(&out_file);;
        gzclose(gz_in);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 6. Set parameters for output gzFile
    gzsetparams(gz_out, Z_DEFAULT_COMPRESSION, Z_DEFAULT_STRATEGY);
    
    // 7. Read from input gzFile
    std::vector<Bytef> read_buffer(1024);
    uInt bytes_read = gzread(gz_in, read_buffer.data(), read_buffer.size());
    
    // 8. Write to output gzFile
    if (bytes_read > 0) {
        gzwrite(gz_out, read_buffer.data(), bytes_read);
        gzputs(gz_out, "fuzz test string");
    }
    
    // 9. Get position in output file
    off64_t pos = gztell64(gz_out);
    (void)pos; // Use variable
    
    // 10. Close gzFiles
    gzclose(gz_in);
    gzclose_w(gz_out);
    
    // 11. Create z_stream for deflation
    z_stream def_strm;
    def_strm.zalloc = Z_NULL;
    def_strm.zfree = Z_NULL;
    def_strm.opaque = Z_NULL;
    
    // 12. Initialize deflation streams using different methods
    int ret = deflateInit_(&def_strm, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 13. Create another deflate stream with different parameters
    z_stream def_strm2;
    def_strm2.zalloc = Z_NULL;
    def_strm2.zfree = Z_NULL;
    def_strm2.opaque = Z_NULL;
    
    ret = deflateInit2_(&def_strm2, Z_BEST_COMPRESSION, Z_DEFLATED, 
                        MAX_WBITS, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY,
                        ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        deflateEnd(&def_strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 14. Copy deflate stream
    z_stream def_strm_copy;
    ret = deflateCopy(&def_strm_copy, &def_strm);
    if (ret == Z_OK) {
        deflateEnd(&def_strm_copy);
    }
    
    // 15. Create inflate stream
    z_stream inf_strm;
    inf_strm.zalloc = Z_NULL;
    inf_strm.zfree = Z_NULL;
    inf_strm.opaque = Z_NULL;
    
    ret = inflateInit_(&inf_strm, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        deflateEnd(&def_strm2);
        deflateEnd(&def_strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 16. Reset streams
    deflateResetKeep(&def_strm);
    inflateReset(&inf_strm);
    
    // 17. Set deflate parameters
    deflateParams(&def_strm, Z_BEST_SPEED, Z_HUFFMAN_ONLY);
    
    // 18. Calculate compression bounds
    uLong bound1 = deflateBound(&def_strm, size);
    z_size_t bound2 = deflateBound_z(&def_strm, size);
    uLong bound3 = compressBound(size);
    z_size_t bound4 = compressBound_z(size);
    
    (void)bound1; (void)bound2; (void)bound3; (void)bound4; // Use variables
    
    // 19. Compress data using different methods
    std::vector<Bytef> compressed_data1(compressBound(size));
    uLongf compressed_len1 = compressed_data1.size();
    compress2(compressed_data1.data(), &compressed_len1, data, size, Z_DEFAULT_COMPRESSION);
    
    std::vector<Bytef> compressed_data2(compressBound_z(size));
    z_size_t compressed_len2 = compressed_data2.size();
    compress_z(compressed_data2.data(), &compressed_len2, data, size);
    
    // 20. Prepare buffers for deflate/inflate operations
    std::vector<Bytef> deflate_buffer(deflateBound(&def_strm, size));
    std::vector<Bytef> inflate_buffer(size * 2);
    
    def_strm.next_in = const_cast<Bytef*>(data);
    def_strm.avail_in = size;
    def_strm.next_out = deflate_buffer.data();
    def_strm.avail_out = deflate_buffer.size();
    
    // 21. Perform deflation
    ret = deflate(&def_strm, Z_FINISH);
    
    // 22. Setup inflate with compressed data
    inf_strm.next_in = deflate_buffer.data();
    inf_strm.avail_in = def_strm.total_out;
    inf_strm.next_out = inflate_buffer.data();
    inf_strm.avail_out = inflate_buffer.size();
    
    // 23. Perform inflation
    ret = inflate(&inf_strm, Z_FINISH);
    
    // 24. Use inflate mark and sync point
    long mark = inflateMark(&inf_strm);
    (void)mark;
    
    int sync_ret = inflateSyncPoint(&inf_strm);
    (void)sync_ret;
    
    // 25. Calculate CRC32 checksums
    uLong crc1 = crc32(0L, Z_NULL, 0);
    crc1 = crc32(crc1, data, size);
    
    uLong crc2 = crc32(0L, Z_NULL, 0);
    if (bytes_read > 0) {
        crc2 = crc32(crc2, read_buffer.data(), bytes_read);
    }
    
    // 26. Combine CRC32 checksums
    uLong combined_crc = crc32_combine(crc1, crc2, size);
    (void)combined_crc;
    
    // 27. Read single character from input (reopen for reading)
    FILE *in_file2 = fmemopen((void *)data, size, "rb");
    if (in_file2) {
        int fd_in2 = fuzz_fileno(in_file2);
        if (fd_in2 >= 0) {
            gzFile gz_in2 = gzdopen(fd_in2, "rb");
            if (gz_in2) {
                int c = gzgetc(gz_in2);
                (void)c;
                gzclose(gz_in2);
            }
            assert_file_closed(&in_file2);;
        }
    }
    
    // 28. Clean up all streams
    inflateBackEnd(&inf_strm);
    inflateEnd(&inf_strm);
    deflateEnd(&def_strm2);
    deflateEnd(&def_strm);
    
    // 29. Close remaining files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&in_file2);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}