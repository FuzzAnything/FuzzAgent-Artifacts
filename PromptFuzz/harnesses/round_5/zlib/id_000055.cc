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
//<score> 36, nr_unique_branch: 2
//<Quality> {"density":12,"unique_branches":{"inflate":[[360,13,360,22,113,0,4,0]],"gzputc":[[334,13,334,31,0,0,4,1]]},"library_calls":["gzopen","gzputc","gztell64","gzclose_w","gzerror","gzopen","inflateInit_","gzclose","deflateCopy","inflatePrime","inflateSyncPoint","inflateSync","gzread","gzerror","inflateEnd","deflateEnd","gzclose"],"critical_calls":["gzopen","gzputc","gztell64","gzclose_w","gzerror","gzopen","inflateInit_","deflateCopy","inflatePrime","inflateSyncPoint","inflateSync","gzread","gzerror","inflateEnd","deflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Step 1: Open a temporary file for writing compressed data
    gzFile gz_out = gzopen("output_file", "wb");
    if (!gz_out) {
        assert_file_name_closed("output_file");
	return 0;
    }

    // Step 2: Write input data to the gz file using gzputc
    for (size_t i = 0; i < size; ++i) {
        gzputc(gz_out, data[i]);
    }

    // Step 3: Get current position in the gz file
    off64_t pos = gztell64(gz_out);
    (void)pos; // Prevent unused variable warning

    // Step 4: Close the gz file for writing
    int close_err = gzclose_w(gz_out);
    if (close_err != Z_OK) {
        // Check error using gzerror (though file is already closed)
        const char *err_msg = gzerror(gz_out, &close_err);
        (void)err_msg; // Prevent unused warning
    }

    // Step 5: Open the same file for reading
    gzFile gz_in = gzopen("output_file", "rb");
    if (!gz_in) {
        // Clean up the output file
        remove("output_file");
        assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	return 0;
    }

    // Step 6: Set up zlib stream for decompression
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    
    // Initialize inflate with default parameters
    if (inflateInit(&strm) != Z_OK) {
        gzclose(gz_in);
        remove("output_file");
        assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	return 0;
    }

    // Step 7: Create a second stream and copy the first one
    z_stream strm_copy;
    memset(&strm_copy, 0, sizeof(z_stream));
    int copy_result = deflateCopy(&strm_copy, &strm);
    (void)copy_result; // Result may fail, but we still continue

    // Step 8: Prime the inflate stream with some bits from input
    if (size > 0) {
        inflatePrime(&strm, 8, data[0] & 0x0F);
    }

    // Step 9: Check if we're at a sync point
    int sync_point = inflateSyncPoint(&strm);
    (void)sync_point; // Prevent unused warning

    // Step 10: Attempt to synchronize the inflate stream
    int sync_result = inflateSync(&strm);
    (void)sync_result; // Result may fail, but we still continue

    // Step 11: Read from the gz file to get decompressed data
    // We'll use a small buffer for reading
    uint8_t read_buf[1024];
    int bytes_read = 0;
    while ((bytes_read = gzread(gz_in, read_buf, sizeof(read_buf))) > 0) {
        // Process the read data if needed
        (void)bytes_read; // Prevent unused warning
    }

    // Step 12: Check for gz errors
    int gz_err = 0;
    const char *err_str = gzerror(gz_in, &gz_err);
    (void)err_str; // Prevent unused warning

    // Step 13: Clean up all resources
    inflateEnd(&strm);
    if (strm_copy.zalloc) { // Only clean up if copy was successful
        deflateEnd(&strm_copy);
    }
    gzclose(gz_in);
    
    // Remove temporary file
    remove("output_file");

    assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	return 0;
}