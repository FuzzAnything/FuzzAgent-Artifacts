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
//<ID> 40
//<Prompt> []
/*<Combination>: [
*/
//<score> 17.333334, nr_unique_branch: 6
//<Quality> {"density":13,"unique_branches":{"deflateInit2_":[[401,9,401,38,0,0,4,1],[409,9,409,36,0,0,4,1]],"inflateBack":[[563,13,563,62,0,0,4,0],[564,13,564,32,0,0,4,1]],"inflateBackInit_":[[37,9,37,38,0,0,4,1],[45,9,45,36,0,0,4,1]]},"library_calls":["compressBound_z","compress_z","gzdopen","gzfwrite","gzseek64","gzseek","gzerror","gzrewind","gzclose","deflateInit_","deflatePrime","deflateEnd","inflateBackInit_","inflateBack","inflateBackEnd","inflateBackEnd","crc32","crc32","crc32_combine_gen","crc32_combine_op"],"critical_calls":["compressBound_z","compress_z","gzdopen","gzfwrite","gzseek64","gzseek","gzerror","gzrewind","gzclose","deflateInit_","deflatePrime","deflateEnd","inflateBackInit_","inflateBack","inflateBackEnd","crc32","crc32","crc32_combine_gen","crc32_combine_op"],"visited":2}
/*Here's a comprehensive fuzz driver that uses all the specified zlib APIs with input data:

*/


// Custom allocator functions to match zlib's interface
static void*  id_000023_my_alloc(void* opaque, unsigned int items, unsigned int size) {
    (void)opaque;
    return malloc(items * size);
}

static void  id_000023_my_free(void* opaque, void* address) {
    (void)opaque;
    free(address);
}

// Input function for inflateBack
static unsigned int  id_000023_input_callback(void* desc, unsigned char** buf) {
    FILE* file = (FILE*)desc;
    static unsigned char local_buf[1024];
    size_t bytes_read = fread(local_buf, 1, sizeof(local_buf), file);
    *buf = local_buf;
    return (unsigned int)bytes_read;
}

// Output function for inflateBack
static int  id_000023_output_callback(void* desc, unsigned char* buf, unsigned int size) {
    FILE* file = (FILE*)desc;
    return (int)fwrite(buf, 1, size, file);
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful testing

    // 1. First compress the input data using compress_z
    z_size_t compressed_len = compressBound_z(size);
    Bytef* compressed_data = (Bytef*)malloc(compressed_len);
    int compress_result = compress_z(compressed_data, &compressed_len, 
                                    (const Bytef*)data, size);
    
    if (compress_result != Z_OK) {
        free(compressed_data);
        return 0;
    }

    // 2. Create a gzFile for writing compressed data
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        free(compressed_data);
        assert_file_closed(&out_file);
	return 0;
    }
    
    int fd = fuzz_fileno(out_file);
    gzFile gz_file = gzdopen(fd, "wb");
    
    // 3. Write compressed data using gzfwrite
    z_size_t items_written = gzfwrite((voidpc)compressed_data, 1, 
                                     compressed_len, gz_file);
    (void)items_written;  // Mark as used
    
    // 4. Seek operations on gzFile
    // Using both gzseek64 and gzseek
    off64_t seek64_result = gzseek64(gz_file, 0, SEEK_SET);
    off_t seek_result = gzseek(gz_file, 0, SEEK_SET);
    (void)seek64_result;  // Mark as used
    (void)seek_result;    // Mark as used
    
    // 5. Check for errors with gzerror
    int errnum;
    const char* error_str = gzerror(gz_file, &errnum);
    (void)error_str;  // Mark as used
    
    // 6. Rewind the file
    int rewind_result = gzrewind(gz_file);
    (void)rewind_result;  // Mark as used
    
    // Close the gzFile
    gzclose(gz_file);
    assert_file_closed(&out_file);;
    free(compressed_data);
    
    // 7. Create and use a deflate stream with deflatePrime
    z_stream deflate_strm;
    deflate_strm.zalloc =  id_000023_my_alloc;
    deflate_strm.zfree =  id_000023_my_free;
    deflate_strm.opaque = Z_NULL;
    
    int init_result = deflateInit_(&deflate_strm, Z_DEFAULT_COMPRESSION,
                                  ZLIB_VERSION, sizeof(z_stream));
    
    if (init_result == Z_OK) {
        // Use deflatePrime to insert bits
        deflatePrime(&deflate_strm, 5, 0x1F);  // Insert 5 bits with value 0x1F
        deflateEnd(&deflate_strm);
    }
    
    // 8. Create and use an inflate stream with inflateBack
    z_stream inflate_strm;
    inflate_strm.zalloc =  id_000023_my_alloc;
    inflate_strm.zfree =  id_000023_my_free;
    inflate_strm.opaque = Z_NULL;
    
    // Initialize inflate for back processing
    unsigned char window[32768];  // 32K window
    int back_init = inflateBackInit_(&inflate_strm, 15, window,
                                    ZLIB_VERSION, sizeof(z_stream));
    
    if (back_init == Z_OK) {
        // Create input FILE* from compressed data
        FILE* in_file = fmemopen((void*)data, size, "rb");
        if (in_file) {
            // Create output FILE for decompressed data
            FILE* decomp_out = fopen("decomp_output", "wb");
            
            if (decomp_out) {
                // Use inflateBack to decompress
                int back_result = inflateBack(&inflate_strm, 
                                              id_000023_input_callback, in_file,
                                              id_000023_output_callback, decomp_out);
                (void)back_result;  // Mark as used
                
                inflateBackEnd(&inflate_strm);
                assert_file_closed(&decomp_out);;
            }
            assert_file_closed(&in_file);;
        } else {
            inflateBackEnd(&inflate_strm);
        }
    }
    
    // 9. Use crc32_combine_gen
    if (size > 0) {
        // Calculate CRC of first and second halves
        size_t split = size / 2;
        uLong crc1 = crc32(0L, (const Bytef*)data, (uInt)split);
        uLong crc2 = crc32(0L, (const Bytef*)data + split, (uInt)(size - split));
        
        // Generate combine value
        off_t len2 = (off_t)(size - split);
        uLong combine_gen = crc32_combine_gen(len2);
        
        // Combine the two CRCs (using the generated combine value)
        uLong combined = crc32_combine_op(crc1, crc2, combine_gen);
        (void)combined;  // Mark as used
    }
    
    // 10. Clean up temporary files
    remove("output_file");
    remove("decomp_output");
    
    assert_file_closed(&out_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}