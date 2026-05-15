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
//<ID> 121
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 1
//<Quality> {"density":5,"unique_branches":{"gzputc":[[331,13,331,32,0,0,4,0]]},"library_calls":["uncompress2_z","uncompress2_z","compressBound","compressBound","compress2","compress2","gzdopen","gzdopen","gzclose_r","gzwrite","gzputc","gzseek","inflateInit_","inflateEnd","gzclose_r","gzclose"],"critical_calls":["uncompress2_z","uncompress2_z","compressBound","compress2","gzdopen","gzdopen","gzwrite","gzputc","gzseek","inflateInit_","inflateEnd","gzclose_r","gzclose"],"visited":0}
/**/


// Required for fmemopen on some platforms
#ifdef __linux__
#endif

extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. First, we'll try to decompress the input data using uncompress2_z
    // to get the original uncompressed data
    Bytef *uncompressed = nullptr;
    z_size_t uncompressed_len = 0;
    z_size_t source_len = size;
    
    // First call to uncompress2_z to get required buffer size
    int ret = uncompress2_z(nullptr, &uncompressed_len, data, &source_len);
    if (ret == Z_OK && uncompressed_len > 0) {
        uncompressed = (Bytef *)malloc(uncompressed_len);
        if (!uncompressed) {
	return 0;
	}
	
        
        // Second call to actually decompress
        z_size_t source_len2 = size;
        ret = uncompress2_z(uncompressed, &uncompressed_len, data, &source_len2);
        if (ret != Z_OK) {
            free(uncompressed);
            uncompressed = nullptr;
            uncompressed_len = 0;
        }
    }
    
    // 2. Use compressBound to get maximum compressed size
    uLong max_compressed_size = 0;
    if (uncompressed && uncompressed_len > 0) {
        max_compressed_size = compressBound((uLong)uncompressed_len);
    } else {
        // If we couldn't decompress, use the input data directly
        max_compressed_size = compressBound((uLong)size);
    }
    
    // 3. Recompress the data using compress2
    Bytef *recompressed = nullptr;
    uLongf recompressed_len = max_compressed_size;
    if (max_compressed_size > 0) {
        recompressed = (Bytef *)malloc(max_compressed_size);
        if (recompressed) {
            if (uncompressed && uncompressed_len > 0) {
                compress2(recompressed, &recompressed_len, 
                         uncompressed, (uLong)uncompressed_len, Z_DEFAULT_COMPRESSION);
            } else {
                // Use original data if decompression failed
                compress2(recompressed, &recompressed_len, 
                         data, (uLong)size, Z_DEFAULT_COMPRESSION);
            }
        }
    }
    
    // 4. Create a FILE* for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        // Clean up and return
        free(uncompressed);
        free(recompressed);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 5. Create a FILE* for writing output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        free(uncompressed);
        free(recompressed);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 6. Use gzdopen to create a gzFile from file descriptor
    int in_fd = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(in_fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        free(uncompressed);
        free(recompressed);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    int out_fd = fuzz_fileno(out_file);
    gzFile gz_out = gzdopen(out_fd, "wb");
    if (!gz_out) {
        gzclose_r(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        free(uncompressed);
        free(recompressed);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 7. Write the recompressed data using gzwrite
    if (recompressed && recompressed_len > 0) {
        gzwrite(gz_out, recompressed, (unsigned int)recompressed_len);
        
        // 8. Write an additional byte using gzputc
        gzputc(gz_out, '\n');
        
        // 9. Seek to beginning of the gzFile
        gzseek(gz_out, 0, SEEK_SET);
    }
    
    // 10. Initialize a z_stream for inflation
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Use inflateInit_ (note the trailing underscore)
    ret = inflateInit_(&strm, ZLIB_VERSION, sizeof(z_stream));
    if (ret == Z_OK) {
        // We have a valid inflation stream that we could use
        // (In a real scenario, we'd set up buffers and call inflate)
        
        // 11. Clean up the inflation stream
        inflateEnd(&strm);
    }
    
    // 12. Close the gzFiles using both gzclose_r and gzclose
    gzclose_r(gz_in);  // For reading file
    
    // Note: gz_out was opened for writing, use gzclose
    gzclose(gz_out);   // For writing file
    
    // Close the regular FILE* objects
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // 13. Clean up allocated memory
    free(uncompressed);
    free(recompressed);
    
    // Remove the output file we created
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}