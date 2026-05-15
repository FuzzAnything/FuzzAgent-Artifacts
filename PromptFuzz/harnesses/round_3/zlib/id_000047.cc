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
//<ID> 93
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"gzflush":[[612,9,612,32,0,0,4,0]]},"library_calls":["gzopen64","gzopen64","gzflush","inflateInit2_","gzclose","inflateEnd","gzclose","inflateEnd","gzclose","inflateReset2","inflate","deflateInit_","inflateEnd","gzclose","deflateReset","deflateBound","deflateEnd","inflateEnd","gzclose","deflateBound","deflate","uncompress2","gzread","gzeof","deflateEnd","inflateEnd","gzclose"],"critical_calls":["gzopen64","gzopen64","gzflush","inflateInit2_","inflateReset2","inflate","deflateInit_","deflateReset","deflateBound","deflateBound","deflate","uncompress2","gzread","gzeof","deflateEnd","inflateEnd","gzclose"],"visited":1}
/*Here's a C++ fuzz driver that uses all specified zlib APIs to handle compressed data:

*/


extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* f_data, size_t f_size) {
	if(f_size<22) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 1024) {
        return 0;
    }

    // Create temporary files for reading/writing
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

    // Get file descriptors
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);

    // 1. Open gzFile for reading using gzopen64
    gzFile gz_in = gzopen64("input_file", fuzz_str_1);
    if (!gz_in) {
        // Create input file with compressed data
        FILE *tmp_in = fopen("input_file", "wb");
        if (tmp_in) {
            fwrite(data, 1, size, tmp_in);
            assert_file_closed(&tmp_in);;
        }
        gz_in = gzopen64("input_file", fuzz_str_2);
    }
    
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 2. Use gzflush during reading
    gzflush(gz_in, fuzz_int32_t_3);

    // 3. Create z_stream structures for inflation
    z_stream strm_inflate;
    memset(&strm_inflate, 0, sizeof(z_stream));
    
    // 4. Initialize inflate stream
    if (inflateInit2(&strm_inflate, 15 + 16) != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 5. Set up input for inflation
    size_t compress_size = size / 2;
    Bytef *compressed_data = (Bytef *)malloc(compress_size);
    if (!compressed_data) {
        inflateEnd(&strm_inflate);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    memcpy(compressed_data, data, compress_size);

    // 6. Prepare output buffer for inflation
    uLongf decompressed_size = size * 4;
    Bytef *decompressed_data = (Bytef *)malloc(decompressed_size);
    if (!decompressed_data) {
        free(compressed_data);
        inflateEnd(&strm_inflate);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 7. Use inflateReset2 to reset the stream
    inflateReset2(&strm_inflate, 15 + 32); // Use gzip window bits

    // 8. Inflate the data
    strm_inflate.next_in = compressed_data;
    strm_inflate.avail_in = compress_size;
    strm_inflate.next_out = decompressed_data;
    strm_inflate.avail_out = decompressed_size;
    
    int inflate_result = inflate(&strm_inflate, fuzz_int32_t_4);

    // 9. Create deflate stream
    z_stream strm_deflate;
    memset(&strm_deflate, 0, sizeof(z_stream));
    
    // 10. Initialize deflate stream
    if (deflateInit(&strm_deflate, Z_DEFAULT_COMPRESSION) != Z_OK) {
        free(decompressed_data);
        free(compressed_data);
        inflateEnd(&strm_inflate);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 11. Use deflateReset
    deflateReset(&strm_deflate);

    // 12. Prepare for compression
    Bytef *recompressed_data = (Bytef *)malloc(deflateBound(&strm_deflate, strm_inflate.total_out));
    if (!recompressed_data) {
        deflateEnd(&strm_deflate);
        free(decompressed_data);
        free(compressed_data);
        inflateEnd(&strm_inflate);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 13. Compress the inflated data
    strm_deflate.next_in = decompressed_data;
    strm_deflate.avail_in = strm_inflate.total_out;
    strm_deflate.next_out = recompressed_data;
    strm_deflate.avail_out = deflateBound(&strm_deflate, strm_inflate.total_out);
    
    int deflate_result = deflate(&strm_deflate, fuzz_int32_t_5);

    // 14. Use uncompress2 on the original data
    uLongf uncompressed_size = size * 4;
    Bytef *uncompressed_data = (Bytef *)malloc(uncompressed_size);
    if (uncompressed_data) {
        uLong source_len = size;
        uncompress2(uncompressed_data, &uncompressed_size, data, &source_len);
    }

    // 15. Read from gzFile until EOF
    Bytef buffer[4096];
    int bytes_read;
    while ((bytes_read = gzread(gz_in, buffer, sizeof(buffer))) > 0) {
        // Process read data if needed
    }

    // 16. Check for EOF using gzeof
    if (gzeof(gz_in)) {
        // Reached end of file
    }

    // 17. Write compressed data to output file
    if (deflate_result == Z_STREAM_END) {
        fwrite(recompressed_data, 1, strm_deflate.total_out, out_file);
        fflush(out_file);
    }

    // Clean up all resources
    if (uncompressed_data) free(uncompressed_data);
    free(recompressed_data);
    deflateEnd(&strm_deflate);
    free(decompressed_data);
    free(compressed_data);
    inflateEnd(&strm_inflate);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    // Remove temporary files
    remove("input_file");
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}