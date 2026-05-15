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
//<ID> 62
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 1
//<Quality> {"density":5,"unique_branches":{"deflateUsed":[[738,9,738,32,0,0,4,0]]},"library_calls":["gzdopen","gzdirect","gztell","gzseek","inflateBackInit_","inflateInit_","inflate","deflateUsed","inflateReset","inflateEnd","inflateBackEnd","adler32","adler32","adler32","adler32","adler32_combine64","gzclose_r","gzdopen","gzwrite","gzclose_w"],"critical_calls":["gzdopen","gzdirect","gztell","gzseek","inflateBackInit_","inflateInit_","inflate","deflateUsed","inflateReset","inflateEnd","inflateBackEnd","adler32","adler32","adler32","adler32","adler32_combine64","gzclose_r","gzdopen","gzwrite","gzclose_w"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a FILE* for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Get file descriptor for reading
    int fd_in = fuzz_fileno(in_file);
    if (fd_in < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Open compressed file for reading using gzdopen
    gzFile gz_in = gzdopen(fd_in, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Check if file is in direct mode
    int is_direct = gzdirect(gz_in);
    
    // Get current position
    off_t current_pos = gztell(gz_in);
    
    // Try to seek to a position (use modulo to avoid overflow)
    if (size > 0) {
        off_t seek_pos = data[0] % (size + 1);
        gzseek(gz_in, seek_pos, SEEK_SET);
    }
    
    // Initialize z_stream for inflation with back-end support
    z_stream strm_back;
    unsigned char window[32768];  // 32K window as per zlib convention
    int windowBits = 15;  // Maximum window size
    int stream_size = sizeof(z_stream);
    
    int ret = inflateBackInit_(&strm_back, windowBits, window, 
                               ZLIB_VERSION, stream_size);
    
    // Initialize another z_stream for regular inflation/deflation
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize for inflation (to decompress the input data)
    if (inflateInit(&strm) == Z_OK) {
        // Set up input buffer from the data
        Bytef *in_buffer = (Bytef *)malloc(size);
        if (in_buffer) {
            memcpy(in_buffer, data, size);
            strm.next_in = in_buffer;
            strm.avail_in = size;
            
            // Create output buffer
            size_t out_size = size * 2 + 1024;  // Extra space for expansion
            Bytef *out_buffer = (Bytef *)malloc(out_size);
            if (out_buffer) {
                strm.next_out = out_buffer;
                strm.avail_out = out_size;
                
                // Try to inflate some data
                int flush = Z_NO_FLUSH;
                ret = inflate(&strm, flush);
                
                // Check how many bits were used by deflate (if we had deflated data)
                int bits_used = 0;
                deflateUsed(&strm, &bits_used);
                
                // Reset the inflation stream
                inflateReset(&strm);
                
                free(out_buffer);
            }
            free(in_buffer);
        }
        
        // End the inflation stream
        inflateEnd(&strm);
    }
    
    // End the back-end inflation stream
    inflateBackEnd(&strm_back);
    
    // Combine Adler-32 checksums from different parts of data
    if (size >= 4) {
        // Calculate Adler-32 for first and second halves
        uLong adler1 = adler32(0L, Z_NULL, 0);
        uLong adler2 = adler32(0L, Z_NULL, 0);
        
        size_t half1 = size / 2;
        size_t half2 = size - half1;
        
        if (half1 > 0) {
            adler1 = adler32(adler1, data, half1);
        }
        
        if (half2 > 0 && half1 < size) {
            adler2 = adler32(adler2, data + half1, half2);
        }
        
        // Combine the checksums (using 64-bit offset, cast appropriately)
        off64_t len1 = half1;
        uLong combined = adler32_combine64(adler1, adler2, len1);
    }
    
    // Close the gzFile for reading
    gzclose_r(gz_in);
    
    // Create a FILE* for writing output data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        int fd_out = fuzz_fileno(out_file);
        if (fd_out >= 0) {
            // Open compressed file for writing
            gzFile gz_out = gzdopen(fd_out, "wb");
            if (gz_out) {
                // Write some test data to the output file
                const char *test_data = "Test output data";
                gzwrite(gz_out, test_data, strlen(test_data));
                
                // Close the gzFile for writing
                gzclose_w(gz_out);
            }
        }
        assert_file_closed(&out_file);;
        
        // Remove the output file
        unlink("output_file");
    }
    
    // Note: assert_file_closed(&in_file); should not be called because gzdopen takes ownership
    // of the file descriptor. gzclose_r will close it.
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
}