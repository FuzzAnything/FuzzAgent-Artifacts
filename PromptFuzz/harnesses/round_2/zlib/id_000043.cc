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
//<ID> 97
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 1
//<Quality> {"density":12,"unique_branches":{"gzputs":[[371,12,371,15,0,0,4,1]]},"library_calls":["zlibVersion","zlibCompileFlags","compressBound_z","compress2","deflateInit_","deflate","deflateReset","deflateEnd","inflateBackInit_","inflateBack","inflateResetKeep","inflatePrime","inflateUndermine","inflateCodesUsed","inflateBackEnd","uncompress2_z","gzdopen","gzputc","gzputs","gzclose"],"critical_calls":["zlibVersion","zlibCompileFlags","compressBound_z","compress2","deflateInit_","deflate","deflateReset","deflateEnd","inflateBackInit_","inflateBack","inflateResetKeep","inflatePrime","inflateUndermine","inflateCodesUsed","inflateBackEnd","uncompress2_z","gzdopen","gzputc","gzputs","gzclose"],"visited":1}
/**/


// Custom input function for inflateBack
unsigned int  id_000043_custom_in_func(void* desc, unsigned char** buf) {
    // Simple implementation: provide the entire input buffer
    static bool provided = false;
    if (!provided) {
        *buf = (unsigned char*)desc;
        provided = true;
        return (unsigned int)(*(size_t*)((char*)desc + 1024));  // Get size from descriptor
    }
    return 0;  // No more data
}

// Custom output function for inflateBack
int  id_000043_custom_out_func(void* desc, unsigned char* buf, unsigned int size) {
    // Just discard the output
    (void)desc;
    (void)buf;
    (void)size;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	
    
    // 1. Check zlib version and compile flags
    const char* version = zlibVersion();
    (void)version;  // Use version to avoid unused variable warning
    uLong compile_flags = zlibCompileFlags();
    (void)compile_flags;
    
    // 2. Create a compressed version of the input data
    Bytef* compressed = nullptr;
    Bytef* decompressed = nullptr;
    z_size_t comp_bound = 0;
    uLongf comp_len = 0;
    z_size_t decomp_len = 0;
    z_size_t source_len = size;
    
    // Allocate memory for compression
    comp_bound = compressBound_z(size);
    if (comp_bound == 0) {
	return 0;
	}
	
    
    compressed = (Bytef*)malloc(comp_bound);
    if (!compressed) {
	return 0;
	}
	
    
    // Compress the input data
    comp_len = (uLongf)comp_bound;
    int ret = compress2(compressed, &comp_len, data, (uLong)size, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        free(compressed);
        return 0;
    }
    
    // 3. Set up deflate stream
    z_stream def_stream;
    def_stream.zalloc = Z_NULL;
    def_stream.zfree = Z_NULL;
    def_stream.opaque = Z_NULL;
    
    ret = deflateInit_(&def_stream, Z_DEFAULT_COMPRESSION, version, sizeof(z_stream));
    if (ret != Z_OK) {
        free(compressed);
        return 0;
    }
    
    // Perform some deflate operations
    def_stream.next_in = (Bytef*)data;
    def_stream.avail_in = (uInt)(size > 100 ? 100 : size);
    def_stream.next_out = compressed;
    def_stream.avail_out = (uInt)comp_bound;
    
    ret = deflate(&def_stream, Z_FINISH);
    (void)ret;  // Result might be Z_OK, Z_STREAM_END, etc.
    
    // Reset deflate stream
    ret = deflateReset(&def_stream);
    (void)ret;
    
    // End deflate stream
    ret = deflateEnd(&def_stream);
    (void)ret;
    
    // 4. Set up inflate stream for back inflation
    z_stream inf_stream;
    inf_stream.zalloc = Z_NULL;
    inf_stream.zfree = Z_NULL;
    inf_stream.opaque = Z_NULL;
    
    unsigned char window[32768];  // 32K window
    ret = inflateBackInit_(&inf_stream, 15, window, version, sizeof(z_stream));
    if (ret != Z_OK) {
        free(compressed);
        return 0;
    }
    
    // Prepare descriptor for custom input function
    struct {
        unsigned char data[1024];
        size_t size;
    } in_desc;
    
    // Copy some compressed data to descriptor
    size_t copy_size = comp_len > 1024 ? 1024 : comp_len;
    memcpy(in_desc.data, compressed, copy_size);
    in_desc.size = copy_size;
    
    // Set up output descriptor (not used in our simple out_func)
    void* out_desc = nullptr;
    
    // Try inflateBack
    ret = inflateBack(&inf_stream,  id_000043_custom_in_func, &in_desc,  id_000043_custom_out_func, out_desc);
    (void)ret;
    
    // Use other inflate functions
    ret = inflateResetKeep(&inf_stream);
    (void)ret;
    
    // Inflate prime (add bits to the bit buffer)
    ret = inflatePrime(&inf_stream, 3, 5);
    (void)ret;
    
    // Inflate undermine (corrupt internal state for testing)
    ret = inflateUndermine(&inf_stream, 1);
    (void)ret;
    
    // Get codes used (even if inflation failed)
    unsigned long codes_used = inflateCodesUsed(&inf_stream);
    (void)codes_used;
    
    // End inflate back
    ret = inflateBackEnd(&inf_stream);
    (void)ret;
    
    // 5. Uncompress using uncompress2_z
    decompressed = (Bytef*)malloc(size * 2);  // Allocate enough space
    if (decompressed) {
        decomp_len = size * 2;
        z_size_t source_consumed = comp_len;
        
        ret = uncompress2_z(decompressed, &decomp_len, compressed, &source_consumed);
        (void)ret;
        
        free(decompressed);
    }
    
    // 6. Use gzFile functions
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        free(compressed);
        assert_file_closed(&in_file);
	return 0;
    }
    
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        free(compressed);
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Open gzip file for writing
    gzFile gz_out = gzdopen(fd, "wb");
    if (!gz_out) {
        assert_file_closed(&in_file);;
        free(compressed);
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Write some data to gzip file
    if (size > 0) {
        // Write a character
        ret = gzputc(gz_out, data[0]);
        (void)ret;
        
        // Write a string if we have enough data
        if (size > 10) {
            char str[11];
            memcpy(str, data, 10);
            str[10] = '\0';
            ret = gzputs(gz_out, str);
            (void)ret;
        }
    }
    
    // Close gzip file (this also closes the underlying file descriptor)
    ret = gzclose(gz_out);
    (void)ret;
    
    // Note: gzclose already closed the file descriptor, so we don't need to assert_file_closed(&in_file);
    
    // 7. Clean up
    free(compressed);
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}