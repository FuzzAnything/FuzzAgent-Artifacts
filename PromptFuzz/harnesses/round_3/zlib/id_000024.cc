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
//<ID> 46
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 7
//<Quality> {"density":5,"unique_branches":{"gzputs":[[355,9,355,21,0,0,4,1],[360,9,360,32,0,0,4,1],[360,37,360,55,0,0,4,1],[366,9,366,21,0,0,4,1],[366,25,366,45,0,0,4,1],[371,12,371,15,0,0,4,0],[371,19,371,27,0,0,4,1]]},"library_calls":["zlibVersion","gzdopen","gzsetparams","gzgetc_","gzdopen","gzputs","gzclose","deflateInit_","deflateUsed","deflateCopy","inflateInit_","inflate","inflateCodesUsed","inflateEnd","deflateEnd","deflateEnd","crc32_z","compressBound_z","compress_z","gzclose"],"critical_calls":["zlibVersion","gzdopen","gzsetparams","gzgetc_","gzdopen","gzputs","gzclose","deflateInit_","deflateUsed","deflateCopy","inflateInit_","inflate","inflateCodesUsed","inflateEnd","deflateEnd","deflateEnd","crc32_z","compressBound_z","compress_z","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<56) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
	//fuzzer shim end}




    // 1. First, get the zlib version
    const char *version = zlibVersion();
    
    // 2. Create a FILE* from the input data for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 3. Get file descriptor and open as gzFile
    int fd = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd, fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 4. Set compression parameters on the gzFile (though it's read-only, will likely fail)
    // We call it anyway to satisfy the requirement
    gzsetparams(gz_in, fuzz_int32_t_2, fuzz_int32_t_3);
    
    // 5. Read some data using gzgetc_
    int ch = gzgetc_(gz_in);
    
    // 6. Write a test string using gzputs (to a different file)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        int out_fd = fuzz_fileno(out_file);
        gzFile gz_out = gzdopen(out_fd, fuzz_str_4);
        if (gz_out) {
            gzputs(gz_out, fuzz_str_5);
            gzclose(gz_out);
        }
        assert_file_closed(&out_file);;
    }
    
    // 7. Create z_stream structures for deflate operations
    z_stream strm1, strm2;
    memset(&strm1, 0, sizeof(z_stream));
    memset(&strm2, 0, sizeof(z_stream));
    
    // 8. Initialize deflate stream
    if (deflateInit(&strm1, Z_DEFAULT_COMPRESSION) == Z_OK) {
        // 9. Get deflate used bits
        int bits_used = 0;
        deflateUsed(&strm1, &bits_used);
        
        // 10. Copy deflate stream
        if (deflateCopy(&strm2, &strm1) == Z_OK) {
            // 11. Get inflate codes used (need an inflate stream)
            z_stream inflate_strm;
            memset(&inflate_strm, 0, sizeof(z_stream));
            
            if (inflateInit(&inflate_strm) == Z_OK) {
                // Set up input for inflate
                inflate_strm.next_in = (Bytef*)data;
                inflate_strm.avail_in = (uInt)(size > UINT_MAX ? UINT_MAX : size);
                
                // Try to inflate a little
                Bytef out_buf[1024];
                inflate_strm.next_out = out_buf;
                inflate_strm.avail_out = sizeof(out_buf);
                inflate(&inflate_strm, fuzz_int32_t_6);
                
                // Get inflate codes used
                unsigned long codes_used = inflateCodesUsed(&inflate_strm);
                
                inflateEnd(&inflate_strm);
            }
            
            deflateEnd(&strm2);
        }
        deflateEnd(&strm1);
    }
    
    // 12. Compute CRC32 of input data
    uLong crc = crc32_z(fuzz_uint64_t_7, data, size);
    
    // 13. Use compress_z to compress the input data
    z_size_t destLen = compressBound_z(size);
    if (destLen > 0) {
        Bytef* dest = (Bytef*)malloc(destLen);
        if (dest) {
            compress_z(dest, &destLen, data, size);
            free(dest);
        }
    }
    
    // 14. Clean up resources
    gzclose(gz_in);
    // Note: gzclose will also close the underlying file descriptor
    // so we don't need to assert_file_closed(&in_file); separately
    
    // Remove the output file we created
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}