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
//<ID> 138
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"gzfwrite":[[303,12,303,15,0,0,4,1]]},"library_calls":["gzdopen","gzgets","gzgetc_","gzseek","gzseek64","gzdopen","gzfwrite","gzclose","inflateBackInit_","inflateBack","inflateBackEnd","deflateInit_","deflateSetDictionary","deflatePrime","deflateBound_z","compress_z","deflateUsed","deflateEnd","crc32_combine_gen","gzclose"],"critical_calls":["gzdopen","gzgets","gzgetc_","gzseek","gzseek64","gzdopen","gzfwrite","gzclose","inflateBackInit_","inflateBack","inflateBackEnd","deflateInit_","deflateSetDictionary","deflatePrime","deflateBound_z","compress_z","deflateUsed","deflateEnd","crc32_combine_gen","gzclose"],"visited":0}
/**/


// Callback functions for inflateBack
static unsigned int  id_000060_in_callback(void* desc, unsigned char** buf) {
    // We'll use a simple static buffer for this example
    static unsigned char dummy_buffer[1024];
    *buf = dummy_buffer;
    return 0; // Return 0 to indicate no more data
}

static int  id_000060_out_callback(void* desc, unsigned char* buf, unsigned int size) {
    // Just discard the output for fuzzing
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit for too small inputs
    if (size < 10) {
	return 0;
	}
	

    // Create FILE* for input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Create output file
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

    // Part 1: Handle compressed data with gz functions
    gzFile gz_in = gzdopen(in_fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Read from gz file using various methods
    char buffer[256];
    int len = size > sizeof(buffer) ? sizeof(buffer) : size;
    
    // Try to read a line
    char* result = gzgets(gz_in, buffer, len);
    
    // Try to read a character
    int ch = gzgetc_(gz_in);
    
    // Try seeking
    off_t seek_result = gzseek(gz_in, 0, SEEK_SET);
    off64_t seek64_result = gzseek64(gz_in, 0, SEEK_SET);
    
    // Write to output gz file
    gzFile gz_out = gzdopen(out_fd, "wb");
    if (gz_out) {
        z_size_t written = gzfwrite(buffer, 1, strlen(buffer), gz_out);
        gzclose(gz_out);
    }

    // Part 2: Use inflateBack for decompression
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize inflate for back interface
    if (inflateBackInit_(&strm, 15, NULL, ZLIB_VERSION, sizeof(z_stream)) == Z_OK) {
        // Prepare input descriptor
        struct {
            const uint8_t* data;
            size_t size;
            size_t pos;
        } in_desc = {data, size, 0};
        
        // Temporary output buffer
        unsigned char out_buf[1024];
        struct {
            unsigned char* buf;
            size_t capacity;
            size_t pos;
        } out_desc = {out_buf, sizeof(out_buf), 0};
        
        // Custom in_func that reads from our data
        auto custom_in_func = [](void* desc, unsigned char** buf) -> unsigned int {
            auto* d = (decltype(in_desc)*)desc;
            if (d->pos >= d->size) return 0;
            
            *buf = (unsigned char*)d->data + d->pos;
            unsigned int to_read = (d->size - d->pos > 1024) ? 1024 : (d->size - d->pos);
            d->pos += to_read;
            return to_read;
        };
        
        // Custom out_func that writes to buffer
        auto custom_out_func = [](void* desc, unsigned char* buf, unsigned int size) -> int {
            auto* d = (decltype(out_desc)*)desc;
            if (d->pos + size > d->capacity) return 1; // Buffer full
            
            memcpy(d->buf + d->pos, buf, size);
            d->pos += size;
            return 0;
        };
        
        // Try inflateBack with custom callbacks
        int ret = inflateBack(&strm, 
                            (in_func)custom_in_func, &in_desc,
                            (out_func)custom_out_func, &out_desc);
        
        inflateBackEnd(&strm);
    }

    // Part 3: Compression with deflate functions
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    
    // Initialize deflate
    if (deflateInit_(&deflate_strm, Z_DEFAULT_COMPRESSION, 
                     ZLIB_VERSION, sizeof(z_stream)) == Z_OK) {
        
        // Set dictionary if we have enough data
        if (size >= 32) {
            deflateSetDictionary(&deflate_strm, data, 32);
        }
        
        // Prime the deflate stream with some bits
        deflatePrime(&deflate_strm, 4, 0x0F);
        
        // Get bound for compressed size
        z_size_t bound = deflateBound_z(&deflate_strm, size);
        
        // Allocate output buffer
        Bytef* compressed = new Bytef[bound];
        z_size_t compressed_len = bound;
        
        // Try compress_z with the input data
        int compress_ret = compress_z(compressed, &compressed_len, data, size);
        
        // Check bits used
        int bits_used;
        deflateUsed(&deflate_strm, &bits_used);
        
        // End deflate
        deflateEnd(&deflate_strm);
        
        delete[] compressed;
    }

    // Part 4: CRC operations
    // Generate CRC combine value (using offset from input size)
    off_t offset = (off_t)(size % 100);
    uLong crc_gen = crc32_combine_gen(offset);

    // Clean up
    if (gz_in) {
        gzclose(gz_in);
    } else {
        assert_file_closed(&in_file);;
    }
    
    // The out_file was already closed when we closed gz_out
    // but we need to close it if gz_out wasn't created
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}