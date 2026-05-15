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
//<ID> 58
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["gzdopen","inflateInit_","gzclose","inflateEnd","gzclose","inflateMark","inflate","inflateEnd","gzclose","inflateEnd","compressBound_z","compress_z","compressBound","compress2","deflateInit_","deflateBound_z","deflateResetKeep","deflateParams","deflateEnd","gzsetparams","gztell64","gzread","gzclose"],"critical_calls":["gzdopen","inflateInit_","inflateMark","inflate","inflateEnd","compressBound_z","compress_z","compressBound","compress2","deflateInit_","deflateBound_z","deflateResetKeep","deflateParams","deflateEnd","gzsetparams","gztell64","gzread","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Create output file
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

    // Use gzdopen to create gzFile from file descriptor
    gzFile gz_in = gzdopen(fd_in, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Allocate buffers for decompression and recompression
    Bytef *decompressed = nullptr;
    Bytef *recompressed = nullptr;
    z_size_t decompressed_size = 0;
    uLongf recompressed_size = 0;
    int ret = Z_OK;

    // Step 1: Try to decompress the input using inflateMark
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize inflate stream
    if (inflateInit(&strm) != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    strm.avail_in = size;
    strm.next_in = (Bytef *)data;
    
    // Allocate output buffer for decompression
    decompressed_size = size * 4;  // Initial guess
    decompressed = (Bytef *)malloc(decompressed_size);
    if (!decompressed) {
        inflateEnd(&strm);
        gzclose(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    strm.avail_out = decompressed_size;
    strm.next_out = decompressed;

    // Use inflateMark to get current position
    long mark = inflateMark(&strm);
    
    // Perform decompression
    ret = inflate(&strm, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK) {
        free(decompressed);
        inflateEnd(&strm);
        gzclose(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    decompressed_size = strm.total_out;
    inflateEnd(&strm);

    // Step 2: Use decompressed data for compression operations
    if (decompressed_size > 0) {
        // First compression using compress_z
        z_size_t comp_size_z = compressBound_z(decompressed_size);
        Bytef *compressed_z = (Bytef *)malloc(comp_size_z);
        if (compressed_z) {
            z_size_t destLen_z = comp_size_z;
            compress_z(compressed_z, &destLen_z, decompressed, decompressed_size);
            free(compressed_z);
        }

        // Second compression using compress2
        uLongf comp_size = compressBound(decompressed_size);
        recompressed = (Bytef *)malloc(comp_size);
        if (recompressed) {
            recompressed_size = comp_size;
            // Use level derived from input data
            int level = data[0] % 10;  // Level 0-9
            compress2(recompressed, &recompressed_size, decompressed, decompressed_size, level);
        }
    }

    // Step 3: Setup deflate stream for advanced operations
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    
    if (deflateInit(&def_strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
        // Use deflateBound_z to get maximum compressed size
        z_size_t max_bound = deflateBound_z(&def_strm, decompressed_size);
        
        // Use deflateResetKeep to reset stream while keeping dictionary
        deflateResetKeep(&def_strm);
        
        // Use deflateParams to change compression parameters
        int level = (data[0] % 3) + 1;  // 1-3
        int strategy = data[0] % 4;     // 0-3
        deflateParams(&def_strm, level, strategy);
        
        deflateEnd(&def_strm);
    }

    // Step 4: gzFile operations
    if (gz_in) {
        // Use gzsetparams to change compression parameters
        int level = (data[0] % 3) + 1;  // 1-3
        int strategy = data[0] % 4;     // 0-3
        gzsetparams(gz_in, level, strategy);
        
        // Use gztell64 to get current position
        off64_t pos = gztell64(gz_in);
        
        // Try to read from gzFile
        Bytef buffer[1024];
        unsigned int bytes_read = 0;
        while ((bytes_read = gzread(gz_in, buffer, sizeof(buffer))) > 0) {
            // Process or ignore the data
        }
    }

    // Cleanup
    if (gz_in) gzclose(gz_in);
    else if (fd_in >= 0) assert_file_closed(&in_file);;
    
    if (out_file) assert_file_closed(&out_file);;
    
    free(decompressed);
    free(recompressed);
    
    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}