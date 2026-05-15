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
//<ID> 65
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["gzdopen","gzclose_r","inflateBackInit_","gzclose_r","inflateBack","gzeof","gzoffset64","inflateBackEnd","gzclose_r"],"critical_calls":["gzdopen","inflateBackInit_","inflateBack","gzeof","gzoffset64","inflateBackEnd","gzclose_r"],"visited":0}
/**/


// Callback function for inflateBack input
static unsigned int  id_000035_in_callback(void* desc, unsigned char** buf) {
    // We'll use the FILE* directly as the descriptor
    FILE* file = (FILE*)desc;
    static unsigned char local_buf[1024];
    
    size_t bytes_read = fread(local_buf, 1, sizeof(local_buf), file);
    *buf = local_buf;
    return (unsigned int)bytes_read;
}

// Callback function for inflateBack output
static int  id_000035_out_callback(void* desc, unsigned char* buf, unsigned int size) {
    // We'll use the FILE* directly as the descriptor
    FILE* file = (FILE*)desc;
    return (fwrite(buf, 1, size, file) == size) ? 0 : 1;
}

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<17) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }
    
    // Create a temporary file with the compressed data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
	int fuzz_fd_0 = fuzz_fileno(in_file);

    
    // Open the input file as a gzFile to use with gzeof and gzoffset64
    gzFile gz_in = gzdopen(fuzz_fd_0, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	return 0;
    }
    
    // Create output file for decompressed data
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        gzclose_r(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	return 0;
    }
    
    // Initialize z_stream for inflateBack
    z_stream strm;
    unsigned char window[1 << 15]; // 32K window as recommended
    
    // Initialize inflateBack
    int ret = inflateBackInit_(&strm, 15, window, fuzz_str_1, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        assert_file_closed(&out_file);;
        gzclose_r(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	return 0;
    }
    
    // Perform decompression using inflateBack
    ret = inflateBack(&strm,  id_000035_in_callback, in_file,  id_000035_out_callback, out_file);
    
    // Check if we reached end of gzFile
    if (gzeof(gz_in)) {
        // We've reached the end of the compressed file
    }
    
    // Get current offset in gzFile
    off64_t offset = gzoffset64(gz_in);
    (void)offset; // Use offset to avoid unused variable warning
    
    // Clean up inflateBack
    inflateBackEnd(&strm);
    
    // Close gzFile
    gzclose_r(gz_in);
    
    // Close regular files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	return 0;
}