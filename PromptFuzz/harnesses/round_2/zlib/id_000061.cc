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
//<ID> 173
//<Prompt> []
/*<Combination>: [
*/
//<score> 22.5, nr_unique_branch: 3
//<Quality> {"density":15,"unique_branches":{"gzputc":[[320,9,320,32,0,0,4,0]],"inflateBack":[[103,17,103,26,95,0,4,1],[103,17,103,26,96,0,4,1]]},"library_calls":["zlibVersion","zlibCompileFlags","gzdopen","gzgets","gzrewind","uncompress2_z","deflateInit_","compressBound_z","deflate","deflateReset","deflateGetDictionary","deflateEnd","inflateBackInit_","inflateBack","inflatePrime","inflateUndermine","inflateResetKeep","inflateBackEnd","gzrewind","gzputc","gzflush","compressBound_z","inflateBackInit_","inflateBack","inflateBackEnd","gzclose"],"critical_calls":["zlibVersion","zlibCompileFlags","gzdopen","gzgets","gzrewind","uncompress2_z","deflateInit_","compressBound_z","deflate","deflateReset","deflateGetDictionary","deflateEnd","inflateBackInit_","inflateBack","inflatePrime","inflateUndermine","inflateResetKeep","inflateBackEnd","gzrewind","gzputc","gzflush","compressBound_z","inflateBackInit_","inflateBack","inflateBackEnd","gzclose"],"visited":1}
/*Here's a comprehensive fuzz driver that exercises all the specified zlib APIs following your instructions:

*/


// Callback functions for inflateBack
static unsigned int  id_000061_in_callback(void *desc, unsigned char **buf) {
    size_t *remaining = (size_t *)desc;
    static unsigned char dummy[1] = {0};
    
    if (*remaining > 0) {
        *buf = dummy;
        *remaining = 0;
        return 1;
    }
    return 0;
}

static int  id_000061_out_callback(void *desc, unsigned char *buf, unsigned int size) {
    // Just consume the output
    return 0;
}

// Wrapper for FILE* descriptor
struct FileDesc {
    FILE *fp;
    size_t pos;
    size_t size;
};

// in_func wrapper for inflateBack
static unsigned int  id_000061_file_in_func(void *desc, unsigned char **buf) {
    FileDesc *fd = (FileDesc *)desc;
    
    if (fd->pos >= fd->size) {
        return 0;
    }
    
    static unsigned char single_byte[1];
    single_byte[0] = fgetc(fd->fp);
    fd->pos++;
    *buf = single_byte;
    return 1;
}

extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimum data
    
    // 1. Get zlib version and compile flags
    const char *version = zlibVersion();
    uLong compile_flags = zlibCompileFlags();
    
    // 2. Create input and output files
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
    
    // 3. Create gzFile from file descriptor
    int fd_in = fuzz_fileno(in_file);
    gzFile gz = gzdopen(fd_in, "rb");
    if (!gz) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 4. Test gzFile operations
    char buf[256];
    char *gz_result = gzgets(gz, buf, sizeof(buf));
    
    // Reset file position
    fseek(in_file, 0, SEEK_SET);
    gzrewind(gz);
    
    // 5. Test compress/decompress using uncompress2_z
    std::vector<Bytef> compressed(data, data + size);
    std::vector<Bytef> uncompressed(size * 2);
    
    z_size_t destLen = uncompressed.size();
    z_size_t srcLen = size;
    int uncompress_result = uncompress2_z(uncompressed.data(), &destLen, 
                                         compressed.data(), &srcLen);
    
    // 6. Test deflate functions
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize deflate
    int init_result = deflateInit_(&strm, Z_DEFAULT_COMPRESSION, 
                                  version, sizeof(z_stream));
    if (init_result == Z_OK) {
        // Setup input/output buffers
        std::vector<Bytef> deflate_in(data, data + std::min(size, (size_t)1024));
        std::vector<Bytef> deflate_out(compressBound_z(deflate_in.size()));
        
        strm.next_in = deflate_in.data();
        strm.avail_in = deflate_in.size();
        strm.next_out = deflate_out.data();
        strm.avail_out = deflate_out.size();
        
        // Perform deflation
        int deflate_result = deflate(&strm, Z_FINISH);
        
        // Test deflateReset
        deflateReset(&strm);
        
        // Test deflateGetDictionary
        Bytef dict_buf[256];
        uInt dict_len = sizeof(dict_buf);
        deflateGetDictionary(&strm, dict_buf, &dict_len);
        
        // End deflate
        deflateEnd(&strm);
    }
    
    // 7. Test inflateBack functions
    z_stream strm_inflate;
    memset(&strm_inflate, 0, sizeof(strm_inflate));
    
    unsigned char window[32768];
    int back_init_result = inflateBackInit_(&strm_inflate, 15, window, 
                                           version, sizeof(z_stream));
    
    if (back_init_result == Z_OK) {
        // Setup callback descriptor
        FileDesc fdesc = {in_file, 0, size};
        fseek(in_file, 0, SEEK_SET);
        
        // Test inflateBack
        int back_result = inflateBack(&strm_inflate,  id_000061_file_in_func, &fdesc, 
                                      id_000061_out_callback, nullptr);
        
        // Test inflatePrime
        inflatePrime(&strm_inflate, 2, 1);
        
        // Test inflateUndermine
        inflateUndermine(&strm_inflate, 1);
        
        // Test inflateResetKeep
        inflateResetKeep(&strm_inflate);
        
        // End inflateBack
        inflateBackEnd(&strm_inflate);
    }
    
    // 8. Test more gzFile operations
    fseek(in_file, 0, SEEK_SET);
    gzrewind(gz);
    
    // Test gzputc
    if (size > 0) {
        gzputc(gz, data[0]);
    }
    
    // Test gzflush
    gzflush(gz, Z_SYNC_FLUSH);
    
    // 9. Test compressBound_z
    z_size_t bound = compressBound_z(size);
    
    // 10. Test inflateBack with simpler callback
    {
        z_stream strm2;
        memset(&strm2, 0, sizeof(strm2));
        
        inflateBackInit_(&strm2, 15, window, version, sizeof(z_stream));
        
        size_t remaining = std::min(size, (size_t)10);
        inflateBack(&strm2,  id_000061_in_callback, &remaining,  id_000061_out_callback, nullptr);
        
        inflateBackEnd(&strm2);
    }
    
    // 11. Cleanup
    gzclose(gz);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_in);
	return 0;
}