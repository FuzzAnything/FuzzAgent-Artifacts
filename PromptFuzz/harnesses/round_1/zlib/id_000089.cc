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
//<ID> 258
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 5
//<Quality> {"density":7,"unique_branches":{"deflateResetKeep":[[647,9,647,32,0,0,4,0]],"deflateReset":[[708,9,708,20,0,0,4,1]],"gzeof":[[505,9,505,31,0,0,4,0],[505,35,505,58,0,0,4,1],[509,12,509,34,0,0,4,1]]},"library_calls":["gzdopen","inflateInit_","gzclose","gzgets","gzseek64","gzeof","gzoffset64","deflateReset","inflateReset","compressBound_z","compress2_z","gzdopen","gzputs","gzoffset64","gzseek64","gzeof","gzclose","inflateEnd","gzclose"],"critical_calls":["gzdopen","inflateInit_","gzgets","gzseek64","gzeof","gzoffset64","deflateReset","inflateReset","compressBound_z","compress2_z","gzdopen","gzputs","gzoffset64","gzseek64","gzeof","gzclose","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_89(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create a FILE* from the input data for reading compressed data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Open gzFile for reading from the file descriptor
    gzFile gz_in = gzdopen(fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Prepare to decompress the data
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize inflation stream with default parameters
    if (inflateInit(&strm) != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Read and decompress data in chunks
    const size_t CHUNK_SIZE = 4096;
    Bytef in[CHUNK_SIZE];
    Bytef out[CHUNK_SIZE];
    
    int ret = Z_OK;
    size_t bytes_read_total = 0;
    
    // First, read some data from gzFile to initialize decompression
    char test_buf[100];
    if (gzgets(gz_in, test_buf, sizeof(test_buf)) == NULL) {
        // Reset file position for next operations
        gzseek64(gz_in, 0, SEEK_SET);
    }
    
    // Check if we're at EOF
    int eof_check = gzeof(gz_in);
    
    // Get current offset
    off64_t offset = gzoffset64(gz_in);
    
    // Reset the inflation stream (even though we haven't used it much)
    if (deflateReset(&strm) != Z_OK) {
        // If deflateReset fails, try inflateReset instead
        inflateReset(&strm);
    }
    
    // Now let's try to re-compress some data using compress2_z
    // We'll use the original input data as source
    Bytef *compressed = nullptr;
    z_size_t compressed_len = 0;
    z_size_t source_len = size > 10000 ? 10000 : size; // Limit for safety
    
    if (source_len > 0) {
        // Calculate maximum compressed size
        compressed_len = compressBound_z(source_len);
        if (compressed_len > 0) {
            compressed = (Bytef*)malloc(compressed_len);
            if (compressed) {
                // Try compression with random level (0-9)
                int level = data[0] % 10;
                int compress_ret = compress2_z(compressed, &compressed_len, 
                                               data, source_len, level);
                
                // Use the result somehow (even if it failed)
                if (compress_ret != Z_OK) {
                    // Compression failed, but that's OK for fuzzing
                    compressed_len = 0;
                }
            }
        }
    }
    
    // Create an output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        int out_fd = fuzz_fileno(out_file);
        if (out_fd >= 0) {
            // Open gzFile for writing
            gzFile gz_out = gzdopen(out_fd, "wb");
            if (gz_out) {
                // Write some test string using gzputs
                const char *test_string = "Fuzz test output\n";
                gzputs(gz_out, test_string);
                
                // If we have compressed data, write it (as binary)
                if (compressed && compressed_len > 0) {
                    // We can't use gzputs for binary data, so seek and write differently
                    // Instead, let's just use the string writing we already did
                }
                
                // Get offset in output file
                off64_t out_offset = gzoffset64(gz_out);
                
                // Seek to a position based on input data
                if (size > 8) {
                    off64_t seek_pos = *(off64_t*)data % 1000;
                    gzseek64(gz_out, seek_pos, SEEK_SET);
                }
                
                // Check EOF on output (should be false since we just wrote)
                int out_eof = gzeof(gz_out);
                
                gzclose(gz_out);
            } else {
                assert_file_closed(&out_file);;
            }
        } else {
            assert_file_closed(&out_file);;
        }
    }
    
    // Clean up resources
    if (compressed) {
        free(compressed);
    }
    
    inflateEnd(&strm);
    gzclose(gz_in);
    
    // Note: in_file is automatically closed by gzclose(gz_in) when using gzdopen
    // But to be safe, we'll close it explicitly
    assert_file_closed(&in_file);;
    
    // Remove the output file if it was created
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}