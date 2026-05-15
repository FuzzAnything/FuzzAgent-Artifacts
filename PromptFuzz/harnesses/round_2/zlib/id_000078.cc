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
//<ID> 260
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 1
//<Quality> {"density":12,"unique_branches":{"inflateSync":[[1272,9,1272,32,0,0,4,0]]},"library_calls":["gzdopen","gzdopen","gzclose_r","gzclose_w","gzdirect","gztell","gzseek","gzclearerr","gzseek","gzclose_r","gzclose_w","uncompress","deflateInit_","deflateSetDictionary","deflateBound","deflate","deflateUsed","deflateEnd","inflateBackInit_","inflateReset","inflateSync","inflateReset2","inflateBackEnd","adler32","adler32","adler32","adler32","adler32_combine64","gzclose_r","gzclose_w"],"critical_calls":["gzdopen","gzdopen","gzdirect","gztell","gzseek","gzclearerr","gzseek","uncompress","deflateInit_","deflateSetDictionary","deflateBound","deflate","deflateUsed","deflateEnd","inflateBackInit_","inflateReset","inflateSync","inflateReset2","inflateBackEnd","adler32","adler32","adler32","adler32","adler32_combine64","gzclose_r","gzclose_w"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_78(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create input and output file handles using fmemopen/fopen
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
    
    // 2. Use gzdopen to create a gzFile from file descriptor
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    gzFile gz_in = gzdopen(fd_in, "rb");
    gzFile gz_out = gzdopen(fd_out, "wb");
    if (!gz_in || !gz_out) {
        if (gz_in) gzclose_r(gz_in);
        if (gz_out) gzclose_w(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 3. Check if gzFile is in direct mode
    int is_direct = gzdirect(gz_in);
    
    // 4. Get current position in gzFile
    off_t pos = gztell(gz_in);
    
    // 5. Try to seek in the gzFile (to beginning)
    if (pos != 0) {
        gzseek(gz_in, 0, SEEK_SET);
    }
    
    // 6. Clear any errors on gzFile
    gzclearerr(gz_in);
    
    // 7. Read one character from gzFile
    int first_char = gzgetc(gz_in);
    
    // 8. Seek back to beginning after reading
    gzseek(gz_in, 0, SEEK_SET);
    
    // 9. Prepare for uncompress operation
    // Estimate decompressed size (heuristic: 10x compressed size)
    uLongf destLen = size * 10;
    Bytef *dest = (Bytef*)malloc(destLen);
    if (!dest) {
        gzclose_r(gz_in);
        gzclose_w(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 10. Try to uncompress the input data
    int uncompress_result = uncompress(dest, &destLen, data, size);
    
    // 11. Initialize deflate stream
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(z_stream));
    int deflate_init_result = deflateInit_(&deflate_strm, Z_DEFAULT_COMPRESSION, 
                                           ZLIB_VERSION, sizeof(z_stream));
    
    if (deflate_init_result == Z_OK) {
        // 12. Set dictionary if we have decompressed data
        if (uncompress_result == Z_OK && destLen > 0) {
            uInt dict_len = destLen < 32768 ? (uInt)destLen : 32768;
            deflateSetDictionary(&deflate_strm, dest, dict_len);
        }
        
        // 13. Prepare source data for deflate (use decompressed data or original)
        Bytef* src_data;
        uLong src_len;
        if (uncompress_result == Z_OK && destLen > 0) {
            src_data = dest;
            src_len = destLen;
        } else {
            src_data = (Bytef*)data;
            src_len = size;
        }
        
        // 14. Perform deflate operation
        deflate_strm.avail_in = src_len;
        deflate_strm.next_in = src_data;
        
        // Create output buffer for deflate
        uLong comprLen = deflateBound(&deflate_strm, src_len);
        Bytef* compr = (Bytef*)malloc(comprLen);
        if (compr) {
            deflate_strm.avail_out = comprLen;
            deflate_strm.next_out = compr;
            
            int deflate_result = deflate(&deflate_strm, Z_FINISH);
            
            // 15. Check deflate used bits
            int used_bits = 0;
            deflateUsed(&deflate_strm, &used_bits);
            
            free(compr);
        }
        
        // 16. End deflate stream
        deflateEnd(&deflate_strm);
    }
    
    // 17. Initialize inflate stream for back decompression
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(z_stream));
    
    // Allocate window for inflateBack
    unsigned char* window = (unsigned char*)malloc(32768);
    if (window) {
        int back_init_result = inflateBackInit_(&inflate_strm, 15, window, 
                                                ZLIB_VERSION, sizeof(z_stream));
        
        if (back_init_result == Z_OK) {
            // 18. Reset inflate stream
            inflateReset(&inflate_strm);
            
            // 19. Try to synchronize inflate stream
            inflateSync(&inflate_strm);
            
            // 20. Reset inflate with different window bits
            inflateReset2(&inflate_strm, 15 + 16); // +16 for gzip
            
            // 21. End inflate back stream
            inflateBackEnd(&inflate_strm);
        }
        
        free(window);
    }
    
    // 22. Compute Adler-32 checksum of the input data
    uLong adler1 = adler32(0L, Z_NULL, 0);
    if (size > 0) {
        uInt chunk_size = size < 1024 ? (uInt)size : 1024;
        adler1 = adler32(adler1, data, chunk_size);
    }
    
    // 23. Compute another Adler-32 for second half (if exists)
    uLong adler2 = adler32(0L, Z_NULL, 0);
    if (size > 1) {
        size_t second_half_start = size / 2;
        uInt second_half_len = (uInt)(size - second_half_start);
        if (second_half_len > 0) {
            adler2 = adler32(adler2, data + second_half_start, second_half_len);
        }
    }
    
    // 24. Combine the two Adler-32 checksums
    off64_t len2 = size > 1 ? size / 2 : 0;
    uLong combined_adler = adler32_combine64(adler1, adler2, len2);
    
    // Cleanup
    free(dest);
    gzclose_r(gz_in);
    gzclose_w(gz_out);
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