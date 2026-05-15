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
//<ID> 0
//<Prompt> []
/*<Combination>: [
*/
//<score> 10.2, nr_unique_branch: 12
//<Quality> {"density":17,"unique_branches":{"gzvprintf":[[428,9,428,25,0,0,4,0],[428,29,428,49,0,0,4,1],[450,9,450,28,0,0,4,0]],"gzungetc":[[512,9,512,31,0,0,4,1],[516,9,516,27,0,0,4,0],[516,31,516,49,0,0,4,1],[520,9,520,27,0,0,4,1],[525,9,525,20,0,0,4,1],[529,9,529,14,0,0,4,1],[533,9,533,27,0,0,4,1],[543,9,543,44,0,0,4,1],[549,9,549,36,0,0,4,1]]},"library_calls":["inflateInit2_","inflate","inflateGetDictionary","inflateCodesUsed","inflateEnd","deflateInit_","deflateSetDictionary","compressBound_z","compress_z","compressBound_z","compress2_z","gzdopen","gzopen","gzprintf","gzwrite","gzclose","gzopen","gzungetc","gzclose","deflateEnd"],"critical_calls":["inflateInit2_","inflate","inflateGetDictionary","inflateCodesUsed","inflateEnd","deflateInit_","deflateSetDictionary","compressBound_z","compress_z","compressBound_z","compress2_z","gzdopen","gzopen","gzprintf","gzwrite","gzclose","gzopen","gzungetc","gzclose","deflateEnd"],"visited":4}
/**/


extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create FILE* for writing output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    
    // Variables for various operations
    int ret;
    z_stream strm;
    z_stream strm2;
    Bytef *dest = nullptr;
    z_size_t destLen = 0;
    Bytef *dest2 = nullptr;
    z_size_t destLen2 = 0;
    Bytef dictionary[256];
    uInt dictLength = 0;
    gzFile gzfile = nullptr;
    const char *format_str = "Test format: %d\n";
    int level = 6;
    unsigned long codes_used = 0;
    
    // Initialize zlib stream for inflation with windowBits parameter
    memset(&strm, 0, sizeof(strm));
    ret = inflateInit2_(&strm, 15 + 16, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        goto cleanup;
    }
    
    // Set up input for inflation from our data
    strm.next_in = (Bytef *)data;
    strm.avail_in = size;
    
    // Allocate output buffer for inflation
    destLen = size * 2;  // Reasonable guess for decompressed size
    dest = (Bytef *)malloc(destLen);
    if (!dest) {
        goto cleanup;
    }
    
    strm.next_out = dest;
    strm.avail_out = destLen;
    
    // Inflate the data
    ret = inflate(&strm, Z_FINISH);
    
    // Get dictionary from inflated stream if available
    dictLength = sizeof(dictionary);
    ret = inflateGetDictionary(&strm, dictionary, &dictLength);
    
    // Get codes used by inflate
    codes_used = inflateCodesUsed(&strm);
    
    // Clean up inflate stream
    inflateEnd(&strm);
    
    // Initialize new stream for compression with dictionary
    memset(&strm2, 0, sizeof(strm2));
    ret = deflateInit(&strm2, level);
    if (ret != Z_OK) {
        goto cleanup;
    }
    
    // Set dictionary for compression if we got one
    if (dictLength > 0) {
        ret = deflateSetDictionary(&strm2, dictionary, dictLength);
    }
    
    // Compress using compress_z (one-shot compression)
    destLen = compressBound_z(size);
    dest = (Bytef *)realloc(dest, destLen);
    if (!dest) {
        goto cleanup;
    }
    
    ret = compress_z(dest, &destLen, data, size);
    
    // Compress using compress2_z with specific level
    destLen2 = compressBound_z(size);
    dest2 = (Bytef *)malloc(destLen2);
    if (!dest2) {
        goto cleanup;
    }
    
    ret = compress2_z(dest2, &destLen2, data, size, level);
    
    // Open gzFile for writing using file descriptor
    gzfile = gzdopen(fd_out, "wb");
    if (!gzfile) {
        // Fallback to using filename
        gzfile = gzopen("output_file", "wb");
        if (!gzfile) {
            goto cleanup;
        }
    }
    
    // Write to gzFile using gzprintf
    ret = gzprintf(gzfile, format_str, 42);
    
    // Write some data to gzFile
    if (dest && destLen > 0) {
        // Use gzwrite to write binary data
        unsigned int write_size = destLen > 100 ? 100 : (unsigned int)destLen;
        ret = gzwrite(gzfile, dest, write_size);
        
        // Now read it back with gzgetc and gzungetc
        gzclose(gzfile);
        gzfile = gzopen("output_file", "rb");
        if (gzfile) {
            int c = gzgetc(gzfile);
            if (c != -1) {
                gzungetc(c, gzfile);
            }
        }
    }
    
cleanup:
    // Clean up all resources
    if (gzfile) {
        gzclose(gzfile);
    }
    
    if (dest) {
        free(dest);
    }
    
    if (dest2) {
        free(dest2);
    }
    
    // Clean up deflate stream if initialized
    deflateEnd(&strm2);
    
    // Close file handles
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    if (out_file && !gzfile) {  // Only close if gzfile didn't already close it
        assert_file_closed(&out_file);;
    }
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}