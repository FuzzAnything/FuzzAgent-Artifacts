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
//<ID> 94
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":7,"unique_branches":{"uncompress2_z":[[42,22,42,36,0,0,4,0]]},"library_calls":["gzopen64","uncompress2","gzclose","uncompress2","deflateInit_","gzclose","deflateReset","inflateInit2_","deflateEnd","gzclose","inflateReset2","gzread","gzeof","gzopen64","gzwrite","gzflush","gzclose","gzclose","inflateEnd","deflateEnd"],"critical_calls":["gzopen64","uncompress2","uncompress2","deflateInit_","deflateReset","inflateInit2_","inflateReset2","gzread","gzeof","gzopen64","gzwrite","gzflush","gzclose","gzclose","inflateEnd","deflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<14) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    // 1. Create a temporary file with the input data (zlib output data)
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Open the file as a gzFile for reading using gzopen64
    int fd = fuzz_fileno(in_file);
    gzFile gz_in = gzopen64("/dev/fd/0", fuzz_str_1);  // Using /dev/fd/0 to read from the file descriptor
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 3. Prepare decompression stream for uncompress2
    Bytef *dest = nullptr;
    uLongf destLen = 0;
    uLong sourceLen = size;
    
    // First call to get required buffer size
    int uncompress_result = uncompress2(dest, &destLen, data, &sourceLen);
    if (uncompress_result == Z_BUF_ERROR) {
        // Allocate buffer based on destLen (which was set by uncompress2)
        dest = (Bytef *)malloc(destLen);
        if (!dest) {
            gzclose(gz_in);
            assert_file_closed(&in_file);;
            assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
        }
        
        // Reset sourceLen for the actual decompression
        sourceLen = size;
        uncompress_result = uncompress2(dest, &destLen, data, &sourceLen);
    }
    
    // 4. Set up z_stream for deflate (compression)
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(z_stream));
    deflate_strm.zalloc = Z_NULL;
    deflate_strm.zfree = Z_NULL;
    deflate_strm.opaque = Z_NULL;
    
    // Initialize deflate
    if (deflateInit(&deflate_strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        free(dest);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Call deflateReset at least once
    deflateReset(&deflate_strm);
    
    // 5. Set up z_stream for inflate (decompression)
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(z_stream));
    inflate_strm.zalloc = Z_NULL;
    inflate_strm.zfree = Z_NULL;
    inflate_strm.opaque = Z_NULL;
    inflate_strm.next_in = (Bytef *)data;
    inflate_strm.avail_in = size;
    
    // Initialize inflate with windowBits
    int windowBits = 15 + 32;  // Add 32 for automatic header detection
    if (inflateInit2(&inflate_strm, windowBits) != Z_OK) {
        deflateEnd(&deflate_strm);
        free(dest);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Call inflateReset2 at least once
    inflateReset2(&inflate_strm, windowBits);
    
    // 6. Read from gzFile and check for EOF using gzeof
    Bytef read_buffer[1024];
    int bytes_read;
    while ((bytes_read = gzread(gz_in, read_buffer, sizeof(read_buffer))) > 0) {
        // Process or ignore the read data
        // We're just ensuring gzread is called
    }
    
    // Check if we reached EOF
    int eof_check = gzeof(gz_in);
    (void)eof_check;  // Use the result to avoid unused variable warning
    
    // 7. Create output file and gzFile for writing
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        int out_fd = fuzz_fileno(out_file);
        gzFile gz_out = gzopen64("/dev/fd/1", fuzz_str_2);  // Using /dev/fd/1 for writing
        
        if (gz_out) {
            // Write some data to the gzFile
            if (dest && destLen > 0) {
                gzwrite(gz_out, dest, destLen);
            }
            
            // Call gzflush at least once
            gzflush(gz_out, fuzz_int32_t_3);
            
            gzclose(gz_out);
        }
        assert_file_closed(&out_file);;
    }
    
    // 8. Clean up all resources
    if (dest) {
        free(dest);
    }
    
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    inflateEnd(&inflate_strm);
    deflateEnd(&deflate_strm);
    
    // Remove the temporary output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}