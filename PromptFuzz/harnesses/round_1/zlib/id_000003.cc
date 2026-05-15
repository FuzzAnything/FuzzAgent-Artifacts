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
//<ID> 5
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 2
//<Quality> {"density":8,"unique_branches":{"gz_write":[[230,13,230,33,0,0,4,0],[230,37,230,69,0,0,4,1]]},"library_calls":["gzopen","gzoffset64","gzopen","gzclose","inflateInit2_","gzclose","gzclose","adler32_z","adler32_z","adler32_combine","gzwrite","inflate","gzwrite","deflateInit_","deflateEnd","inflateEnd","gzclose_w","gzclose"],"critical_calls":["gzopen","gzoffset64","gzopen","inflateInit2_","adler32_z","adler32_z","adler32_combine","gzwrite","inflate","gzwrite","deflateInit_","deflateEnd","inflateEnd","gzclose_w","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
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

    // Create a temporary file with the compressed data
    FILE *tmp_in = fmemopen((void *)data, size, "rb");
    if (!tmp_in) {
        assert_file_closed(&tmp_in);
	return 0;
    }

    // Generate file names
    const char *input_filename = "input_file";
    const char *output_filename = "output_file";

    // Write input data to a file for gzopen
    FILE *input_file = fopen(input_filename, "wb");
    if (!input_file) {
        assert_file_closed(&tmp_in);;
        assert_file_closed(&input_file);
	assert_file_closed(&tmp_in);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    fwrite(data, 1, size, input_file);
    assert_file_closed(&input_file);;

    // 1. Open compressed file for reading with gzopen
    gzFile gz_in = gzopen(input_filename, "rb");
    if (!gz_in) {
        remove(input_filename);
        assert_file_closed(&input_file);
	assert_file_closed(&tmp_in);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // 2. Get file offset using gzoffset64
    off64_t offset = gzoffset64(gz_in);

    // 3. Create output file for writing compressed data
    gzFile gz_out = gzopen(output_filename, "wb");
    if (!gz_out) {
        gzclose(gz_in);
        remove(input_filename);
        assert_file_closed(&input_file);
	assert_file_closed(&tmp_in);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // 4. Initialize inflation stream with inflateInit2_
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    // Try different windowBits values
    int windowBits_values[] = {MAX_WBITS, MAX_WBITS + 16, -MAX_WBITS};
    int init_result = Z_OK;
    
    for (int i = 0; i < 3; i++) {
        init_result = inflateInit2_(&strm, windowBits_values[i], ZLIB_VERSION, sizeof(z_stream));
        if (init_result == Z_OK) {
            break;
        }
    }
    
    if (init_result != Z_OK) {
        gzclose(gz_out);
        gzclose(gz_in);
        remove(input_filename);
        remove(output_filename);
        assert_file_closed(&input_file);
	assert_file_closed(&tmp_in);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // 5. Read and process data
    Bytef in_buffer[4096];
    Bytef out_buffer[8192];
    uLong adler1 = 0, adler2 = 0;
    
    // First pass: compute Adler-32 checksum of input data
    size_t bytes_processed = 0;
    while (bytes_processed < size) {
        size_t to_read = (size - bytes_processed) > sizeof(in_buffer) ? 
                        sizeof(in_buffer) : (size - bytes_processed);
        memcpy(in_buffer, data + bytes_processed, to_read);
        
        // Compute Adler-32 checksum using adler32_z
        adler1 = adler32_z(adler1, in_buffer, to_read);
        bytes_processed += to_read;
    }

    // 6. Compute second Adler-32 checksum (simulate processing of output)
    // Create some dummy data for demonstration
    Bytef dummy_data[100];
    memset(dummy_data, 'A', sizeof(dummy_data));
    adler2 = adler32_z(0, dummy_data, sizeof(dummy_data));

    // 7. Combine the two Adler-32 checksums using adler32_combine
    uLong combined_adler = adler32_combine(adler1, adler2, offset);

    // 8. Write combined Adler value to gzip file header (simplified)
    char header[100];
    snprintf(header, sizeof(header), "Combined Adler: %lu\n", combined_adler);
    gzwrite(gz_out, header, strlen(header));

    // 9. Process input file through inflation stream
    rewind(tmp_in);
    strm.avail_in = 0;
    int ret;
    
    do {
        if (strm.avail_in == 0) {
            strm.avail_in = fread(in_buffer, 1, sizeof(in_buffer), tmp_in);
            strm.next_in = in_buffer;
        }
        
        strm.avail_out = sizeof(out_buffer);
        strm.next_out = out_buffer;
        
        ret = inflate(&strm, Z_NO_FLUSH);
        
        size_t have = sizeof(out_buffer) - strm.avail_out;
        if (have > 0) {
            gzwrite(gz_out, out_buffer, have);
        }
    } while (ret == Z_OK);

    // 10. Clean up deflateEnd (even though we used inflate, we demonstrate deflateEnd)
    // Create a dummy deflate stream for demonstration
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = Z_NULL;
    dstrm.zfree = Z_NULL;
    dstrm.opaque = Z_NULL;
    
    if (deflateInit(&dstrm, Z_DEFAULT_COMPRESSION) == Z_OK) {
        // End the deflate stream
        deflateEnd(&dstrm);
    }

    // 11. End the inflation stream properly (not in the required list, but needed)
    inflateEnd(&strm);

    // 12. Close files with gzclose_w for output file
    gzclose_w(gz_out);
    
    // Close input gz file (using regular gzclose since it was opened for reading)
    gzclose(gz_in);

    // 13. Clean up temporary files
    assert_file_closed(&tmp_in);;
    remove(input_filename);
    remove(output_filename);

    assert_file_closed(&input_file);
	assert_file_closed(&tmp_in);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}