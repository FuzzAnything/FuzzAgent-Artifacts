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
//<ID> 92
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":20,"unique_branches":{},"library_calls":["gzdopen","gzdirect","inflateInit2_","gzclose","gzread","gzerror","inflate","inflateSyncPoint","inflateCopy","deflateInit2_","deflateGetDictionary","deflateBound","compress2","deflateEnd","gzopen","gzputc","gzwrite","gzclose","adler32","adler32","adler32","adler32","adler32_combine64","inflateEnd","inflateEnd","gzclose"],"critical_calls":["gzdopen","gzdirect","inflateInit2_","gzread","gzerror","inflate","inflateSyncPoint","inflateCopy","deflateInit2_","deflateGetDictionary","deflateBound","compress2","deflateEnd","gzopen","gzputc","gzwrite","gzclose","adler32","adler32","adler32","adler32","adler32_combine64","inflateEnd","inflateEnd","gzclose"],"visited":1}
/*Here's a fuzz driver that uses all the specified zlib APIs with the given input constraints:

*/


extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if size is too small for meaningful processing
    if (size < 10) {
	return 0;
	}
	
    
    // Open input file from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Open output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    
    // 1. Create gzFile for reading using gzdopen
    gzFile gz_in = gzdopen(fd_in, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 2. Check if gzFile is direct
    int is_direct = gzdirect(gz_in);
    
    // 3. Create inflation stream for compressed data
    z_stream strm_inflate;
    memset(&strm_inflate, 0, sizeof(z_stream));
    
    // Initialize inflation with automatic header detection (windowBits = 15 + 32)
    if (inflateInit2(&strm_inflate, 15 + 32) != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Allocate buffers for processing
    size_t decompress_size = size * 2;  // Estimate decompressed size
    if (decompress_size < 1024) decompress_size = 1024;
    if (decompress_size > 10 * 1024 * 1024) decompress_size = 10 * 1024 * 1024;
    
    Bytef *decompressed = new Bytef[decompress_size];
    Bytef *recompressed = new Bytef[decompress_size];
    Bytef *dict_buffer = new Bytef[32768];  // Max dictionary size
    uInt dict_len = 32768;
    
    // 4. Try to read from gzFile to check for errors
    char read_buffer[1024];
    int read_result = gzread(gz_in, read_buffer, sizeof(read_buffer));
    
    // 5. Get error from gzFile if any
    int gz_errnum;
    const char *gz_error_msg = gzerror(gz_in, &gz_errnum);
    
    // 6. Try inflation with the input data
    strm_inflate.next_in = (Bytef*)data;
    strm_inflate.avail_in = size > UINT_MAX ? UINT_MAX : (uInt)size;
    strm_inflate.next_out = decompressed;
    strm_inflate.avail_out = decompress_size;
    
    int inflate_result = inflate(&strm_inflate, Z_FINISH);
    
    // 7. Check sync point during inflation
    int sync_point_result = inflateSyncPoint(&strm_inflate);
    
    // 8. Create a copy of the inflation stream
    z_stream strm_inflate_copy;
    memset(&strm_inflate_copy, 0, sizeof(z_stream));
    int copy_result = inflateCopy(&strm_inflate_copy, &strm_inflate);
    
    // 9. Create deflation stream for recompression
    z_stream strm_deflate;
    memset(&strm_deflate, 0, sizeof(z_stream));
    
    if (deflateInit2(&strm_deflate, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                     15 + 16, 8, Z_DEFAULT_STRATEGY) == Z_OK) {
        
        // 10. Get dictionary from deflate stream
        int dict_result = deflateGetDictionary(&strm_deflate, dict_buffer, &dict_len);
        
        // 11. Calculate bound for compression
        uLong bound_size = deflateBound(&strm_deflate, strm_inflate.total_out);
        
        // 12. Recompress the decompressed data using compress2
        uLongf recompressed_len = decompress_size;
        if (strm_inflate.total_out > 0) {
            int compress_result = compress2(recompressed, &recompressed_len,
                                          decompressed, strm_inflate.total_out,
                                          Z_BEST_COMPRESSION);
        }
        
        // Clean up deflate stream
        deflateEnd(&strm_deflate);
    }
    
    // 13. Create gzFile for writing and use gzputc
    gzFile gz_out = gzopen("output_file", "wb");
    if (gz_out) {
        // Write a character
        gzputc(gz_out, 'X');
        
        // Write some of the decompressed data if available
        if (strm_inflate.total_out > 0 && strm_inflate.total_out < 100) {
            gzwrite(gz_out, decompressed, strm_inflate.total_out);
        }
        
        gzclose(gz_out);
    }
    
    // 14. Calculate Adler-32 checksums and combine them
    if (strm_inflate.total_out > 100) {
        // Split data into two parts for checksum calculation
        uLong adler1 = adler32(0L, Z_NULL, 0);
        uLong adler2 = adler32(0L, Z_NULL, 0);
        
        size_t split_point = strm_inflate.total_out / 2;
        adler1 = adler32(adler1, decompressed, split_point);
        adler2 = adler32(adler2, decompressed + split_point, 
                        strm_inflate.total_out - split_point);
        
        // Combine the checksums
        uLong combined_adler = adler32_combine64(adler1, adler2, 
                                                (off64_t)strm_inflate.total_out);
    }
    
    // Clean up resources
    if (copy_result == Z_OK) {
        inflateEnd(&strm_inflate_copy);
    }
    
    inflateEnd(&strm_inflate);
    gzclose(gz_in);
    
    delete[] decompressed;
    delete[] recompressed;
    delete[] dict_buffer;
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}