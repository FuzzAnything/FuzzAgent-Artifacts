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
//<ID> 26
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 13
//<Quality> {"density":6,"unique_branches":{"gz_skip":[[298,18,298,28,0,0,4,1],[304,17,304,38,0,0,4,1],[307,14,307,25,0,0,4,0]],"inflate":[[570,44,570,61,0,0,4,1],[579,44,579,61,0,0,4,1],[590,44,590,61,0,0,4,1],[601,48,601,65,0,0,4,1],[622,52,622,69,0,0,4,1],[644,48,644,65,0,0,4,1],[666,48,666,65,0,0,4,1],[679,21,679,38,0,0,4,1],[1100,21,1100,38,0,0,4,1],[1111,9,1111,18,0,0,4,0]]},"library_calls":["zlibVersion","inflateInit2_","deflateInit_","inflateEnd","inflateResetKeep","deflateParams","inflateBackInit_","inflateBackEnd","inflateValidate","gzdopen","gzseek","gzread","gzclose","deflatePrime","inflate","deflate","deflate","deflateEnd","inflateEnd"],"critical_calls":["zlibVersion","inflateInit2_","deflateInit_","inflateResetKeep","deflateParams","inflateBackInit_","inflateBackEnd","inflateValidate","gzdopen","gzseek","gzread","gzclose","deflatePrime","inflate","deflate","deflate","deflateEnd","inflateEnd"],"visited":1}
/*Here's a C++ fuzz driver that uses all the specified zlib APIs to process zlib-compressed input data:

*/


extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful fuzzing

    // Get zlib version
    const char *version = zlibVersion();
    (void)version; // Use version to avoid unused variable warning

    // Create input FILE* from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Create output FILE* 
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
    (void)in_fd; (void)out_fd; // Use to avoid warnings

    // Initialize zlib structures
    z_stream strm_inflate, strm_deflate;
    memset(&strm_inflate, 0, sizeof(z_stream));
    memset(&strm_deflate, 0, sizeof(z_stream));

    // Setup inflation (decompression) with gzip/zlib format detection
    int ret = inflateInit2(&strm_inflate, 15 + 32); // 15+32 for gzip/zlib auto detection
    if (ret != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Setup deflation (compression)
    ret = deflateInit(&strm_deflate, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        inflateEnd(&strm_inflate);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Allocate buffers
    const size_t CHUNK = 1024;
    Bytef in_buffer[CHUNK];
    Bytef out_buffer[CHUNK];
    
    // 1. Use inflateResetKeep - reset inflate stream keeping history
    ret = inflateResetKeep(&strm_inflate);
    if (ret != Z_OK) {
        // Continue anyway
    }

    // 2. Use deflateParams - change compression parameters
    // Use data to determine level and strategy
    int level = data[0] % 10; // 0-9 compression level
    int strategy = data[1] % 4; // 0-3 strategy
    ret = deflateParams(&strm_deflate, level, strategy);
    if (ret != Z_OK) {
        // Continue anyway
    }

    // 3. Use inflateBackInit_ for back-inflation with custom window
    unsigned char *window = (unsigned char*)malloc(32768); // 32KB window
    if (window) {
        z_stream strm_back;
        memset(&strm_back, 0, sizeof(z_stream));
        
        // Initialize back inflation
        int windowBits = 15; // Maximum window size
        ret = inflateBackInit_(&strm_back, windowBits, window, 
                              ZLIB_VERSION, sizeof(z_stream));
        
        if (ret == Z_OK) {
            // Clean up back inflation stream
            inflateBackEnd(&strm_back);
        }
        free(window);
    }

    // 4. Use inflateValidate - enable/disable header validation
    int validate = data[2] & 1; // 0 or 1
    ret = inflateValidate(&strm_inflate, validate);
    if (ret != Z_OK) {
        // Continue anyway
    }

    // 5. Create gzFile and use gzseek
    gzFile gz_in = gzdopen(in_fd, "rb");
    if (gz_in) {
        // Seek to a position based on input data
        off_t seek_pos = (off_t)(data[3] % (size > 50 ? 50 : size));
        ret = gzseek(gz_in, seek_pos, SEEK_SET);
        
        // Read some data from gzFile
        char gz_buffer[100];
        int bytes_read = gzread(gz_in, gz_buffer, sizeof(gz_buffer));
        (void)bytes_read; // Use to avoid warning
        
        gzclose(gz_in);
    } else {
        // gzdopen failed, restore file pointer
        rewind(in_file);
    }

    // 6. Use deflatePrime - add bits to the bit buffer
    int bits = data[4] % 16; // 0-15 bits
    int value = data[5] % 256; // 0-255 value
    ret = deflatePrime(&strm_deflate, bits, value);
    if (ret != Z_OK) {
        // Continue anyway
    }

    // Process input data through inflate/deflate cycle
    strm_inflate.next_in = (Bytef*)data;
    strm_inflate.avail_in = size;
    
    do {
        // Set up output buffer
        strm_inflate.next_out = out_buffer;
        strm_inflate.avail_out = CHUNK;
        
        // Inflate (decompress) data
        ret = inflate(&strm_inflate, Z_NO_FLUSH);
        
        if (ret == Z_OK || ret == Z_STREAM_END) {
            size_t inflated_size = CHUNK - strm_inflate.avail_out;
            
            if (inflated_size > 0) {
                // Set up deflate with inflated data
                strm_deflate.next_in = out_buffer;
                strm_deflate.avail_in = inflated_size;
                
                do {
                    strm_deflate.next_out = in_buffer;
                    strm_deflate.avail_out = CHUNK;
                    
                    // Deflate (compress) the inflated data
                    ret = deflate(&strm_deflate, Z_NO_FLUSH);
                    
                    if (ret == Z_OK || ret == Z_STREAM_END) {
                        size_t deflated_size = CHUNK - strm_deflate.avail_out;
                        if (deflated_size > 0 && out_file) {
                            fwrite(in_buffer, 1, deflated_size, out_file);
                        }
                    }
                } while (strm_deflate.avail_out == 0 && ret == Z_OK);
            }
        }
    } while (strm_inflate.avail_in > 0 && ret == Z_OK);

    // Finalize deflation
    strm_deflate.avail_in = 0;
    do {
        strm_deflate.next_out = in_buffer;
        strm_deflate.avail_out = CHUNK;
        ret = deflate(&strm_deflate, Z_FINISH);
        
        if (ret == Z_OK || ret == Z_STREAM_END) {
            size_t deflated_size = CHUNK - strm_deflate.avail_out;
            if (deflated_size > 0 && out_file) {
                fwrite(in_buffer, 1, deflated_size, out_file);
            }
        }
    } while (ret == Z_OK);

    // Clean up
    deflateEnd(&strm_deflate);
    inflateEnd(&strm_inflate);
    
    assert_file_closed(&in_file);;
    if (out_file) assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}