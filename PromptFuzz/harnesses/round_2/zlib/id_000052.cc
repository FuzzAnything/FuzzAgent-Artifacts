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
//<ID> 125
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 2
//<Quality> {"density":9,"unique_branches":{"gzerror":[[520,9,520,31,0,0,4,0],[520,35,520,58,0,0,4,1]]},"library_calls":["compressBound","compress","inflateInit2_","inflateEnd","deflateBound","inflateEnd","gzdopen","gzsetparams","gzopen","gzputs","gzwrite","gzerror","gzclose","gzerror","gzclose"],"critical_calls":["compressBound","compress","inflateInit2_","inflateEnd","deflateBound","inflateEnd","gzdopen","gzsetparams","gzopen","gzputs","gzwrite","gzerror","gzclose","gzerror","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Prepare variables
    int ret;
    int errnum = 0;
    const char *errstr = nullptr;
    gzFile gz_file = nullptr;
    FILE *file_ptr = nullptr;
    z_stream strm;
    Bytef *dest = nullptr;
    uLongf dest_len = 0;
    uLong source_len = 0;
    uLong bound_size = 0;
    int level = Z_DEFAULT_COMPRESSION;
    int strategy = Z_DEFAULT_STRATEGY;
    int window_bits = MAX_WBITS + 16; // For gzip format

    // 1. Use compress() to compress some sample data
    const char *sample_source = "Hello, World!";
    source_len = strlen(sample_source) + 1;
    dest_len = compressBound(source_len);
    dest = (Bytef *)malloc(dest_len);
    if (dest) {
        ret = compress(dest, &dest_len, (const Bytef *)sample_source, source_len);
        // Clean up compressed data
        free(dest);
        dest = nullptr;
    }

    // 2. Initialize inflation stream for gzip format
    memset(&strm, 0, sizeof(strm));
    ret = inflateInit2_(&strm, window_bits, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        // Clean up stream if initialization failed
        inflateEnd(&strm);
    }

    // 3. Calculate deflate bound for sample data
    if (ret == Z_OK) {
        bound_size = deflateBound(&strm, source_len);
        // End the inflation stream since we won't use it further
        inflateEnd(&strm);
    }

    // 4. Create a file from input data and open it as gzFile
    file_ptr = fmemopen((void *)data, size, "rb");
    if (file_ptr) {
        int fd = fuzz_fileno(file_ptr);
        if (fd != -1) {
            gz_file = gzdopen(fd, "rb");
            // Note: gzdopen takes ownership of fd, so we don't close it separately
        } else {
            assert_file_closed(&file_ptr);;
            file_ptr = nullptr;
        }
    }

    // 5. Read from gzFile if opened successfully
    if (gz_file) {
        // Read one character
        int ch = gzgetc(gz_file);
        (void)ch; // Use variable to avoid warning

        // Try to set compression parameters (may fail for read-only file)
        gzsetparams(gz_file, level, strategy);

        // Write some data to another gzFile (for writing)
        gzFile gz_out = gzopen("output_file", "wb");
        if (gz_out) {
            // Write a string
            gzputs(gz_out, "Test output from fuzzer");
            
            // Write binary data
            const char *test_data = "Additional data";
            gzwrite(gz_out, test_data, strlen(test_data));
            
            // Check for errors
            errstr = gzerror(gz_out, &errnum);
            
            gzclose(gz_out);
        }

        // Check for errors on input gzFile
        errstr = gzerror(gz_file, &errnum);
        
        // Close the gzFile (also closes the underlying file descriptor)
        gzclose(gz_file);
        // Don't fclose file_ptr since gzdopen took ownership
        file_ptr = nullptr;
    } else {
        // If gz_file wasn't opened, close the FILE* if we created it
        if (file_ptr) {
            assert_file_closed(&file_ptr);;
            file_ptr = nullptr;
        }
    }

    // 6. Additional cleanup for inflation stream if not already done
    // (Already handled above, but included for completeness)
    
    assert_file_closed(&file_ptr);
	return 0;
}