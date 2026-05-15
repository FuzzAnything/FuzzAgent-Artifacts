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
//<ID> 202
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["gzdopen","gzeof","gzungetc","gzrewind","deflateInit_","deflateReset","deflateEnd","compressBound_z","inflateInit2_","inflateSyncPoint","inflateUndermine","inflateEnd","inflateBackInit_","inflatePrime","inflateBack","inflateBackEnd","uncompress2_z","crc32","crc32","crc32","crc32","crc32_combine_gen","crc32_combine_op","gzclose"],"critical_calls":["gzdopen","gzeof","gzungetc","gzrewind","deflateInit_","deflateReset","deflateEnd","compressBound_z","inflateInit2_","inflateSyncPoint","inflateUndermine","inflateEnd","inflateBackInit_","inflatePrime","inflateBack","inflateBackEnd","uncompress2_z","crc32","crc32","crc32","crc32","crc32_combine_gen","crc32_combine_op","gzclose"],"visited":0}
/**/


// Helper structure for inflateBack callbacks
struct back_io_desc {
    const uint8_t* data;
    size_t size;
    size_t pos;
};

// Input callback for inflateBack
static unsigned int  id_000068_back_in_func(void* desc, unsigned char** buf) {
    back_io_desc* d = static_cast<back_io_desc*>(desc);
    
    // Return available data
    size_t available = d->size - d->pos;
    if (available == 0) {
        return 0;  // No more data
    }
    
    *buf = const_cast<unsigned char*>(d->data + d->pos);
    size_t to_read = (available > 4096) ? 4096 : available;
    d->pos += to_read;
    
    return static_cast<unsigned int>(to_read);
}

// Output callback for inflateBack
static int  id_000068_back_out_func(void* desc, unsigned char* buf, unsigned int len) {
    // Just discard the output for fuzzing
    (void)desc;
    (void)buf;
    (void)len;
    return 0;  // Success
}

extern "C" int LLVMFuzzerTestOneInput_68(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }
    
    // Create FILE* for input data
    FILE* in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptor for gzdopen
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 1. Open compressed data as gzFile
    gzFile gzfile = gzdopen(fd, "rb");
    if (!gzfile) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 2. Check if we're at EOF
    int is_eof = gzeof(gzfile);
    (void)is_eof;  // Use the value
    
    // 3. Try to push a character back
    int pushed = gzungetc('A', gzfile);
    (void)pushed;
    
    // 4. Rewind the gzFile
    int rewind_result = gzrewind(gzfile);
    (void)rewind_result;
    
    // 5. Initialize deflate stream (for CRC32 combine operations)
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(z_stream));
    
    int deflate_init_result = deflateInit_(&deflate_strm, Z_DEFAULT_COMPRESSION,
                                         ZLIB_VERSION, sizeof(z_stream));
    if (deflate_init_result == Z_OK) {
        // 6. Reset deflate stream
        deflateReset(&deflate_strm);
        
        // 7. End deflate stream
        deflateEnd(&deflate_strm);
    }
    
    // 8. Calculate upper bound for compressed size
    z_size_t bound = compressBound_z(size);
    (void)bound;
    
    // 9. Prepare for inflateBack operations
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(z_stream));
    
    // 10. Initialize inflate with windowBits
    int windowBits = 15;  // Default window size
    int inflate_init_result = inflateInit2_(&inflate_strm, windowBits,
                                           ZLIB_VERSION, sizeof(z_stream));
    
    if (inflate_init_result == Z_OK) {
        // 11. Try to find sync point
        int sync_point = inflateSyncPoint(&inflate_strm);
        (void)sync_point;
        
        // 12. Try to undermine inflate (test error handling)
        int undermine_result = inflateUndermine(&inflate_strm, 0);
        (void)undermine_result;
        
        // 13. End inflate
        inflateEnd(&inflate_strm);
    }
    
    // 14. Initialize for inflateBack with custom window
    unsigned char window[32768];  // 32K window
    z_stream back_strm;
    memset(&back_strm, 0, sizeof(z_stream));
    
    int back_init_result = inflateBackInit_(&back_strm, windowBits, window,
                                           ZLIB_VERSION, sizeof(z_stream));
    
    if (back_init_result == Z_OK) {
        // 15. Prepare I/O descriptors
        back_io_desc io_desc;
        io_desc.data = data;
        io_desc.size = size;
        io_desc.pos = 0;
        
        // 16. Insert prime bits
        inflatePrime(&back_strm, 2, 1);  // Insert 2 bits with value 1
        
        // 17. Perform inflateBack
        int back_result = inflateBack(&back_strm,  id_000068_back_in_func, &io_desc,
                                      id_000068_back_out_func, nullptr);
        (void)back_result;
        
        // 18. End inflateBack
        inflateBackEnd(&back_strm);
    }
    
    // 19. Try uncompress2_z on the data
    if (size > 0) {
        // Allocate destination buffer
        z_size_t dest_len = size * 4;  // Guess expansion factor
        Bytef* dest = static_cast<Bytef*>(malloc(dest_len));
        if (dest) {
            z_size_t source_len = size;
            int uncompress_result = uncompress2_z(dest, &dest_len,
                                                 data, &source_len);
            (void)uncompress_result;
            free(dest);
        }
    }
    
    // 20. Calculate CRC32 of input data
    uLong crc1 = crc32(0L, Z_NULL, 0);
    uInt chunk_size = (size > 1024) ? 1024 : static_cast<uInt>(size);
    if (chunk_size > 0) {
        crc1 = crc32(crc1, data, chunk_size);
    }
    
    // 21. Calculate another CRC32 for second chunk (if available)
    uLong crc2 = crc32(0L, Z_NULL, 0);
    if (size > chunk_size && (size - chunk_size) > 0) {
        uInt second_chunk = (size - chunk_size > 1024) ? 
                          1024 : static_cast<uInt>(size - chunk_size);
        crc2 = crc32(crc2, data + chunk_size, second_chunk);
    }
    
    // 22. Generate combine operation for CRC32
    off_t len2 = static_cast<off_t>(chunk_size);
    uLong combine_gen = crc32_combine_gen(len2);
    
    // 23. Combine the two CRC32 values
    uLong combined_crc = crc32_combine_op(crc1, crc2, combine_gen);
    (void)combined_crc;
    
    // 24. Close gzFile
    gzclose(gzfile);
    
    // 25. Close the original FILE*
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}