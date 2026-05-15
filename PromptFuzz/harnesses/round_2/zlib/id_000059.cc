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
//<ID> 159
//<Prompt> []
/*<Combination>: [
*/
//<score> 27, nr_unique_branch: 5
//<Quality> {"density":9,"unique_branches":{"inflate":[[360,13,360,22,104,0,4,0],[360,13,360,22,113,0,4,0]],"gzungetc":[[516,31,516,49,0,0,4,1],[520,9,520,27,0,0,4,0],[520,31,520,56,0,0,4,1]]},"library_calls":["compressBound","compress","compressBound_z","compress_z","crc32_z","crc32_z","crc32_z","crc32_z","crc32_combine","gzdopen","gzputc","gzclose_w","gzdopen","gzungetc","gzeof","gzfread","gzclose_r","inflateInit2_","inflateValidate","inflate","inflateEnd"],"critical_calls":["compressBound","compress","compressBound_z","compress_z","crc32_z","crc32_z","crc32_z","crc32_z","crc32_combine","gzdopen","gzputc","gzclose_w","gzdopen","gzungetc","gzeof","gzfread","gzclose_r","inflateInit2_","inflateValidate","inflate","inflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Return early if input is too small for meaningful processing
    if (size < 10) {
	return 0;
	}
	
    
    // Create input/output files using the specified methods
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    
    // Step 1: Process with compress/compress_z APIs
    // First compress the input data
    uLongf destLen = compressBound(size);
    Bytef *compressed = (Bytef *)malloc(destLen);
    if (!compressed) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Use compress API
    int compress_result = compress(compressed, &destLen, data, size);
    
    // Use compress_z API with the same data
    z_size_t destLen_z = compressBound_z(size);
    Bytef *compressed_z = (Bytef *)malloc(destLen_z);
    if (compressed_z) {
        compress_z(compressed_z, &destLen_z, data, size);
    }
    
    // Step 2: CRC32 calculations
    uLong crc1 = crc32_z(0L, Z_NULL, 0);
    uLong crc2 = crc32_z(0L, Z_NULL, 0);
    
    // Calculate CRC32 for first half of data
    size_t half_size = size / 2;
    if (half_size > 0) {
        crc1 = crc32_z(crc1, data, half_size);
    }
    
    // Calculate CRC32 for second half of data
    if (size - half_size > 0) {
        crc2 = crc32_z(crc2, data + half_size, size - half_size);
    }
    
    // Combine the two CRC32 values
    uLong combined_crc = crc32_combine(crc1, crc2, (off_t)size);
    
    // Step 3: gzFile operations
    // Open gzFile for writing
    gzFile gz_out = gzdopen(fd_out, "wb");
    if (gz_out) {
        // Write some data using gzputc
        for (size_t i = 0; i < std::min(size, (size_t)10); i++) {
            gzputc(gz_out, data[i]);
        }
        
        // Close and reopen for reading
        gzclose_w(gz_out);
    }
    
    // Reopen output file for reading
    assert_file_closed(&out_file);;
    out_file = fopen("output_file", "rb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        free(compressed);
        free(compressed_z);
        assert_file_closed(&out_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    int fd_out_read = fuzz_fileno(out_file);
    gzFile gz_in = gzdopen(fd_out_read, "rb");
    
    if (gz_in) {
        // Use gzungetc to push back a character
        int first_char = gzgetc(gz_in);
        if (first_char != EOF) {
            gzungetc(first_char, gz_in);
        }
        
        // Read data using gzfread
        Bytef read_buffer[1024];
        while (!gzeof(gz_in)) {
            z_size_t items_read = gzfread(read_buffer, 1, sizeof(read_buffer), gz_in);
            if (items_read == 0) break;
        }
        
        gzclose_r(gz_in);
    }
    
    // Step 4: Inflate validation
    // Prepare z_stream for inflate validation
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Use some of the compressed data for inflation validation
    if (destLen > 0) {
        strm.next_in = compressed;
        strm.avail_in = std::min((uInt)destLen, (uInt)100);
        
        // Initialize for inflation
        if (inflateInit2(&strm, 15 + 32) == Z_OK) {
            // Enable validation
            inflateValidate(&strm, 1);
            
            // Attempt to inflate a little
            Bytef out_buf[128];
            strm.next_out = out_buf;
            strm.avail_out = sizeof(out_buf);
            
            inflate(&strm, Z_SYNC_FLUSH);
            
            inflateEnd(&strm);
        }
    }
    
    // Clean up resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    free(compressed);
    if (compressed_z) free(compressed_z);
    
    // Remove the temporary output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out_read);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}