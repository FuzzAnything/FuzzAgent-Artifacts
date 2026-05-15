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
//<ID> 290
//<Prompt> []
/*<Combination>: [
*/
//<score> 29, nr_unique_branch: 1
//<Quality> {"density":29,"unique_branches":{"gzread":[[403,9,403,31,0,0,4,0]]},"library_calls":["zlibVersion","gzdopen","gzbuffer","gzwrite","gzputc","gzflush","compressBound_z","uncompress2","uncompress2_z","deflateInit_","deflateSetDictionary","deflateGetDictionary","deflate","deflateReset","deflateEnd","inflateBackInit_","inflateBack","inflateBackEnd","inflateInit2_","inflateUndermine","inflateResetKeep","inflatePrime","inflateValidate","inflateEnd","gzrewind","gzread","gzeof","gzungetc","gzclose"],"critical_calls":["zlibVersion","gzdopen","gzbuffer","gzwrite","gzputc","gzflush","compressBound_z","uncompress2","uncompress2_z","deflateInit_","deflateSetDictionary","deflateGetDictionary","deflate","deflateReset","deflateEnd","inflateBackInit_","inflateBack","inflateBackEnd","inflateInit2_","inflateUndermine","inflateResetKeep","inflatePrime","inflateValidate","inflateEnd","gzrewind","gzread","gzeof","gzungetc","gzclose"],"visited":1}
/**/


// Callback functions for inflateBack
static unsigned int  id_000085_input_callback(void* desc, unsigned char** buf) {
    // Simple implementation that returns no data
    *buf = nullptr;
    return 0;
}

static int  id_000085_output_callback(void* desc, unsigned char* buf, unsigned int size) {
    // Simple implementation that discards output
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimum data
    
    // 1. Get zlib version
    const char* version = zlibVersion();
    
    // 2. Create input FILE* from data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 3. Create output FILE*
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // 5. Open gzip file for writing
    gzFile gz_out = gzdopen(out_fd, "wb");
    if (!gz_out) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 6. Set gzip buffer size
    gzbuffer(gz_out, 8192);
    
    // 7. Write some data to gzip file
    // Use first 100 bytes or available size
    size_t write_size = size > 100 ? 100 : size;
    gzwrite(gz_out, data, write_size);
    
    // 8. Write a single character
    gzputc(gz_out, 'X');
    
    // 9. Flush the gzip file
    gzflush(gz_out, Z_SYNC_FLUSH);
    
    // 10. Get compression bound
    z_size_t bound = compressBound_z(size);
    
    // 11. Try to uncompress using uncompress2
    Bytef* dest = (Bytef*)malloc(bound);
    uLongf destLen = bound;
    uLong sourceLen = size;
    int uncompress_result = uncompress2(dest, &destLen, data, &sourceLen);
    
    // 12. Try uncompress2_z version
    z_size_t destLen_z = bound;
    z_size_t sourceLen_z = size;
    int uncompress_result_z = uncompress2_z(dest, &destLen_z, data, &sourceLen_z);
    
    free(dest);
    
    // 13. Initialize deflate stream
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(z_stream));
    deflate_strm.zalloc = Z_NULL;
    deflate_strm.zfree = Z_NULL;
    deflate_strm.opaque = Z_NULL;
    
    int deflate_init_result = deflateInit_(&deflate_strm, Z_DEFAULT_COMPRESSION, 
                                          version, sizeof(z_stream));
    
    if (deflate_init_result == Z_OK) {
        // 14. Set dictionary if we have enough data
        if (size > 50) {
            deflateSetDictionary(&deflate_strm, data, 50);
        }
        
        // 15. Get dictionary back
        Bytef dict_buf[100];
        uInt dict_len = 100;
        deflateGetDictionary(&deflate_strm, dict_buf, &dict_len);
        
        // 16. Prepare input for deflate
        Bytef* input_copy = (Bytef*)malloc(size);
        memcpy(input_copy, data, size);
        deflate_strm.next_in = input_copy;
        deflate_strm.avail_in = size > 100 ? 100 : size;
        
        // 17. Prepare output buffer
        Bytef* out_buf = (Bytef*)malloc(bound);
        deflate_strm.next_out = out_buf;
        deflate_strm.avail_out = bound;
        
        // 18. Perform deflate
        int deflate_result = deflate(&deflate_strm, Z_FINISH);
        
        // 19. Reset deflate stream
        deflateReset(&deflate_strm);
        
        free(input_copy);
        free(out_buf);
        
        // 20. End deflate stream
        deflateEnd(&deflate_strm);
    }
    
    // 21. Initialize inflate stream for back functions
    z_stream inflate_back_strm;
    memset(&inflate_back_strm, 0, sizeof(z_stream));
    inflate_back_strm.zalloc = Z_NULL;
    inflate_back_strm.zfree = Z_NULL;
    inflate_back_strm.opaque = Z_NULL;
    
    // 22. Initialize for inflateBack
    unsigned char window[32768];
    int back_init_result = inflateBackInit_(&inflate_back_strm, 15, window, 
                                           version, sizeof(z_stream));
    
    if (back_init_result == Z_OK) {
        // 23. Perform inflateBack (even though we have no input data)
        inflateBack(&inflate_back_strm,  id_000085_input_callback, nullptr, 
                    id_000085_output_callback, nullptr);
        
        // 24. End inflateBack
        inflateBackEnd(&inflate_back_strm);
    }
    
    // 25. Initialize another inflate stream for other functions
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(z_stream));
    inflate_strm.zalloc = Z_NULL;
    inflate_strm.zfree = Z_NULL;
    inflate_strm.opaque = Z_NULL;
    
    // 26. Initialize inflate with windowBits
    int inflate_init_result = inflateInit2_(&inflate_strm, 15, version, 
                                           sizeof(z_stream));
    
    if (inflate_init_result == Z_OK) {
        // 27. Try to undermine inflation (for testing)
        inflateUndermine(&inflate_strm, 0);
        
        // 28. Reset and keep dictionary
        inflateResetKeep(&inflate_strm);
        
        // 29. Prime the stream with some bits
        if (size > 2) {
            inflatePrime(&inflate_strm, 8, data[0]);
        }
        
        // 30. Validate inflation
        inflateValidate(&inflate_strm, 1);
        
        // 31. End inflate stream
        inflateEnd(&inflate_strm);
    }
    
    // 32. Rewind gzip file for reading
    gzrewind(gz_out);
    
    // 33. Read from gzip file
    char read_buf[100];
    gzread(gz_out, read_buf, 50);
    
    // 34. Check if at EOF
    int is_eof = gzeof(gz_out);
    
    // 35. Try to unget a character
    gzungetc('A', gz_out);
    
    // 36. Close gzip file (writing side)
    gzclose(gz_out);
    
    // 37. Close input file
    assert_file_closed(&in_file);;
    
    // Note: gzclose already closed the underlying file descriptor,
    // so we don't need to assert_file_closed(&out_file); separately
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}