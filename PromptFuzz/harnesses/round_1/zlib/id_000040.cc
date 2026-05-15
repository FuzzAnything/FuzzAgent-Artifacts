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
//<ID> 78
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 2
//<Quality> {"density":12,"unique_branches":{"deflate":[[1204,13,1204,28,0,0,4,0]],"deflate_huff":[[1644,8,1644,31,5,0,4,0]]},"library_calls":["gzdopen","gzdopen","gzclose","gzclose_w","inflateInit2_","gzclose","gzclose_w","deflateInit2_","inflateEnd","gzclose","gzclose_w","deflateSetHeader","inflate","deflateGetDictionary","deflateParams","deflate","gzwrite","adler32_z","adler32_z","adler32_combine","gzoffset64","deflateEnd","inflateEnd","gzclose","gzclose_w"],"critical_calls":["gzdopen","gzdopen","inflateInit2_","deflateInit2_","deflateSetHeader","inflate","deflateGetDictionary","deflateParams","deflate","gzwrite","adler32_z","adler32_z","adler32_combine","gzoffset64","deflateEnd","inflateEnd","gzclose","gzclose_w"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if no data
    if (size == 0) {
	return 0;
	}
	
    
    // Open input file for reading using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Open output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 1: Get file descriptors
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    
    // Step 2: Open gzip files using file descriptors
    gzFile gz_in = gzdopen(fd_in, "rb");
    gzFile gz_out = gzdopen(fd_out, "wb");
    if (!gz_in || !gz_out) {
        if (gz_in) gzclose(gz_in);
        if (gz_out) gzclose_w(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Step 3: Initialize zlib streams
    z_stream strm_inflate, strm_deflate;
    
    // Initialize inflate with gzip header detection
    int ret = inflateInit2_(&strm_inflate, MAX_WBITS + 16, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_in);
        gzclose_w(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Initialize deflate with gzip format
    ret = deflateInit2_(&strm_deflate, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                       MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY,
                       ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        inflateEnd(&strm_inflate);
        gzclose(gz_in);
        gzclose_w(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Step 4: Set gzip header for deflate
    gz_header gzheader;
    memset(&gzheader, 0, sizeof(gz_header));
    gzheader.os = 255;  // Unknown OS
    deflateSetHeader(&strm_deflate, &gzheader);
    
    // Step 5: Prepare buffers
    Bytef *in_buf = new Bytef[size];
    size_t bytes_read = fread(in_buf, 1, size, in_file);
    
    // Setup inflate input
    strm_inflate.next_in = in_buf;
    strm_inflate.avail_in = static_cast<uInt>(bytes_read);
    
    // Create output buffer for inflated data
    Bytef *out_buf = new Bytef[size * 2];
    strm_inflate.next_out = out_buf;
    strm_inflate.avail_out = static_cast<uInt>(size * 2);
    
    // Step 6: Inflate the data
    ret = inflate(&strm_inflate, Z_FINISH);
    
    // Step 7: Get dictionary if available
    Bytef dict_buf[32768];
    uInt dict_len = sizeof(dict_buf);
    deflateGetDictionary(&strm_deflate, dict_buf, &dict_len);
    
    // Step 8: Set deflate parameters
    deflateParams(&strm_deflate, Z_BEST_COMPRESSION, Z_HUFFMAN_ONLY);
    
    // Step 9: Setup deflate with inflated data
    strm_deflate.next_in = out_buf;
    strm_deflate.avail_in = static_cast<uInt>(strm_inflate.total_out);
    
    // Create buffer for deflated output
    Bytef *deflated_buf = new Bytef[size * 2];
    strm_deflate.next_out = deflated_buf;
    strm_deflate.avail_out = static_cast<uInt>(size * 2);
    
    // Step 10: Deflate the inflated data
    ret = deflate(&strm_deflate, Z_FINISH);
    
    // Step 11: Write deflated data to gzip file
    uLong bytes_to_write = strm_deflate.total_out;
    int write_ret = gzwrite(gz_out, deflated_buf, static_cast<unsigned int>(bytes_to_write));
    
    // Step 12: Calculate Adler-32 checksums
    uLong adler1 = adler32_z(0L, in_buf, static_cast<z_size_t>(size/2));
    uLong adler2 = adler32_z(0L, in_buf + size/2, static_cast<z_size_t>(size - size/2));
    uLong combined_adler = adler32_combine(adler1, adler2, static_cast<off_t>(size));
    
    // Step 13: Get file offset
    off64_t offset = gzoffset64(gz_in);
    
    // Step 14: Clean up resources
    deflateEnd(&strm_deflate);
    inflateEnd(&strm_inflate);
    
    gzclose(gz_in);      // This also closes the file descriptor
    gzclose_w(gz_out);   // This also closes the file descriptor
    
    // Note: Do not fclose in_file and out_file as gzdopen took ownership of file descriptors
    // The file descriptors are closed by gzclose/gzclose_w
    
    delete[] in_buf;
    delete[] out_buf;
    delete[] deflated_buf;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}