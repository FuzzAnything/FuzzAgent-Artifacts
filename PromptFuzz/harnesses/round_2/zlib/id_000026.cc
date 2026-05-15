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
//<ID> 52
//<Prompt> []
/*<Combination>: [
*/
//<score> 8.666667, nr_unique_branch: 1
//<Quality> {"density":13,"unique_branches":{"gz_read":[[323,9,323,17,0,0,4,0]]},"library_calls":["gzdopen","gzoffset","gzrewind","gzclearerr","gzread","deflateInit_","deflateTune","deflateGetDictionary","deflateBound","inflateInit_","inflateGetHeader","inflateGetDictionary","inflateSyncPoint","inflateEnd","compressBound_z","compress2_z","deflateEnd","gzopen","gzread","gzclose","gzclose"],"critical_calls":["gzdopen","gzoffset","gzrewind","gzclearerr","gzread","deflateInit_","deflateTune","deflateGetDictionary","deflateBound","inflateInit_","inflateGetHeader","inflateGetDictionary","inflateSyncPoint","inflateEnd","compressBound_z","compress2_z","deflateEnd","gzopen","gzread","gzclose","gzclose"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // If size is too small for meaningful operations, return early
    if (size < 10) {
        return 0;
    }

    // Step 1: Create a temporary input file with the compressed data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 2: Open the compressed file using gzdopen (requires file descriptor)
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // We'll use the first byte to determine compression level (0-9)
    int level = data[0] % 10;
    const char *mode = (data[1] % 2) ? "rb" : "r";  // Randomly choose mode

    gzFile gz_in = gzdopen(fd, mode);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Step 3: Use gzoffset to get current offset (should be 0 initially)
    off_t offset = gzoffset(gz_in);
    (void)offset;  // Avoid unused variable warning

    // Step 4: Try to rewind the gz file
    int rewind_result = gzrewind(gz_in);
    (void)rewind_result;  // Avoid unused variable warning

    // Step 5: Clear any errors in the gz file
    gzclearerr(gz_in);

    // Step 6: Read some data from the gz file
    // Use a small portion of size for reading to avoid buffer overflow
    size_t read_size = size % 1024;
    if (read_size > size) read_size = size;
    
    voidp read_buf = malloc(read_size);
    if (read_buf) {
        int bytes_read = gzread(gz_in, read_buf, read_size);
        (void)bytes_read;  // Avoid unused variable warning
        
        // If we successfully read some data, use it for further operations
        if (bytes_read > 0) {
            // Step 7: Initialize a z_stream for deflation
            z_stream def_strm;
            memset(&def_strm, 0, sizeof(def_strm));
            
            // Initialize deflate with specific level and version
            int init_result = deflateInit_(&def_strm, level, ZLIB_VERSION, sizeof(z_stream));
            if (init_result == Z_OK) {
                // Step 8: Tune deflate parameters
                // Use data bytes to generate tuning parameters
                int good_length = data[2] % 256;
                int max_lazy = data[3] % 256;
                int nice_length = data[4] % 256;
                int max_chain = data[5] % 256;
                deflateTune(&def_strm, good_length, max_lazy, nice_length, max_chain);
                
                // Step 9: Get dictionary from deflate stream (if any)
                Bytef dict_buf[256];
                uInt dict_len = 256;
                int get_dict_result = deflateGetDictionary(&def_strm, dict_buf, &dict_len);
                (void)get_dict_result;  // Avoid unused variable warning
                
                // Step 10: Calculate deflate bound
                uLong source_len = bytes_read;
                uLong bound = deflateBound(&def_strm, source_len);
                (void)bound;  // Avoid unused variable warning
                
                // Step 11: Initialize a z_stream for inflation
                z_stream inf_strm;
                memset(&inf_strm, 0, sizeof(inf_strm));
                
                // Use the data we read as input for inflation
                inf_strm.next_in = (Bytef *)read_buf;
                inf_strm.avail_in = bytes_read;
                
                // Initialize inflate
                int inflate_init_result = inflateInit_(&inf_strm, ZLIB_VERSION, sizeof(z_stream));
                if (inflate_init_result == Z_OK) {
                    // Step 12: Get header from inflate stream
                    gz_header head;
                    memset(&head, 0, sizeof(head));
                    int header_result = inflateGetHeader(&inf_strm, &head);
                    (void)header_result;  // Avoid unused variable warning
                    
                    // Step 13: Get dictionary from inflate stream (if any)
                    Bytef inf_dict_buf[256];
                    uInt inf_dict_len = 256;
                    int inf_get_dict = inflateGetDictionary(&inf_strm, inf_dict_buf, &inf_dict_len);
                    (void)inf_get_dict;  // Avoid unused variable warning
                    
                    // Step 14: Check sync point in inflate stream
                    int sync_point = inflateSyncPoint(&inf_strm);
                    (void)sync_point;  // Avoid unused variable warning
                    
                    // Clean up inflate stream
                    inflateEnd(&inf_strm);
                }
                
                // Step 15: Use compress2_z with the read data
                z_size_t dest_len = compressBound_z(bytes_read);
                Bytef *compressed_buf = (Bytef *)malloc(dest_len);
                if (compressed_buf) {
                    // Use a random level from the data
                    int comp_level = data[6] % 10;
                    int compress_result = compress2_z(compressed_buf, &dest_len, 
                                                     (const Bytef *)read_buf, bytes_read, comp_level);
                    (void)compress_result;  // Avoid unused variable warning
                    free(compressed_buf);
                }
                
                // Step 16: Clean up deflate stream
                deflateEnd(&def_strm);
            }
        }
        
        free(read_buf);
    }

    // Step 17: Try to open another gz file using gzopen
    // We need to write some data to a file first
    const char *temp_filename = "temp_input.gz";
    FILE *temp_file = fopen(temp_filename, "wb");
    if (temp_file) {
        // Write a small portion of the input data
        size_t write_size = (size < 100) ? size : 100;
        fwrite(data, 1, write_size, temp_file);
        assert_file_closed(&temp_file);;
        
        // Now open it with gzopen
        gzFile gz_in2 = gzopen(temp_filename, "rb");
        if (gz_in2) {
            // Read a small amount of data
            char small_buf[100];
            int read_result = gzread(gz_in2, small_buf, sizeof(small_buf));
            (void)read_result;  // Avoid unused variable warning
            
            // Close the gz file
            gzclose(gz_in2);
        }
        
        // Remove temporary file
        remove(temp_filename);
    }

    // Step 18: Close the original gz file
    gzclose(gz_in);
    
    // Step 19: Close the input file (fmemopen)
    assert_file_closed(&in_file);;

    assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}