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
//<ID> 60
//<Prompt> []
/*<Combination>: [
*/
//<score> 3, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["gzdopen","gzdirect","gztell","gzseek","gztell","gzclose_r","gzdopen","gzclose_w","inflateBackInit_","inflateEnd","inflateReset","deflateInit_","deflate","deflateUsed","deflateEnd","adler32","adler32","adler32","adler32","adler32_combine64","inflateBackEnd"],"critical_calls":["gzdopen","gzdirect","gztell","gzseek","gztell","gzclose_r","gzdopen","gzclose_w","inflateBackInit_","inflateReset","deflateInit_","deflate","deflateUsed","deflateEnd","adler32","adler32","adler32","adler32","adler32_combine64","inflateBackEnd"],"visited":1}
/*Here's a fuzz driver that uses all the specified zlib APIs to process compressed data from a byte stream:

*/


extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Process compressed data using gzFile operations
    if (size == 0) {
	return 0;
	}
	
    
    // Create a file for the input data
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
    
    // Open compressed file for reading
    gzFile gz_in = gzdopen(fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Check if file is direct access
    int is_direct = gzdirect(gz_in);
    (void)is_direct;  // Suppress unused variable warning
    
    // Get current position
    off_t pos1 = gztell(gz_in);
    
    // Seek to a position (try to seek within bounds)
    if (pos1 > 0 && pos1 < (off_t)size) {
        gzseek(gz_in, pos1 / 2, SEEK_SET);
    }
    
    // Get position again
    off_t pos2 = gztell(gz_in);
    
    // Close reading file
    gzclose_r(gz_in);
    // Note: in_file is closed by gzclose_r when using gzdopen
    
    // 2. Process the same data using inflate/deflate operations
    if (size < 10) {
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
	}
	  // Need enough data for processing
    
    // Create output file for writing compressed data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
	}
	
    
    int out_fd = fuzz_fileno(out_file);
    if (out_fd < 0) {
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Open file for writing compressed data
    gzFile gz_out = gzdopen(out_fd, "wb");
    if (!gz_out) {
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Close writing file
    gzclose_w(gz_out);
    // out_file is closed by gzclose_w when using gzdopen
    
    // 3. Setup inflation stream with inflateBackInit_
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Allocate window buffer
    unsigned char window[32768];  // 32K window
    
    // Initialize inflateBack stream
    int ret = inflateBackInit_(&strm, 15, window, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        // Cleanup if initialization failed
        inflateEnd(&strm);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Reset the inflation stream
    ret = inflateReset(&strm);
    (void)ret;  // Suppress unused variable warning
    
    // 4. Setup deflation stream for deflateUsed
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    
    // Initialize deflation
    ret = deflateInit(&def_strm, Z_DEFAULT_COMPRESSION);
    if (ret == Z_OK) {
        // Provide some input data for deflation
        if (size > 100) {
            uint8_t *compressed = (uint8_t *)malloc(size * 2);
            uint8_t *uncompressed = (uint8_t *)malloc(100);
            
            if (compressed && uncompressed) {
                // Copy some input data as uncompressed input
                size_t chunk_size = size > 100 ? 100 : size;
                memcpy(uncompressed, data, chunk_size);
                
                // Setup deflation stream
                def_strm.next_in = uncompressed;
                def_strm.avail_in = chunk_size;
                def_strm.next_out = compressed;
                def_strm.avail_out = size * 2;
                
                // Deflate some data
                ret = deflate(&def_strm, Z_FINISH);
                
                // Get bits used by deflate
                int bits_used = 0;
                ret = deflateUsed(&def_strm, &bits_used);
                (void)ret;  // Suppress unused variable warning
                (void)bits_used;  // Suppress unused variable warning
                
                free(compressed);
                free(uncompressed);
            } else {
                if (compressed) free(compressed);
                if (uncompressed) free(uncompressed);
            }
        }
        
        // End deflation
        deflateEnd(&def_strm);
    }
    
    // 5. Use adler32_combine64
    if (size >= 20) {
        // Split data into two parts
        size_t split = size / 2;
        
        // Calculate Adler-32 checksums for two parts
        uLong adler1 = adler32(0L, Z_NULL, 0);
        uLong adler2 = adler32(0L, Z_NULL, 0);
        
        if (split > 0) {
            adler1 = adler32(adler1, data, split);
        }
        if (size - split > 0) {
            adler2 = adler32(adler2, data + split, size - split);
        }
        
        // Combine the checksums
        off64_t len1 = split;
        uLong combined = adler32_combine64(adler1, adler2, len1);
        (void)combined;  // Suppress unused variable warning
    }
    
    // 6. Cleanup inflateBack stream
    inflateBackEnd(&strm);
    
    // Cleanup: remove temporary output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
}