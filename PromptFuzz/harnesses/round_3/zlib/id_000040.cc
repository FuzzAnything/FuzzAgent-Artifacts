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
//<score> 88, nr_unique_branch: 8
//<Quality> {"density":22,"unique_branches":{"gz_read":[[352,32,352,57,0,0,4,1],[357,18,357,36,0,0,4,1],[357,40,357,62,0,0,4,0]],"gzread":[[407,9,407,27,0,0,4,0],[407,31,407,56,0,0,4,0],[407,31,407,56,0,0,4,1],[407,60,407,73,0,0,4,0]],"gzgetc":[[497,12,497,38,0,0,4,1]]},"library_calls":["zlibVersion","compressBound","compress2","compressBound_z","compress_z","deflateInit_","inflateInit_","deflateEnd","deflateCopy","deflateEnd","inflateEnd","deflateEnd","inflate","inflateCodesUsed","inflateEnd","deflateEnd","inflateEnd","deflateEnd","gzdopen","inflateEnd","deflateEnd","gzsetparams","gzputs","gzwrite","gzclose_w","inflateEnd","deflateEnd","gzdopen","inflateEnd","deflateEnd","gzgetc_","gzrewind","crc32_z","gzread","crc32_z","gzclose","inflateEnd","deflateEnd"],"critical_calls":["zlibVersion","compressBound","compress2","compressBound_z","compress_z","deflateInit_","inflateInit_","deflateCopy","deflateEnd","inflate","inflateCodesUsed","gzdopen","gzsetparams","gzputs","gzwrite","gzclose_w","gzdopen","gzgetc_","gzrewind","crc32_z","gzread","crc32_z","gzclose","inflateEnd","deflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<59) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
	//fuzzer shim end}




    // 1. Get zlib version
    const char *version = zlibVersion();
    if (!version) {
	return 0;
	}
	

    // 2. First compress the input data to get valid zlib output
    Bytef *compressed = nullptr;
    uLongf compressed_size = 0;
    int compress_result = 0;
    
    if (size > 0) {
        // First compression using compress2
        compressed_size = compressBound(size);
        if (compressed_size == 0) {
	return 0;
	}
	
        
        compressed = (Bytef *)malloc(compressed_size);
        if (!compressed) {
	return 0;
	}
	
        
        compress_result = compress2(compressed, &compressed_size, data, size, fuzz_int32_t_1);
        if (compress_result != Z_OK) {
            free(compressed);
            return 0;
        }
        
        // Also test compress_z
        z_size_t compressed_size_z = compressBound_z(size);
        Bytef *compressed_z = (Bytef *)malloc(compressed_size_z);
        if (compressed_z) {
            z_size_t destLen = compressed_size_z;
            int result_z = compress_z(compressed_z, &destLen, data, size);
            free(compressed_z);
        }
    } else {
        // If input is empty, we can't compress it, so just return
        return 0;
    }

    // 3. Setup deflate stream for compression
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    
    int deflate_init_result = deflateInit_(&deflate_strm, fuzz_int32_t_2, 
                                           version, sizeof(z_stream));
    if (deflate_init_result != Z_OK) {
        free(compressed);
        return 0;
    }

    // 4. Setup inflate stream for decompression
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    
    int inflate_init_result = inflateInit_(&inflate_strm, version, sizeof(z_stream));
    if (inflate_init_result != Z_OK) {
        deflateEnd(&deflate_strm);
        free(compressed);
        return 0;
    }

    // 5. Test deflateCopy
    z_stream deflate_copy_strm;
    memset(&deflate_copy_strm, 0, sizeof(deflate_copy_strm));
    int copy_result = deflateCopy(&deflate_copy_strm, &deflate_strm);
    if (copy_result == Z_OK) {
        deflateEnd(&deflate_copy_strm);
    }

    // 6. Process the compressed data through inflate
    Bytef *decompressed = (Bytef *)malloc(size * 2); // Allocate enough space
    if (!decompressed) {
        inflateEnd(&inflate_strm);
        deflateEnd(&deflate_strm);
        free(compressed);
        return 0;
    }
    
    inflate_strm.next_in = compressed;
    inflate_strm.avail_in = compressed_size;
    inflate_strm.next_out = decompressed;
    inflate_strm.avail_out = size * 2;
    
    int inflate_result = inflate(&inflate_strm, fuzz_int32_t_3);
    
    // 7. Get inflate codes used
    unsigned long codes_used = inflateCodesUsed(&inflate_strm);

    // 8. Create a file from the original data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        free(decompressed);
        inflateEnd(&inflate_strm);
        deflateEnd(&deflate_strm);
        free(compressed);
        assert_file_closed(&in_file);
	return 0;
    }

    // 9. Open a gzFile for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        free(decompressed);
        inflateEnd(&inflate_strm);
        deflateEnd(&deflate_strm);
        free(compressed);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 10. Create gzFile from file descriptor
    int fd_out = fuzz_fileno(out_file);
    gzFile gz_out = gzdopen(fd_out, "wb");
    if (!gz_out) {
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        free(decompressed);
        inflateEnd(&inflate_strm);
        deflateEnd(&deflate_strm);
        free(compressed);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	return 0;
    }

    // 11. Set gzFile parameters
    int param_result = gzsetparams(gz_out, fuzz_int32_t_4, fuzz_int32_t_5);

    // 12. Write a test string to gzFile
    const char *test_string = "Test string for gzputs\n";
    int puts_result = gzputs(gz_out, fuzz_str_6);

    // 13. Write some of the decompressed data to gzFile
    if (inflate_result == Z_STREAM_END || inflate_result == Z_OK) {
        size_t decompressed_size = size * 2 - inflate_strm.avail_out;
        if (decompressed_size > 0) {
            // Write in chunks
            size_t chunk_size = 1024;
            for (size_t i = 0; i < decompressed_size; i += chunk_size) {
                size_t write_size = (i + chunk_size) > decompressed_size ? 
                                   decompressed_size - i : chunk_size;
                gzwrite(gz_out, decompressed + i, write_size);
            }
        }
    }

    // 14. Close gzFile for writing
    int close_w_result = gzclose_w(gz_out);
    
    // Note: gzclose_w already closed the underlying file descriptor
    // So we don't need to assert_file_closed(&out_file); separately

    // 15. Reopen the output file for reading as gzFile
    FILE *reopen_file = fopen("output_file", "rb");
    if (!reopen_file) {
        assert_file_closed(&in_file);;
        free(decompressed);
        inflateEnd(&inflate_strm);
        deflateEnd(&deflate_strm);
        free(compressed);
        assert_file_closed(&reopen_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	return 0;
    }
    
    int fd_in = fuzz_fileno(reopen_file);
    gzFile gz_in = gzdopen(fd_in, fuzz_str_7);
    if (!gz_in) {
        assert_file_closed(&reopen_file);;
        assert_file_closed(&in_file);;
        free(decompressed);
        inflateEnd(&inflate_strm);
        deflateEnd(&deflate_strm);
        free(compressed);
        assert_file_closed(&reopen_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	assert_fd_closed(fd_out);
	return 0;
    }

    // 16. Test gzgetc_
    int getc_result = gzgetc_(gz_in);

    // 17. Test gzrewind
    int rewind_result = gzrewind(gz_in);

    // 18. Read and compute CRC32 of what we read
    Bytef read_buffer[4096];
    uLong crc = crc32_z(fuzz_uint64_t_8, Z_NULL, 0);
    
    int bytes_read;
    while ((bytes_read = gzread(gz_in, read_buffer, sizeof(read_buffer))) > 0) {
        crc = crc32_z(crc, read_buffer, bytes_read);
    }

    // 19. Close gzFile for reading
    int close_result = gzclose(gz_in);
    // gzclose already closed the underlying file descriptor

    // 20. Clean up all resources
    assert_file_closed(&in_file);;
    free(decompressed);
    inflateEnd(&inflate_strm);
    deflateEnd(&deflate_strm);
    free(compressed);

    // 21. Remove the temporary output file
    remove("output_file");

    assert_file_closed(&reopen_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_in);
	assert_fd_closed(fd_out);
	return 0;
}