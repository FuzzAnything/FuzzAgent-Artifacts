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
//<ID> 293
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 2
//<Quality> {"density":18,"unique_branches":{"inflate":[[1078,21,1078,38,0,0,4,1],[1082,21,1082,38,0,0,4,1]]},"library_calls":["deflateInit2_","deflateTune","deflateSetHeader","deflateGetDictionary","deflateBound","deflateEnd","inflateInit2_","deflateEnd","inflateInit_","inflateValidate","deflate","inflateEnd","inflateEnd","deflateEnd","inflate","adler32_z","adler32_z","adler32_combine","gzdopen","gzungetc","gzclose","gzdopen","gzclose","deflateEnd","inflateEnd","inflateEnd"],"critical_calls":["deflateInit2_","deflateTune","deflateSetHeader","deflateGetDictionary","deflateBound","inflateInit2_","inflateInit_","inflateValidate","deflate","inflate","adler32_z","adler32_z","adler32_combine","gzdopen","gzungetc","gzclose","gzdopen","gzclose","deflateEnd","inflateEnd","inflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_96(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if input is too small for meaningful processing
    if (size < 10) {
	return 0;
	}
	
    
    // Prepare input and output FILE* streams
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
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // --- DEFLATION SETUP ---
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    
    // Initialize deflation with custom parameters
    int windowBits = 15;
    int memLevel = 8;
    int strategy = Z_DEFAULT_STRATEGY;
    
    if (deflateInit2_(&def_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                      windowBits, memLevel, strategy,
                      ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Tune deflation parameters using data-derived values
    deflateTune(&def_strm,
                data[0] % 256,     // good_length
                data[1] % 256,     // max_lazy
                data[2] % 256,     // nice_length
                data[3] % 256);    // max_chain
    
    // Set gzip header if enough data available
    gz_header gz_head;
    memset(&gz_head, 0, sizeof(gz_head));
    if (size > 20) {
        gz_head.text = data[4] & 1;
        gz_head.time = (data[5] << 24) | (data[6] << 16) | (data[7] << 8) | data[8];
        deflateSetHeader(&def_strm, &gz_head);
    }
    
    // Get dictionary if available
    Bytef dict_buf[256];
    uInt dict_len = 0;
    deflateGetDictionary(&def_strm, dict_buf, &dict_len);
    
    // Calculate bound for compressed output
    uLong bound = deflateBound(&def_strm, size);
    Bytef *compressed = (Bytef*)malloc(bound);
    if (!compressed) {
        deflateEnd(&def_strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // --- INFLATION SETUP ---
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    
    // Initialize inflation with automatic header detection
    if (inflateInit2_(&inf_strm, 15 + 32, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        free(compressed);
        deflateEnd(&def_strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Also initialize with basic inflateInit for comparison
    z_stream inf_strm2;
    memset(&inf_strm2, 0, sizeof(inf_strm2));
    inflateInit_(&inf_strm2, ZLIB_VERSION, sizeof(z_stream));
    
    // Set validation
    inflateValidate(&inf_strm, data[9] & 1);
    
    // --- PROCESS DATA ---
    // First pass: compress input data
    def_strm.next_in = (Bytef*)data;
    def_strm.avail_in = size;
    def_strm.next_out = compressed;
    def_strm.avail_out = bound;
    
    int def_ret = deflate(&def_strm, Z_FINISH);
    size_t compressed_size = bound - def_strm.avail_out;
    
    // Second pass: decompress the compressed data
    Bytef *decompressed = (Bytef*)malloc(size);
    if (!decompressed) {
        free(compressed);
        inflateEnd(&inf_strm);
        inflateEnd(&inf_strm2);
        deflateEnd(&def_strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    inf_strm.next_in = compressed;
    inf_strm.avail_in = compressed_size;
    inf_strm.next_out = decompressed;
    inf_strm.avail_out = size;
    
    int inf_ret = inflate(&inf_strm, Z_FINISH);
    
    // --- CHECKSUM CALCULATIONS ---
    // Calculate Adler-32 checksum of original data
    uLong adler1 = adler32_z(0L, data, size);
    
    // Calculate Adler-32 checksum of decompressed data
    size_t decompressed_size = size - inf_strm.avail_out;
    uLong adler2 = adler32_z(0L, decompressed, decompressed_size);
    
    // Combine checksums
    off_t combine_len = (off_t)(size + decompressed_size);
    uLong combined_adler = adler32_combine(adler1, adler2, combine_len);
    
    (void)combined_adler; // Prevent unused variable warning
    
    // --- GZFILE OPERATIONS ---
    // Create gzFile from file descriptor
    gzFile gz_in = gzdopen(in_fd, "rb");
    if (gz_in) {
        // Try to unget a character
        int c = gzgetc(gz_in);
        if (c != -1) {
            gzungetc(c, gz_in);
        }
        gzclose(gz_in);
    } else {
        // If gzdopen failed, we still need to close the FILE*
        assert_file_closed(&in_file);;
    }
    
    // Another gzFile for output (though we won't write much)
    gzFile gz_out = gzdopen(out_fd, "wb");
    if (gz_out) {
        gzclose(gz_out);
    } else {
        assert_file_closed(&out_file);;
    }
    
    // --- CLEANUP ---
    free(compressed);
    free(decompressed);
    deflateEnd(&def_strm);
    inflateEnd(&inf_strm);
    inflateEnd(&inf_strm2);
    
    // Close files if not already closed by gzclose
    if (gz_in) {
        // gzclose already closed in_file via the file descriptor
        // so we don't need to assert_file_closed(&in_file); again
    }
    if (gz_out) {
        // Similarly for out_file
    } else {
        // If gzdopen failed for output, ensure file is closed
        if (out_file) assert_file_closed(&out_file);;
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}