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
//<ID> 205
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 1
//<Quality> {"density":8,"unique_branches":{"gzputs":[[371,12,371,15,0,0,4,1]]},"library_calls":["gzdopen","gzclose","gzclose","gzclose","gzdopen","gzclose","gzputs","gzflush","compressBound","gzclose","gzclose","compress","deflateInit_","gzclose","gzclose","deflate","deflatePending","deflateEnd","crc32","crc32","crc32","crc32","crc32_combine","gzclose","gzclose"],"critical_calls":["gzdopen","gzdopen","gzputs","gzflush","compressBound","compress","deflateInit_","deflate","deflatePending","deflateEnd","crc32","crc32","crc32","crc32","crc32_combine","gzclose","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_76(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Ensure minimum input size for meaningful operations
    if (size < 10) {
        return 0;
    }

    // 1. Create a FILE* for input using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Get file descriptor and open as gzFile for reading
    int fd_in = fuzz_fileno(in_file);
    if (fd_in < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    gzFile gz_in = gzdopen(fd_in, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 3. Read compressed data using gzgetc (decompress on-the-fly)
    size_t read_buffer_size = size * 2;  // Estimate decompressed size
    Bytef *read_buffer = (Bytef *)malloc(read_buffer_size);
    if (!read_buffer) {
        gzclose(gz_in);  // This closes fd_in too
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    int bytes_read = 0;
    int ch;
    while ((ch = gzgetc(gz_in)) != EOF && bytes_read < read_buffer_size - 1) {
        read_buffer[bytes_read++] = (Bytef)ch;
    }

    // 4. Create FILE* for output
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        free(read_buffer);
        gzclose(gz_in);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 5. Get file descriptor and open as gzFile for writing
    int fd_out = fuzz_fileno(out_file);
    if (fd_out < 0) {
        assert_file_closed(&out_file);;
        free(read_buffer);
        gzclose(gz_in);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    gzFile gz_out = gzdopen(fd_out, "wb");
    if (!gz_out) {
        assert_file_closed(&out_file);;
        free(read_buffer);
        gzclose(gz_in);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 6. Write data using gzputs (compresses on-the-fly)
    // Convert first part of read_buffer to null-terminated string for gzputs
    size_t str_len = bytes_read > 100 ? 100 : bytes_read;
    read_buffer[str_len] = '\0';
    if (gzputs(gz_out, (const char *)read_buffer) < 0) {
        // Handle write error
    }

    // 7. Flush the gzFile
    if (gzflush(gz_out, Z_SYNC_FLUSH) != Z_OK) {
        // Handle flush error
    }

    // 8. Prepare second buffer for compression using compress()
    // Use middle section of input data as source
    size_t compress_src_len = size > 100 ? 100 : size;
    const Bytef *compress_src = data + (size - compress_src_len) / 2;
    
    uLongf destLen = compressBound((uLong)compress_src_len);
    Bytef *compress_dest = (Bytef *)malloc(destLen);
    if (!compress_dest) {
        gzclose(gz_out);
        free(read_buffer);
        gzclose(gz_in);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 9. Compress data
    if (compress(compress_dest, &destLen, compress_src, (uLong)compress_src_len) != Z_OK) {
        // Handle compression error
    }

    // 10. Prepare deflate stream for deflatePending
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        free(compress_dest);
        gzclose(gz_out);
        free(read_buffer);
        gzclose(gz_in);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Feed some data to deflate stream
    strm.avail_in = compress_src_len;
    strm.next_in = (Bytef *)compress_src;
    
    Bytef deflate_out[100];
    strm.avail_out = sizeof(deflate_out);
    strm.next_out = deflate_out;
    
    deflate(&strm, Z_FINISH);

    // 11. Use deflatePending
    unsigned int pending;
    int bits;
    if (deflatePending(&strm, &pending, &bits) == Z_OK) {
        // Got pending data information
    }

    deflateEnd(&strm);

    // 12. Combine CRC32 values (simulating combination of two data segments)
    uLong crc1 = crc32(0L, Z_NULL, 0);
    uLong crc2 = crc32(0L, Z_NULL, 0);
    
    // Calculate CRC for two segments of the input
    size_t segment1_len = size / 2;
    size_t segment2_len = size - segment1_len;
    
    if (segment1_len > 0) {
        crc1 = crc32(crc1, data, (uInt)segment1_len);
    }
    if (segment2_len > 0) {
        crc2 = crc32(crc2, data + segment1_len, (uInt)segment2_len);
    }
    
    // Combine the two CRC values
    off_t total_len = (off_t)size;
    uLong combined_crc = crc32_combine(crc1, crc2, total_len);

    // 13. Cleanup
    free(compress_dest);
    gzclose(gz_out);  // This closes fd_out too
    free(read_buffer);
    gzclose(gz_in);   // This closes fd_in too
    
    // Prevent unused variable warnings
    (void)combined_crc;
    (void)bytes_read;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}