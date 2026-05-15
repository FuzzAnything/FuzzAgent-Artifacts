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
//<ID> 12
//<Prompt> []
/*<Combination>: [
*/
//<score> 2.5, nr_unique_branch: 0
//<Quality> {"density":5,"unique_branches":{},"library_calls":["compressBound_z","compress2_z","gzdopen","gzclose_r","gzdopen","gzputs","gzclose","inflateInit_","gzclose_r","inflateEnd","gzclose_r","inflateBack","inflateBackEnd","gzclose_r"],"critical_calls":["compressBound_z","compress2_z","gzdopen","gzdopen","gzputs","gzclose","inflateInit_","inflateBack","inflateBackEnd","gzclose_r"],"visited":1}
/*Looking at the APIs provided, I need to create a fuzz driver that:
1. Uses all the specified APIs at least once
2. Processes zlib output data
3. Handles compression and decompression operations

Here's my step-by-step implementation:

*/


// Callback functions for inflateBack
static unsigned int  id_000006_in_callback(void *in_desc, unsigned char **buf) {
    // This callback should provide input data for inflateBack
    static unsigned char static_buf[1024];
    *buf = static_buf;
    return 0;  // Return 0 to indicate no more data
}

static int  id_000006_out_callback(void *out_desc, unsigned char *buf, unsigned int len) {
    // This callback processes output from inflateBack
    return len;  // Return the number of bytes processed
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if we don't have enough data
    if (size < 100) {
	return 0;
	}
	
    
    // Part 1: Create compressed data using compress2_z
    z_size_t compressed_len = compressBound_z(size);
    Bytef *compressed_data = (Bytef *)malloc(compressed_len);
    if (!compressed_data) {
	return 0;
	}
	
    
    int compress_result = compress2_z(
        compressed_data, 
        &compressed_len, 
        data, 
        size, 
        Z_DEFAULT_COMPRESSION
    );
    
    if (compress_result != Z_OK) {
        free(compressed_data);
        return 0;
    }
    
    // Part 2: Create a FILE* for the compressed data
    FILE *compressed_file = fmemopen((void *)compressed_data, compressed_len, "rb");
    if (!compressed_file) {
        free(compressed_data);
        assert_file_closed(&compressed_file);
	return 0;
    }
    
    // Get file descriptor and create gzFile for reading
    int fd = fuzz_fileno(compressed_file);
    gzFile gz_read = gzdopen(fd, "rb");
    if (!gz_read) {
        assert_file_closed(&compressed_file);;
        free(compressed_data);
        assert_file_closed(&compressed_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Part 3: Use gzputs (we need a gzFile opened for writing)
    // Create a temporary file for writing
    FILE *temp_out = fopen("output_file", "wb");
    if (!temp_out) {
        gzclose_r(gz_read);
        free(compressed_data);
        assert_file_closed(&temp_out);
	assert_file_closed(&compressed_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    int temp_fd = fuzz_fileno(temp_out);
    gzFile gz_write = gzdopen(temp_fd, "wb");
    if (gz_write) {
        // Write a test string using gzputs
        const char *test_str = "Test string for gzputs";
        gzputs(gz_write, test_str);
        
        // Close the writing gzFile
        gzclose(gz_write);  // Note: Using gzclose for writing
    }
    assert_file_closed(&temp_out);;
    
    // Part 4: Initialize z_stream for inflateBack
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    int init_result = inflateInit_(&strm, ZLIB_VERSION, sizeof(z_stream));
    if (init_result != Z_OK) {
        gzclose_r(gz_read);
        free(compressed_data);
        assert_file_closed(&temp_out);
	assert_file_closed(&compressed_file);
	assert_fd_closed(temp_fd);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Part 5: Use inflateBack with the compressed data
    // Create a buffer for output
    Bytef *out_buffer = (Bytef *)malloc(size * 2);
    if (!out_buffer) {
        inflateEnd(&strm);
        gzclose_r(gz_read);
        free(compressed_data);
        assert_file_closed(&temp_out);
	assert_file_closed(&compressed_file);
	assert_fd_closed(temp_fd);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Prepare data for inflateBack callback
    struct {
        const Bytef *data;
        size_t remaining;
    } callback_data;
    
    callback_data.data = compressed_data;
    callback_data.remaining = compressed_len;
    
    // Define a custom input callback
    auto custom_in_callback = [](void *desc, unsigned char **buf) -> unsigned int {
        auto *cb_data = static_cast<decltype(callback_data)*>(desc);
        if (cb_data->remaining == 0) {
            return 0;
        }
        
        // Return pointer to remaining data and its length
        *buf = (unsigned char *)cb_data->data;
        unsigned int to_return = (unsigned int)cb_data->remaining;
        if (to_return > 1024) to_return = 1024;
        
        // Update position
        cb_data->data += to_return;
        cb_data->remaining -= to_return;
        
        return to_return;
    };
    
    // Define a custom output callback
    auto custom_out_callback = [](void *desc, unsigned char *buf, unsigned int len) -> int {
        // Just consume the data without storing it
        return (int)len;
    };
    
    // Try to decompress using inflateBack
    int back_result = inflateBack(
        &strm,
        custom_in_callback,
        &callback_data,
        custom_out_callback,
        nullptr
    );
    
    // Clean up inflateBack resources
    inflateBackEnd(&strm);
    
    // Part 6: Close the reading gzFile
    gzclose_r(gz_read);
    
    // Clean up
    free(out_buffer);
    free(compressed_data);
    
    assert_file_closed(&temp_out);
	assert_file_closed(&compressed_file);
	assert_fd_closed(temp_fd);
	assert_fd_closed(fd);
	return 0;
}