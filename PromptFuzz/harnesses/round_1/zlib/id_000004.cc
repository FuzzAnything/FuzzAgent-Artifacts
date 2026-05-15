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
//<ID> 6
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["gzopen","gzdopen","gzclose","inflateInit2_","gzclose","gzclose","inflateEnd","gzclose","gzclose","inflateEnd","gzclose","gzclose","gzread","inflate","adler32_z","gzoffset64","gzopen","gzwrite","adler32_z","adler32_combine","deflateInit_","deflateEnd","gzclose_w","inflateEnd","inflateInit2_","inflateEnd","gzclose","gzclose"],"critical_calls":["gzopen","gzdopen","inflateInit2_","gzread","inflate","adler32_z","gzoffset64","gzopen","gzwrite","adler32_z","adler32_combine","deflateInit_","deflateEnd","gzclose_w","inflateEnd","inflateInit2_","inflateEnd","gzclose","gzclose"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified zlib APIs to process compressed data:

*/


extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Early return for insufficient data
    if (size < 10) {
        return 0;
    }

    // Create input file from the fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 1. Use gzopen to open the compressed file for reading
    gzFile gz_in = gzopen("input_file", "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }

    // Get file descriptor and re-open with gzdopen for consistency
    gzFile gz_in2 = gzdopen(fd, "rb");
    if (!gz_in2) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }

    // 2. Use inflateInit2_ to initialize inflation with appropriate window bits
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    
    int windowBits = 15 + 32; // Add 32 for automatic header detection
    int ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_in2);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }

    // Set up input buffer for inflation
    size_t chunk_size = size > 4096 ? 4096 : size;
    Bytef *in_buf = (Bytef *)malloc(chunk_size);
    if (!in_buf) {
        inflateEnd(&strm);
        gzclose(gz_in2);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }

    // Output buffer for inflation
    size_t out_size = size * 2;
    Bytef *out_buf = (Bytef *)malloc(out_size);
    if (!out_buf) {
        free(in_buf);
        inflateEnd(&strm);
        gzclose(gz_in2);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }

    // Read some data from gz file
    Bytef gz_read_buf[1024];
    int bytes_read = gzread(gz_in, gz_read_buf, sizeof(gz_read_buf));

    // Process data through inflation
    strm.avail_in = chunk_size;
    memcpy(in_buf, data, chunk_size);
    strm.next_in = in_buf;
    
    strm.avail_out = out_size;
    strm.next_out = out_buf;

    int inflate_ret = inflate(&strm, Z_NO_FLUSH);
    size_t inflated_bytes = out_size - strm.avail_out;

    // 3. Use adler32_z to compute Adler-32 checksum of inflated data
    uLong adler1 = 0;
    if (inflated_bytes > 0) {
        adler1 = adler32_z(adler1, out_buf, inflated_bytes);
    }

    // 4. Use gzoffset64 to get current offset (though file is at beginning)
    off64_t offset = gzoffset64(gz_in);

    // Create output file for writing compressed data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        int out_fd = fuzz_fileno(out_file);
        
        // 5. Use gzopen for writing compressed output
        gzFile gz_out = gzopen("output_file", "wb");
        if (gz_out) {
            // Write some data to the gzipped output file
            if (inflated_bytes > 0) {
                gzwrite(gz_out, out_buf, inflated_bytes);
            }
            
            // 6. Use adler32_combine to combine Adler-32 checksums
            // (using dummy values since we don't have two streams to combine)
            uLong adler2 = adler32_z(0, (const Bytef *)"test", 4);
            uLong combined = adler32_combine(adler1, adler2, 4);
            
            // 7. Use deflateEnd (even though we didn't use deflate,
            // we'll create a deflate stream just to end it)
            z_stream def_strm;
            def_strm.zalloc = Z_NULL;
            def_strm.zfree = Z_NULL;
            def_strm.opaque = Z_NULL;
            
            // Initialize deflate just to end it
            if (deflateInit(&def_strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
                // 8. Call deflateEnd on the stream
                deflateEnd(&def_strm);
            }
            
            // 9. Use gzclose_w to close the writing gz file
            gzclose_w(gz_out);
        }
        assert_file_closed(&out_file);;
    }

    // Clean up inflate stream
    inflateEnd(&strm);

    // 10. Also try to use inflateInit2_ again with different window bits
    // to ensure the API is called multiple times if possible
    z_stream strm2;
    strm2.zalloc = Z_NULL;
    strm2.zfree = Z_NULL;
    strm2.opaque = Z_NULL;
    inflateInit2_(&strm2, 15, ZLIB_VERSION, sizeof(z_stream));
    inflateEnd(&strm2);

    // Clean up all allocated resources
    free(in_buf);
    free(out_buf);
    
    // Close gz files
    gzclose(gz_in2);
    gzclose(gz_in);
    
    // Close file pointer
    assert_file_closed(&in_file);;

    // Remove temporary files if they exist
    remove("input_file");
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}