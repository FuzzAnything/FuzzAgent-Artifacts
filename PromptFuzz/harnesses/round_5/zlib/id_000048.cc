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
//<ID> 79
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 1
//<Quality> {"density":15,"unique_branches":{"inflateBackInit_":[[33,27,33,43,0,0,4,0]]},"library_calls":["inflateBackInit_","inflateBack","inflateBackEnd","deflateInit_","deflateBound_z","deflate","deflateCopy","deflatePrime","deflateEnd","gzdopen","gzseek64","gzgetc_","gzclose","crc32","crc32","crc32","crc32","crc32_combine_op","deflateEnd"],"critical_calls":["inflateBackInit_","inflateBack","inflateBackEnd","deflateInit_","deflateBound_z","deflate","deflateCopy","deflatePrime","deflateEnd","gzdopen","gzseek64","gzgetc_","gzclose","crc32","crc32","crc32","crc32","crc32_combine_op","deflateEnd"],"visited":0}
/**/


// Define types needed for inflateBack callbacks
struct in_desc {
    const uint8_t* data;
    size_t size;
    size_t pos;
};

struct out_desc {
    uint8_t* buffer;
    size_t capacity;
    size_t pos;
};

// Callback function for inflateBack input
static unsigned int  id_000048_in_callback(void* desc, unsigned char** buf) {
    in_desc* d = (in_desc*)desc;
    
    // Return available data
    size_t available = d->size - d->pos;
    if (available == 0) {
        *buf = nullptr;
        return 0;
    }
    
    *buf = (unsigned char*)(d->data + d->pos);
    d->pos += available;
    return (unsigned int)available;
}

// Callback function for inflateBack output
static int  id_000048_out_callback(void* desc, unsigned char* buf, unsigned int size) {
    out_desc* d = (out_desc*)desc;
    
    // Check if we need to resize output buffer
    if (d->pos + size > d->capacity) {
        size_t new_capacity = d->capacity * 2 + size;
        uint8_t* new_buffer = (uint8_t*)realloc(d->buffer, new_capacity);
        if (!new_buffer) {
            return 1; // Error
        }
        d->buffer = new_buffer;
        d->capacity = new_capacity;
    }
    
    // Copy data to output buffer
    memcpy(d->buffer + d->pos, buf, size);
    d->pos += size;
    return 0; // Success
}

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
        return 0; // Need enough data for meaningful testing
    }
    
    // 1. First, we'll decompress the input data using inflateBack
    z_stream strm_inflate;
    memset(&strm_inflate, 0, sizeof(z_stream));
    
    // Initialize inflate for back-to-back decompression
    if (inflateBackInit_(&strm_inflate, 15, nullptr, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        return 0;
    }
    
    // Set up input/output descriptors for inflateBack
    in_desc in_desc_data = {data, size, 0};
    out_desc out_desc_data;
    out_desc_data.buffer = (uint8_t*)malloc(4096);
    out_desc_data.capacity = 4096;
    out_desc_data.pos = 0;
    
    // Perform inflateBack decompression
    int result = inflateBack(&strm_inflate,  id_000048_in_callback, &in_desc_data, 
                              id_000048_out_callback, &out_desc_data);
    
    // Clean up inflateBack
    inflateBackEnd(&strm_inflate);
    
    if (result != Z_STREAM_END && result != Z_OK) {
        free(out_desc_data.buffer);
        return 0; // Decompression failed
    }
    
    // 2. Now compress the decompressed data using deflate
    z_stream strm_deflate;
    memset(&strm_deflate, 0, sizeof(z_stream));
    
    // Initialize deflate
    if (deflateInit_(&strm_deflate, Z_DEFAULT_COMPRESSION, 
                     ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        free(out_desc_data.buffer);
        return 0;
    }
    
    // Calculate maximum compressed size using deflateBound_z
    z_size_t max_compressed_size = deflateBound_z(&strm_deflate, out_desc_data.pos);
    uint8_t* compressed_data = (uint8_t*)malloc(max_compressed_size);
    
    // Set up deflate input/output
    strm_deflate.next_in = out_desc_data.buffer;
    strm_deflate.avail_in = (uInt)out_desc_data.pos;
    strm_deflate.next_out = compressed_data;
    strm_deflate.avail_out = (uInt)max_compressed_size;
    
    // Perform deflate compression
    deflate(&strm_deflate, Z_FINISH);
    z_size_t compressed_size = max_compressed_size - strm_deflate.avail_out;
    
    // 3. Create a copy of the deflate stream using deflateCopy
    z_stream strm_copy;
    memset(&strm_copy, 0, sizeof(z_stream));
    if (deflateCopy(&strm_copy, &strm_deflate) == Z_OK) {
        // Add some bits to the copied stream using deflatePrime
        // Using a few bits from the input data as the value
        int bits = data[0] % 16 + 1;  // 1-16 bits
        int value = data[1] & ((1 << bits) - 1);
        deflatePrime(&strm_copy, bits, value);
        
        // Clean up the copied stream
        deflateEnd(&strm_copy);
    }
    
    // 4. Create a gzFile from the compressed data and use gzseek64 and gzgetc_
    FILE* tmp_file = fmemopen((void*)compressed_data, compressed_size, "rb");
    if (tmp_file) {
        int fd = fuzz_fileno(tmp_file);
        gzFile gz = gzdopen(fd, "rb");
        
        if (gz) {
            // Seek to a position using gzseek64
            // Use some data to determine seek position
            off64_t seek_pos = (off64_t)(data[2] % (compressed_size / 2 + 1));
            gzseek64(gz, seek_pos, SEEK_SET);
            
            // Read a character using gzgetc_
            gzgetc_(gz);
            
            // Close gz file
            gzclose(gz);
        }
        assert_file_closed(&tmp_file);;
    }
    
    // 5. Combine CRC32 values using crc32_combine_op
    // Compute CRC32 for first and second halves of decompressed data
    size_t half_pos = out_desc_data.pos / 2;
    uLong crc1 = crc32(0L, Z_NULL, 0);
    uLong crc2 = crc32(0L, Z_NULL, 0);
    
    if (out_desc_data.pos > 0) {
        crc1 = crc32(crc1, out_desc_data.buffer, (uInt)half_pos);
        if (out_desc_data.pos > half_pos) {
            crc2 = crc32(crc2, out_desc_data.buffer + half_pos, 
                        (uInt)(out_desc_data.pos - half_pos));
        }
        
        // Generate combine operation from input data
        uLong op = (uLong)data[3] | ((uLong)data[4] << 8) | 
                  ((uLong)data[5] << 16) | ((uLong)data[6] << 24);
        
        // Combine the two CRC32 values
        uLong combined = crc32_combine_op(crc1, crc2, op);
        (void)combined; // Use result to avoid unused variable warning
    }
    
    // 6. Clean up all allocated resources
    deflateEnd(&strm_deflate);
    free(out_desc_data.buffer);
    free(compressed_data);
    
    assert_file_closed(&tmp_file);
	return 0;
}