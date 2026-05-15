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
//<ID> 27
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 2
//<Quality> {"density":10,"unique_branches":{"gztell64":[[453,9,453,31,0,0,4,0],[453,35,453,58,0,0,4,1]]},"library_calls":["gzdopen","gzdopen","gzclose","gzclose","gzclose","compressBound_z","compress2_z","gzputc","compressBound","compress2","gzputs","inflateInit_","gztell64","gzclose","gzclose_r","inflateEnd"],"critical_calls":["gzdopen","gzdopen","compressBound_z","compress2_z","gzputc","compressBound","compress2","gzputs","inflateInit_","gztell64","gzclose","gzclose_r","inflateEnd"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if input is too small for meaningful processing
    if (size < 4) {
        return 0;
    }

    // Step 1: Create input/output files using the provided methods
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

    // Step 2: Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Step 3: Use gzdopen to create gzFile handles
    gzFile gz_in = gzdopen(in_fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    gzFile gz_out = gzdopen(out_fd, "wb");
    if (!gz_out) {
        gzclose(gz_in);  // This will also close in_file
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 4: Prepare buffers for compression
    // Use first byte for compression level (mask to valid range)
    int level = (data[0] % 11) - 1;  // -1 to 9 range
    
    // Calculate source sizes (using part of input data)
    size_t source1_len = size / 2;
    size_t source2_len = size - source1_len;
    
    // Allocate and prepare source buffers
    Bytef *source1 = (Bytef *)malloc(source1_len);
    Bytef *source2 = (Bytef *)malloc(source2_len);
    
    if (!source1 || !source2) {
        free(source1);
        free(source2);
        gzclose(gz_out);
        gzclose(gz_in);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    memcpy(source1, data, source1_len);
    memcpy(source2, data + source1_len, source2_len);

    // Step 5: Use compress2_z (with z_size_t parameters)
    z_size_t dest1_len_z = compressBound_z(source1_len);
    Bytef *dest1 = (Bytef *)malloc(dest1_len_z);
    if (dest1) {
        z_size_t actual_dest1_len = dest1_len_z;
        compress2_z(dest1, &actual_dest1_len, source1, source1_len, level);
        // Write compressed data using gzputc
        for (z_size_t i = 0; i < actual_dest1_len && i < 100; i++) {
            gzputc(gz_out, dest1[i]);
        }
        free(dest1);
    }

    // Step 6: Use compress2 (with uLongf parameters)
    uLongf dest2_len = compressBound(source2_len);
    Bytef *dest2 = (Bytef *)malloc(dest2_len);
    if (dest2) {
        uLongf actual_dest2_len = dest2_len;
        compress2(dest2, &actual_dest2_len, source2, source2_len, level);
        // Write some compressed data using gzputs (convert to string-like data)
        char small_buf[32];
        size_t to_write = actual_dest2_len < 31 ? actual_dest2_len : 31;
        memcpy(small_buf, dest2, to_write);
        small_buf[to_write] = '\0';
        gzputs(gz_out, small_buf);
        free(dest2);
    }

    // Step 7: Initialize inflation stream using inflateInit_
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    
    int ret = inflateInit_(&strm, ZLIB_VERSION, sizeof(z_stream));
    if (ret == Z_OK) {
        // Just initialize and immediately prepare for cleanup
        // We could do actual inflation here but keeping it minimal
    }

    // Step 8: Get current position in gz_out using gztell64
    off64_t pos = gztell64(gz_out);
    (void)pos; // Use pos to avoid unused variable warning

    // Step 9: Clean up all resources
    free(source1);
    free(source2);
    
    // Close gzip files using both required functions
    gzclose(gz_out);      // This closes the writing handle
    gzclose_r(gz_in);     // This closes the reading handle
    
    // Note: The underlying FILE* are closed by gzclose/gzclose_r
    // No need to call fclose separately on in_file/out_file
    
    // Clean up inflation stream if initialized
    if (ret == Z_OK) {
        inflateEnd(&strm);
    }

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}