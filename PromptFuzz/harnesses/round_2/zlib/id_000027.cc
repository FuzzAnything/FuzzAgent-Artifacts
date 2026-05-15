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
//<ID> 55
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["gzdopen","gztell64","deflateInit2_","deflateBound_z","deflateEnd","gzclose","compress2","compress_z","gzdopen","gzsetparams","gzwrite","gztell64","gzclose","inflateInit2_","inflate","inflateMark","deflateInit2_","deflateParams","deflateResetKeep","deflateEnd","inflateEnd","gzclose"],"critical_calls":["gzdopen","gztell64","deflateInit2_","deflateBound_z","deflateEnd","compress2","compress_z","gzdopen","gzsetparams","gzwrite","gztell64","gzclose","inflateInit2_","inflate","inflateMark","deflateInit2_","deflateParams","deflateResetKeep","deflateEnd","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Return early if input is too small for meaningful processing
    if (size < 10) {
	return 0;
	}
	

    // Step 1: Create a FILE* for input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Step 2: Get file descriptor and open as gzFile
    int fd = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Step 3: Use gztell64 to get current position
    off64_t pos = gztell64(gz_in);
    (void)pos; // Mark as used

    // Step 4: Allocate buffers for compression
    Bytef *source = (Bytef *)data;
    uLong sourceLen = size;
    
    // Use deflateBound_z to estimate maximum compressed size
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15, 8, Z_DEFAULT_STRATEGY);
    z_size_t bound = deflateBound_z(&strm, sourceLen);
    deflateEnd(&strm);

    // Allocate destination buffer based on bound
    uLongf destLen = bound;
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Step 5: Use compress2 to compress data
    int compress2_result = compress2(dest, &destLen, source, sourceLen, Z_DEFAULT_COMPRESSION);
    (void)compress2_result;

    // Step 6: Use compress_z to compress again (different buffer)
    z_size_t destLen_z = bound;
    Bytef *dest2 = (Bytef *)malloc(destLen_z);
    if (dest2) {
        z_size_t sourceLen_z = sourceLen;
        int compress_z_result = compress_z(dest2, &destLen_z, source, sourceLen_z);
        (void)compress_z_result;
        free(dest2);
    }

    // Step 7: Create output file and gzFile for writing
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        int out_fd = fuzz_fileno(out_file);
        gzFile gz_out = gzdopen(out_fd, "wb");
        if (gz_out) {
            // Step 8: Use gzsetparams on output file
            gzsetparams(gz_out, Z_BEST_COMPRESSION, Z_DEFAULT_STRATEGY);
            
            // Write compressed data to gzFile
            gzwrite(gz_out, dest, destLen);
            
            // Use gztell64 on output file
            off64_t out_pos = gztell64(gz_out);
            (void)out_pos;
            
            gzclose(gz_out);
        }
        assert_file_closed(&out_file);;
    }

    // Step 9: Create another z_stream for decompression
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    inflateInit2(&inf_strm, 15 + 32); // Enable gzip decoding
    
    // Set up input for decompression
    inf_strm.next_in = dest;
    inf_strm.avail_in = destLen;
    
    // Allocate decompression buffer
    uLongf decompLen = sourceLen * 2;
    Bytef *decomp = (Bytef *)malloc(decompLen);
    if (decomp) {
        inf_strm.next_out = decomp;
        inf_strm.avail_out = decompLen;
        
        // Perform partial decompression
        int inflate_result = inflate(&inf_strm, Z_SYNC_FLUSH);
        (void)inflate_result;
        
        // Step 10: Use inflateMark during decompression
        long mark = inflateMark(&inf_strm);
        (void)mark;
        
        free(decomp);
    }
    
    // Step 11: Create another z_stream for compression parameter testing
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    deflateInit2(&def_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15, 8, Z_DEFAULT_STRATEGY);
    
    // Step 12: Use deflateParams to change compression parameters
    deflateParams(&def_strm, Z_BEST_COMPRESSION, Z_HUFFMAN_ONLY);
    
    // Step 13: Use deflateResetKeep to reset compression state
    deflateResetKeep(&def_strm);
    
    // Clean up deflate stream
    deflateEnd(&def_strm);
    
    // Clean up inflate stream
    inflateEnd(&inf_strm);

    // Step 14: Release all allocated resources
    free(dest);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    
    // Remove output file if created
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}