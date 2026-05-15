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
//<ID> 240
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"gz_read":[[368,18,368,36,0,0,4,1]]},"library_calls":["gzopen","gzclearerr","compressBound_z","gzclose","gzread","inflateInit2_","inflateGetDictionary","inflateEnd","adler32","adler32","adler32_combine64","compressBound_z","gzclose"],"critical_calls":["gzopen","gzclearerr","compressBound_z","gzread","inflateInit2_","inflateGetDictionary","inflateEnd","adler32","adler32","adler32_combine64","compressBound_z","gzclose"],"visited":0}
/*Here's a fuzz driver that uses the specified zlib APIs to achieve decompression and checksum combination:

*/


extern "C" int LLVMFuzzerTestOneInput_86(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 4) {
        return 0;
    }

    // Use gzopen to open a temporary file containing the compressed data
    FILE *tmp_in = fopen("input_file", "wb");
    if (!tmp_in) {
        assert_file_closed(&tmp_in);
	return 0;
    }
    fwrite(data, 1, size, tmp_in);
    assert_file_closed(&tmp_in);;

    // Open the compressed file with gzopen
    gzFile gz = gzopen("input_file", "rb");
    if (!gz) {
        remove("input_file");
        assert_file_closed(&tmp_in);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // Clear any existing error state
    gzclearerr(gz);

    // Calculate bound for decompressed size using compressBound_z
    // Note: compressBound_z gives maximum compressed size, so we use it as 
    // an estimate for decompression buffer
    z_size_t max_decompressed_size = compressBound_z(size * 2) * 2;
    if (max_decompressed_size == 0 || max_decompressed_size > 10 * 1024 * 1024) {
        max_decompressed_size = 10 * 1024 * 1024; // Cap at 10MB
    }

    Bytef *decompressed = (Bytef*)malloc(max_decompressed_size);
    if (!decompressed) {
        gzclose(gz);
        remove("input_file");
        assert_file_closed(&tmp_in);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // Decompress the data
    int decompressed_size = gzread(gz, decompressed, max_decompressed_size - 1);
    
    // Get the dictionary used for inflation (if any)
    z_stream strm;
    Bytef dictionary[32768];
    uInt dictLength = sizeof(dictionary);
    memset(&strm, 0, sizeof(strm));
    
    // Initialize a z_stream for inflation to get dictionary
    if (inflateInit2(&strm, MAX_WBITS + 16) == Z_OK) {
        // Try to get the dictionary (this may not succeed if no dictionary was used)
        inflateGetDictionary(&strm, dictionary, &dictLength);
        inflateEnd(&strm);
    }

    // Calculate Adler-32 checksums for two halves of decompressed data
    uLong adler1 = 0, adler2 = 0;
    size_t half_size = decompressed_size / 2;
    
    if (half_size > 0) {
        adler1 = adler32(0L, decompressed, half_size);
        if (decompressed_size - half_size > 0) {
            adler2 = adler32(0L, decompressed + half_size, decompressed_size - half_size);
        }
    }

    // Combine the two Adler-32 checksums using adler32_combine64
    // Use file size as the length parameter
    off64_t file_size = decompressed_size;
    uLong combined_adler = adler32_combine64(adler1, adler2, file_size);

    // Use compressBound_z to calculate maximum compressed size
    // (Even though we're decompressing, this API must be used)
    z_size_t bound_size = compressBound_z(decompressed_size);

    // Cleanup
    free(decompressed);
    gzclose(gz);
    remove("input_file");
    
    // Use the results to avoid optimization
    volatile uLong dummy = combined_adler;
    volatile z_size_t dummy2 = bound_size;
    (void)dummy;
    (void)dummy2;

    assert_file_closed(&tmp_in);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}