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
//<ID> 80
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["compressBound_z","compress_z","gzdopen","gzdopen","gzfwrite","gzclose","gzrewind","gzeof","gzgetc_","gzseek","gzgets","gzseek64","gzeof","deflateInit_","deflatePrime","deflateEnd","inflateBackInit_","inflateBackEnd","inflateBack","inflateBackEnd","crc32","crc32","crc32","crc32","crc32_combine_gen","gzclose"],"critical_calls":["compressBound_z","compress_z","gzdopen","gzdopen","gzfwrite","gzclose","gzrewind","gzeof","gzgetc_","gzseek","gzgets","gzseek64","gzeof","deflateInit_","deflatePrime","deflateEnd","inflateBackInit_","inflateBack","inflateBackEnd","crc32","crc32","crc32","crc32","crc32_combine_gen","gzclose"],"visited":0}
/**/


#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations for inflateBack callback functions
unsigned int  id_000042_in_func_callback(void *desc, unsigned char **buf);
int  id_000042_out_func_callback(void *desc, unsigned char *buf, unsigned int size);

#ifdef __cplusplus
}
#endif

// Global variables for inflateBack callbacks
struct InBackDesc {
    const uint8_t *data;
    size_t size;
    size_t pos;
};

struct OutBackDesc {
    Bytef *buffer;
    uInt capacity;
    uInt used;
};

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
        return 0;
    }

    // Variables for compression
    Bytef *compressed_data = nullptr;
    z_size_t compressed_len = 0;
    z_size_t bound_len = 0;
    
    // Variables for gzFile operations
    gzFile gz_file = nullptr;
    FILE *tmp_file = nullptr;
    int fd = -1;
    
    // Variables for deflate/inflate
    z_stream strm;
    int ret;
    
    // Variables for inflateBack
    InBackDesc in_desc;
    OutBackDesc out_desc;
    
    // Variables for reading
    char read_buf[256];
    off_t seek_offset;
    off64_t seek64_offset;
    int gz_char;
    
    // 1. Calculate maximum compressed size
    bound_len = compressBound_z(size);
    if (bound_len == 0) {
        goto cleanup;
    }
    
    // 2. Allocate memory for compressed data
    compressed_data = (Bytef *)malloc(bound_len);
    if (!compressed_data) {
        goto cleanup;
    }
    
    // 3. Compress the input data
    compressed_len = bound_len;
    ret = compress_z(compressed_data, &compressed_len, data, size);
    if (ret != Z_OK) {
        goto cleanup;
    }
    
    // 4. Create a temporary file for gz operations
    tmp_file = fopen("output_file", "wb+");
    if (!tmp_file) {
        goto cleanup;
    }
    
    // 5. Get file descriptor and open as gzFile
    fd = fuzz_fileno(tmp_file);
    if (fd < 0) {
        goto cleanup;
    }
    
    // 6. Write compressed data to file
    if (fwrite(compressed_data, 1, compressed_len, tmp_file) != compressed_len) {
        goto cleanup;
    }
    fflush(tmp_file);
    rewind(tmp_file);
    
    // 7. Open file as gzFile for reading
    gz_file = gzdopen(fd, "rb");
    if (!gz_file) {
        goto cleanup;
    }
    
    // 8. Try gzfwrite (for completeness, though we're reading)
    // Note: We'll write a small amount to a different file
    {
        FILE *out_file = fopen("output_file2", "wb");
        if (out_file) {
            int out_fd = fuzz_fileno(out_file);
            gzFile gz_out = gzdopen(out_fd, "wb");
            if (gz_out) {
                // Write first 100 bytes of compressed data if available
                z_size_t write_size = compressed_len > 100 ? 100 : compressed_len;
                z_size_t items_written = gzfwrite(compressed_data, 1, write_size, gz_out);
                (void)items_written; // Prevent unused variable warning
                gzclose(gz_out);
            } else {
                assert_file_closed(&out_file);;
            }
        }
    }
    
    // 9. Rewind the gzFile
    ret = gzrewind(gz_file);
    if (ret != 0) {
        goto cleanup;
    }
    
    // 10. Read some data using gzgetc_
    for (int i = 0; i < 10 && !gzeof(gz_file); i++) {
        gz_char = gzgetc_(gz_file);
        (void)gz_char; // Prevent unused variable warning
    }
    
    // 11. Seek using gzseek
    seek_offset = 5;
    ret = gzseek(gz_file, seek_offset, SEEK_SET);
    (void)ret; // Prevent unused variable warning
    
    // 12. Read a line using gzgets
    if (gzgets(gz_file, read_buf, sizeof(read_buf)) == nullptr) {
        // Handle error or EOF
    }
    
    // 13. Seek using gzseek64
    seek64_offset = 10;
    ret = gzseek64(gz_file, seek64_offset, SEEK_SET);
    (void)ret; // Prevent unused variable warning
    
    // 14. Check if at EOF
    ret = gzeof(gz_file);
    (void)ret; // Prevent unused variable warning
    
    // 15. Initialize deflate stream
    memset(&strm, 0, sizeof(strm));
    ret = deflateInit_(&strm, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        goto cleanup;
    }
    
    // 16. Use deflatePrime to insert bits
    ret = deflatePrime(&strm, 2, 3); // Insert 2 bits with value 3
    (void)ret; // Prevent unused variable warning
    
    // 17. Clean up deflate stream
    ret = deflateEnd(&strm);
    (void)ret; // Prevent unused variable warning
    
    // 18. Set up inflateBack
    memset(&strm, 0, sizeof(strm));
    ret = inflateBackInit(&strm, 15, nullptr); // 15 window bits
    if (ret != Z_OK) {
        goto cleanup;
    }
    
    // 19. Set up callback descriptors
    in_desc.data = compressed_data;
    in_desc.size = compressed_len;
    in_desc.pos = 0;
    
    out_desc.buffer = (Bytef *)malloc(4096);
    out_desc.capacity = 4096;
    out_desc.used = 0;
    
    if (!out_desc.buffer) {
        inflateBackEnd(&strm);
        goto cleanup;
    }
    
    // 20. Perform inflateBack
    ret = inflateBack(&strm,  id_000042_in_func_callback, &in_desc, 
                      id_000042_out_func_callback, &out_desc);
    (void)ret; // Prevent unused variable warning
    
    // 21. Clean up inflateBack
    inflateBackEnd(&strm);
    
    // 22. Generate CRC combine value
    {
        off_t len1 = 100;
        off_t len2 = 200;
        uLong crc1 = crc32(0L, Z_NULL, 0);
        uLong crc2 = crc32(0L, Z_NULL, 0);
        
        // Calculate some dummy CRC values
        if (size > 100) {
            crc1 = crc32(0L, data, 100);
        }
        if (size > 200) {
            crc2 = crc32(0L, data + 100, 100);
        }
        
        // Generate combine value
        uLong combine_gen = crc32_combine_gen(len1);
        (void)combine_gen; // Prevent unused variable warning
    }

cleanup:
    // 23. Close gzFile if opened
    if (gz_file) {
        gzclose(gz_file);
    } else if (fd >= 0) {
        // If gzdopen failed, we still need to close the file
        if (tmp_file) {
            assert_file_closed(&tmp_file);;
        }
    }
    
    // 24. Close temporary file if still open
    if (tmp_file && fd >= 0) {
        // File was passed to gzdopen, so don't close it here
        // gzdopen takes ownership of the file descriptor
    } else if (tmp_file) {
        assert_file_closed(&tmp_file);;
    }
    
    // 25. Free allocated memory
    if (compressed_data) {
        free(compressed_data);
    }
    
    if (out_desc.buffer) {
        free(out_desc.buffer);
    }
    
    // 26. Remove temporary files
    remove("output_file");
    remove("output_file2");
    
    assert_file_closed(&tmp_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}

// inflateBack input callback implementation
unsigned int  id_000042_in_func_callback(void *desc, unsigned char **buf) {
    InBackDesc *d = (InBackDesc *)desc;
    
    if (d->pos >= d->size) {
        return 0; // No more data
    }
    
    // Calculate how much data we can provide
    size_t available = d->size - d->pos;
    
    // Return pointer to remaining data
    *buf = (unsigned char *)(d->data + d->pos);
    
    // Update position
    d->pos = d->size;
    
    return (unsigned int)(available > UINT_MAX ? UINT_MAX : available);
}

// inflateBack output callback implementation
int  id_000042_out_func_callback(void *desc, unsigned char *buf, unsigned int size) {
    OutBackDesc *d = (OutBackDesc *)desc;
    
    // Check if we have enough capacity
    if (d->used + size > d->capacity) {
        // Resize buffer if needed
        uInt new_capacity = d->capacity * 2;
        if (new_capacity < d->used + size) {
            new_capacity = d->used + size;
        }
        
        Bytef *new_buffer = (Bytef *)realloc(d->buffer, new_capacity);
        if (!new_buffer) {
            return 1; // Error
        }
        
        d->buffer = new_buffer;
        d->capacity = new_capacity;
    }
    
    // Copy data to buffer
    memcpy(d->buffer + d->used, buf, size);
    d->used += size;
    
    return 0; // Success
}