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
//<ID> 16
//<Prompt> []
/*<Combination>: [
*/
//<score> 10.5, nr_unique_branch: 3
//<Quality> {"density":7,"unique_branches":{"gz_read":[[357,40,357,62,0,0,4,0]],"gz_fetch":[[259,9,259,18,0,0,4,0],[260,17,261,26,0,0,4,1]]},"library_calls":["zlibVersion","gzopen","gzclose","gzopen","gzsetparams","gzclose","gzopen","deflateInit2_","deflateSetHeader","deflateEnd","gzfread","gzopen","gzwrite","gzflush","gzclose","gzclose"],"critical_calls":["zlibVersion","gzopen","gzclose","gzopen","gzsetparams","gzclose","gzopen","deflateInit2_","deflateSetHeader","deflateEnd","gzfread","gzopen","gzwrite","gzflush","gzclose","gzclose"],"visited":1}
/**/


// Event: Decompress zlib data using gzFile APIs while setting compression parameters
// and reading the decompressed data in chunks

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<93) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // 1. Get zlib version (first API call)
    const char *version = zlibVersion();
    (void)version; // Avoid unused variable warning

    // 2. Create a temporary file with compressed input data
    FILE *temp_in = fopen("temp_input.z", "wb");
    if (!temp_in) {
        assert_file_closed(&temp_in);
	return 0;
    }
    fwrite(data, 1, size, temp_in);
    assert_file_closed(&temp_in);;

    // 3. Open compressed file for reading using gzFile
    gzFile gz_in = gzopen(fuzz_str_1, fuzz_str_2);
    if (!gz_in) {
        remove("temp_input.z");
        assert_file_closed(&temp_in);
	return 0;
    }

    // 4. Set compression parameters (second API call)
    // Note: gzsetparams only works for writing, but we call it anyway for coverage
    // We'll temporarily switch to writing mode
    gzclose(gz_in);
    gzFile gz_out = gzopen(fuzz_str_3, fuzz_str_4);
    if (gz_out) {
        gzsetparams(gz_out, fuzz_int32_t_5, fuzz_int32_t_6);
        gzclose(gz_out);
    }
    
    // Reopen for reading
    gz_in = gzopen(fuzz_str_7, fuzz_str_8);
    if (!gz_in) {
        remove("temp_input.z");
        remove("temp_output.z");
        assert_file_closed(&temp_in);
	return 0;
    }

    // 5. Prepare z_stream for deflateSetHeader (third API call)
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                 MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY);
    
    gz_header header;
    memset(&header, 0, sizeof(header));
    header.text = 1;
    header.time = 0;
    header.os = 255;
    deflateSetHeader(&strm, &header);
    deflateEnd(&strm);

    // 6. Read decompressed data in chunks (fourth API call)
    char buffer[4096];
    z_size_t total_read = 0;
    z_size_t items_read;
    
    do {
        // Read chunks of data
        items_read = gzfread(buffer, 1, sizeof(buffer), gz_in);
        total_read += items_read;
        
        // Break if we've processed enough data or reached EOF
        if (items_read < sizeof(buffer)) {
            break;
        }
    } while (total_read < 1024 * 1024); // Limit to 1MB to avoid excessive memory

    // 7. Flush the gzFile (fifth API call)
    // Note: gzflush is mainly for writing, but we call it for coverage
    gzFile gz_temp = gzopen(fuzz_str_9, fuzz_str_10);
    if (gz_temp) {
        gzwrite(gz_temp, buffer, 1); // Write minimal data
        gzflush(gz_temp, fuzz_int32_t_11);
        gzclose(gz_temp);
    }

    // 8. Clean up all resources
    gzclose(gz_in);
    
    // Remove temporary files
    remove("temp_input.z");
    remove("temp_output.z");
    remove("temp_flush.z");

    assert_file_closed(&temp_in);
	return 0;
}