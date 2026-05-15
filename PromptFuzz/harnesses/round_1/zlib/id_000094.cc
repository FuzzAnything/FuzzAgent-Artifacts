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
//<ID> 283
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 1
//<Quality> {"density":13,"unique_branches":{"inflateBack":[[103,17,103,26,90,0,4,1]]},"library_calls":["zlibCompileFlags","gzdopen","gzerror","gzeof","gzrewind","gzgetc_","gzseek","gzseek64","gzgets","gzclose","gzclose","compressBound_z","gzclose","compress_z","gzclose","gzdopen","gzfwrite","crc32_combine_gen","gzclose","gzclose","inflateBackInit_","inflateBack","inflateBackEnd","deflateInit_","deflatePrime","deflateEnd","gzclose"],"critical_calls":["zlibCompileFlags","gzdopen","gzerror","gzeof","gzrewind","gzgetc_","gzseek","gzseek64","gzgets","compressBound_z","compress_z","gzdopen","gzfwrite","crc32_combine_gen","gzclose","inflateBackInit_","inflateBack","inflateBackEnd","deflateInit_","deflatePrime","deflateEnd","gzclose"],"visited":0}
/**/


// Forward declarations for the inflateBack callback functions
static unsigned int  id_000094_in_callback(void* desc, unsigned char** buf);
static int  id_000094_out_callback(void* desc, unsigned char* buf, unsigned int size);

// Structure to hold input data for inflateBack callbacks
struct InflateBackData {
    const uint8_t* data;
    size_t size;
    size_t pos;
    FILE* out_file;
};

extern "C" int LLVMFuzzerTestOneInput_94(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if we don't have enough data for basic operations
    if (size < 4) {
        return 0;
    }

    // 1. Get zlib compile flags
    uLong flags = zlibCompileFlags();
    (void)flags;  // Prevent unused variable warning

    // 2. Create a memory file from input data for gzdopen
    FILE* mem_file = fmemopen((void*)data, size, "rb");
    if (!mem_file) {
        assert_file_closed(&mem_file);
	return 0;
    }
    
    // 3. Open the memory file as a gzFile using gzdopen
    int fd = fuzz_fileno(mem_file);
    gzFile gz = gzdopen(fd, "rb");
    if (!gz) {
        assert_file_closed(&mem_file);;
        assert_file_closed(&mem_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 4. Check for errors using gzerror
    int errnum = 0;
    const char* error_msg = gzerror(gz, &errnum);
    (void)error_msg;  // Prevent unused variable warning

    // 5. Test gzeof (likely false at start)
    int at_eof = gzeof(gz);
    (void)at_eof;  // Prevent unused variable warning

    // 6. Rewind the gzFile
    gzrewind(gz);

    // 7. Read some data using gzgetc and gzgetc_
    int c1 = gzgetc(gz);
    int c2 = gzgetc_(gz);
    (void)c1; (void)c2;  // Prevent unused variable warnings

    // 8. Seek using both gzseek and gzseek64
    // Try to seek back to beginning if we read something
    if (c1 != -1) {
        off_t seek_result = gzseek(gz, 0, SEEK_SET);
        (void)seek_result;
        
        off64_t seek64_result = gzseek64(gz, 0, SEEK_SET);
        (void)seek64_result;
    }

    // 9. Read a line using gzgets
    char buf[256];
    char* line = gzgets(gz, buf, sizeof(buf));
    (void)line;  // Prevent unused variable warning

    // 10. Create an output file for writing compressed data
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        gzclose(gz);
        assert_file_closed(&mem_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&mem_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 11. Create a simple source buffer to compress
    // Use the first part of input data as source, but ensure we have enough
    size_t source_len = size > 1024 ? 1024 : size;
    Bytef* source = (Bytef*)malloc(source_len);
    if (!source) {
        assert_file_closed(&out_file);;
        gzclose(gz);
        assert_file_closed(&mem_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&mem_file);
	assert_fd_closed(fd);
	return 0;
    }
    memcpy(source, data, source_len);

    // 12. Compress using compress_z
    z_size_t dest_len = compressBound_z(source_len);
    Bytef* dest = (Bytef*)malloc(dest_len);
    if (!dest) {
        free(source);
        assert_file_closed(&out_file);;
        gzclose(gz);
        assert_file_closed(&mem_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&mem_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    int compress_result = compress_z(dest, &dest_len, source, source_len);
    (void)compress_result;  // Prevent unused variable warning

    // 13. Write compressed data using gzfwrite
    // First need a gzFile opened for writing
    assert_file_closed(&out_file);;  // Close and reopen for gzdopen
    out_file = fopen("output_file", "wb");
    if (!out_file) {
        free(dest);
        free(source);
        gzclose(gz);
        assert_file_closed(&mem_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&out_file);
	assert_file_closed(&mem_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    int out_fd = fuzz_fileno(out_file);
    gzFile gz_out = gzdopen(out_fd, "wb");
    if (gz_out && dest_len > 0) {
        z_size_t written = gzfwrite(dest, 1, dest_len, gz_out);
        (void)written;  // Prevent unused variable warning
        
        // 14. Use crc32_combine_gen
        off_t combine_distance = 10;
        uLong combine_gen = crc32_combine_gen(combine_distance);
        (void)combine_gen;  // Prevent unused variable warning
        
        gzclose(gz_out);
    } else if (gz_out) {
        gzclose(gz_out);
    }
    assert_file_closed(&out_file);;

    // 15. Set up inflateBack for decompression
    // Prepare input data structure
    InflateBackData ib_data;
    ib_data.data = data;
    ib_data.size = size;
    ib_data.pos = 0;
    ib_data.out_file = fopen("decompressed_output", "wb");
    
    if (ib_data.out_file) {
        z_stream strm;
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        
        // 16. Initialize inflateBack
        unsigned char window[32768];  // 32K window
        int init_result = inflateBackInit_(&strm, 15, window, 
                                          ZLIB_VERSION, sizeof(z_stream));
        
        if (init_result == Z_OK) {
            // 17. Perform inflateBack decompression
            int back_result = inflateBack(&strm,  id_000094_in_callback, &ib_data,
                                          id_000094_out_callback, ib_data.out_file);
            (void)back_result;  // Prevent unused variable warning
            
            // 18. End inflateBack
            inflateBackEnd(&strm);
        }
        
        fclose(ib_data.out_file);
    }

    // 19. Create a separate deflate stream for deflatePrime and deflateEnd
    z_stream deflate_strm;
    deflate_strm.zalloc = Z_NULL;
    deflate_strm.zfree = Z_NULL;
    deflate_strm.opaque = Z_NULL;
    
    // Initialize deflate
    if (deflateInit(&deflate_strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
        // 20. Use deflatePrime to insert bits
        int prime_result = deflatePrime(&deflate_strm, 2, 3);  // Insert 2 bits with value 3
        (void)prime_result;  // Prevent unused variable warning
        
        // 21. End the deflate stream
        deflateEnd(&deflate_strm);
    }

    // 22. Clean up
    free(dest);
    free(source);
    
    // 23. Close the gzFile (using gzclose which handles both reading and writing)
    // Note: We already closed gz_out earlier if it was created
    gzclose(gz);
    
    // 24. Close the memory file
    assert_file_closed(&mem_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&out_file);
	assert_file_closed(&mem_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
}

// Callback function for inflateBack input
static unsigned int  id_000094_in_callback(void* desc, unsigned char** buf) {
    InflateBackData* data = (InflateBackData*)desc;
    
    if (data->pos >= data->size) {
        return 0;  // No more data
    }
    
    // Calculate how much data we can return
    size_t available = data->size - data->pos;
    size_t to_return = available > 4096 ? 4096 : available;
    
    // Point buf to the current position
    *buf = (unsigned char*)(data->data + data->pos);
    data->pos += to_return;
    
    return (unsigned int)to_return;
}

// Callback function for inflateBack output
static int  id_000094_out_callback(void* desc, unsigned char* buf, unsigned int size) {
    FILE* out_file = (FILE*)desc;
    
    if (out_file && size > 0) {
        size_t written = fwrite(buf, 1, size, out_file);
        if (written != size) {
            return 1;  // Error
        }
    }
    
    return 0;  // Success
}