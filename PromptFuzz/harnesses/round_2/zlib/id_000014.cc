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
//<ID> 18
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 2
//<Quality> {"density":10,"unique_branches":{"gzfread":[[452,9,452,27,0,0,4,0],[452,31,452,56,0,0,4,1]]},"library_calls":["gzopen64","gzfread","adler32_z","adler32_z","gzseek64","gzerror","zError","gzfread","adler32_z","gzclose"],"critical_calls":["gzopen64","gzfread","adler32_z","adler32_z","gzseek64","gzerror","zError","gzfread","adler32_z","gzclose"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Early exit if there's no data
    if (size == 0) {
        return 0;
    }

    // Create a temporary file with the compressed data
    FILE *tmp_file = fopen("input_file", "wb");
    if (!tmp_file) {
        assert_file_closed(&tmp_file);
	return 0;
    }
    fwrite(data, 1, size, tmp_file);
    assert_file_closed(&tmp_file);;

    // Open the compressed file with gzopen64
    gzFile gz_file = gzopen64("input_file", "rb");
    if (gz_file == NULL) {
        remove("input_file");
        assert_file_closed(&tmp_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // Read some data from the gzipped file
    const int BUF_SIZE = 4096;
    unsigned char buffer[BUF_SIZE];
    z_size_t bytes_read = gzfread(buffer, 1, BUF_SIZE, gz_file);

    // Use adler32_z to compute checksum on the read data
    uLong adler = adler32_z(0L, Z_NULL, 0);
    if (bytes_read > 0) {
        adler = adler32_z(adler, buffer, bytes_read);
    }

    // Attempt to seek within the gzipped file
    // Try seeking to a position derived from the input size
    off64_t seek_pos = size % 100;  // Small offset to avoid large seeks
    off64_t seek_result = gzseek64(gz_file, seek_pos, SEEK_SET);

    // Check for errors using gzerror
    int errnum = 0;
    const char *error_msg = gzerror(gz_file, &errnum);
    if (errnum != Z_OK) {
        // Get the corresponding zlib error string
        const char *zlib_error = zError(errnum);
        // Error strings could be used for debugging
    }

    // Perform another read after seeking
    if (seek_result >= 0) {
        bytes_read = gzfread(buffer, 1, BUF_SIZE, gz_file);
        if (bytes_read > 0) {
            adler = adler32_z(adler, buffer, bytes_read);
        }
    }

    // Clean up
    gzclose(gz_file);
    remove("input_file");

    assert_file_closed(&tmp_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}