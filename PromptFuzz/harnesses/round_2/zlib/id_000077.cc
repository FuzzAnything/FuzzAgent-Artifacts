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
//<ID> 255
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 2
//<Quality> {"density":13,"unique_branches":{"gzgetc":[[485,9,485,27,0,0,4,0],[485,31,485,56,0,0,4,1]]},"library_calls":["deflateInit_","deflateSetDictionary","deflateGetDictionary","deflatePending","gzdopen","deflateEnd","gzbuffer","gzwrite","gzputs","gzclose","deflateEnd","gzdopen","deflateEnd","gzbuffer","gztell","gzgets","gztell","gzoffset","crc32","crc32","crc32","crc32","crc32_combine","deflateEnd","gzclose_r"],"critical_calls":["deflateInit_","deflateSetDictionary","deflateGetDictionary","deflatePending","gzdopen","gzbuffer","gzwrite","gzputs","gzclose","gzdopen","gzbuffer","gztell","gzgets","gztell","gzoffset","crc32","crc32","crc32","crc32","crc32_combine","deflateEnd","gzclose_r"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if data is too small for meaningful operations
    if (size < 10) {
	return 0;
	}
	

    // Create temporary file with input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Create output file for compressed data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Initialize zlib stream for compression
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    int ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Set dictionary from input data (first 256 bytes or less)
    uInt dict_len = (size > 256) ? 256 : (uInt)size;
    ret = deflateSetDictionary(&strm, data, dict_len);
    
    // Get dictionary back to verify
    Bytef dict_buf[256];
    uInt dict_ret_len = 256;
    deflateGetDictionary(&strm, dict_buf, &dict_ret_len);

    // Check for pending data
    unsigned int pending;
    int bits;
    deflatePending(&strm, &pending, &bits);

    // Open gzFile for writing
    gzFile gz_out = gzdopen(out_fd, "wb");
    if (!gz_out) {
        deflateEnd(&strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Set buffer size for gzFile
    gzbuffer(gz_out, 8192);

    // Write data to gzFile in chunks
    size_t offset = 0;
    const size_t chunk_size = 1024;
    while (offset < size) {
        size_t to_write = (size - offset > chunk_size) ? chunk_size : size - offset;
        
        // Write using gzwrite
        int write_ret = gzwrite(gz_out, data + offset, (unsigned)to_write);
        if (write_ret <= 0) break;
        
        // Write using gzputs (for string data)
        if (offset == 0) {
            gzputs(gz_out, "\n--- Data Boundary ---\n");
        }
        
        offset += to_write;
    }

    // Close writing gzFile and reopen for reading
    gzclose(gz_out);
    assert_file_closed(&out_file);;
    
    // Reopen output file for reading
    out_file = fopen("output_file", "rb");
    if (!out_file) {
        deflateEnd(&strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    int read_fd = fuzz_fileno(out_file);
    gzFile gz_in = gzdopen(read_fd, "rb");
    if (!gz_in) {
        deflateEnd(&strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(read_fd);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Set buffer size for reading gzFile
    gzbuffer(gz_in, 4096);

    // Read data back using various methods
    char read_buf[1024];
    
    // Get current position
    off_t current_pos = gztell(gz_in);
    
    // Read using gzgets
    if (gzgets(gz_in, read_buf, sizeof(read_buf)) != Z_NULL) {
        // Process the line if needed
    }
    
    // Get position after read
    off_t after_pos = gztell(gz_in);
    
    // Get file offset
    off_t file_offset = gzoffset(gz_in);
    
    // Read single character
    int single_char = gzgetc(gz_in);
    
    // Calculate CRC32 of two segments for combining
    uLong crc1 = crc32(0L, Z_NULL, 0);
    uLong crc2 = crc32(0L, Z_NULL, 0);
    
    // Calculate CRC32 for first half of data
    size_t half = size / 2;
    if (half > 0) {
        crc1 = crc32(crc1, data, (uInt)half);
    }
    
    // Calculate CRC32 for second half of data
    if (size - half > 0) {
        crc2 = crc32(crc2, data + half, (uInt)(size - half));
    }
    
    // Combine the two CRC32 values
    uLong combined_crc = crc32_combine(crc1, crc2, (off_t)(size - half));

    // Clean up resources
    deflateEnd(&strm);
    gzclose_r(gz_in);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove temporary output file
    unlink("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(read_fd);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}