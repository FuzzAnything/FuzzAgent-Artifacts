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
//<ID> 112
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["gzdopen","gzdopen","gzclose","gzbuffer","gzbuffer","inflateInit2_","zError","gzclose","gzclose","deflateInit2_","zError","inflateEnd","gzclose","gzclose","inflateBackInit_","zError","inflateGetHeader","inflate","zError","gzfwrite","gzputc","deflate","zError","deflatePending","deflateUsed","gzread","gzrewind","deflate","deflateGetDictionary","compressBound","compress2","inflateEnd","deflateEnd","inflateBackEnd","gzclose","gzclose","inflateBackEnd"],"critical_calls":["gzdopen","gzdopen","gzbuffer","gzbuffer","inflateInit2_","deflateInit2_","inflateBackInit_","zError","inflateGetHeader","inflate","zError","gzfwrite","gzputc","deflate","zError","deflatePending","deflateUsed","gzread","gzrewind","deflate","deflateGetDictionary","compressBound","compress2","inflateEnd","deflateEnd","inflateBackEnd","gzclose","gzclose","inflateBackEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
	if(f_size<93) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create input file from the fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file for compressed data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    
    // 1. Open compressed input stream
    gzFile gz_in = gzdopen(fd_in, fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 2. Open output stream for recompressed data
    gzFile gz_out = gzdopen(fd_out, fuzz_str_2);
    if (!gz_out) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 3. Set buffer size for gz streams
    gzbuffer(gz_in, fuzz_uint32_t_3);
    gzbuffer(gz_out, fuzz_uint32_t_4);
    
    // 4. Initialize inflate stream for decompression
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    
    int windowBits = 15 + 32; // Enable gzip decoding
    int ret = inflateInit2_(&inflate_strm, windowBits, fuzz_str_5, sizeof(z_stream));
    if (ret != Z_OK) {
        const char* err_msg = zError(ret);
        gzclose(gz_out);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 5. Initialize deflate stream for recompression
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    
    int level = data[0] % 10; // Use first byte to determine compression level
    int method = Z_DEFLATED;
    int memLevel = 8;
    int strategy = Z_DEFAULT_STRATEGY;
    
    ret = deflateInit2_(&deflate_strm, level, fuzz_int32_t_7, windowBits, fuzz_int32_t_8, 
                       fuzz_int32_t_9, fuzz_str_6, sizeof(z_stream));
    if (ret != Z_OK) {
        const char* err_msg = zError(ret);
        inflateEnd(&inflate_strm);
        gzclose(gz_out);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 6. Initialize inflate back stream (though we won't fully use it)
    unsigned char back_window[32768];
    z_stream back_strm;
    memset(&back_strm, 0, sizeof(back_strm));
    
    ret = inflateBackInit_(&back_strm, 15, back_window, fuzz_str_10, sizeof(z_stream));
    if (ret != Z_OK) {
        const char* err_msg = zError(ret);
    }
    
    // 7. Try to get gzip header during inflation
    gz_header gzheader;
    memset(&gzheader, 0, sizeof(gzheader));
    inflateGetHeader(&inflate_strm, &gzheader);
    
    // 8. Read decompressed data using gzread
    Bytef read_buffer[4096];
    unsigned int bytes_read = 0;
    
    // 9. Decompress and recompress loop
    Bytef decompress_buffer[8192];
    Bytef compress_buffer[8192];
    
    inflate_strm.next_in = (Bytef*)data;
    inflate_strm.avail_in = size > 8192 ? 8192 : size;
    size_t data_offset = inflate_strm.avail_in;
    
    do {
        // Decompress using inflate
        inflate_strm.next_out = decompress_buffer;
        inflate_strm.avail_out = sizeof(decompress_buffer);
        
        ret = inflate(&inflate_strm, fuzz_int32_t_11);
        
        if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR) {
            const char* err_msg = zError(ret);
            break;
        }
        
        // Get decompressed bytes
        uInt decompressed_bytes = sizeof(decompress_buffer) - inflate_strm.avail_out;
        
        if (decompressed_bytes > 0) {
            // Write to gz output using gzfwrite
            gzfwrite(decompress_buffer, 1, decompressed_bytes, gz_out);
            
            // Also write single byte using gzputc
            if (decompressed_bytes > 0) {
                gzputc(gz_out, decompress_buffer[0]);
            }
            
            // Recompress using deflate
            deflate_strm.next_in = decompress_buffer;
            deflate_strm.avail_in = decompressed_bytes;
            
            do {
                deflate_strm.next_out = compress_buffer;
                deflate_strm.avail_out = sizeof(compress_buffer);
                
                ret = deflate(&deflate_strm, fuzz_int32_t_12);
                if (ret != Z_OK && ret != Z_BUF_ERROR) {
                    const char* err_msg = zError(ret);
                    break;
                }
                
                uInt compressed_bytes = sizeof(compress_buffer) - deflate_strm.avail_out;
                
                // Check deflate state
                unsigned int pending;
                int bits;
                deflatePending(&deflate_strm, &pending, &bits);
                deflateUsed(&deflate_strm, &bits);
                
            } while (deflate_strm.avail_out == 0);
        }
        
        // Read more using gzread
        if (gz_in) {
            bytes_read = gzread(gz_in, read_buffer, sizeof(read_buffer));
            if (bytes_read > 0) {
                // Use gzrewind to reset position (though not meaningful with sequential read)
                gzrewind(gz_in);
            }
        }
        
        // Feed more input to inflate if needed
        if (inflate_strm.avail_in == 0 && data_offset < size) {
            size_t remaining = size - data_offset;
            size_t to_copy = remaining > 8192 ? 8192 : remaining;
            
            inflate_strm.next_in = (Bytef*)(data + data_offset);
            inflate_strm.avail_in = to_copy;
            data_offset += to_copy;
        }
        
    } while (ret != Z_STREAM_END && inflate_strm.avail_in > 0);
    
    // 10. Finish deflation
    deflate_strm.next_in = NULL;
    deflate_strm.avail_in = 0;
    
    do {
        deflate_strm.next_out = compress_buffer;
        deflate_strm.avail_out = sizeof(compress_buffer);
        
        ret = deflate(&deflate_strm, fuzz_int32_t_13);
        
        uInt compressed_bytes = sizeof(compress_buffer) - deflate_strm.avail_out;
        
    } while (ret == Z_OK);
    
    // 11. Try to get dictionary
    Bytef dictionary[32768];
    uInt dictLength = 0;
    deflateGetDictionary(&deflate_strm, dictionary, &dictLength);
    
    // 12. Calculate compression bound
    uLong bound = compressBound(size);
    
    // 13. Use compress2 on original data
    uLongf destLen = bound;
    Bytef* compressed_data = (Bytef*)malloc(bound);
    if (compressed_data) {
        compress2(compressed_data, &destLen, data, size > 65536 ? 65536 : size, level);
        free(compressed_data);
    }
    
    // 14. Clean up
    inflateEnd(&inflate_strm);
    deflateEnd(&deflate_strm);
    inflateBackEnd(&back_strm);
    
    gzclose(gz_out);
    gzclose(gz_in);
    inflateBackEnd(&back_strm);  // Called again as required
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove temporary output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}