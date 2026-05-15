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
//<ID> 20
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["gzopen64","gzopen64","inflateInit2_","gzclose","inflateGetHeader","deflateInit2_","inflateEnd","gzclose","deflateBound_z","inflateEnd","deflateEnd","gzclose","gzread","gzdopen","gzwrite","gzflush","gzclose","inflate","inflateReset","deflate","deflateResetKeep","deflateGetDictionary","crc32_z","crc32_z","inflateEnd","deflateEnd","gzclose"],"critical_calls":["gzopen64","gzopen64","inflateInit2_","inflateGetHeader","deflateInit2_","deflateBound_z","gzread","gzdopen","gzwrite","gzflush","gzclose","inflate","inflateReset","deflate","deflateResetKeep","deflateGetDictionary","crc32_z","crc32_z","inflateEnd","deflateEnd","gzclose"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified zlib APIs to process compressed data:

*/


extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	  // Need enough data for meaningful operations

    // Create input file from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // 1. Open gzip file for reading using gzopen64
    gzFile gz_in = gzopen64("input_file", "rb");
    if (!gz_in) {
        // Create input file from our data first
        FILE *tmp_in = fopen("input_file", "wb");
        if (tmp_in) {
            fwrite(data, 1, size, tmp_in);
            assert_file_closed(&tmp_in);;
            gz_in = gzopen64("input_file", "rb");
        }
    }
    
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // 2. Initialize zlib streams
    z_stream strm_inflate, strm_deflate;
    Bytef *dict_buf = nullptr;
    uInt dict_len = 0;
    gz_header header;
    int ret;
    
    // Initialize inflate stream
    memset(&strm_inflate, 0, sizeof(z_stream));
    memset(&header, 0, sizeof(gz_header));
    ret = inflateInit2(&strm_inflate, 15 + 16);  // 15+16 for gzip decoding
    
    if (ret != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 3. Set up inflate header
    inflateGetHeader(&strm_inflate, &header);
    
    // 4. Initialize deflate stream
    memset(&strm_deflate, 0, sizeof(z_stream));
    ret = deflateInit2(&strm_deflate, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                      15 + 16, 8, Z_DEFAULT_STRATEGY);
    
    if (ret != Z_OK) {
        inflateEnd(&strm_inflate);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 5. Use deflateBound_z to estimate compressed size
    z_size_t source_len = size > 1000 ? 1000 : size;  // Use reasonable size
    z_size_t bound_size = deflateBound_z(&strm_deflate, source_len);
    
    // 6. Allocate buffers
    Bytef *in_buf = (Bytef *)malloc(size);
    Bytef *out_buf = (Bytef *)malloc(bound_size);
    Bytef *dict_out_buf = (Bytef *)malloc(32768);  // Max dictionary size
    
    if (!in_buf || !out_buf || !dict_out_buf) {
        free(in_buf);
        free(out_buf);
        free(dict_out_buf);
        inflateEnd(&strm_inflate);
        deflateEnd(&strm_deflate);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Copy input data to buffer
    memcpy(in_buf, data, size);
    
    // 7. Process data through gzFile interface
    // Read from gz file
    unsigned char read_buf[1024];
    int bytes_read = gzread(gz_in, read_buf, sizeof(read_buf));
    
    // Write to gz file (simulate)
    if (bytes_read > 0) {
        gzFile gz_out = gzdopen(out_fd, "wb");
        if (gz_out) {
            gzwrite(gz_out, read_buf, bytes_read);
            
            // 8. Use gzflush
            gzflush(gz_out, Z_SYNC_FLUSH);
            
            gzclose(gz_out);
        }
    }
    
    // 9. Process with inflate stream
    strm_inflate.next_in = in_buf;
    strm_inflate.avail_in = size > 100 ? 100 : size;  // Use portion of input
    
    // Set up output buffer for inflate
    Bytef inflate_out[4096];
    strm_inflate.next_out = inflate_out;
    strm_inflate.avail_out = sizeof(inflate_out);
    
    // Perform inflation
    ret = inflate(&strm_inflate, Z_NO_FLUSH);
    
    // 10. Use inflateReset
    if (ret == Z_OK || ret == Z_STREAM_END) {
        inflateReset(&strm_inflate);
    }
    
    // 11. Process with deflate stream
    strm_deflate.next_in = in_buf;
    strm_deflate.avail_in = size > 500 ? 500 : size;  // Use different portion
    
    strm_deflate.next_out = out_buf;
    strm_deflate.avail_out = bound_size;
    
    // Perform deflation
    ret = deflate(&strm_deflate, Z_FINISH);
    
    // 12. Use deflateResetKeep
    deflateResetKeep(&strm_deflate);
    
    // 13. Use deflateGetDictionary
    deflateGetDictionary(&strm_deflate, dict_out_buf, &dict_len);
    
    // 14. Calculate CRC32 using crc32_z
    uLong crc = crc32_z(0L, Z_NULL, 0);
    if (size > 50) {
        crc = crc32_z(crc, in_buf, size > 1000 ? 1000 : size);
    }
    
    // Cleanup
    inflateEnd(&strm_inflate);
    deflateEnd(&strm_deflate);
    gzclose(gz_in);
    
    free(in_buf);
    free(out_buf);
    free(dict_out_buf);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}