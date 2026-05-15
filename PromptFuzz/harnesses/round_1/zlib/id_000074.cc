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
//<ID> 200
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["gzdopen","compressBound_z","compress_z","gzfwrite","gzclose_w","gzdopen","deflateInit_","deflateSetDictionary","deflatePrime","deflateEnd","inflateBackInit_","inflateValidate","inflateBack","inflateBackEnd","gzrewind","gzseek","gzseek64","gzeof","gzgetc_","gzgets","gzeof","crc32","crc32","crc32_combine_gen","gzclose"],"critical_calls":["gzdopen","compressBound_z","compress_z","gzfwrite","gzclose_w","gzdopen","deflateInit_","deflateSetDictionary","deflatePrime","deflateEnd","inflateBackInit_","inflateValidate","inflateBack","inflateBackEnd","gzrewind","gzseek","gzseek64","gzeof","gzgetc_","gzgets","gzeof","crc32","crc32","crc32_combine_gen","gzclose"],"visited":0}
/**/


// Callback functions for inflateBack
static unsigned int  id_000074_infunc(void* desc, unsigned char** buf) {
    static unsigned char static_buf[1024];
    FILE* in_file = (FILE*)desc;
    size_t bytes_read = fread(static_buf, 1, sizeof(static_buf), in_file);
    *buf = static_buf;
    return (unsigned int)bytes_read;
}

static int  id_000074_outfunc(void* desc, unsigned char* buf, unsigned int size) {
    // Discard output for fuzzing
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_74(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create input file from fuzzer data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Create output file
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
    
    // 3. Open gzip file for writing
    gzFile gz_out = gzdopen(out_fd, "wb");
    if (!gz_out) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 4. Calculate maximum compressed size
    z_size_t max_compressed = compressBound_z(size);
    
    // 5. Compress the input data
    Bytef* compressed = new Bytef[max_compressed];
    z_size_t compressed_len = max_compressed;
    
    int compress_result = compress_z(compressed, &compressed_len, data, size);
    
    // 6. Write compressed data to gzip file
    if (compress_result == Z_OK) {
        z_size_t items_written = gzfwrite(compressed, 1, compressed_len, gz_out);
        (void)items_written; // Avoid unused warning
    }
    
    // 7. Close the gzip file for writing
    gzclose_w(gz_out);
    assert_file_closed(&out_file);;
    
    // 8. Reopen output file for reading
    out_file = fopen("output_file", "rb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        delete[] compressed;
        assert_file_closed(&out_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 9. Open gzip file for reading
    out_fd = fuzz_fileno(out_file);
    gzFile gz_in = gzdopen(out_fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        delete[] compressed;
        assert_file_closed(&out_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 10. Initialize deflate stream for dictionary operations
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(z_stream));
    
    int deflate_init = deflateInit_(&deflate_strm, Z_DEFAULT_COMPRESSION, 
                                   ZLIB_VERSION, sizeof(z_stream));
    
    if (deflate_init == Z_OK && size > 0) {
        // 11. Set dictionary from input data (use first min(size, 32768) bytes)
        uInt dict_len = size > 32768 ? 32768 : (uInt)size;
        deflateSetDictionary(&deflate_strm, data, dict_len);
        
        // 12. Prime deflate stream with some bits from input
        if (size >= 2) {
            deflatePrime(&deflate_strm, 8, data[0]);
        }
        
        // 13. End deflate stream
        deflateEnd(&deflate_strm);
    }
    
    // 14. Initialize inflate back stream
    z_stream inflate_back_strm;
    memset(&inflate_back_strm, 0, sizeof(z_stream));
    
    unsigned char window[32768];
    int back_init = inflateBackInit_(&inflate_back_strm, MAX_WBITS, 
                                     window, ZLIB_VERSION, sizeof(z_stream));
    
    if (back_init == Z_OK) {
        // 15. Reset file pointer to beginning
        fseek(in_file, 0, SEEK_SET);
        
        // 16. Set validation
        inflateValidate(&inflate_back_strm, 1);
        
        // 17. Perform back inflation
        inflateBack(&inflate_back_strm,  id_000074_infunc, in_file,  id_000074_outfunc, NULL);
        
        // 18. End back inflation
        inflateBackEnd(&inflate_back_strm);
    }
    
    // 19. Operations on gzip file
    gzrewind(gz_in);
    
    // 20. Seek operations
    gzseek(gz_in, 0, SEEK_SET);
    
    // 21. 64-bit seek (if supported)
    off64_t offset = 0;
    gzseek64(gz_in, offset, SEEK_SET);
    
    // 22. Read characters
    while (!gzeof(gz_in)) {
        // 23. Read one character
        gzgetc_(gz_in);
        
        // 24. Read a line (if any text)
        char buf[256];
        gzgets(gz_in, buf, sizeof(buf));
        
        // Check if we're at end
        if (gzeof(gz_in)) break;
    }
    
    // 25. Calculate CRC32 of input data
    uLong crc = crc32(0L, Z_NULL, 0);
    if (size > 0) {
        uInt chunk = size > 1024 ? 1024 : (uInt)size;
        crc = crc32(crc, data, chunk);
    }
    
    // 26. Generate CRC combine generator
    off_t combine_len = 100;
    uLong gen = crc32_combine_gen(combine_len);
    (void)gen; // Avoid unused warning
    
    // 27. Cleanup
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    delete[] compressed;
    
    // Remove temporary file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}