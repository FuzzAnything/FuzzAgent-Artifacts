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
//<ID> 90
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 1
//<Quality> {"density":8,"unique_branches":{"gz_skip":[[298,32,298,57,0,0,4,1]]},"library_calls":["gzopen64","gzseek64","gzread","adler32_z","adler32_z","gzerror","zError","gzclose"],"critical_calls":["gzopen64","gzseek64","gzread","adler32_z","adler32_z","gzerror","zError","gzclose"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified zlib APIs to process compressed data:

*/


extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size == 0) {
        return 0;
    }

    // Write input data to a temporary file for gzopen64 to read
    FILE *tmp_in = fopen("input_file", "wb");
    if (!tmp_in) {
        assert_file_closed(&tmp_in);
	return 0;
    }
    fwrite(data, 1, size, tmp_in);
    assert_file_closed(&tmp_in);;

    // Open the compressed file using gzopen64
    gzFile gz = gzopen64("input_file", "rb");
    if (!gz) {
        remove("input_file");
        assert_file_closed(&tmp_in);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // Try to seek within the file
    off64_t seek_pos = 0;
    if (size > 4) {
        // Use first 4 bytes to determine seek position (modulo size to stay within bounds)
        seek_pos = (data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24)) % size;
    }
    
    // Attempt to seek in the compressed file
    if (gzseek64(gz, seek_pos, SEEK_SET) != -1) {
        // If seek succeeded, read some data to compute Adler-32 checksum
        
        // Determine read size (at most 4096 bytes or remaining data)
        size_t read_size = 4096;
        if (size - seek_pos < read_size) {
            read_size = size - seek_pos;
        }
        
        Bytef *buffer = (Bytef *)malloc(read_size);
        if (buffer) {
            // Read decompressed data
            int bytes_read = gzread(gz, buffer, read_size);
            
            if (bytes_read > 0) {
                // Compute Adler-32 checksum of the read data
                uLong adler = adler32_z(0L, Z_NULL, 0);
                adler = adler32_z(adler, buffer, bytes_read);
                (void)adler; // Use the result to avoid compiler warnings
            }
            
            free(buffer);
        }
    }

    // Check for errors in gzip operations
    int errnum = 0;
    const char *error_msg = gzerror(gz, &errnum);
    if (errnum != Z_OK) {
        // Also get the error string using zError
        const char *z_error = zError(errnum);
        (void)z_error; // Use to avoid compiler warnings
        (void)error_msg;
    }

    // Close the gzip file
    int close_result = gzclose(gz);
    if (close_result != Z_OK) {
        // Handle close error if needed
    }

    // Clean up temporary file
    remove("input_file");

    assert_file_closed(&tmp_in);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}