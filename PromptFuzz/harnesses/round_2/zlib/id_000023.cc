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
//<ID> 45
//<Prompt> []
/*<Combination>: [
*/
//<score> 10.666667, nr_unique_branch: 1
//<Quality> {"density":16,"unique_branches":{"gzdopen":[[302,9,302,17,0,0,4,0]]},"library_calls":["compressBound_z","compress2_z","gzdopen","gzungetc","inflateBackInit_","gzclose","inflatePrime","inflateBack","inflateUndermine","inflateCodesUsed","inflateBackEnd","deflateInit_","gzclose","deflateReset","compressBound_z","compressBound_z","uncompress2_z","compress2_z","crc32","crc32","crc32","crc32","crc32_combine_op","deflateEnd","gzclose"],"critical_calls":["compressBound_z","compress2_z","gzdopen","gzungetc","inflateBackInit_","inflatePrime","inflateBack","inflateUndermine","inflateCodesUsed","inflateBackEnd","deflateInit_","deflateReset","compressBound_z","compressBound_z","uncompress2_z","compress2_z","crc32","crc32","crc32","crc32","crc32_combine_op","deflateEnd","gzclose"],"visited":2}
/**/


// Custom structures for inflateBack callbacks
struct InBackData {
    const uint8_t* data;
    size_t size;
    size_t pos;
};

struct OutBackData {
    uint8_t* buffer;
    size_t capacity;
    size_t pos;
};

// Input callback for inflateBack
static unsigned int  id_000023_in_back_callback(void* desc, unsigned char** buf) {
    InBackData* d = (InBackData*)desc;
    if (d->pos >= d->size) {
        return 0; // No more data
    }
    
    *buf = (unsigned char*)&d->data[d->pos];
    size_t remaining = d->size - d->pos;
    
    // Return small chunks to exercise the inflateBack properly
    size_t chunk = (remaining > 1024) ? 1024 : remaining;
    d->pos += chunk;
    
    return (unsigned int)chunk;
}

// Output callback for inflateBack
static int  id_000023_out_back_callback(void* desc, unsigned char* buf, unsigned int len) {
    OutBackData* d = (OutBackData*)desc;
    
    // Check if we need to grow the buffer
    if (d->pos + len > d->capacity) {
        size_t new_capacity = d->capacity * 2;
        if (new_capacity < d->pos + len) {
            new_capacity = d->pos + len;
        }
        
        uint8_t* new_buffer = (uint8_t*)realloc(d->buffer, new_capacity);
        if (!new_buffer) {
            return 1; // Error
        }
        
        d->buffer = new_buffer;
        d->capacity = new_capacity;
    }
    
    // Copy data to buffer
    memcpy(&d->buffer[d->pos], buf, len);
    d->pos += len;
    
    return 0; // Success
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
        return 0; // Need enough data
    }

    // 1. First, let's create compressed data to work with
    size_t compressed_bound = compressBound_z(size);
    if (compressed_bound == 0) {
        return 0;
    }

    Bytef* compressed = (Bytef*)malloc(compressed_bound);
    z_size_t compressed_len = compressed_bound;
    
    // Compress the input data
    if (compress2_z(compressed, &compressed_len, data, size, Z_DEFAULT_COMPRESSION) != Z_OK) {
        free(compressed);
        return 0;
    }

    // 2. Create a temporary file with the compressed data
    FILE* tmp_file = fopen("temp_compressed.gz", "wb");
    if (!tmp_file) {
        free(compressed);
        assert_file_closed(&tmp_file);
	return 0;
    }
    fwrite(compressed, 1, compressed_len, tmp_file);
    assert_file_closed(&tmp_file);;
    
    // 3. Open the compressed file as gzFile
    tmp_file = fopen("temp_compressed.gz", "rb");
    int fd = fuzz_fileno(tmp_file);
    gzFile gz = gzdopen(fd, "rb");
    if (!gz) {
        assert_file_closed(&tmp_file);;
        free(compressed);
        remove("temp_compressed.gz");
        assert_file_closed(&tmp_file);
	assert_file_closed(&tmp_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 4. Try to read and unget a character
    int first_char = gzgetc(gz);
    if (first_char != -1) {
        gzungetc(first_char, gz);
    }

    // 5. Setup inflateBack for decompression
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    unsigned char window[32768]; // 32K window as required by inflateBack
    if (inflateBackInit_(&strm, MAX_WBITS, window, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        gzclose(gz);
        assert_file_closed(&tmp_file);;
        free(compressed);
        remove("temp_compressed.gz");
        assert_file_closed(&tmp_file);
	assert_file_closed(&tmp_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 6. Prepare data structures for callbacks
    InBackData in_data;
    in_data.data = compressed;
    in_data.size = compressed_len;
    in_data.pos = 0;
    
    OutBackData out_data;
    out_data.capacity = size * 2; // Start with some capacity
    out_data.buffer = (uint8_t*)malloc(out_data.capacity);
    out_data.pos = 0;
    
    // 7. Prime the inflate stream with some bits from input
    // Use some bits from the beginning of the original data
    if (size >= 2) {
        inflatePrime(&strm, 8, data[0]);
    }

    // 8. Perform inflateBack decompression
    int result = inflateBack(&strm,  id_000023_in_back_callback, &in_data, 
                             id_000023_out_back_callback, &out_data);
    
    // 9. Try to undermine the stream (this may fail, that's OK)
    if (size >= 3) {
        inflateUndermine(&strm, data[1] & 1);
    }

    // 10. Get code usage statistics
    unsigned long codes_used = inflateCodesUsed(&strm);

    // 11. Clean up inflateBack
    inflateBackEnd(&strm);
    
    // 12. Now let's try deflate on the decompressed data
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    
    if (deflateInit_(&deflate_strm, Z_DEFAULT_COMPRESSION, 
                     ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        free(out_data.buffer);
        gzclose(gz);
        assert_file_closed(&tmp_file);;
        free(compressed);
        remove("temp_compressed.gz");
        assert_file_closed(&tmp_file);
	assert_file_closed(&tmp_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 13. Reset deflate stream (even though we just initialized)
    deflateReset(&deflate_strm);
    
    // 14. Actually perform deflation (simplified)
    Bytef* recompressed = (Bytef*)malloc(compressBound_z(out_data.pos));
    z_size_t recompressed_len = compressBound_z(out_data.pos);
    
    if (out_data.pos > 0) {
        // Use uncompress2_z to decompress then compress again
        z_size_t uncompressed_len = size * 2;
        Bytef* uncompressed = (Bytef*)malloc(uncompressed_len);
        z_size_t source_len = compressed_len;
        
        if (uncompress2_z(uncompressed, &uncompressed_len, 
                         compressed, &source_len) == Z_OK) {
            // Now compress it again
            compress2_z(recompressed, &recompressed_len, 
                       uncompressed, uncompressed_len, 
                       Z_DEFAULT_COMPRESSION);
        }
        free(uncompressed);
    }
    
    // 15. Combine CRCs for verification
    uLong crc1 = crc32(0L, Z_NULL, 0);
    uLong crc2 = crc32(0L, Z_NULL, 0);
    
    if (out_data.pos > 0) {
        crc1 = crc32(crc1, out_data.buffer, (uInt)(out_data.pos > 65535 ? 65535 : out_data.pos));
    }
    if (size > 0) {
        crc2 = crc32(crc2, data, (uInt)(size > 65535 ? 65535 : size));
    }
    
    // Use a simple XOR operation for combining
    uLong combined_crc = crc32_combine_op(crc1, crc2, 1);

    // 16. Clean up everything
    deflateEnd(&deflate_strm);
    free(out_data.buffer);
    free(recompressed);
    gzclose(gz);
    assert_file_closed(&tmp_file);;
    free(compressed);
    remove("temp_compressed.gz");

    // Use the results somehow to avoid optimization
    if (result != Z_STREAM_END && result != Z_OK) {
        // Error in decompression - not a problem for fuzzing
    }
    
    if (codes_used > 1000000) {
        // This should never happen, but use the variable
        assert_file_closed(&tmp_file);
	assert_file_closed(&tmp_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    if (combined_crc == 0) {
        // Unlikely, but use the variable
        assert_file_closed(&tmp_file);
	assert_file_closed(&tmp_file);
	assert_fd_closed(fd);
	return 0;
    }

    assert_file_closed(&tmp_file);
	assert_file_closed(&tmp_file);
	assert_fd_closed(fd);
	return 0;
}