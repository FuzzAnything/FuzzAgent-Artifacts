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
//<ID> 42
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 1
//<Quality> {"density":13,"unique_branches":{"deflate_slow":[[1644,8,1644,31,27,0,4,0]]},"library_calls":["compressBound_z","compress_z","gzdopen","gzfwrite","gzflush","gzerror","gzrewind","gzclose","gzdopen","gzseek","gzseek64","gzerror","gzclose","deflateInit_","deflatePrime","deflateEnd","inflateBackInit_","inflateBack","inflateBackEnd","crc32_combine_gen"],"critical_calls":["compressBound_z","compress_z","gzdopen","gzfwrite","gzflush","gzerror","gzrewind","gzclose","gzdopen","gzseek","gzseek64","gzerror","gzclose","deflateInit_","deflatePrime","deflateEnd","inflateBackInit_","inflateBack","inflateBackEnd","crc32_combine_gen"],"visited":1}
/**/


// Callback functions for inflateBack
static unsigned int  id_000024_infunc(void* opaque, unsigned char** buf) {
    // Simple input function - just returns 0 to indicate no more data
    *buf = nullptr;
    return 0;
}

static int  id_000024_outfunc(void* opaque, unsigned char* buf, unsigned int size) {
    // Simple output function - does nothing with the data
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create input and output files
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // 1. First use compress_z to compress the input data
    z_size_t compressed_len = compressBound_z(size);
    Bytef* compressed = (Bytef*)malloc(compressed_len);
    if (!compressed) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    int compress_result = compress_z(compressed, &compressed_len, data, size);
    if (compress_result != Z_OK) {
        free(compressed);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 2. Create a gzFile for writing using the compressed data
    gzFile gz_out = gzdopen(out_fd, "wb");
    if (!gz_out) {
        free(compressed);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 3. Write compressed data to gzFile using gzfwrite
    z_size_t items_written = gzfwrite((voidpc)compressed, 1, compressed_len, gz_out);
    
    // 4. Flush and get error status
    gzflush(gz_out, Z_SYNC_FLUSH);
    int gz_err = 0;
    const char* gz_error_msg = gzerror(gz_out, &gz_err);
    
    // 5. Try gzrewind (though it may fail for write-only files)
    gzrewind(gz_out);
    
    // 6. Use gzseek and gzseek64 (dummy operations since we can't seek in write-only mode)
    // First close the write file and open for reading
    gzclose(gz_out);
    assert_file_closed(&out_file);;
    
    // Re-open the output file for reading
    out_file = fopen("output_file", "rb");
    if (out_file) {
        int read_fd = fuzz_fileno(out_file);
        gzFile gz_in = gzdopen(read_fd, "rb");
        if (gz_in) {
            // Try to seek (these may fail depending on file contents)
            off_t seek_result = gzseek(gz_in, 10, SEEK_SET);
            off64_t seek64_result = gzseek64(gz_in, 20, SEEK_SET);
            
            // Check for errors after seeking
            gzerror(gz_in, &gz_err);
            
            gzclose(gz_in);
        }
        assert_file_closed(&out_file);;
    }
    
    // 7. Create a z_stream for deflate operations
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    // Initialize deflate
    int init_result = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
    if (init_result == Z_OK) {
        // 8. Use deflatePrime to insert bits
        // Using some bits from the input data
        int bits_to_insert = (size > 0) ? (data[0] & 0x07) : 1;
        int value_to_insert = (size > 1) ? (data[1] & 0x0F) : 1;
        deflatePrime(&strm, bits_to_insert, value_to_insert);
        
        deflateEnd(&strm);
    }
    
    // 9. Create another z_stream for inflateBack
    z_stream strm2;
    strm2.zalloc = Z_NULL;
    strm2.zfree = Z_NULL;
    strm2.opaque = Z_NULL;
    
    // Initialize for inflateBack
    unsigned char window[32768];  // 32K window
    int back_init = inflateBackInit_(&strm2, 15, window, ZLIB_VERSION, sizeof(z_stream));
    
    if (back_init == Z_OK) {
        // 10. Use inflateBack (with dummy callbacks)
        inflateBack(&strm2,  id_000024_infunc, nullptr,  id_000024_outfunc, nullptr);
        inflateBackEnd(&strm2);
    }
    
    // 11. Use crc32_combine_gen with a dummy offset
    uLong combine_gen = crc32_combine_gen(100);
    
    // Cleanup
    free(compressed);
    assert_file_closed(&in_file);;
    
    // Remove the output file we created
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}