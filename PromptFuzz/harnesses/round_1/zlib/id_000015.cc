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
//<ID> 29
//<Prompt> []
/*<Combination>: [
*/
//<score> 3.3333333, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["gzdopen","inflateInit_","gzclose_r","gzungetc","gzgets","gztell64","inflateEnd","gzclose_r","inflateEnd","gzclose_r","gzdopen","inflateEnd","gzclose_r","gzputc","gzputs","compressBound","compress2","gzputc","compressBound_z","compress2_z","gzputc","gzclose","inflateEnd","gzclose_r"],"critical_calls":["gzdopen","inflateInit_","gzungetc","gzgets","gztell64","gzdopen","gzputc","gzputs","compressBound","compress2","gzputc","compressBound_z","compress2_z","gzputc","gzclose","inflateEnd","gzclose_r"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create a FILE* for input using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Get file descriptor for input
    int fd_in = fuzz_fileno(in_file);
    if (fd_in == -1) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 3. Open gzip file for reading using gzdopen
    gzFile gz_in = gzdopen(fd_in, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 4. Initialize z_stream for inflation
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int ret_init = inflateInit_(&strm, ZLIB_VERSION, sizeof(z_stream));
    if (ret_init != Z_OK) {
        gzclose_r(gz_in);  // Use gzclose_r for reading
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 5. Read from gzip file using gzputc (reading character by character)
    //    and gzputs (reading string)
    const int buf_size = 1024;
    char buffer[buf_size];
    int total_read = 0;
    
    // Read using gzgetc (not in required APIs, but we need to read to demonstrate)
    // We'll simulate reading by getting a char and putting it back
    for (int i = 0; i < buf_size - 1 && i < (int)size; i++) {
        int c = gzgetc(gz_in);
        if (c == -1) break;
        buffer[i] = (char)c;
        total_read++;
        
        // Put it back using gzungetc to demonstrate similar functionality
        gzungetc(c, gz_in);
        c = gzgetc(gz_in);  // Read it again
    }
    buffer[total_read] = '\0';
    
    // Try to read as string using gzgets
    char str_buf[256];
    if (gzgets(gz_in, str_buf, sizeof(str_buf)) == NULL) {
        // Handle end of file or error
    }

    // 6. Get current position using gztell64
    off64_t pos = gztell64(gz_in);
    (void)pos;  // Use pos to avoid unused variable warning

    // 7. Create output FILE* and get descriptor
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        inflateEnd(&strm);
        gzclose_r(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    int fd_out = fuzz_fileno(out_file);
    if (fd_out == -1) {
        assert_file_closed(&out_file);;
        inflateEnd(&strm);
        gzclose_r(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 8. Open gzip file for writing using gzdopen
    gzFile gz_out = gzdopen(fd_out, "wb");
    if (!gz_out) {
        assert_file_closed(&out_file);;
        inflateEnd(&strm);
        gzclose_r(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 9. Write to gzip file using gzputc and gzputs
    // Write the buffer we read earlier
    for (int i = 0; i < total_read; i++) {
        gzputc(gz_out, buffer[i]);
    }
    
    // Write a string using gzputs
    const char *test_string = "Processed by zlib fuzzer\n";
    gzputs(gz_out, test_string);

    // 10. Use compress2 and compress2_z on the input data
    // First with compress2
    uLongf compressed_size = compressBound((uLong)size);
    Bytef *compressed = (Bytef *)malloc(compressed_size);
    if (compressed) {
        int ret_compress = compress2(compressed, &compressed_size, 
                                     (const Bytef *)data, (uLong)size, 6);
        (void)ret_compress;  // Use return value to avoid warning
        
        // Write compressed data to gzip file
        for (uLongf i = 0; i < compressed_size && i < 100; i++) {
            gzputc(gz_out, compressed[i]);  // Write first 100 bytes
        }
        free(compressed);
    }

    // Then with compress2_z
    z_size_t compressed_size_z = compressBound_z((z_size_t)size);
    Bytef *compressed_z = (Bytef *)malloc(compressed_size_z);
    if (compressed_z) {
        int ret_compress_z = compress2_z(compressed_z, &compressed_size_z,
                                        (const Bytef *)data, (z_size_t)size, 6);
        (void)ret_compress_z;  // Use return value to avoid warning
        
        // Write compressed data to gzip file
        for (z_size_t i = 0; i < compressed_size_z && i < 100; i++) {
            gzputc(gz_out, compressed_z[i]);  // Write first 100 bytes
        }
        free(compressed_z);
    }

    // 11. Clean up all resources
    // Close writing gzip file with gzclose
    gzclose(gz_out);
    
    // Note: gzclose already closed the underlying fd_out and out_file
    // We don't need to assert_file_closed(&out_file); separately
    
    // Clean up inflation stream
    inflateEnd(&strm);
    
    // Close reading gzip file with gzclose_r
    gzclose_r(gz_in);
    
    // Close input file
    assert_file_closed(&in_file);;

    // Remove the output file we created
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}