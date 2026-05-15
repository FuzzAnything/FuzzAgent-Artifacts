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
//<ID> 148
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 1
//<Quality> {"density":8,"unique_branches":{"inflateBackInit_":[[34,9,34,23,0,0,4,0]]},"library_calls":["gzdopen","gzfread","gztell64","gzseek","gzopen","gzprintf","gzclearerr","gzclose","compress_z","compress2_z","zlibVersion","inflateBackInit_","inflateBackEnd","gzclearerr","gzclose"],"critical_calls":["gzdopen","gzfread","gztell64","gzseek","gzopen","gzprintf","gzclearerr","gzclose","compress_z","compress2_z","zlibVersion","inflateBackInit_","inflateBackEnd","gzclearerr","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create a FILE* from input data for gzFile operations
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Open output file for compressed data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    int fd = fuzz_fileno(in_file);  // Get file descriptor
    gzFile gz_in = gzdopen(fd, "rb");  // Open gzFile for reading
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 1. Use gzfread to read from gzFile
    const z_size_t CHUNK_SIZE = 1024;
    char buffer[CHUNK_SIZE];
    z_size_t items_read = gzfread(buffer, 1, CHUNK_SIZE, gz_in);
    
    // 2. Use gzgetc
    gzgetc(gz_in);  // Read one more byte
    
    // 3. Use gzseek and gztell64
    off64_t pos = gztell64(gz_in);
    if (pos > 0) {
        gzseek(gz_in, 0, SEEK_SET);  // Seek to beginning
    }
    
    // 4. Use gzprintf (write to a different gzFile for output)
    gzFile gz_out = gzopen("output_file.gz", "wb");
    if (gz_out) {
        gzprintf(gz_out, "Processed %zu bytes\n", size);
        gzclearerr(gz_out);  // 5. Use gzclearerr
        gzclose(gz_out);
    }
    
    // 6. Use compress_z and compress2_z with the read data
    Bytef compressed1[compressBound_z(items_read)];
    Bytef compressed2[compressBound_z(items_read)];
    z_size_t destLen1 = sizeof(compressed1);
    z_size_t destLen2 = sizeof(compressed2);
    
    // Use compress_z
    int ret1 = compress_z(compressed1, &destLen1, (Bytef*)buffer, items_read);
    
    // Use compress2_z with maximum compression
    int ret2 = compress2_z(compressed2, &destLen2, (Bytef*)buffer, items_read, 9);
    
    // 7. Use inflateBackInit_ for decompression setup
    z_stream strm;
    unsigned char window[32768];  // 32KB window
    const char *version = zlibVersion();
    
    // Initialize inflateBack for raw deflate data (windowBits negative)
    int init_ret = inflateBackInit_(&strm, -15, window, version, sizeof(z_stream));
    
    if (init_ret == Z_OK) {
        // Clean up inflateBack resources
        inflateBackEnd(&strm);
    }
    
    // 8. Additional gzFile operations
    gzclearerr(gz_in);  // Clear any errors
    
    // Cleanup
    gzclose(gz_in);  // This also closes the underlying fd
    // Note: gzdopen duplicates the fd, so we should close in_file separately
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove temporary files
    remove("output_file");
    remove("output_file.gz");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file.gz");
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file.gz");
	assert_fd_closed(fd);
	return 0;
}