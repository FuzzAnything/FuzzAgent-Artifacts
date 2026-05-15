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
//<ID> 210
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["gzdopen","gzdirect","gztell","gzseek","gzclearerr","gzclose_r","gzclose_r","gzdopen","gzclose_r","deflateInit_","gzclose_w","gzclose_r","deflateSetDictionary","gzread","deflate","gzwrite","deflate","gzwrite","deflateUsed","deflateEnd","inflateBackInit_","gzclose_w","gzclose_r","inflateReset","inflateReset2","inflateBackEnd","adler32","adler32","adler32","adler32_combine64","gzclose_w","gzclose_r"],"critical_calls":["gzdopen","gzdirect","gztell","gzseek","gzclearerr","gzdopen","deflateInit_","deflateSetDictionary","gzread","deflate","gzwrite","deflate","gzwrite","deflateUsed","deflateEnd","inflateBackInit_","inflateReset","inflateReset2","inflateBackEnd","adler32","adler32","adler32","adler32_combine64","gzclose_w","gzclose_r"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a temporary file from the input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Get file descriptor for the input file
    int in_fd = fuzz_fileno(in_file);
    if (in_fd == -1) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 1. Open the input file as a gzFile for reading
    gzFile gz_in = gzdopen(in_fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 2. Check if the gzFile is in direct mode
    int is_direct = gzdirect(gz_in);
    (void)is_direct; // Use the result to avoid unused variable warning
    
    // 3. Get current position in the gzFile
    off_t current_pos = gztell(gz_in);
    
    // 4. Seek to a different position (wrap around using modulo)
    if (size > 0) {
        off_t seek_pos = current_pos % (size / 2 + 1);
        gzseek(gz_in, seek_pos, SEEK_SET);
    }
    
    // 5. Clear any error flags on the gzFile
    gzclearerr(gz_in);
    
    // Create output file for compressed data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        gzclose_r(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Get file descriptor for output file
    int out_fd = fuzz_fileno(out_file);
    if (out_fd == -1) {
        assert_file_closed(&out_file);;
        gzclose_r(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 6. Open output file as a gzFile for writing
    gzFile gz_out = gzdopen(out_fd, "wb");
    if (!gz_out) {
        assert_file_closed(&out_file);;
        gzclose_r(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Initialize z_stream for compression
    z_stream c_stream;
    memset(&c_stream, 0, sizeof(z_stream));
    
    // 7. Initialize deflate with default parameters
    int ret = deflateInit_(&c_stream, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose_w(gz_out);
        assert_file_closed(&out_file);;
        gzclose_r(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 8. Set dictionary for compression if we have enough data
    if (size >= 256) {
        Bytef dict[256];
        memcpy(dict, data, 256);
        deflateSetDictionary(&c_stream, dict, 256);
    }
    
    // Create input and output buffers
    Bytef in_buf[1024];
    Bytef out_buf[1024];
    
    // 9. Read from gzFile and compress the data
    int bytes_read;
    while ((bytes_read = gzread(gz_in, in_buf, sizeof(in_buf))) > 0) {
        c_stream.next_in = in_buf;
        c_stream.avail_in = bytes_read;
        
        do {
            c_stream.next_out = out_buf;
            c_stream.avail_out = sizeof(out_buf);
            
            // 10. Perform compression
            ret = deflate(&c_stream, Z_NO_FLUSH);
            if (ret != Z_OK && ret != Z_STREAM_END) break;
            
            size_t compressed_size = sizeof(out_buf) - c_stream.avail_out;
            if (compressed_size > 0) {
                // Write compressed data to output gzFile
                gzwrite(gz_out, out_buf, compressed_size);
            }
        } while (c_stream.avail_out == 0);
    }
    
    // 11. Finish compression
    do {
        c_stream.next_out = out_buf;
        c_stream.avail_out = sizeof(out_buf);
        
        ret = deflate(&c_stream, Z_FINISH);
        
        size_t compressed_size = sizeof(out_buf) - c_stream.avail_out;
        if (compressed_size > 0) {
            gzwrite(gz_out, out_buf, compressed_size);
        }
    } while (c_stream.avail_out == 0);
    
    // 12. Check used bits in deflate stream
    int used_bits;
    deflateUsed(&c_stream, &used_bits);
    (void)used_bits; // Use the result to avoid unused variable warning
    
    // 13. Clean up compression stream
    deflateEnd(&c_stream);
    
    // Initialize z_stream for inflation (backwards)
    z_stream i_stream;
    memset(&i_stream, 0, sizeof(z_stream));
    
    // Create window buffer for inflation
    unsigned char window[32768]; // 32K window
    
    // 14. Initialize inflateBack
    ret = inflateBackInit_(&i_stream, 15, window, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose_w(gz_out);
        assert_file_closed(&out_file);;
        gzclose_r(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 15. Reset inflate stream
    inflateReset(&i_stream);
    
    // 16. Reset inflate stream with different window bits
    inflateReset2(&i_stream, 15);
    
    // 17. Clean up inflation stream
    inflateBackEnd(&i_stream);
    
    // 18. Compute Adler-32 checksum of input data
    uLong adler1 = adler32(0L, Z_NULL, 0);
    if (size > 0) {
        size_t len1 = size / 2;
        adler1 = adler32(adler1, data, len1);
        
        // 19. Combine Adler-32 checksums
        uLong adler2 = adler32(0L, data + len1, size - len1);
        uLong combined = adler32_combine64(adler1, adler2, size);
        (void)combined; // Use the result to avoid unused variable warning
    }
    
    // Close all resources
    gzclose_w(gz_out);
    assert_file_closed(&out_file);;
    gzclose_r(gz_in);
    assert_file_closed(&in_file);;
    
    // Remove the output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}