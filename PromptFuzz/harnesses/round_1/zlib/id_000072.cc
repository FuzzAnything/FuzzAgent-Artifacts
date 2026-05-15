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
//<ID> 186
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"gzwrite":[[264,9,264,32,0,0,4,0]]},"library_calls":["gzdopen","gzdirect","gzread","gzerror","inflateInit2_","inflate","inflateSyncPoint","inflateEnd","deflateInit2_","deflateBound","deflate","deflateGetDictionary","deflateEnd","uncompress2","compressBound","compress2","gzwrite","gzputc","gztell64","gzseek64","gzclose"],"critical_calls":["gzdopen","gzdirect","gzread","gzerror","inflateInit2_","inflate","inflateSyncPoint","inflateEnd","deflateInit2_","deflateBound","deflate","deflateGetDictionary","deflateEnd","uncompress2","compressBound","compress2","gzwrite","gzputc","gztell64","gzseek64","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create temporary files for reading and writing
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
    
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    
    // 1. Open compressed stream using gzdopen
    gzFile gzfile = gzdopen(fd_in, "rb");
    if (!gzfile) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 2. Check if file is in gzip format
    int is_direct = gzdirect(gzfile);
    
    // 3. Read from gzfile
    unsigned char read_buf[4096];
    int bytes_read = gzread(gzfile, read_buf, sizeof(read_buf));
    
    // 4. Check for gz errors
    int gz_errnum;
    const char *gz_error_msg = gzerror(gzfile, &gz_errnum);
    
    // 5. Prepare for decompression using z_stream
    z_stream strm_inflate;
    memset(&strm_inflate, 0, sizeof(strm_inflate));
    
    // Initialize inflate with gzip/zlib auto-detection
    int window_bits = 15 + 32; // auto-detection of gzip/zlib
    int init_result = inflateInit2_(&strm_inflate, window_bits, 
                                   ZLIB_VERSION, sizeof(z_stream));
    
    if (init_result == Z_OK) {
        // Setup input for inflation
        strm_inflate.next_in = (Bytef*)data;
        strm_inflate.avail_in = size > UINT_MAX ? UINT_MAX : (uInt)size;
        
        // Create output buffer
        uLongf dest_len = 4096;
        Bytef* dest = (Bytef*)malloc(dest_len);
        
        if (dest) {
            strm_inflate.next_out = dest;
            strm_inflate.avail_out = dest_len;
            
            // 6. Perform inflation
            int inflate_result = inflate(&strm_inflate, Z_FINISH);
            
            // 7. Check sync point
            int sync_point = inflateSyncPoint(&strm_inflate);
            
            // 8. Clean up inflate
            inflateEnd(&strm_inflate);
            free(dest);
        }
    }
    
    // 9. Prepare for compression using z_stream
    z_stream strm_deflate;
    memset(&strm_deflate, 0, sizeof(strm_deflate));
    
    // Initialize deflate for gzip compression
    int deflate_init = deflateInit2_(&strm_deflate, Z_DEFAULT_COMPRESSION, 
                                    Z_DEFLATED, 15 + 16, 8, Z_DEFAULT_STRATEGY,
                                    ZLIB_VERSION, sizeof(z_stream));
    
    if (deflate_init == Z_OK) {
        // Setup input for compression (use the data we read earlier if available)
        uLong source_len = bytes_read > 0 ? bytes_read : 100;
        Bytef* source = (Bytef*)malloc(source_len);
        
        if (source) {
            if (bytes_read > 0) {
                memcpy(source, read_buf, bytes_read);
            } else {
                memset(source, 'A', source_len);
            }
            
            strm_deflate.next_in = source;
            strm_deflate.avail_in = source_len;
            
            // 10. Calculate bound for compressed data
            uLong bound = deflateBound(&strm_deflate, source_len);
            Bytef* compressed = (Bytef*)malloc(bound);
            
            if (compressed) {
                strm_deflate.next_out = compressed;
                strm_deflate.avail_out = bound;
                
                // Perform deflation
                deflate(&strm_deflate, Z_FINISH);
                
                // 11. Get dictionary (if any)
                uInt dict_length = 0;
                Bytef dictionary[256];
                deflateGetDictionary(&strm_deflate, dictionary, &dict_length);
                
                // Clean up deflate
                deflateEnd(&strm_deflate);
                free(compressed);
            }
            free(source);
        }
    }
    
    // 12. Test uncompress2
    if (size > 0) {
        uLongf uncompressed_len = 4096;
        Bytef* uncompressed_buf = (Bytef*)malloc(uncompressed_len);
        uLong source_len_used = size;
        
        if (uncompressed_buf) {
            uncompress2(uncompressed_buf, &uncompressed_len, 
                       data, &source_len_used);
            free(uncompressed_buf);
        }
    }
    
    // 13. Test compress2
    if (size > 0 && size < 1024) {
        uLongf compressed_len = compressBound(size);
        Bytef* compressed_buf = (Bytef*)malloc(compressed_len);
        
        if (compressed_buf) {
            compress2(compressed_buf, &compressed_len, data, size, Z_DEFAULT_COMPRESSION);
            free(compressed_buf);
        }
    }
    
    // 14. Write to gzfile
    const char* test_string = "Test data for writing";
    int write_result = gzwrite(gzfile, test_string, strlen(test_string));
    
    // 15. Put a character
    gzputc(gzfile, 'X');
    
    // 16. Get current position
    off64_t position = gztell64(gzfile);
    
    // 17. Seek in gzfile (if we can)
    if (position > 10) {
        gzseek64(gzfile, 10, SEEK_SET);
    }
    
    // 18. Close gzfile (both read and write)
    gzclose(gzfile);  // This closes the underlying fd_in
    
    // 19. Explicitly close for reading (already closed by gzclose, but call for coverage)
    // Note: gzfile is already closed, so we skip gzclose_r(gzfile) to avoid double close
    
    // Close the regular files
    assert_file_closed(&in_file);;  // Already closed by gzdopen? Actually fmemopen still needs closing
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}