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
//<ID> 217
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 1
//<Quality> {"density":21,"unique_branches":{"inflatePrime":[[222,9,222,32,0,0,4,0]]},"library_calls":["gzdopen","gzclose","deflateInit_","gzclose","deflateSetHeader","deflatePrime","deflateSetDictionary","deflateEnd","gzclose","deflateBound","deflateEnd","gzclose","deflate","deflateEnd","gzfwrite","gzclose_w","compressBound_z","compress_z","crc32","crc32","crc32_combine_gen","gzdopen","gzseek","gzseek64","gztell","gzrewind","gzeof","gzgets","gzgetc_","inflateBackInit_","gzclose","inflateValidate","inflatePrime","inflateBack","inflateBackEnd","gzclose"],"critical_calls":["gzdopen","deflateInit_","deflateSetHeader","deflatePrime","deflateSetDictionary","deflateBound","deflate","deflateEnd","gzfwrite","gzclose_w","compressBound_z","compress_z","crc32","crc32","crc32_combine_gen","gzdopen","gzseek","gzseek64","gztell","gzrewind","gzeof","gzgets","gzgetc_","inflateBackInit_","inflateValidate","inflatePrime","inflateBack","inflateBackEnd","gzclose"],"visited":1}
/**/


// Define custom in_func and out_func types for inflateBack
typedef unsigned int (*in_func)(void *, unsigned char **);
typedef int (*out_func)(void *, unsigned char *, unsigned int);

// Callback function for inflateBack input
static unsigned int  id_000080_inf_back_in_func(void *desc, unsigned char **buf) {
    FILE *file = (FILE *)desc;
    static unsigned char local_buf[1024];
    size_t bytes_read = fread(local_buf, 1, sizeof(local_buf), file);
    *buf = local_buf;
    return (unsigned int)bytes_read;
}

// Callback function for inflateBack output
static int  id_000080_inf_back_out_func(void *desc, unsigned char *buf, unsigned int size) {
    FILE *file = (FILE *)desc;
    return (int)fwrite(buf, 1, size, file) == size ? 0 : -1;
}

extern "C" int LLVMFuzzerTestOneInput_80(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Skip empty inputs
    if (size == 0) {
	return 0;
	}
	
    
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    int fd = fuzz_fileno(out_file);
    gzFile gz_out = gzdopen(fd, "wb");
    if (!gz_out) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Get a dictionary from input data (first 256 bytes or less)
    uInt dict_len = size > 256 ? 256 : (uInt)size;
    Bytef *dictionary = (Bytef*)malloc(dict_len);
    if (!dictionary) {
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    memcpy(dictionary, data, dict_len);
    
    // 1. deflateInit_ example
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    int ret = deflateInit_(&strm, Z_DEFAULT_COMPRESSION, 
                          ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        free(dictionary);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 2. deflateSetHeader example
    gz_header header;
    memset(&header, 0, sizeof(header));
    header.os = 255; // Unknown OS
    deflateSetHeader(&strm, &header);
    
    // 3. deflatePrime example
    deflatePrime(&strm, 5, 31); // Prime with some bits
    
    // 4. deflateSetDictionary example
    deflateSetDictionary(&strm, dictionary, dict_len);
    
    // Prepare input buffer for deflate
    Bytef *in_buf = (Bytef*)malloc(size);
    if (!in_buf) {
        deflateEnd(&strm);
        free(dictionary);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    memcpy(in_buf, data, size);
    
    strm.avail_in = (uInt)size;
    strm.next_in = in_buf;
    
    // Prepare output buffer
    uLong bound = deflateBound(&strm, (uLong)size);
    Bytef *out_buf = (Bytef*)malloc(bound);
    if (!out_buf) {
        free(in_buf);
        deflateEnd(&strm);
        free(dictionary);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    strm.avail_out = (uInt)bound;
    strm.next_out = out_buf;
    
    // Perform deflation
    ret = deflate(&strm, Z_FINISH);
    
    // 5. deflateEnd
    deflateEnd(&strm);
    
    // 6. gzfwrite example - write compressed data to gz file
    z_size_t compressed_size = (z_size_t)(strm.total_out);
    gzfwrite(out_buf, 1, compressed_size, gz_out);
    
    // 7. gzclose_w
    gzclose_w(gz_out);
    
    // 8. compressBound_z and compress_z example
    z_size_t source_len = (z_size_t)size;
    z_size_t dest_len = compressBound_z(source_len);
    Bytef *compressed = (Bytef*)malloc(dest_len);
    if (compressed) {
        compress_z(compressed, &dest_len, data, source_len);
        free(compressed);
    }
    
    // 9. crc32 example
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, data, (uInt)size);
    
    // 10. crc32_combine_gen example
    uLong combine_gen = crc32_combine_gen((off_t)size);
    
    // Reopen output_file for reading
    assert_file_closed(&out_file);;
    out_file = fopen("output_file", "rb");
    if (!out_file) {
        free(in_buf);
        free(out_buf);
        free(dictionary);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
	int fuzz_fd_0 = fuzz_fileno(out_file);

    
    gzFile gz_in = gzdopen(fuzz_fd_0, "rb");
    if (!gz_in) {
        free(in_buf);
        free(out_buf);
        free(dictionary);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 11. gzseek example
    gzseek(gz_in, 0, SEEK_SET);
    
    // 12. gzseek64 example
    gzseek64(gz_in, 0, SEEK_SET);
    
    // 13. gztell example
    off_t pos = gztell(gz_in);
    
    // 14. gzrewind example
    gzrewind(gz_in);
    
    // 15. gzeof example
    int is_eof = gzeof(gz_in);
    
    // 16. gzgets example
    char line_buf[256];
    gzgets(gz_in, line_buf, sizeof(line_buf));
    
    // 17. gzgetc_ example
    int ch = gzgetc_(gz_in);
    
    // 18. inflateBackInit_ example
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    unsigned char window[32768]; // 32K window
    ret = inflateBackInit_(&inf_strm, 15, window, 
                          ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_in);
        free(in_buf);
        free(out_buf);
        free(dictionary);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 19. inflateValidate example
    inflateValidate(&inf_strm, 1);
    
    // 20. inflatePrime example
    inflatePrime(&inf_strm, 5, 31);
    
    // 21. inflateBack example
    rewind(out_file);
    FILE *decomp_out = tmpfile();
    if (decomp_out) {
        inflateBack(&inf_strm,  id_000080_inf_back_in_func, out_file, 
                    id_000080_inf_back_out_func, decomp_out);
        assert_file_closed(&decomp_out);;
    }
    
    // 22. inflateBackEnd
    inflateBackEnd(&inf_strm);
    
    // 23. gzclose
    gzclose(gz_in);
    
    // Clean up all resources
    free(in_buf);
    free(out_buf);
    free(dictionary);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove temporary output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fuzz_fd_0);
	assert_fd_closed(fd);
	return 0;
}