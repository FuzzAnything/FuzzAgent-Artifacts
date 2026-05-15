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
//<ID> 76
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["inflateInit2_","inflateEnd","inflateEnd","inflate","inflateEnd","adler32_z","adler32_z","adler32_combine","inflateEnd","deflateInit2_","deflateSetHeader","deflateEnd","deflateParams","deflateEnd","deflateGetDictionary","deflateBound","deflateEnd","deflate","deflateEnd","deflateEnd","gzopen","gzwrite","gzclose_w","gzoffset64","gzclose_w","gzopen","gzoffset64","gzclose"],"critical_calls":["inflateInit2_","inflate","adler32_z","adler32_z","adler32_combine","inflateEnd","deflateInit2_","deflateSetHeader","deflateParams","deflateGetDictionary","deflateBound","deflate","deflateEnd","gzopen","gzwrite","gzoffset64","gzclose_w","gzopen","gzoffset64","gzclose"],"visited":1}
/*Here's a fuzz driver that uses all the specified zlib APIs to achieve an event where zlib-compressed data is decompressed, processed, and recompressed with various operations:

*/


extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 4) {
	return 0;
	}
	

    // Create input and output files
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

    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);

    z_stream strm_inflate, strm_deflate;
    Bytef *inflate_buf = nullptr;
    Bytef *deflate_buf = nullptr;
    gzFile gz_file = nullptr;
    gz_header header;
    int ret;
    
    // Initialize inflate stream for decompression
    memset(&strm_inflate, 0, sizeof(z_stream));
    ret = inflateInit2_(&strm_inflate, MAX_WBITS + 16, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Read compressed data from file
    fseek(in_file, 0, SEEK_END);
    long input_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    Bytef *compressed_data = (Bytef*)malloc(input_size);
    if (!compressed_data) {
        inflateEnd(&strm_inflate);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    fread(compressed_data, 1, input_size, in_file);
    
    // Set up inflate stream
    strm_inflate.avail_in = (uInt)input_size;
    strm_inflate.next_in = compressed_data;
    
    // Allocate buffer for decompressed data
    size_t inflate_buf_size = input_size * 4;
    inflate_buf = (Bytef*)malloc(inflate_buf_size);
    if (!inflate_buf) {
        free(compressed_data);
        inflateEnd(&strm_inflate);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    strm_inflate.avail_out = (uInt)inflate_buf_size;
    strm_inflate.next_out = inflate_buf;
    
    // Decompress the data
    ret = inflate(&strm_inflate, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK) {
        free(inflate_buf);
        free(compressed_data);
        inflateEnd(&strm_inflate);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    size_t decompressed_size = inflate_buf_size - strm_inflate.avail_out;
    
    // Compute Adler-32 checksum of decompressed data
    uLong adler1 = adler32_z(0L, inflate_buf, decompressed_size);
    
    // Split data for Adler-32 combine
    size_t half_size = decompressed_size / 2;
    if (half_size > 0) {
        uLong adler2 = adler32_z(0L, inflate_buf + half_size, decompressed_size - half_size);
        // Combine checksums
        uLong combined_adler = adler32_combine(adler1, adler2, decompressed_size - half_size);
        (void)combined_adler; // Prevent unused variable warning
    }
    
    // End inflation
    inflateEnd(&strm_inflate);
    
    // Initialize deflate stream for recompression
    memset(&strm_deflate, 0, sizeof(z_stream));
    ret = deflateInit2_(&strm_deflate, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                       MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY,
                       ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        free(inflate_buf);
        free(compressed_data);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Set gzip header
    memset(&header, 0, sizeof(gz_header));
    header.text = 1;
    header.os = 255; // Unknown OS
    ret = deflateSetHeader(&strm_deflate, &header);
    if (ret != Z_OK) {
        deflateEnd(&strm_deflate);
        free(inflate_buf);
        free(compressed_data);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Change compression parameters
    ret = deflateParams(&strm_deflate, Z_BEST_COMPRESSION, Z_FILTERED);
    if (ret != Z_OK) {
        deflateEnd(&strm_deflate);
        free(inflate_buf);
        free(compressed_data);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Get dictionary (even if empty)
    Bytef dict_buf[256];
    uInt dict_len = 256;
    deflateGetDictionary(&strm_deflate, dict_buf, &dict_len);
    
    // Allocate buffer for recompressed data
    size_t deflate_buf_size = deflateBound(&strm_deflate, decompressed_size);
    deflate_buf = (Bytef*)malloc(deflate_buf_size);
    if (!deflate_buf) {
        deflateEnd(&strm_deflate);
        free(inflate_buf);
        free(compressed_data);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Compress the decompressed data
    strm_deflate.avail_in = (uInt)decompressed_size;
    strm_deflate.next_in = inflate_buf;
    strm_deflate.avail_out = (uInt)deflate_buf_size;
    strm_deflate.next_out = deflate_buf;
    
    ret = deflate(&strm_deflate, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK) {
        deflateEnd(&strm_deflate);
        free(deflate_buf);
        free(inflate_buf);
        free(compressed_data);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    size_t recompressed_size = deflate_buf_size - strm_deflate.avail_out;
    
    // End deflation
    deflateEnd(&strm_deflate);
    
    // Open gzip file for writing
    gz_file = gzopen("input_file", "wb");
    if (!gz_file) {
        free(deflate_buf);
        free(inflate_buf);
        free(compressed_data);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Write recompressed data to gzip file
    int write_ret = gzwrite(gz_file, deflate_buf, recompressed_size);
    if (write_ret <= 0) {
        gzclose_w(gz_file);
        free(deflate_buf);
        free(inflate_buf);
        free(compressed_data);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Get current offset in gzip file
    off64_t offset = gzoffset64(gz_file);
    (void)offset; // Prevent unused variable warning
    
    // Close gzip file for writing
    gzclose_w(gz_file);
    
    // Now open the gzip file for reading
    gz_file = gzopen("input_file", "rb");
    if (gz_file) {
        // Get current offset (should be 0 at beginning)
        offset = gzoffset64(gz_file);
        (void)offset;
        
        // Close gzip file
        gzclose(gz_file);
    }
    
    // Write recompressed data to output file
    fwrite(deflate_buf, 1, recompressed_size, out_file);
    
    // Clean up
    free(deflate_buf);
    free(inflate_buf);
    free(compressed_data);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove temporary file
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}