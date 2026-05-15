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
//<ID> 98
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["compressBound","compress2","gzdopen","inflateInit2_","gzclose","deflateInit_","inflateEnd","gzclose","inflateGetHeader","inflatePrime","inflateValidate","gzread","gzflush","deflateUsed","uncompress_z","deflateEnd","inflateEnd","gzclose"],"critical_calls":["compressBound","compress2","gzdopen","inflateInit2_","deflateInit_","inflateGetHeader","inflatePrime","inflateValidate","gzread","gzflush","deflateUsed","uncompress_z","deflateEnd","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if we don't have enough data for meaningful operations
    if (size < 100) {
	return 0;
	}
	

    // ==================== PHASE 1: Prepare data buffers ====================
    // Allocate buffers for compressed data
    uLongf compressed_len = compressBound(size);
    Bytef* compressed = (Bytef*)malloc(compressed_len);
    if (!compressed) {
	return 0;
	}
	

    Bytef* uncompressed = (Bytef*)malloc(size * 2);
    if (!uncompressed) {
        free(compressed);
        return 0;
    }

    // ==================== PHASE 2: compress2 API ====================
    // First compress the input data using compress2
    uLongf dest_len = compressed_len;
    int level = data[0] % 10;  // Use first byte for compression level
    int compress_result = compress2(compressed, &dest_len, data, size, level);
    if (compress_result != Z_OK) {
        free(compressed);
        free(uncompressed);
        return 0;
    }

    // ==================== PHASE 3: Create FILE* handles ====================
    // Create FILE* for reading compressed data
    FILE* in_file = fmemopen(compressed, dest_len, "rb");
    if (!in_file) {
        free(compressed);
        free(uncompressed);
        assert_file_closed(&in_file);
	return 0;
    }

    // Create FILE* for writing uncompressed data
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        free(compressed);
        free(uncompressed);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // ==================== PHASE 4: Setup gzip stream ====================
    // Create gzFile from file descriptor
    int fd = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        free(compressed);
        free(uncompressed);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // ==================== PHASE 5: Setup zlib streams ====================
    z_stream inflate_stream;
    z_stream deflate_stream;
    memset(&inflate_stream, 0, sizeof(z_stream));
    memset(&deflate_stream, 0, sizeof(z_stream));

    // Initialize inflate stream with gzip header support
    if (inflateInit2(&inflate_stream, 16 + MAX_WBITS) != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&out_file);;
        free(compressed);
        free(uncompressed);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Initialize deflate stream
    if (deflateInit(&deflate_stream, Z_DEFAULT_COMPRESSION) != Z_OK) {
        inflateEnd(&inflate_stream);
        gzclose(gz_in);
        assert_file_closed(&out_file);;
        free(compressed);
        free(uncompressed);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // ==================== PHASE 6: inflateGetHeader API ====================
    gz_header header;
    memset(&header, 0, sizeof(gz_header));
    inflate_stream.avail_in = 0;
    inflateGetHeader(&inflate_stream, &header);

    // ==================== PHASE 7: inflatePrime API ====================
    // Prime the inflate stream with some bits from the input
    if (dest_len > 10) {
        inflatePrime(&inflate_stream, 8, compressed[0]);
    }

    // ==================== PHASE 8: inflateValidate API ====================
    // Set validation based on input data
    int validate_flag = (data[1] % 2) ? 1 : 0;
    inflateValidate(&inflate_stream, validate_flag);

    // ==================== PHASE 9: Process data through gzFile ====================
    // Read from gzFile
    char gz_buffer[1024];
    int bytes_read;
    while ((bytes_read = gzread(gz_in, gz_buffer, sizeof(gz_buffer))) > 0) {
        // Write to output file
        fwrite(gz_buffer, 1, bytes_read, out_file);
        
        // Use gzflush API periodically
        if (bytes_read > 512) {
            gzflush(gz_in, Z_SYNC_FLUSH);
        }
    }

    // ==================== PHASE 10: deflateUsed API ====================
    int bits_used;
    deflateUsed(&deflate_stream, &bits_used);

    // ==================== PHASE 11: uncompress_z API ====================
    // Use the high-level uncompress_z function
    z_size_t uncompressed_len = size * 2;
    z_size_t source_len = dest_len;
    int uncompress_result = uncompress_z(uncompressed, &uncompressed_len, 
                                         compressed, source_len);
    // Result ignored intentionally for fuzzing

    // ==================== PHASE 12: Cleanup ====================
    // Clean up zlib streams
    deflateEnd(&deflate_stream);
    inflateEnd(&inflate_stream);
    
    // Clean up gzFile
    gzclose(gz_in);
    
    // Clean up FILE pointers
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Free allocated memory
    free(compressed);
    free(uncompressed);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}