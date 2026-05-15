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
//<ID> 25
//<Prompt> []
/*<Combination>: [
*/
//<score> 5, nr_unique_branch: 1
//<Quality> {"density":5,"unique_branches":{"gzputs":[[360,9,360,32,0,0,4,0]]},"library_calls":["inflateInit_","compressBound","inflateEnd","compress2","compress2_z","gzdopen","gzputs","gzputc","gztell64","gzclose_r","gzdopen","gzputs","gzputc","gzclose","inflateEnd"],"critical_calls":["inflateInit_","compressBound","compress2","compress2_z","gzdopen","gzputs","gzputc","gztell64","gzclose_r","gzdopen","gzputs","gzputc","gzclose","inflateEnd"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
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
    
    // Variables for compression/decompression
    int ret;
    z_stream strm;
    Bytef *compressed_data = nullptr;
    Bytef *decompressed_data = nullptr;
    uLongf compr_len, decompr_len;
    z_size_t compr_len_z, decompr_len_z;
    gzFile gz_file = nullptr;
    
    // Initialize z_stream for inflation
    memset(&strm, 0, sizeof(strm));
    ret = inflateInit_(&strm, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Allocate buffers for compression/decompression
    uLongf max_compr_len = compressBound(size);
    uLongf max_decompr_len = size * 10; // Conservative estimate
    
    compressed_data = (Bytef *)malloc(max_compr_len);
    decompressed_data = (Bytef *)malloc(max_decompr_len);
    
    if (!compressed_data || !decompressed_data) {
        free(compressed_data);
        free(decompressed_data);
        inflateEnd(&strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Step 1: Use compress2 with input data
    compr_len = max_compr_len;
    ret = compress2(compressed_data, &compr_len, data, size, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        // Cleanup and continue
    }

    // Step 2: Use compress2_z with input data
    compr_len_z = max_compr_len;
    ret = compress2_z(compressed_data, &compr_len_z, data, size, Z_BEST_COMPRESSION);
    if (ret != Z_OK) {
        // Cleanup and continue
    }

    // Step 3: Open gzFile using gzdopen with input file descriptor
    gz_file = gzdopen(fd_in, "rb");
    if (!gz_file) {
        // Cleanup
    } else {
        // Step 4: Use gzputs with gzFile
        const char *test_string = "Test string for gzputs\n";
        gzputs(gz_file, test_string);
        
        // Step 5: Use gzputc with gzFile
        gzputc(gz_file, 'A');
        
        // Step 6: Get position with gztell64
        off64_t pos = gztell64(gz_file);
        (void)pos; // Use pos to avoid unused variable warning
        
        // Step 7: Close gzFile for reading
        gzclose_r(gz_file);
    }

    // Step 8: Open gzFile for writing using output file descriptor
    gz_file = gzdopen(fd_out, "wb");
    if (gz_file) {
        // Write some test data
        const char *output_str = "Fuzzer output data\n";
        gzputs(gz_file, output_str);
        gzputc(gz_file, 'Z');
        
        // Step 9: Close gzFile for writing
        gzclose(gz_file);
    }

    // Cleanup
    inflateEnd(&strm);
    free(compressed_data);
    free(decompressed_data);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove temporary output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}