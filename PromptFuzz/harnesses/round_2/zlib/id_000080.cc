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
//<ID> 270
//<Prompt> []
/*<Combination>: [
*/
//<score> 50, nr_unique_branch: 11
//<Quality> {"density":10,"unique_branches":{"gz_open":[[277,13,277,31,0,0,4,0]],"gzrewind":[[360,9,360,55,0,0,4,0]],"gz_fetch":[[254,17,254,37,0,0,4,0]],"gzputc":[[331,13,331,32,0,0,4,0]],"gz_look":[[141,9,141,30,0,0,4,0]],"gz_load":[[35,9,35,16,0,0,4,0],[36,13,36,28,0,0,4,1],[36,32,36,52,0,0,4,1]],"gz_read":[[359,42,359,60,0,0,4,0]],"gzclose_r":[[667,12,667,15,0,0,4,0]],"gz_avail":[[75,13,76,62,0,0,4,0]]},"library_calls":["zlibVersion","gzdopen","gzwrite","gzflush","gzputc","gzclose","gzdopen","gzrewind","gzread","gzeof","gzclose","compressBound_z","uncompress2_z","deflateInit_","deflateGetDictionary","deflateReset","deflate","deflateEnd","inflateBackInit_","inflateBack","inflateUndermine","inflateResetKeep","inflatePrime","inflateBackEnd","inflateInit2_","inflateValidate","inflateEnd"],"critical_calls":["zlibVersion","gzdopen","gzwrite","gzflush","gzputc","gzclose","gzdopen","gzrewind","gzread","gzeof","gzclose","compressBound_z","uncompress2_z","deflateInit_","deflateGetDictionary","deflateReset","deflate","deflateEnd","inflateBackInit_","inflateBack","inflateUndermine","inflateResetKeep","inflatePrime","inflateBackEnd","inflateInit2_","inflateValidate","inflateEnd"],"visited":1}
/**/


// Custom callback structures for inflateBack
struct InData {
    const uint8_t* data;
    size_t size;
    size_t pos;
};

struct OutData {
    uint8_t* buffer;
    size_t capacity;
    size_t pos;
};

// Input callback for inflateBack
static unsigned int  id_000080_in_cb(void* desc, unsigned char** buf) {
    InData* in = (InData*)desc;
    if (in->pos >= in->size) {
        return 0;  // No more data
    }
    *buf = (unsigned char*)&in->data[in->pos];
    unsigned int available = (unsigned int)(in->size - in->pos);
    if (available > 65536) available = 65536;  // Reasonable chunk size
    in->pos += available;
    return available;
}

// Output callback for inflateBack
static int  id_000080_out_cb(void* desc, unsigned char* buf, unsigned int len) {
    OutData* out = (OutData*)desc;
    if (out->pos + len > out->capacity) {
        // Reallocate buffer
        size_t new_capacity = out->pos + len + 65536;
        uint8_t* new_buffer = (uint8_t*)realloc(out->buffer, new_capacity);
        if (!new_buffer) return 1;  // Error
        out->buffer = new_buffer;
        out->capacity = new_capacity;
    }
    memcpy(&out->buffer[out->pos], buf, len);
    out->pos += len;
    return 0;  // Success
}

extern "C" int LLVMFuzzerTestOneInput_80(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Get zlib version
    const char* version = zlibVersion();
    if (!version) {
        // Just continue if version check fails
    }
    
    // Create FILE* for input data
    FILE* in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;  // Failed to create memory file
    }
    
    // Create temporary file for gz operations
    FILE* temp_file = fopen("temp.gz", "wb+");
    if (!temp_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int temp_fd = fuzz_fileno(temp_file);
    
    // 1. GZIP operations with the input data
    
    // Open gzFile for writing
    gzFile gz_write = gzdopen(temp_fd, "wb");
    if (!gz_write) {
        assert_file_closed(&in_file);;
        assert_file_closed(&temp_file);;
        assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	assert_fd_closed(temp_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Write input data to gz file
    size_t bytes_written = 0;
    while (bytes_written < size) {
        size_t chunk = size - bytes_written;
        if (chunk > 4096) chunk = 4096;
        
        int ret = gzwrite(gz_write, &data[bytes_written], (unsigned int)chunk);
        if (ret <= 0) break;
        bytes_written += ret;
    }
    
    // Flush gz file
    gzflush(gz_write, Z_SYNC_FLUSH);
    
    // Write one more character
    gzputc(gz_write, 'X');
    
    // Close writing gzFile
    gzclose(gz_write);
    
    // Rewind temp file for reading
    fseek(temp_file, 0, SEEK_SET);
    
    // Open gzFile for reading
    gzFile gz_read = gzdopen(temp_fd, "rb");
    if (gz_read) {
        // Rewind gz file
        gzrewind(gz_read);
        
        // Read a few bytes
        uint8_t buf[1024];
        gzread(gz_read, buf, sizeof(buf));
        
        // Check if at EOF
        gzeof(gz_read);
        
        // Close reading gzFile
        gzclose(gz_read);
    }
    
    // 2. Direct compression/decompression operations
    
    // Prepare source data (use original input or generated data)
    Bytef* source = (Bytef*)malloc(size);
    if (!source) {
        assert_file_closed(&in_file);;
        assert_file_closed(&temp_file);;
        assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	assert_fd_closed(temp_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    memcpy(source, data, size);
    
    // Get compressed size bound
    z_size_t bound = compressBound_z((z_size_t)size);
    
    // Allocate destination buffer
    Bytef* dest = (Bytef*)malloc(bound);
    if (!dest) {
        free(source);
        assert_file_closed(&in_file);;
        assert_file_closed(&temp_file);;
        assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	assert_fd_closed(temp_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Compress using uncompress2_z (actually for decompression)
    // We'll use it to try decompressing the input
    z_size_t destLen = bound;
    z_size_t sourceLen = size;
    int ret = uncompress2_z(dest, &destLen, source, &sourceLen);
    // Ignore return value - input might not be valid compressed data
    
    // 3. Stream-based operations
    
    // Setup deflate stream
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(z_stream));
    
    // Initialize deflate
    ret = deflateInit_(&def_strm, Z_DEFAULT_COMPRESSION, 
                      ZLIB_VERSION, sizeof(z_stream));
    if (ret == Z_OK) {
        // Get dictionary (if any)
        Bytef dict[256];
        uInt dictLength = sizeof(dict);
        deflateGetDictionary(&def_strm, dict, &dictLength);
        
        // Reset deflate
        deflateReset(&def_strm);
        
        // Prepare input/output buffers
        Bytef in_buf[4096];
        Bytef out_buf[4096];
        
        // Use a small chunk of input data
        size_t chunk = size > sizeof(in_buf) ? sizeof(in_buf) : size;
        memcpy(in_buf, data, chunk);
        
        def_strm.next_in = in_buf;
        def_strm.avail_in = (uInt)chunk;
        def_strm.next_out = out_buf;
        def_strm.avail_out = sizeof(out_buf);
        
        // Perform deflation
        deflate(&def_strm, Z_FINISH);
        
        // End deflate
        deflateEnd(&def_strm);
    }
    
    // Setup inflate stream for Back
    z_stream inf_strm_back;
    memset(&inf_strm_back, 0, sizeof(z_stream));
    
    // Allocate window for inflateBack
    unsigned char* window = (unsigned char*)malloc(32768);
    if (window) {
        // Initialize inflateBack
        ret = inflateBackInit_(&inf_strm_back, 15, window, 
                             ZLIB_VERSION, sizeof(z_stream));
        if (ret == Z_OK) {
            // Setup callback data
            InData in_data = {data, size, 0};
            OutData out_data = {NULL, 0, 0};
            out_data.buffer = (uint8_t*)malloc(65536);
            if (out_data.buffer) {
                out_data.capacity = 65536;
                
                // Perform inflateBack
                inflateBack(&inf_strm_back,  id_000080_in_cb, &in_data, 
                            id_000080_out_cb, &out_data);
                
                // Try undermine (for testing)
                inflateUndermine(&inf_strm_back, 0);
                
                // Free output buffer
                free(out_data.buffer);
            }
            
            // Try reset keep
            inflateResetKeep(&inf_strm_back);
            
            // Try prime
            inflatePrime(&inf_strm_back, 8, 0xFF);
            
            // End inflateBack
            inflateBackEnd(&inf_strm_back);
        }
        free(window);
    }
    
    // Setup another inflate stream
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(z_stream));
    
    // Initialize inflate with windowBits
    ret = inflateInit2_(&inf_strm, 15, ZLIB_VERSION, sizeof(z_stream));
    if (ret == Z_OK) {
        // Try validate
        inflateValidate(&inf_strm, 1);
        
        // End inflate
        inflateEnd(&inf_strm);
    }
    
    // Cleanup
    free(source);
    free(dest);
    assert_file_closed(&in_file);;
    assert_file_closed(&temp_file);;
    
    // Remove temporary file
    remove("temp.gz");
    
    assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	assert_fd_closed(temp_fd);
	assert_fd_closed(in_fd);
	return 0;
}