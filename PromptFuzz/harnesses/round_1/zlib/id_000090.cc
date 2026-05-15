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
//<ID> 267
//<Prompt> []
/*<Combination>: [
*/
//<score> 27, nr_unique_branch: 2
//<Quality> {"density":9,"unique_branches":{"inflateCodesUsed":[[1410,9,1410,32,0,0,4,0]],"inflateStateCheck":[[90,9,90,23,0,0,4,0]]},"library_calls":["gzdopen","gzread","zError","inflateCodesUsed","inflateInit2_","zError","gzclose","inflateEnd","deflateInit2_","zError","gzclose","compressBound","deflateBound_z","deflateEnd","gzclose","deflate","zError","deflateResetKeep","gzdopen","gzputc","crc32_combine_gen64","deflateEnd","gzclose","gzclose"],"critical_calls":["gzdopen","gzread","zError","inflateCodesUsed","inflateInit2_","inflateEnd","deflateInit2_","compressBound","deflateBound_z","deflate","zError","deflateResetKeep","gzdopen","gzputc","crc32_combine_gen64","deflateEnd","gzclose","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_90(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create FILE* for output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);

    // Variables for error checking
    int ret;
    const char *err_msg;

    // Event: Decompress input data using gzdopen and gzread,
    // then recompress it using deflate APIs

    // 1. Decompress using gzdopen and gzread
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

    // Read decompressed data
    unsigned char decompressed[4096];
    unsigned int decompressed_size = 0;
    
    while ((ret = gzread(gz_in, decompressed, sizeof(decompressed))) > 0) {
        decompressed_size += ret;
    }

    // Check for read errors
    if (ret < 0) {
        err_msg = zError(ret);
    }

    // Get remaining codes used in inflate stream
    unsigned long codes_used = inflateCodesUsed(nullptr);

    // 2. Setup inflate stream for analysis (even though we already read via gzread)
    z_stream strm_inflate;
    memset(&strm_inflate, 0, sizeof(strm_inflate));
    
    ret = inflateInit2_(&strm_inflate, 15 + 16, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        err_msg = zError(ret);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Clean up inflate stream
    inflateEnd(&strm_inflate);

    // 3. Setup deflate stream for recompression
    z_stream strm_deflate;
    memset(&strm_deflate, 0, sizeof(z_stream));
    
    ret = deflateInit2_(&strm_deflate, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                        15 + 16, 8, Z_DEFAULT_STRATEGY,
                        ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        err_msg = zError(ret);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Calculate bound for compressed output
    uLong bound1 = compressBound(decompressed_size);
    z_size_t bound2 = deflateBound_z(&strm_deflate, decompressed_size);

    // Allocate output buffer
    unsigned char *compressed = new unsigned char[bound1];
    if (!compressed) {
        deflateEnd(&strm_deflate);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Prepare deflate stream
    strm_deflate.next_in = decompressed;
    strm_deflate.avail_in = decompressed_size;
    strm_deflate.next_out = compressed;
    strm_deflate.avail_out = bound1;

    // Compress the data
    ret = deflate(&strm_deflate, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK) {
        err_msg = zError(ret);
    }

    // Test deflateResetKeep
    deflateResetKeep(&strm_deflate);

    // Write compressed data to output file using gzFile
    gzFile gz_out = gzdopen(fd_out, "wb");
    if (gz_out) {
        // Write a single character first
        if (strm_deflate.total_out > 0) {
            gzputc(gz_out, compressed[0]);
        }
        
        // Write the rest of compressed data
        if (strm_deflate.total_out > 1) {
            fwrite(compressed + 1, 1, strm_deflate.total_out - 1, out_file);
        }
    }

    // Generate CRC combine value (demonstrative use)
    uLong crc_gen = crc32_combine_gen64(static_cast<off64_t>(size));

    // Clean up resources
    delete[] compressed;
    deflateEnd(&strm_deflate);
    
    if (gz_out) {
        gzclose(gz_out);
    }
    
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}