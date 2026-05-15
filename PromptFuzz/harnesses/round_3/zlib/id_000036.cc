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
//<ID> 68
//<Prompt> []
/*<Combination>: [
*/
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":5,"unique_branches":{},"library_calls":["gzdopen","inflateBackInit_","gzclose_r","inflateBack","gzeof","gzoffset64","inflateBackEnd","gzclose_r"],"critical_calls":["gzdopen","inflateBackInit_","inflateBack","gzeof","gzoffset64","inflateBackEnd","gzclose_r"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified zlib APIs to achieve decompression of zlib-compressed data using `inflateBack`:

*/


// Custom structure to hold input data for inflateBack callbacks
struct input_desc {
    const uint8_t* data;
    size_t size;
    size_t pos;
};

// Input callback for inflateBack
static unsigned int  id_000036_input_callback(void* desc, unsigned char** buf) {
    input_desc* id = static_cast<input_desc*>(desc);
    
    if (id->pos >= id->size) {
        return 0; // No more data available
    }
    
    *buf = const_cast<unsigned char*>(id->data + id->pos);
    size_t available = id->size - id->pos;
    
    // Return at most 0xFFFFFFFF bytes (uInt max)
    if (available > 0xFFFFFFFF) {
        available = 0xFFFFFFFF;
    }
    
    id->pos += available;
    return static_cast<unsigned int>(available);
}

// Output callback for inflateBack
static int  id_000036_output_callback(void* desc, unsigned char* buf, unsigned int len) {
    FILE* out_file = static_cast<FILE*>(desc);
    return (fwrite(buf, 1, len, out_file) == len) ? 0 : 1;
}

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<17) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    // Early return if input is too small for meaningful zlib data
    if (size < 2) {
        return 0;
    }
    
    // Create input file from the fuzzer data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file for decompressed data
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptor for gzdopen
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Open the input file as a gzFile for gz* operations
    gzFile gz_in = gzdopen(fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Initialize z_stream for inflateBack
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Allocate window buffer for inflateBack
    const int windowBits = 15; // MAX_WBITS for zlib format
    unsigned char* window = new unsigned char[1 << windowBits];
    
    // Initialize inflateBack with the window
    int ret = inflateBackInit_(&strm, windowBits, window, 
                              fuzz_str_1, static_cast<int>(sizeof(z_stream)));
    if (ret != Z_OK) {
        delete[] window;
        gzclose_r(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Set up input descriptor for inflateBack callbacks
    input_desc idesc;
    idesc.data = data;
    idesc.size = size;
    idesc.pos = 0;
    
    // Perform decompression using inflateBack
    ret = inflateBack(&strm,  id_000036_input_callback, &idesc, 
                      id_000036_output_callback, out_file);
    
    // Check gzFile status using required APIs
    int eof_check = gzeof(gz_in);
    off64_t offset = gzoffset64(gz_in);
    
    // Clean up inflateBack
    inflateBackEnd(&strm);
    
    // Clean up gzFile
    gzclose_r(gz_in);
    
    // Clean up other resources
    delete[] window;
    assert_file_closed(&out_file);;
    
    // Note: in_file is already closed by gzclose_r
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}