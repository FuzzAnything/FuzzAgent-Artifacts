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
//<ID> 189
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 6
//<Quality> {"density":8,"unique_branches":{"deflate_stored":[[1689,13,1689,38,0,0,4,0],[1802,37,1802,70,0,0,4,1],[1810,13,1810,36,0,0,4,0],[1837,37,1837,59,0,0,4,1],[1651,20,1651,29,15,0,4,0],[1651,20,1651,29,19,0,4,0]]},"library_calls":["gzdopen","inflateInit_","gzclose","inflateGetHeader","inflateCopy","inflateEnd","gzclose","inflateSetDictionary","inflate","uncompress2_z","crc32_z","deflateInit2_","deflate","deflateEnd","inflateEnd","inflateEnd","gzclose"],"critical_calls":["gzdopen","inflateInit_","inflateGetHeader","inflateCopy","inflateSetDictionary","inflate","uncompress2_z","crc32_z","deflateInit2_","deflate","deflateEnd","inflateEnd","inflateEnd","gzclose"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified zlib APIs to process compressed input data:

*/


extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimum data for meaningful operations

    // 1. Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Create gzFile from the file descriptor
    int fd = fuzz_fileno(in_file);
    gzFile gz_file = gzdopen(fd, "rb");
    if (!gz_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 3. Use gzgetc to read first byte
    int first_byte = gzgetc(gz_file);
    (void)first_byte; // Mark as used
    
    // 4. Initialize z_stream for inflation
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Use inflateInit_ (required API)
    if (inflateInit_(&strm, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 5. Set up gz_header for header extraction
    gz_header header;
    memset(&header, 0, sizeof(header));
    
    // Use inflateGetHeader (required API)
    if (inflateGetHeader(&strm, &header) != Z_OK) {
        // Continue anyway - not all compressed data has headers
    }
    
    // 6. Prepare input for inflation
    strm.next_in = (Bytef *)data;
    strm.avail_in = size;
    
    // 7. Use inflateCopy (required API) to create a copy
    z_stream strm_copy;
    if (inflateCopy(&strm_copy, &strm) != Z_OK) {
        inflateEnd(&strm);
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 8. Try to set dictionary if we have enough data
    // Use inflateSetDictionary (required API)
    if (size > 100) {
        Bytef dictionary[100];
        memcpy(dictionary, data, 100);
        inflateSetDictionary(&strm, dictionary, 100);
    }
    
    // 9. Allocate output buffer for inflation
    size_t out_size = size * 10;  // Assume expansion factor of 10
    if (out_size > 10 * 1024 * 1024) out_size = 10 * 1024 * 1024;  // Cap at 10MB
    std::vector<Bytef> out_buf(out_size);
    
    strm.next_out = out_buf.data();
    strm.avail_out = out_size;
    
    // 10. Perform inflation
    int inflate_result = inflate(&strm, Z_FINISH);
    size_t inflated_size = out_size - strm.avail_out;
    
    // 11. Use uncompress2_z (required API) as alternative decompression method
    z_size_t dest_len = out_size;
    z_size_t src_len = size;
    std::vector<Bytef> out_buf2(out_size);
    
    // Only try if we have valid compressed data
    if (inflate_result == Z_STREAM_END || inflate_result == Z_OK) {
        int uncompress_result = uncompress2_z(out_buf2.data(), &dest_len, 
                                             (const Bytef *)data, &src_len);
        (void)uncompress_result; // Mark as used
    }
    
    // 12. Use crc32_z (required API) to calculate CRC of inflated data
    uLong crc = 0;
    if (inflated_size > 0) {
        crc = crc32_z(crc, out_buf.data(), inflated_size);
    }
    
    // 13. Use deflateInit2_ (required API) for re-compression
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    
    // Try different compression levels
    for (int level = 0; level <= 9; level += 3) {
        if (deflateInit2_(&deflate_strm, level, Z_DEFLATED, 
                         MAX_WBITS, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY,
                         ZLIB_VERSION, sizeof(z_stream)) == Z_OK) {
            
            // Prepare input for deflation (use inflated data if available)
            if (inflated_size > 0) {
                deflate_strm.next_in = out_buf.data();
                deflate_strm.avail_in = inflated_size;
            } else {
                deflate_strm.next_in = (Bytef *)data;
                deflate_strm.avail_in = size;
            }
            
            // Allocate output buffer for deflation
            size_t deflate_out_size = size * 2;
            std::vector<Bytef> deflate_out_buf(deflate_out_size);
            deflate_strm.next_out = deflate_out_buf.data();
            deflate_strm.avail_out = deflate_out_size;
            
            // Perform deflation
            int deflate_result = deflate(&deflate_strm, Z_FINISH);
            (void)deflate_result; // Mark as used
            
            // Clean up deflate stream
            deflateEnd(&deflate_strm);
            
            // Break after one successful compression
            if (deflate_result == Z_STREAM_END || deflate_result == Z_OK) {
                break;
            }
        }
    }
    
    // 14. Clean up all resources
    inflateEnd(&strm_copy);
    inflateEnd(&strm);
    gzclose(gz_file);
    assert_file_closed(&in_file);;
    
    // 15. Create output file (for demonstration)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        if (inflated_size > 0) {
            fwrite(out_buf.data(), 1, inflated_size, out_file);
        }
        assert_file_closed(&out_file);;
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}