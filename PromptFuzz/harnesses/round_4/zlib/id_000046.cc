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
//<ID> 95
//<Prompt> []
/*<Combination>: [
*/
//<score> 216, nr_unique_branch: 22
//<Quality> {"density":27,"unique_branches":{"gzclearerr":[[538,9,538,31,0,0,4,1],[542,9,542,31,0,0,4,0]],"inflateBack":[[219,12,219,26,0,0,4,0],[281,21,281,32,0,0,4,1],[424,30,424,41,0,0,4,1],[516,49,516,76,0,0,4,1],[528,21,528,32,0,0,4,0],[536,21,536,41,0,0,4,1],[542,22,542,40,0,0,4,0],[153,13,153,22,51,0,4,0],[157,17,157,41,51,0,4,1],[153,13,153,22,64,0,4,0],[157,17,157,41,64,0,4,1]],"compress2_z":[[61,14,61,25,0,0,4,0],[65,12,65,31,0,0,4,1]],"adler32_z":[[90,13,90,26,0,0,4,0]],"inflate_fast":[[217,33,217,44,0,0,4,0],[222,42,222,46,0,0,4,0],[222,42,222,46,0,0,4,1]],"inflate":[[700,9,700,18,0,0,4,0],[1111,9,1111,18,0,0,4,0]],"deflate":[[1256,17,1256,37,0,0,4,0]]},"library_calls":["gzdopen","gzdopen","gzclose","gzdirect","inflateInit_","gzclose_w","gzclose","deflateInit_","inflateEnd","gzclose_w","gzclose","inflateBackInit_","deflateEnd","inflateEnd","gzclose_w","gzclose","deflateBound","compressBound","inflateValidate","inflate","deflate","deflateUsed","gzfwrite","gzflush","compress2_z","crc32","crc32","crc32","crc32","crc32_combine64","inflateBack","inflateReset","deflateReset","gzclearerr","inflateBackEnd","inflateEnd","deflateEnd","gzclose_w","gzclose"],"critical_calls":["gzdopen","gzdopen","gzdirect","inflateInit_","deflateInit_","inflateBackInit_","deflateBound","compressBound","inflateValidate","inflate","deflate","deflateUsed","gzfwrite","gzflush","compress2_z","crc32","crc32","crc32","crc32","crc32_combine64","inflateBack","inflateReset","deflateReset","gzclearerr","inflateBackEnd","inflateEnd","deflateEnd","gzclose_w","gzclose"],"visited":0}
/*Here's a comprehensive fuzz driver that uses all the specified zlib APIs to achieve a complete compression/decompression cycle with various features:

*/


// Callback functions for inflateBack
static unsigned int  id_000046_back_in(void* desc, unsigned char** buf) {
    // Return 0 to indicate no more input
    return 0;
}

static int  id_000046_back_out(void* desc, unsigned char* buf, unsigned int len) {
    // Just discard the output for fuzzing
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* f_data, size_t f_size) {
	if(f_size<96) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need enough data for meaningful operations

    // 1. Create input file from the data
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
    
    // 3. Open gzFile for reading
    int fd_in = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd_in, fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 4. Open gzFile for writing
    int fd_out = fuzz_fileno(out_file);
    gzFile gz_out = gzdopen(fd_out, "wb");
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
    
    // 5. Check if file is direct
    int direct = gzdirect(gz_in);
    
    // 6. Initialize streams
    z_stream strm_inflate, strm_deflate, strm_inflate_back;
    memset(&strm_inflate, 0, sizeof(z_stream));
    memset(&strm_deflate, 0, sizeof(z_stream));
    memset(&strm_inflate_back, 0, sizeof(z_stream));
    
    // 7. Initialize inflate stream
    int ret = inflateInit_(&strm_inflate, fuzz_str_2, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose_w(gz_out);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 8. Initialize deflate stream
    ret = deflateInit_(&strm_deflate, fuzz_int32_t_4, fuzz_str_3, sizeof(z_stream));
    if (ret != Z_OK) {
        inflateEnd(&strm_inflate);
        gzclose_w(gz_out);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 9. Initialize inflateBack stream
    unsigned char window[32768];
    ret = inflateBackInit_(&strm_inflate_back, MAX_WBITS, window, fuzz_str_5, sizeof(z_stream));
    if (ret != Z_OK) {
        deflateEnd(&strm_deflate);
        inflateEnd(&strm_inflate);
        gzclose_w(gz_out);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 10. Set up input buffers
    size_t chunk_size = size / 4;
    if (chunk_size < 1) chunk_size = 1;
    
    // 11. Process data in chunks
    size_t processed = 0;
    while (processed < size && processed < 1024 * 1024) {  // Limit processing
        size_t current_chunk = (size - processed > chunk_size) ? chunk_size : size - processed;
        
        // Allocate buffers
        uLong bound = deflateBound(&strm_deflate, current_chunk);
        uLong compress_bound = compressBound(current_chunk);
        
        Bytef* in_buf = (Bytef*)malloc(current_chunk);
        Bytef* out_buf = (Bytef*)malloc(bound);
        Bytef* compress_buf = (Bytef*)malloc(compress_bound);
        
        if (!in_buf || !out_buf || !compress_buf) {
            free(in_buf);
            free(out_buf);
            free(compress_buf);
            break;
        }
        
        // Read data
        memcpy(in_buf, data + processed, current_chunk);
        
        // 12. Try to inflate (decompress) the data
        strm_inflate.next_in = in_buf;
        strm_inflate.avail_in = current_chunk;
        strm_inflate.next_out = out_buf;
        strm_inflate.avail_out = bound;
        
        // Enable validation
        inflateValidate(&strm_inflate, fuzz_int32_t_6);
        
        int flush = (processed + current_chunk >= size) ? Z_FINISH : Z_NO_FLUSH;
        ret = inflate(&strm_inflate, flush);
        
        // 13. If inflation succeeded, try to deflate (compress) the output
        if (ret == Z_OK || ret == Z_STREAM_END || ret == Z_BUF_ERROR) {
            size_t inflated_size = bound - strm_inflate.avail_out;
            
            if (inflated_size > 0) {
                // 14. Deflate the inflated data
                strm_deflate.next_in = out_buf;
                strm_deflate.avail_in = inflated_size;
                strm_deflate.next_out = compress_buf;
                strm_deflate.avail_out = compress_bound;
                
                ret = deflate(&strm_deflate, flush);
                
                // 15. Check deflate usage
                int bits_used = 0;
                deflateUsed(&strm_deflate, &bits_used);
                
                // 16. Write to gzFile
                z_size_t written = gzfwrite(out_buf, 1, inflated_size, gz_out);
                
                // 17. Flush gzFile
                gzflush(gz_out, fuzz_int32_t_7);
                
                // 18. Also compress using compress2_z
                z_size_t dest_len = compress_bound;
                if (inflated_size > 0) {
                    compress2_z(compress_buf, &dest_len, out_buf, inflated_size, fuzz_int32_t_8);
                }
                
                // 19. Compute CRC32
                uLong crc = crc32(fuzz_uint64_t_9, Z_NULL, 0);
                crc = crc32(crc, out_buf, inflated_size < 65536 ? inflated_size : 65536);
                
                // 20. Combine CRC32 values (using dummy values for demonstration)
                uLong crc1 = crc32(fuzz_uint64_t_10, in_buf, current_chunk < 100 ? current_chunk : 100);
                uLong crc2 = crc32(fuzz_uint64_t_11, in_buf + 100, current_chunk > 200 ? 100 : 0);
                if (current_chunk > 200) {
                    uLong combined = crc32_combine64(crc1, crc2, 100);
                    (void)combined;  // Prevent unused variable warning
                }
            }
        }
        
        // 21. Try inflateBack with the same data
        strm_inflate_back.next_in = in_buf;
        strm_inflate_back.avail_in = current_chunk;
        ret = inflateBack(&strm_inflate_back,  id_000046_back_in, NULL,  id_000046_back_out, NULL);
        
        free(in_buf);
        free(out_buf);
        free(compress_buf);
        
        processed += current_chunk;
        
        // Reset streams for next chunk if needed
        if (ret == Z_STREAM_END) {
            inflateReset(&strm_inflate);
            deflateReset(&strm_deflate);
        }
    }
    
    // 22. Clear errors on gzFile
    gzclearerr(gz_in);
    
    // 23. Clean up
    inflateBackEnd(&strm_inflate_back);
    inflateEnd(&strm_inflate);
    deflateEnd(&strm_deflate);
    gzclose_w(gz_out);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}