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
//<ID> 183
//<Prompt> []
/*<Combination>: [
*/
//<score> 33, nr_unique_branch: 3
//<Quality> {"density":11,"unique_branches":{"gz_vacate":[[386,9,386,66,0,0,4,1],[389,9,389,28,0,0,4,0]],"gzputc":[[331,13,331,32,0,0,4,0]]},"library_calls":["inflateInit2_","inflateEnd","inflateEnd","inflateValidate","inflate","inflateSync","inflateReset","inflate","inflateGetDictionary","deflateInit_","inflateEnd","deflateGetDictionary","compressBound_z","compress_z","compress2_z","gzdopen","gzwrite","gzputc","gzputc","gzungetc","gzprintf","gzclose","deflateEnd","inflateEnd"],"critical_calls":["inflateInit2_","inflateValidate","inflate","inflateSync","inflateReset","inflate","inflateGetDictionary","deflateInit_","deflateGetDictionary","compressBound_z","compress_z","compress2_z","gzdopen","gzwrite","gzputc","gzputc","gzungetc","gzprintf","gzclose","deflateEnd","inflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create input file from the fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file for compressed data
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

    // Step 1: Initialize inflation stream for decompression
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    
    // Use inflateInit2_ with negative windowBits for raw decompression
    int ret = inflateInit2_(&inflate_strm, -MAX_WBITS, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Set up input buffer for inflation
    Bytef *inflate_input = (Bytef *)malloc(size);
    if (!inflate_input) {
        inflateEnd(&inflate_strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    memcpy(inflate_input, data, size);
    inflate_strm.next_in = inflate_input;
    inflate_strm.avail_in = size;

    // Allocate output buffer for decompressed data
    size_t decompressed_size = size * 4;  // Reasonable initial guess
    Bytef *decompressed = (Bytef *)malloc(decompressed_size);
    if (!decompressed) {
        free(inflate_input);
        inflateEnd(&inflate_strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    inflate_strm.next_out = decompressed;
    inflate_strm.avail_out = decompressed_size;

    // Step 2: Perform inflation with validation enabled
    inflateValidate(&inflate_strm, 1);
    
    // Inflate the data
    ret = inflate(&inflate_strm, Z_FINISH);
    
    // Try sync if inflation didn't complete
    if (ret == Z_BUF_ERROR || ret == Z_DATA_ERROR) {
        inflateSync(&inflate_strm);
        // Reset and try again
        inflateReset(&inflate_strm);
        inflate_strm.next_in = inflate_input;
        inflate_strm.avail_in = size;
        inflate_strm.next_out = decompressed;
        inflate_strm.avail_out = decompressed_size;
        ret = inflate(&inflate_strm, Z_SYNC_FLUSH);
    }

    size_t decompressed_actual = decompressed_size - inflate_strm.avail_out;

    // Step 3: Get dictionary from inflation stream
    Bytef inflate_dict[32768];
    uInt inflate_dict_len = sizeof(inflate_dict);
    inflateGetDictionary(&inflate_strm, inflate_dict, &inflate_dict_len);

    // Step 4: Initialize deflation stream for recompression
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    
    ret = deflateInit_(&deflate_strm, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        free(decompressed);
        free(inflate_input);
        inflateEnd(&inflate_strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 5: Get dictionary from deflation stream
    Bytef deflate_dict[32768];
    uInt deflate_dict_len = sizeof(deflate_dict);
    deflateGetDictionary(&deflate_strm, deflate_dict, &deflate_dict_len);

    // Step 6: Calculate compression bound and compress using compress_z
    z_size_t compress_bound = compressBound_z(decompressed_actual);
    Bytef *compressed1 = (Bytef *)malloc(compress_bound);
    if (compressed1) {
        z_size_t compressed_len1 = compress_bound;
        compress_z(compressed1, &compressed_len1, decompressed, decompressed_actual);
        free(compressed1);
    }

    // Step 7: Compress using compress2_z with different level
    Bytef *compressed2 = (Bytef *)malloc(compress_bound);
    if (compressed2) {
        z_size_t compressed_len2 = compress_bound;
        compress2_z(compressed2, &compressed_len2, decompressed, decompressed_actual, Z_BEST_COMPRESSION);
        free(compressed2);
    }

    // Step 8: Use gzFile functions
    gzFile gz_out = gzdopen(out_fd, "wb");
    if (gz_out) {
        // Write decompressed data using gzwrite
        if (decompressed_actual > 0) {
            gzwrite(gz_out, decompressed, decompressed_actual);
        }
        
        // Use gzputc to add a character
        gzputc(gz_out, '\n');
        
        // Use gzungetc (though it typically works with reading)
        // We'll simulate by writing and then "ungetting" (though not standard for writing)
        int written_char = 'X';
        gzputc(gz_out, written_char);
        // Note: gzungetc is for input streams, but we'll call it anyway
        gzungetc(written_char, gz_out);
        
        // Use gzprintf to write formatted data
        gzprintf(gz_out, "Processed %zu bytes\n", decompressed_actual);
        
        // Close gz file
        gzclose(gz_out);
    } else {
        // If gzdopen failed, we still need to close the regular file
        assert_file_closed(&out_file);;
    }

    // Step 9: Clean up all resources
    deflateEnd(&deflate_strm);
    inflateEnd(&inflate_strm);
    free(decompressed);
    free(inflate_input);
    assert_file_closed(&in_file);;
    
    // Note: out_file was closed by gzclose if gzdopen succeeded,
    // otherwise we already closed it above
    if (gz_out) {
        // out_file is already closed by gzclose
    } else {
        // We already closed out_file in the else block above
    }

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}