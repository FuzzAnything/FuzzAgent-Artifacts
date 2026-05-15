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
//<ID> 144
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 4
//<Quality> {"density":7,"unique_branches":{"deflateBound_z":[[917,28,917,49,0,0,4,0],[918,17,918,42,0,0,4,0],[918,46,918,54,0,0,4,0],[920,16,920,39,0,0,4,1]]},"library_calls":["gzdopen","gzclose","gzoffset","gzerror","zError","deflateInit2_","deflateBound","deflate","deflateCopy","deflate","deflateEnd","deflateEnd","gzclose"],"critical_calls":["gzdopen","gzoffset","gzerror","zError","deflateInit2_","deflateBound","deflate","deflateCopy","deflate","deflateEnd","deflateEnd","gzclose"],"visited":0}
/*Here's a fuzz driver that uses the specified zlib APIs to process compressed data:

*/


extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor and open as gzFile
    int fd = fuzz_fileno(in_file);
    gzFile gz_file = gzdopen(fd, "rb");
    if (!gz_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    int err = 0;
    int *errnum = &err;
    int read_char = 0;
    off_t offset = 0;

    // 1. Read compressed data byte by byte using gzgetc
    while ((read_char = gzgetc(gz_file)) != -1) {
        // Write to output file
        fputc(read_char, out_file);
    }

    // 2. Get current offset in compressed file
    offset = gzoffset(gz_file);

    // 3. Check for errors using gzerror
    const char *error_msg = gzerror(gz_file, errnum);
    
    // 4. Use zError to get error string for the error code
    const char *zlib_error_msg = zError(*errnum);

    // 5. Create and use deflateCopy
    z_stream source_stream, dest_stream;
    memset(&source_stream, 0, sizeof(z_stream));
    memset(&dest_stream, 0, sizeof(z_stream));

    // Initialize source stream for compression
    int init_result = deflateInit2(&source_stream, 
                                   Z_DEFAULT_COMPRESSION,
                                   Z_DEFLATED,
                                   MAX_WBITS,
                                   MAX_MEM_LEVEL,
                                   Z_DEFAULT_STRATEGY);
    
    if (init_result == Z_OK) {
        // Set up source stream with some data
        Bytef source_data[] = "test data";
        uLong source_len = strlen((char *)source_data);
        
        source_stream.next_in = source_data;
        source_stream.avail_in = source_len;
        
        // Allocate output buffer
        uLong dest_len = deflateBound(&source_stream, source_len);
        Bytef *dest_data = (Bytef *)malloc(dest_len);
        
        if (dest_data) {
            source_stream.next_out = dest_data;
            source_stream.avail_out = dest_len;
            
            // Perform some compression
            deflate(&source_stream, Z_FINISH);
            
            // Now copy the compression state
            int copy_result = deflateCopy(&dest_stream, &source_stream);
            
            if (copy_result == Z_OK) {
                // Use the copied stream
                Bytef copied_dest[1024];
                dest_stream.next_out = copied_dest;
                dest_stream.avail_out = sizeof(copied_dest);
                
                deflate(&dest_stream, Z_FINISH);
                
                // Clean up copied stream
                deflateEnd(&dest_stream);
            }
            
            free(dest_data);
        }
        
        // Clean up source stream
        deflateEnd(&source_stream);
    }

    // Clean up all resources
    assert_file_closed(&out_file);;
    gzclose(gz_file);
    assert_file_closed(&in_file);;

    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}