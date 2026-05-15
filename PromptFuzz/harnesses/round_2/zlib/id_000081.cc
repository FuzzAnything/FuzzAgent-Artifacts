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
//<ID> 273
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["zlibVersion","uncompress2_z","inflateInit2_","inflateValidate","inflate","inflateUndermine","inflateResetKeep","inflatePrime","inflateEnd","inflateBackInit_","inflateBack","inflateBackEnd","deflateInit_","deflateGetDictionary","compressBound_z","deflate","deflateReset","deflateEnd","gzdopen","gzopen","gzputc","gzflush","gzrewind","gzclose","gzclose"],"critical_calls":["zlibVersion","uncompress2_z","inflateInit2_","inflateValidate","inflate","inflateUndermine","inflateResetKeep","inflatePrime","inflateEnd","inflateBackInit_","inflateBack","inflateBackEnd","deflateInit_","deflateGetDictionary","compressBound_z","deflate","deflateReset","deflateEnd","gzdopen","gzopen","gzputc","gzflush","gzrewind","gzclose","gzclose"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified zlib APIs to test decompression and compression of zlib-formatted data:

*/


// Inflate callback functions for inflateBack
static unsigned int  id_000081_inf_in_callback(void *desc, unsigned char **buf) {
    // Simple callback that returns no data
    *buf = nullptr;
    return 0;
}

static int  id_000081_inf_out_callback(void *desc, unsigned char *buf, unsigned int size) {
    // Simple output callback that discards data
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_81(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for testing
    
    // 1. Get zlib version
    const char *version = zlibVersion();
    (void)version;  // Use version to avoid unused variable warning
    
    // 2. Try uncompress2_z first to decompress the input (assuming it's zlib compressed data)
    Bytef *uncompressed = nullptr;
    z_size_t uncompressed_len = 0;
    z_size_t source_len = size;
    
    // Allocate buffer for uncompressed data (guess size)
    uncompressed_len = size * 10;  // Arbitrary expansion factor
    uncompressed = (Bytef *)malloc(uncompressed_len);
    if (!uncompressed) {
	return 0;
	}
	
    
    int uncompress_result = uncompress2_z(uncompressed, &uncompressed_len, 
                                         data, &source_len);
    // We don't care if uncompress fails - input might not be valid zlib data
    
    // 3. Set up for inflate operations
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // 4. Initialize inflate with automatic header detection
    int init_result = inflateInit2_(&strm, 15 + 32, ZLIB_VERSION, sizeof(z_stream));
    if (init_result != Z_OK) {
        free(uncompressed);
        return 0;
    }
    
    // 5. Set validation
    inflateValidate(&strm, 1);
    
    // 6. Try to inflate the data
    strm.next_in = (Bytef *)data;
    strm.avail_in = size < 1000 ? size : 1000;  // Use first part for inflation
    strm.next_out = uncompressed;
    strm.avail_out = uncompressed_len;
    
    int inflate_result = inflate(&strm, Z_NO_FLUSH);
    
    // 7. Try inflateUndermine (only valid for gzip format)
    inflateUndermine(&strm, 0);
    
    // 8. Reset inflate stream keeping dictionary
    inflateResetKeep(&strm);
    
    // 9. Try inflatePrime to add bits to the bit buffer
    inflatePrime(&strm, 2, 1);
    
    // 10. Clean up inflate
    inflateEnd(&strm);
    
    // 11. Now try inflateBack
    z_stream back_strm;
    memset(&back_strm, 0, sizeof(back_strm));
    unsigned char window[32768];
    
    // 12. Initialize inflateBack
    int back_init = inflateBackInit_(&back_strm, 15, window, 
                                    ZLIB_VERSION, sizeof(z_stream));
    if (back_init == Z_OK) {
        // 13. Try inflateBack
        inflateBack(&back_strm,  id_000081_inf_in_callback, nullptr, 
                    id_000081_inf_out_callback, nullptr);
        
        // 14. End inflateBack
        inflateBackEnd(&back_strm);
    }
    
    // 15. Set up for deflate operations if we got some uncompressed data
    if (uncompressed_len > 0 && uncompressed_len < 1000000) {
        z_stream def_strm;
        memset(&def_strm, 0, sizeof(def_strm));
        
        // 16. Initialize deflate
        deflateInit_(&def_strm, Z_DEFAULT_COMPRESSION, 
                    ZLIB_VERSION, sizeof(z_stream));
        
        // 17. Get dictionary (should be empty initially)
        Bytef dict[32768];
        uInt dict_len = 0;
        deflateGetDictionary(&def_strm, dict, &dict_len);
        
        // 18. Compress some data
        size_t compress_bound = compressBound_z(uncompressed_len);
        Bytef *compressed = (Bytef *)malloc(compress_bound);
        if (compressed) {
            def_strm.next_in = uncompressed;
            def_strm.avail_in = uncompressed_len < 1000 ? uncompressed_len : 1000;
            def_strm.next_out = compressed;
            def_strm.avail_out = compress_bound;
            
            // 19. Perform deflation
            deflate(&def_strm, Z_FINISH);
            
            // 20. Reset deflate stream
            deflateReset(&def_strm);
            
            free(compressed);
        }
        
        // 21. End deflate
        deflateEnd(&def_strm);
    }
    
    // 22. Try gzFile operations
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        int fd = fuzz_fileno(in_file);
        gzFile gz = gzdopen(fd, "rb");
        
        if (gz) {
            // 23. Try to read a byte
            gzgetc(gz);
            
            // 24. Try to put a byte back (simulate with different gzFile)
            gzFile gz2 = gzopen("output_file", "wb");
            if (gz2) {
                // 25. Write a byte
                gzputc(gz2, 'A');
                
                // 26. Flush
                gzflush(gz2, Z_SYNC_FLUSH);
                
                // 27. Try rewind (though it might fail for write files)
                gzrewind(gz2);
                
                // 28. Close the file
                gzclose(gz2);
            }
            
            // 29. Close the gzFile
            gzclose(gz);
        }
        
        assert_file_closed(&in_file);;
        
        // Clean up temporary output file
        remove("output_file");
    }
    
    // 30. Clean up
    free(uncompressed);
    
    assert_file_closed(&in_file);
	return 0;
}