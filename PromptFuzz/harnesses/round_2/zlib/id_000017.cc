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
//<ID> 24
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["zlibVersion","gzdopen","gzdirect","crc32","gzread","crc32","gzeof","gzdopen","deflateInit_","deflateSetDictionary","deflate","gzwrite","deflateReset","deflateEnd","gzclose","inflateInit_","inflate","inflateReset","inflateEnd","gzclose"],"critical_calls":["zlibVersion","gzdopen","gzdirect","crc32","gzread","crc32","gzeof","gzdopen","deflateInit_","deflateSetDictionary","deflate","gzwrite","deflateReset","deflateEnd","gzclose","inflateInit_","inflate","inflateReset","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Get zlib version
    const char* version = zlibVersion();
    (void)version; // Use version to avoid unused variable warning
    
    // 2. Create FILE* from input data using fmemopen
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Get file descriptor and open as gzFile
    int fd = fuzz_fileno(in_file);
    gzFile gz_input = gzdopen(fd, "rb");
    if (!gz_input) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 4. Check if the file is in direct mode (uncompressed)
    int direct = gzdirect(gz_input);
    (void)direct; // Use direct to avoid unused variable warning
    
    // 5. Prepare buffer for reading decompressed data
    Bytef buffer[4096];
    uLong crc = crc32(0L, Z_NULL, 0); // Initialize CRC
    
    // 6. Read data and compute CRC32
    int bytes_read = 0;
    while ((bytes_read = gzread(gz_input, buffer, sizeof(buffer))) > 0) {
        crc = crc32(crc, buffer, bytes_read);
    }
    
    // 7. Check for end of file
    int is_eof = gzeof(gz_input);
    (void)is_eof; // Use is_eof to avoid unused variable warning
    
    // 8. Create output file for writing compressed data
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        int out_fd = fuzz_fileno(out_file);
        gzFile gz_output = gzdopen(out_fd, "wb");
        
        if (gz_output) {
            // 9. Create z_stream for compression
            z_stream strm;
            memset(&strm, 0, sizeof(strm));
            
            // 10. Initialize deflate with default parameters
            if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
                // 11. Set dictionary (use first 256 bytes of input as dictionary if available)
                if (size > 256) {
                    Bytef dictionary[256];
                    memcpy(dictionary, data, 256);
                    deflateSetDictionary(&strm, dictionary, 256);
                }
                
                // 12. Prepare input for compression (use the original input data)
                strm.next_in = (Bytef*)data;
                strm.avail_in = (uInt)size;
                
                // 13. Compress data
                Bytef out_buffer[4096];
                do {
                    strm.next_out = out_buffer;
                    strm.avail_out = sizeof(out_buffer);
                    
                    int ret = deflate(&strm, Z_FINISH);
                    
                    // Write compressed data to gz file
                    size_t compressed_size = sizeof(out_buffer) - strm.avail_out;
                    if (compressed_size > 0) {
                        gzwrite(gz_output, out_buffer, compressed_size);
                    }
                } while (strm.avail_out == 0);
                
                // 14. Reset deflate stream
                deflateReset(&strm);
                
                // 15. End deflate
                deflateEnd(&strm);
            }
            
            // 16. Close output gz file
            gzclose(gz_output);
        }
        
        // 17. Close output file
        assert_file_closed(&out_file);;
        
        // Remove the output file we created
        remove("output_file");
    }
    
    // 18. Create another z_stream for inflation to use inflateReset
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    
    // 19. Initialize inflate
    if (inflateInit(&inflate_strm) == Z_OK) {
        // Set up input for inflation (use the original data)
        inflate_strm.next_in = (Bytef*)data;
        inflate_strm.avail_in = (uInt)(size > 100 ? 100 : size); // Use first 100 bytes
        
        Bytef out_buf[1024];
        inflate_strm.next_out = out_buf;
        inflate_strm.avail_out = sizeof(out_buf);
        
        // Try to inflate a small amount
        inflate(&inflate_strm, Z_SYNC_FLUSH);
        
        // 20. Reset inflate stream
        inflateReset(&inflate_strm);
        
        // 21. End inflate
        inflateEnd(&inflate_strm);
    }
    
    // 22. Clean up
    gzclose(gz_input); // This also closes the underlying file descriptor
    // Note: gzclose already closes the file descriptor, so we don't need to assert_file_closed(&in_file);
    // But to be safe, we'll still close it if gzclose didn't
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}