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
//<ID> 28
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["inflateInit2_","inflateEnd","inflateValidate","inflate","inflateEnd","gzdopen","inflateEnd","gzfwrite","gzdirect","gztell64","compressBound_z","compress2_z","deflateInit2_","deflateBound","deflate","deflateUsed","deflateEnd","crc32","crc32","crc32","crc32","crc32_combine64","gzclose","inflateEnd"],"critical_calls":["inflateInit2_","inflateValidate","inflate","gzdopen","gzfwrite","gzdirect","gztell64","compressBound_z","compress2_z","deflateInit2_","deflateBound","deflate","deflateUsed","deflateEnd","crc32","crc32","crc32","crc32","crc32_combine64","gzclose","inflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<53) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
	//fuzzer shim end}




    // 1. First, we'll try to decompress the input data (assuming it's zlib-compressed)
    // using inflateValidate to check validity
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    
    // Initialize inflation stream
    if (inflateInit2(&inflate_strm, MAX_WBITS) != Z_OK) {
        return 0;
    }
    
    // Set input data for inflation
    inflate_strm.next_in = (Bytef*)data;
    inflate_strm.avail_in = size;
    
    // Allocate output buffer for decompressed data
    size_t decompressed_size = size * 4;  // Reasonable guess
    Bytef* decompressed_data = (Bytef*)malloc(decompressed_size);
    if (!decompressed_data) {
        inflateEnd(&inflate_strm);
        return 0;
    }
    
    inflate_strm.next_out = decompressed_data;
    inflate_strm.avail_out = decompressed_size;
    
    // Validate the compressed data
    int validate_result = inflateValidate(&inflate_strm, fuzz_int32_t_1);
    // Note: inflateValidate may return Z_STREAM_ERROR if called at wrong time
    
    // Try to decompress
    int inflate_result = inflate(&inflate_strm, fuzz_int32_t_2);
    
    // Get actual decompressed size
    size_t actual_decompressed = decompressed_size - inflate_strm.avail_out;
    
    // 2. Create a gzip file and write the decompressed data using gzfwrite
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        free(decompressed_data);
        inflateEnd(&inflate_strm);
        assert_file_closed(&out_file);
	return 0;
    }
    
    int fd = fuzz_fileno(out_file);
    gzFile gz_out = gzdopen(fd, fuzz_str_3);
    if (!gz_out) {
        assert_file_closed(&out_file);;
        free(decompressed_data);
        inflateEnd(&inflate_strm);
        assert_file_closed(&out_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Write decompressed data using gzfwrite
    z_size_t items_written = 0;
    if (actual_decompressed > 0) {
        items_written = gzfwrite(decompressed_data, actual_decompressed, 1, gz_out);
    }
    
    // Check if file is in direct mode (not gzip compressed)
    int is_direct = gzdirect(gz_out);
    
    // Get current position in file
    off64_t position = gztell64(gz_out);
    
    // 3. Now recompress the decompressed data using compress2_z
    z_size_t recompressed_size = compressBound_z(actual_decompressed);
    Bytef* recompressed_data = (Bytef*)malloc(recompressed_size);
    if (recompressed_data && actual_decompressed > 0) {
        int compress_result = compress2_z(
            recompressed_data, &recompressed_size,
            decompressed_data, actual_decompressed,
            fuzz_int32_t_4
        );
    }
    
    // 4. Create a deflation stream to use deflateUsed
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    
    if (deflateInit2(&deflate_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                     MAX_WBITS, 8, Z_DEFAULT_STRATEGY) == Z_OK) {
        
        // Set input for deflation
        deflate_strm.next_in = decompressed_data;
        deflate_strm.avail_in = actual_decompressed;
        
        // Allocate output buffer
        size_t deflate_out_size = deflateBound(&deflate_strm, actual_decompressed);
        Bytef* deflate_out = (Bytef*)malloc(deflate_out_size);
        if (deflate_out) {
            deflate_strm.next_out = deflate_out;
            deflate_strm.avail_out = deflate_out_size;
            
            // Perform deflation
            int deflate_result = deflate(&deflate_strm, fuzz_int32_t_5);
            
            // Use deflateUsed to get bits used
            int bits_used = 0;
            int used_result = deflateUsed(&deflate_strm, &bits_used);
            
            free(deflate_out);
        }
        
        deflateEnd(&deflate_strm);
    }
    
    // 5. Combine CRC32 values using crc32_combine64
    uLong crc1 = crc32(fuzz_uint64_t_6, Z_NULL, 0);
    uLong crc2 = crc32(fuzz_uint64_t_7, Z_NULL, 0);
    
    if (actual_decompressed > 0) {
        crc1 = crc32(fuzz_uint64_t_8, decompressed_data, actual_decompressed);
        
        // Create another CRC from first half of data
        size_t half_size = actual_decompressed / 2;
        if (half_size > 0) {
            crc2 = crc32(fuzz_uint64_t_9, decompressed_data, half_size);
        }
    }
    
    // Combine the two CRC values
    uLong combined_crc = crc32_combine64(crc1, crc2, (off64_t)actual_decompressed);
    
    // 6. Cleanup
    gzclose(gz_out);  // This also closes the underlying file descriptor
    // Don't call assert_file_closed(&out_file); as gzclose already closed it
    
    free(decompressed_data);
    if (recompressed_data) {
        free(recompressed_data);
    }
    
    inflateEnd(&inflate_strm);
    
    assert_file_closed(&out_file);
	assert_fd_closed(fd);
	return 0;
}