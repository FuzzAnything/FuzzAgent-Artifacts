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
//<ID> 7
//<Prompt> []
/*<Combination>: [
*/
//<score> 1.5, nr_unique_branch: 0
//<Quality> {"density":3,"unique_branches":{},"library_calls":["gzopen","gzopen","gzoffset64","inflateInit2_","adler32_z","adler32_z","adler32_combine","gzopen","gzwrite","gzclose_w","deflateInit2_","deflateEnd","inflateEnd","gzclose"],"critical_calls":["gzopen","gzopen","gzoffset64","inflateInit2_","adler32_z","adler32_z","adler32_combine","gzopen","gzwrite","gzclose_w","deflateInit2_","deflateEnd","inflateEnd","gzclose"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // 1. Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Open the input as a gzip file for reading
    gzFile gz_in = gzopen("input_file", "rb");
    if (!gz_in) {
        // Need to write data to file first since gzopen needs a file
        FILE *tmp = fopen("input_file", "wb");
        if (tmp) {
            fwrite(data, 1, size, tmp);
            assert_file_closed(&tmp);;
        }
        gz_in = gzopen("input_file", "rb");
    }
    
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 3. Get the current offset in the gzip file
    off64_t offset = gzoffset64(gz_in);
    
    // 4. Initialize inflation stream with windowBits for gzip format
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = Z_NULL;
    strm.avail_in = 0;
    
    // Use windowBits = 15 + 32 to automatically detect gzip header
    int init_result = inflateInit2_(&strm, 15 + 32, ZLIB_VERSION, sizeof(z_stream));
    
    // 5. Calculate Adler-32 checksums
    // First calculate checksum for part of the data
    size_t half_size = size / 2;
    uLong adler1 = 0;
    if (half_size > 0) {
        adler1 = adler32_z(adler1, data, half_size);
    }
    
    // Calculate checksum for the second part
    uLong adler2 = 0;
    if (size - half_size > 0) {
        adler2 = adler32_z(adler2, data + half_size, size - half_size);
    }
    
    // 6. Combine the Adler-32 checksums
    uLong combined_adler = adler32_combine(adler1, adler2, size - half_size);
    
    // 7. Open a gzip file for writing
    gzFile gz_out = gzopen("output_file", "wb");
    if (gz_out) {
        // Write some dummy data to the output file
        const char *dummy = "test data";
        gzwrite(gz_out, dummy, strlen(dummy));
        
        // Close the output file with gzclose_w
        gzclose_w(gz_out);
    }
    
    // 8. Setup deflation stream and end it
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    def_strm.zalloc = Z_NULL;
    def_strm.zfree = Z_NULL;
    def_strm.opaque = Z_NULL;
    
    // Initialize deflation (though we won't actually use it)
    int def_init = deflateInit2_(&def_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                                15 + 16, 8, Z_DEFAULT_STRATEGY,
                                ZLIB_VERSION, sizeof(z_stream));
    
    if (def_init == Z_OK) {
        // End the deflation stream (required API)
        deflateEnd(&def_strm);
    }
    
    // 9. Clean up resources
    
    // End inflation stream if initialized successfully
    if (init_result == Z_OK) {
        inflateEnd(&strm);
    }
    
    // Close input gzip file
    if (gz_in) {
        gzclose(gz_in);
    }
    
    // Close the file descriptor
    assert_file_closed(&in_file);;
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    // 10. Use the calculated values to prevent optimization
    volatile off64_t use_offset = offset;
    volatile uLong use_adler = combined_adler;
    (void)use_offset;
    (void)use_adler;
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}