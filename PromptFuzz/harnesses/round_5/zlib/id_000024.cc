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
//<ID> 29
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":7,"unique_branches":{"deflatePrime":[[762,13,762,23,0,0,4,1]]},"library_calls":["zlibVersion","gzdopen","gzseek","gzseek","gzseek","inflateBackInit_","gzclose","inflateValidate","deflateInit_","inflateBackEnd","gzclose","inflateResetKeep","deflateParams","deflatePrime","deflate","deflateEnd","inflateBackEnd","gzclose"],"critical_calls":["zlibVersion","gzdopen","gzseek","gzseek","gzseek","inflateBackInit_","inflateValidate","deflateInit_","inflateResetKeep","deflateParams","deflatePrime","deflate","deflateEnd","inflateBackEnd","gzclose"],"visited":0}
/**/


// Forward declaration for callback functions
static unsigned int  id_000024_inflate_back_input_cb(void* desc, unsigned char** buf);

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Get zlib version
    const char* version = zlibVersion();
    (void)version; // Use version to avoid unused variable warning
    
    if (size < 100) {
	return 0;
	}
	 // Need enough data for meaningful fuzzing
    
    // 2. Create a FILE* from input data for gzseek
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
	int fuzz_fd_0 = fuzz_fileno(in_file);

    
    // 3. Create gzFile for gzseek
    gzFile gz_file = gzdopen(fuzz_fd_0, "rb");
    if (!gz_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	return 0;
    }
    
    // 4. Use gzseek to navigate in the compressed stream
    // Try to seek to different positions
    if (size > 10) {
        gzseek(gz_file, size/4, SEEK_SET);
        gzseek(gz_file, size/2, SEEK_SET);
        gzseek(gz_file, 0, SEEK_SET);
    }
    
    // 5. Prepare for inflateBack - need a window buffer
    unsigned char window[32768]; // 32K window as required by inflateBack
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // 6. Initialize inflateBack with window
    int ret = inflateBackInit_(&strm, 15, window, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	return 0;
    }
    
    // 7. Set up inflateBack callbacks (simplified - we won't fully execute inflateBack
    // due to complexity, but we initialize it properly)
    strm.avail_in = 0;
    strm.next_in = nullptr;
    
    // 8. Enable validation during inflation
    inflateValidate(&strm, 1);
    
    // 9. Create a second stream for deflation
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    def_strm.zalloc = Z_NULL;
    def_strm.zfree = Z_NULL;
    def_strm.opaque = Z_NULL;
    
    // 10. Initialize deflation
    ret = deflateInit(&def_strm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        inflateBackEnd(&strm);
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	return 0;
    }
    
    // 11. Use inflateResetKeep to reset inflation stream while keeping dictionary
    inflateResetKeep(&strm);
    
    // 12. Set deflation parameters using fuzzed data
    int level = data[0] % 10; // 0-9 compression level
    int strategy = data[1] % 4; // 0-3 strategy
    deflateParams(&def_strm, level, strategy);
    
    // 13. Use deflatePrime to insert bits (using first few bytes of data)
    if (size > 20) {
        int bits = data[2] % 16 + 1; // 1-16 bits
        int value = data[3] & ((1 << bits) - 1); // Mask to fit in bits
        deflatePrime(&def_strm, bits, value);
    }
    
    // 14. Prepare input/output buffers
    unsigned char inbuf[1024];
    unsigned char outbuf[1024];
    
    // Use some of the fuzzed data as input for deflation
    size_t data_to_use = size > sizeof(inbuf) ? sizeof(inbuf) : size;
    memcpy(inbuf, data, data_to_use);
    
    def_strm.next_in = inbuf;
    def_strm.avail_in = data_to_use;
    def_strm.next_out = outbuf;
    def_strm.avail_out = sizeof(outbuf);
    
    // 15. Perform a deflation step
    deflate(&def_strm, Z_FINISH);
    
    // 16. Clean up
    deflateEnd(&def_strm);
    inflateBackEnd(&strm);
    gzclose(gz_file);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	return 0;
}

// Dummy callback for inflateBack (not actually used in this simplified version)
static unsigned int  id_000024_inflate_back_input_cb(void* desc, unsigned char** buf) {
    *buf = nullptr;
    return 0;
}