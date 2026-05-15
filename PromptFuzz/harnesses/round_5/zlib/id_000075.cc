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
//<ID> 202
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 9
//<Quality> {"density":9,"unique_branches":{"deflate":[[1086,17,1086,32,0,0,4,0],[1241,24,1241,40,0,0,4,1],[133,31,133,38,10,0,4,0],[133,31,133,38,11,0,4,0]],"gzsetparams":[[655,9,655,20,0,0,4,0],[657,13,657,27,0,0,4,0],[657,13,657,27,0,0,4,1],[657,31,657,60,0,0,4,1]],"gzdirect":[[637,9,637,31,0,0,4,1]]},"library_calls":["gzdopen","compressBound","compress2","gzfwrite","gzdirect","gzrewind","gzsetparams","gzflush","gzclose"],"critical_calls":["gzdopen","compressBound","compress2","gzfwrite","gzdirect","gzrewind","gzsetparams","gzflush","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_75(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a FILE* to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a FILE* to write output data
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
    
    // 1. Create a gzFile for writing
    gzFile gz = gzdopen(fd_out, "wb");
    if (!gz) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 2. Use compress2 to compress the input data
    // First calculate bound for compressed data
    uLongf compressed_bound = compressBound(size);
    Bytef *compressed_data = (Bytef *)malloc(compressed_bound);
    uLongf compressed_len = compressed_bound;
    
    if (compressed_data && size > 0) {
        int compress_result = compress2(compressed_data, &compressed_len,
                                       (const Bytef *)data, size, Z_DEFAULT_COMPRESSION);
        
        if (compress_result == Z_OK) {
            // 3. Write the compressed data to gzFile using gzfwrite
            z_size_t items_written = gzfwrite(compressed_data, 1, compressed_len, gz);
            
            // 4. Check if the file is in direct mode
            int is_direct = gzdirect(gz);
            
            // 5. Attempt to rewind the file (may fail for write-only files)
            int rewind_result = gzrewind(gz);
            
            // 6. Set parameters for the gzFile
            int setparams_result = gzsetparams(gz, Z_BEST_COMPRESSION, Z_DEFAULT_STRATEGY);
            
            // Flush any pending data
            gzflush(gz, Z_FINISH);
        }
        
        free(compressed_data);
    }
    
    // Clean up resources
    gzclose(gz);  // This will also close fd_out
    
    // Note: gzclose already closed the file descriptor, so we don't need to close out_file
    // But we still need to close in_file
    assert_file_closed(&in_file);;
    
    // Remove the output file since we don't need it
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}