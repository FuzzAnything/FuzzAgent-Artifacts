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
//<ID> 88
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"inflateGetHeader":[[1225,9,1225,31,0,0,4,0]]},"library_calls":["gzopen","gzbuffer","inflateInit_","gzclose","inflateGetHeader","crc32","inflate","crc32","gzfwrite","gztell64","deflateInit_","deflateParams","deflateEnd","crc32_combine_gen","inflateEnd","gzclose"],"critical_calls":["gzopen","gzbuffer","inflateInit_","inflateGetHeader","crc32","inflate","crc32","gzfwrite","gztell64","deflateInit_","deflateParams","deflateEnd","crc32_combine_gen","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
	if(f_size<29) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}




    // Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Open a gzip file for writing
    gzFile gz_out = gzopen("output_file.gz", fuzz_str_1);
    if (!gz_out) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file.gz");
	return 0;
    }

    // Set buffer size for gzip file
    gzbuffer(gz_out, fuzz_uint32_t_2);

    // Create z_stream for inflation (decompression)
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize inflation with zlib wrapper
    if (inflateInit(&strm) != Z_OK) {
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file.gz");
	return 0;
    }

    // Set up gzip header
    gz_header header;
    memset(&header, 0, sizeof(header));
    header.os = 255; // Unknown OS
    inflateGetHeader(&strm, &header);

    // Buffers for processing
    Bytef in_buf[4096];
    Bytef out_buf[4096];
    int ret = Z_OK;
    uLong crc = crc32(fuzz_uint64_t_3, Z_NULL, 0);
    off64_t file_pos = 0;

    // Process the input data
    while (!feof(in_file) && ret != Z_STREAM_END) {
        size_t bytes_read = fread(in_buf, 1, sizeof(in_buf), in_file);
        if (bytes_read == 0 && ferror(in_file)) {
            break;
        }

        strm.avail_in = (uInt)bytes_read;
        strm.next_in = in_buf;

        do {
            strm.avail_out = sizeof(out_buf);
            strm.next_out = out_buf;

            ret = inflate(&strm, fuzz_int32_t_4);
            
            if (ret == Z_OK || ret == Z_STREAM_END) {
                uInt have = sizeof(out_buf) - strm.avail_out;
                
                if (have > 0) {
                    // Update CRC of decompressed data
                    crc = crc32(crc, out_buf, have);
                    
                    // Write decompressed data to gzip file
                    z_size_t written = gzfwrite(out_buf, have, 1, gz_out);
                    if (written == 0) {
                        // Error writing
                        break;
                    }
                    
                    // Get current position in output file
                    file_pos = gztell64(gz_out);
                }
            }
        } while (strm.avail_out == 0);
    }

    // Create another z_stream for testing deflateParams
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    
    if (deflateInit(&deflate_strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
        // Change compression parameters
        deflateParams(&deflate_strm, fuzz_int32_t_5, fuzz_int32_t_6);
        
        // Clean up deflate stream
        deflateEnd(&deflate_strm);
    }

    // Generate CRC combination value
    // Using file_pos as the offset for combination
    uLong crc_combine_gen = crc32_combine_gen(file_pos);

    // Clean up
    inflateEnd(&strm);
    gzclose(gz_out);
    assert_file_closed(&in_file);;

    // Remove temporary output file
    remove("output_file.gz");

    assert_file_closed(&in_file);
	assert_file_name_closed("output_file.gz");
	assert_file_name_closed("output_file.gz");
	return 0;
}