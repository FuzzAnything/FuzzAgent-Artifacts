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
//<ID> 114
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["gzdopen","gzdopen","gzclose","gzclose","deflateInit_","gzclose","gzclose","deflate","deflateEnd","gzclose","gzclose","deflatePending","gzgets","gzputs","crc32_z","crc32_z","crc32_z","crc32_combine64","inflateInit2_","inflate","inflateValidate","inflateEnd","inflateEnd","deflateEnd","gzclose","gzclose"],"critical_calls":["gzdopen","gzdopen","deflateInit_","deflate","deflatePending","gzgets","gzputs","crc32_z","crc32_z","crc32_z","crc32_combine64","inflateInit2_","inflate","inflateValidate","inflateEnd","deflateEnd","gzclose","gzclose"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit for minimal input requirements
    if (size < 10) {
	return 0;
	}
	

    // 1. Setup input/output files using provided patterns
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 2. gzdopen - Create gzFile from file descriptors
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    gzFile gz_in = gzdopen(fd_in, "rb");
    gzFile gz_out = gzdopen(fd_out, "wb");
    if (!gz_in || !gz_out) {
        if (gz_in) gzclose(gz_in);
        if (gz_out) gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 3. deflateInit_ - Initialize deflate stream
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    def_strm.zalloc = Z_NULL;
    def_strm.zfree = Z_NULL;
    def_strm.opaque = Z_NULL;
    
    int ret = deflateInit_(&def_strm, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_in);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 4. Use input data for deflate operation
    Bytef deflate_in[256];
    Bytef deflate_out[512];
    size_t deflate_input_size = size > 256 ? 256 : size;
    memcpy(deflate_in, data, deflate_input_size);
    
    def_strm.next_in = deflate_in;
    def_strm.avail_in = deflate_input_size;
    def_strm.next_out = deflate_out;
    def_strm.avail_out = sizeof(deflate_out);
    
    // Perform deflate with partial flush
    ret = deflate(&def_strm, Z_PARTIAL_FLUSH);
    if (ret != Z_OK && ret != Z_BUF_ERROR) {
        deflateEnd(&def_strm);
        gzclose(gz_in);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 5. deflatePending - Check for pending data
    unsigned int pending;
    int bits;
    ret = deflatePending(&def_strm, &pending, &bits);
    // We don't care about the return value for fuzzing

    // 6. gzgets/gzputs - Read from compressed input and write to compressed output
    char gzbuf[128];
    while (gzgets(gz_in, gzbuf, sizeof(gzbuf)) != nullptr) {
        // Write the read data to output gz file
        gzputs(gz_out, gzbuf);
        
        // Break early to avoid infinite loops with small inputs
        if (strlen(gzbuf) < sizeof(gzbuf) - 1) break;
    }

    // 7. crc32_z - Compute CRC32 checksum
    uLong crc1 = crc32_z(0L, Z_NULL, 0);
    if (size > 50) {
        crc1 = crc32_z(crc1, data, 50);
    }

    // 8. crc32_combine64 - Combine CRC32 values
    uLong crc2 = 0;
    if (size > 100) {
        crc2 = crc32_z(0L, data + 50, size - 50 > 50 ? 50 : size - 50);
    }
    uLong combined_crc = crc32_combine64(crc1, crc2, 100);

    // 9. inflateValidate - We need to set up an inflate stream for validation
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    inf_strm.zalloc = Z_NULL;
    inf_strm.zfree = Z_NULL;
    inf_strm.opaque = Z_NULL;
    
    // Initialize inflate stream (using inflateInit2_ from provided APIs)
    ret = inflateInit2_(&inf_strm, MAX_WBITS, ZLIB_VERSION, sizeof(z_stream));
    if (ret == Z_OK) {
        // Use a small portion of input data for inflation attempt
        if (size > 20) {
            Bytef inflate_in[128];
            Bytef inflate_out[256];
            memcpy(inflate_in, data, size > 128 ? 128 : size);
            
            inf_strm.next_in = inflate_in;
            inf_strm.avail_in = size > 128 ? 128 : size;
            inf_strm.next_out = inflate_out;
            inf_strm.avail_out = sizeof(inflate_out);
            
            // Try to inflate - this will likely fail with random data
            inflate(&inf_strm, Z_NO_FLUSH);
            
            // Attempt validation (this API validates compressed data)
            inflateValidate(&inf_strm, 1);
            
            inflateEnd(&inf_strm);
        } else {
            inflateEnd(&inf_strm);
        }
    }

    // 10. Cleanup
    deflateEnd(&def_strm);
    gzclose(gz_in);
    gzclose(gz_out);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove the output file we created
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}