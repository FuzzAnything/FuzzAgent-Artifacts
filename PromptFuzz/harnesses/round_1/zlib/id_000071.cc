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
//<ID> 176
//<Prompt> []
/*<Combination>: [
*/
//<score> 52.5, nr_unique_branch: 13
//<Quality> {"density":15,"unique_branches":{"deflateBound_z":[[917,28,917,49,0,0,4,0],[918,17,918,42,0,0,4,0],[918,46,918,54,0,0,4,0],[920,16,920,39,0,0,4,1]],"gz_comp":[[116,21,116,29,0,0,4,0],[117,25,117,40,0,0,4,1],[117,44,117,64,0,0,4,1]],"gzoffset64":[[482,9,482,21,0,0,4,0]],"gzclose_w":[[685,9,685,39,0,0,4,0],[696,9,696,31,0,0,4,0]],"gz_write":[[245,17,245,26,0,0,4,0],[246,24,246,36,0,0,4,1]],"deflateInit2_":[[434,25,434,49,0,0,4,0]]},"library_calls":["inflateInit2_","inflateValidate","inflateEnd","inflate","adler32_z","adler32_z","adler32_z","adler32_z","adler32_combine","deflateInit2_","inflateEnd","deflateTune","deflateSetHeader","deflateGetDictionary","deflateBound","deflateEnd","inflateEnd","deflate","deflateEnd","inflateEnd","gzdopen","deflateEnd","inflateEnd","gzwrite","gzoffset64","gzclose_w","gzdopen","gzclose","deflateEnd","inflateEnd"],"critical_calls":["inflateInit2_","inflateValidate","inflate","adler32_z","adler32_z","adler32_z","adler32_z","adler32_combine","deflateInit2_","deflateTune","deflateSetHeader","deflateGetDictionary","deflateBound","deflate","gzdopen","gzwrite","gzoffset64","gzclose_w","gzdopen","gzclose","deflateEnd","inflateEnd"],"visited":1}
/*Here's a comprehensive fuzz driver that uses all the specified zlib APIs to achieve the event of decompressing input data, validating it, re-compressing it with various settings, and writing the result to a gzip file:

*/


extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Use input data as zlib-compressed data
    const Bytef *compressed_data = data;
    uLong compressed_size = (uLong)size;

    // Step 1: Decompression setup
    z_stream decomp_strm;
    memset(&decomp_strm, 0, sizeof(decomp_strm));
    
    // Initialize decompression with window bits for automatic header detection
    int window_bits = MAX_WBITS + 32; // Enable gzip/zlib header detection
    if (inflateInit2_(&decomp_strm, window_bits, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        return 0;
    }

    // Enable validation during decompression
    inflateValidate(&decomp_strm, 1);

    // Step 2: Prepare decompression buffers
    uLong decomp_bound = compressed_size * 10; // Heuristic for decompressed size
    if (decomp_bound < 100) decomp_bound = 100;
    
    Bytef *decompressed = (Bytef *)malloc(decomp_bound);
    if (!decompressed) {
        inflateEnd(&decomp_strm);
        return 0;
    }

    // Step 3: Decompress the input data
    decomp_strm.next_in = (Bytef *)compressed_data;
    decomp_strm.avail_in = compressed_size;
    decomp_strm.next_out = decompressed;
    decomp_strm.avail_out = decomp_bound;

    int decomp_result = inflate(&decomp_strm, Z_FINISH);
    uLong decompressed_size = decomp_bound - decomp_strm.avail_out;

    // Step 4: Compute Adler-32 checksum
    uLong adler1 = adler32_z(0L, Z_NULL, 0);
    uLong adler2 = adler32_z(adler1, decompressed, (z_size_t)decompressed_size);
    
    // Simulate combining checksums (split data in half)
    uLong half_point = decompressed_size / 2;
    uLong adler_first = adler32_z(0L, decompressed, (z_size_t)half_point);
    uLong adler_second = adler32_z(0L, decompressed + half_point, 
                                  (z_size_t)(decompressed_size - half_point));
    uLong combined_adler = adler32_combine(adler_first, adler_second, 
                                          (off_t)(decompressed_size - half_point));

    // Step 5: Re-compression setup
    z_stream comp_strm;
    memset(&comp_strm, 0, sizeof(comp_strm));
    
    // Get compression parameters from input data
    int level = data[0] % 10; // 0-9 compression level
    int method = Z_DEFLATED;
    int mem_level = 8 + (data[0] % 4); // 8-11 memory level
    int strategy = data[0] % 4; // 0-3 compression strategy
    
    // Initialize compression with gzip wrapper
    if (deflateInit2_(&comp_strm, level, method, MAX_WBITS + 16, mem_level, 
                      strategy, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        free(decompressed);
        inflateEnd(&decomp_strm);
        return 0;
    }

    // Step 6: Configure compression parameters
    // Tune compression parameters
    int good_length = 32 + (data[0] % 32);
    int max_lazy = 128 + (data[0] % 128);
    int nice_length = 258;
    int max_chain = 1024 + (data[0] % 1024);
    deflateTune(&comp_strm, good_length, max_lazy, nice_length, max_chain);

    // Set gzip header
    gz_header gzheader;
    memset(&gzheader, 0, sizeof(gzheader));
    gzheader.os = 255; // Unknown OS
    deflateSetHeader(&comp_strm, &gzheader);

    // Get dictionary (even if empty)
    Bytef dictionary[256];
    uInt dict_length = 256;
    deflateGetDictionary(&comp_strm, dictionary, &dict_length);

    // Step 7: Prepare compression buffers
    uLong comp_bound = deflateBound(&comp_strm, decompressed_size);
    Bytef *recompressed = (Bytef *)malloc(comp_bound);
    if (!recompressed) {
        deflateEnd(&comp_strm);
        free(decompressed);
        inflateEnd(&decomp_strm);
        return 0;
    }

    // Step 8: Re-compress the data
    comp_strm.next_in = decompressed;
    comp_strm.avail_in = decompressed_size;
    comp_strm.next_out = recompressed;
    comp_strm.avail_out = comp_bound;

    int comp_result = deflate(&comp_strm, Z_FINISH);
    uLong recompressed_size = comp_bound - comp_strm.avail_out;

    // Step 9: Write to gzip file using gzFile APIs
    const char *output_filename = "output_file.gz";
    
    // Create output file
    FILE *out_file = fopen(output_filename, "wb");
    if (!out_file) {
        free(recompressed);
        free(decompressed);
        deflateEnd(&comp_strm);
        inflateEnd(&decomp_strm);
        assert_file_closed(&out_file);
	assert_file_name_closed("output_file.gz");
	return 0;
    }
    
    // Get file descriptor for gzdopen
    int fd = fuzz_fileno(out_file);
    assert_file_closed(&out_file);; // Close the FILE*, we'll use gzFile instead
    
    // Open with gzdopen for gzip writing
    gzFile gz_out = gzdopen(fd, "wb");
    if (!gz_out) {
        free(recompressed);
        free(decompressed);
        deflateEnd(&comp_strm);
        inflateEnd(&decomp_strm);
        assert_file_closed(&out_file);
	assert_file_name_closed("output_file.gz");
	assert_fd_closed(fd);
	return 0;
    }

    // Write the re-compressed data
    int write_result = gzwrite(gz_out, recompressed, (unsigned int)recompressed_size);
    
    // Get file offset before closing
    off64_t offset = gzoffset64(gz_out);
    
    // Close the gzip file
    int close_result = gzclose_w(gz_out);
    
    // Also test regular gzclose on a dummy file
    FILE *dummy_file = fopen("dummy.gz", "wb");
    if (dummy_file) {
        int dummy_fd = fuzz_fileno(dummy_file);
        assert_file_closed(&dummy_file);;
        gzFile dummy_gz = gzdopen(dummy_fd, "wb");
        if (dummy_gz) {
            gzclose(dummy_gz);
        }
    }

    // Step 10: Cleanup
    free(recompressed);
    free(decompressed);
    deflateEnd(&comp_strm);
    inflateEnd(&decomp_strm);

    // Remove temporary files
    remove("output_file.gz");
    remove("dummy.gz");

    assert_file_closed(&dummy_file);
	assert_file_closed(&out_file);
	assert_file_name_closed("output_file.gz");
	assert_file_name_closed("output_file.gz");
	assert_fd_closed(fd);
	return 0;
}