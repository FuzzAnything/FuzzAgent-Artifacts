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
//<ID> 213
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 1
//<Quality> {"density":21,"unique_branches":{"uncompress2_z":[[36,31,36,45,0,0,4,1]]},"library_calls":["gzopen64","gzbuffer","gzsetparams","gzread","inflateInit_","inflateSync","deflateInit_","deflateBound_z","deflateParams","gzopen64","inflateEnd","deflateEnd","gzclose","gzfwrite","crc32","crc32","crc32","crc32","crc32_combine_gen64","crc32_combine_op","uncompress","deflateEnd","inflateEnd","gzclose","gzclose"],"critical_calls":["gzopen64","gzbuffer","gzsetparams","gzread","inflateInit_","inflateSync","deflateInit_","deflateBound_z","deflateParams","gzopen64","gzfwrite","crc32","crc32","crc32","crc32","crc32_combine_gen64","crc32_combine_op","uncompress","deflateEnd","inflateEnd","gzclose","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_80(const uint8_t* f_data, size_t f_size) {
	if(f_size<84) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_12, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // 1. Open a FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Get file descriptor for gzopen64
    int fd_in = fuzz_fileno(in_file);
    
    // 3. Open gzFile for reading compressed data
    gzFile gz_in = gzopen64("input_file", fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 4. Set buffer size for gz_in
    gzbuffer(gz_in, fuzz_uint32_t_2);
    
    // 5. Set compression parameters (though we're reading, this API must be called)
    gzsetparams(gz_in, fuzz_int32_t_3, fuzz_int32_t_4);
    
    // 6. Read compressed data from gzFile
    const size_t READ_BUF_SIZE = 4096;
    Bytef read_buf[READ_BUF_SIZE];
    int bytes_read = gzread(gz_in, read_buf, READ_BUF_SIZE);
    
    // 7. Initialize z_stream for inflation
    z_stream inflate_stream;
    memset(&inflate_stream, 0, sizeof(inflate_stream));
    inflateInit_(&inflate_stream, fuzz_str_5, sizeof(z_stream));
    
    // 8. Set up inflation with the read data
    inflate_stream.next_in = read_buf;
    inflate_stream.avail_in = bytes_read > 0 ? bytes_read : 0;
    
    // 9. Try to sync inflation stream
    inflateSync(&inflate_stream);
    
    // 10. Initialize z_stream for deflation
    z_stream deflate_stream;
    memset(&deflate_stream, 0, sizeof(deflate_stream));
    deflateInit_(&deflate_stream, fuzz_int32_t_7, fuzz_str_6, sizeof(z_stream));
    
    // 11. Calculate bound for deflated size
    z_size_t source_len = size;
    z_size_t deflate_bound = deflateBound_z(&deflate_stream, source_len);
    
    // 12. Set deflation parameters
    deflateParams(&deflate_stream, fuzz_int32_t_8, fuzz_int32_t_9);
    
    // 13. Open gzFile for writing
    gzFile gz_out = gzopen64("output_file", fuzz_str_10);
    if (!gz_out) {
        inflateEnd(&inflate_stream);
        deflateEnd(&deflate_stream);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 14. Write data using gzfwrite (convert to voidpc)
    z_size_t items_written = gzfwrite((voidpc)data, 1, size, gz_out);
    
    // 15. Compute CRC32 of original data
    uLong crc1 = crc32(fuzz_uint64_t_11, Z_NULL, 0);
    crc1 = crc32(crc1, data, size);
    
    // 16. Compute CRC32 of read data
    uLong crc2 = crc32(fuzz_uint64_t_12, Z_NULL, 0);
    crc2 = crc32(crc2, read_buf, bytes_read > 0 ? bytes_read : 0);
    
    // 17. Generate combine operation
    off64_t len2 = bytes_read;
    uLong op = crc32_combine_gen64(len2);
    
    // 18. Combine CRCs
    uLong combined_crc = crc32_combine_op(crc1, crc2, op);
    
    // 19. Try to uncompress (though data might be already compressed)
    Bytef *uncompressed_buf = (Bytef*)malloc(deflate_bound);
    uLongf uncompressed_len = deflate_bound;
    if (uncompressed_buf) {
        // Use the data read from gzFile as source
        int uncompress_result = uncompress(uncompressed_buf, &uncompressed_len, 
                                          read_buf, bytes_read > 0 ? bytes_read : 0);
        free(uncompressed_buf);
    }
    
    // 20. Clean up
    deflateEnd(&deflate_stream);
    inflateEnd(&inflate_stream);
    gzclose(gz_out);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    
    // Remove temporary files (optional but good practice)
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd_in);
	return 0;
}