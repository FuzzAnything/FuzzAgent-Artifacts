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
//<ID> 170
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"crc32_combine_op":[[970,9,970,16,0,0,4,0]]},"library_calls":["gzdopen","gzbuffer","crc32","crc32","gzread","crc32","gzrewind","crc32","gzgetc_","gzrewind","gzclearerr","gzclose","gzclose","gzdopen","gzclose","gzsetparams","gzwrite","inflateInit2_","gzclose","gzclose","inflate","crc32_combine","crc32_combine_op","inflateEnd","gzclose","gzclose"],"critical_calls":["gzdopen","gzbuffer","crc32","crc32","gzread","crc32","gzrewind","crc32","gzgetc_","gzrewind","gzclearerr","gzdopen","gzsetparams","gzwrite","inflateInit2_","inflate","crc32_combine","crc32_combine_op","inflateEnd","gzclose","gzclose"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_70(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor for gzdopen
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Open compressed stream for reading
    gzFile gz_in = gzdopen(fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Set buffer size
    gzbuffer(gz_in, 8192);

    // Prepare for CRC32 calculation
    uLong crc1 = crc32(0L, Z_NULL, 0);
    uLong crc2 = crc32(0L, Z_NULL, 0);
    
    // Buffer for reading
    Bytef buffer[1024];
    uInt bytes_read;

    // Read and compute CRC32 in chunks
    while ((bytes_read = gzread(gz_in, buffer, sizeof(buffer))) > 0) {
        crc1 = crc32(crc1, buffer, bytes_read);
        
        // Also use gzgetc for single byte reading
        for (uInt i = 0; i < bytes_read; i++) {
            // Use both gzgetc and gzgetc_
            int ch1 = gzgetc(gz_in);
            if (ch1 == -1) {
                // Reset file position after reading
                gzrewind(gz_in);
                break;
            }
            Bytef byte = static_cast<Bytef>(ch1);
            crc2 = crc32(crc2, &byte, 1);
            
            // Also use gzgetc_ (internal version)
            int ch2 = gzgetc_(gz_in);
            if (ch2 == -1) {
                gzrewind(gz_in);
                break;
            }
        }
    }

    // Clear any errors
    gzclearerr(gz_in);

    // Create output file for compression
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        gzclose(gz_in);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    int out_fd = fuzz_fileno(out_file);
    if (out_fd < 0) {
        assert_file_closed(&out_file);;
        gzclose(gz_in);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
    }

    // Open output file for writing with compression
    gzFile gz_out = gzdopen(out_fd, "wb");
    if (!gz_out) {
        assert_file_closed(&out_file);;
        gzclose(gz_in);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
    }

    // Set compression parameters
    gzsetparams(gz_out, Z_DEFAULT_COMPRESSION, Z_DEFAULT_STRATEGY);

    // Write some test data
    const char *test_data = "Test data for compression";
    gzwrite(gz_out, test_data, strlen(test_data));

    // Prepare for inflate operations
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = Z_NULL;
    strm.avail_in = 0;

    // Initialize inflate with gzip/zlib auto detection
    int windowBits = 15 + 32; // Enable gzip decoding
    int ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_out);
        gzclose(gz_in);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
    }

    // Setup input for inflate
    strm.next_in = (Bytef*)data;
    strm.avail_in = static_cast<uInt>(size > UINT_MAX ? UINT_MAX : size);
    
    // Setup output buffer
    Bytef out_buffer[1024];
    strm.next_out = out_buffer;
    strm.avail_out = sizeof(out_buffer);

    // Perform inflation
    ret = inflate(&strm, Z_FINISH);
    
    // Combine CRC32 values
    uLong combined_crc = crc32_combine(crc1, crc2, 0);
    
    // Also use crc32_combine_op
    uLong op = 0; // Typically this would be a precomputed operation
    uLong op_result = crc32_combine_op(crc1, crc2, op);

    // Cleanup
    inflateEnd(&strm);
    gzclose(gz_out);
    gzclose(gz_in);
    
    // The output file descriptor is closed by gzclose,
    // so we don't need to assert_file_closed(&out_file); separately
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
}