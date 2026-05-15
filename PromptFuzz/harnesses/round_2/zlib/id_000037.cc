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
//<ID> 75
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 4
//<Quality> {"density":24,"unique_branches":{"deflateBound_z":[[917,28,917,49,0,0,4,0],[918,17,918,42,0,0,4,0],[918,46,918,54,0,0,4,0],[920,16,920,39,0,0,4,1]]},"library_calls":["zlibCompileFlags","gzdopen","gzclose","gzclose","gzdopen","gzclose","gzsetparams","inflateInit_","gzclose_w","gzclose","deflateInit2_","inflateEnd","gzclose_w","gzclose","deflateInit_","gzread","crc32","crc32_combine","gzwrite","gztell64","gzclose","gzclose_w","deflateEnd","deflateEnd","inflateEnd","deflateEnd","deflateEnd","inflateEnd","inflate","inflateMark","inflateSyncPoint","inflateReset","compressBound","compressBound_z","deflateEnd","deflateEnd","inflateEnd","compress2","compress_z","deflateBound","deflateBound_z","deflate","deflateParams","deflateResetKeep","deflateEnd","deflateEnd","inflateEnd","inflateBackEnd"],"critical_calls":["zlibCompileFlags","gzdopen","gzdopen","gzsetparams","inflateInit_","deflateInit2_","deflateInit_","gzread","crc32","crc32_combine","gzwrite","gztell64","gzclose","gzclose_w","inflate","inflateMark","inflateSyncPoint","inflateReset","compressBound","compressBound_z","compress2","compress_z","deflateBound","deflateBound_z","deflate","deflateParams","deflateResetKeep","deflateEnd","deflateEnd","inflateEnd","inflateBackEnd"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if no data
    if (size == 0) {
        return 0;
    }

    // Get zlib compile flags first
    uLong compile_flags = zlibCompileFlags();

    // Open memory file for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Open gzFile for reading
    gzFile gz_in = gzdopen(fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        gzclose(gz_in);  // This will close the fd and in_file
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    int out_fd = fuzz_fileno(out_file);
    if (out_fd < 0) {
        assert_file_closed(&out_file);;
        gzclose(gz_in);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
    }

    // Open gzFile for writing
    gzFile gz_out = gzdopen(out_fd, "wb");
    if (!gz_out) {
        assert_file_closed(&out_file);;
        gzclose(gz_in);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
    }

    // Set parameters for gz output
    gzsetparams(gz_out, Z_DEFAULT_COMPRESSION, Z_DEFAULT_STRATEGY);

    // Initialize z_stream structures for decompression and compression
    z_stream d_stream, c_stream;
    memset(&d_stream, 0, sizeof(z_stream));
    memset(&c_stream, 0, sizeof(z_stream));

    // Initialize decompression stream
    if (inflateInit_(&d_stream, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        gzclose_w(gz_out);  // Close writer
        gzclose(gz_in);     // Close reader
        assert_file_closed(&out_file);;   // Already closed by gzdopen, but safe
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
    }

    // Initialize compression stream with specific parameters
    if (deflateInit2_(&c_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                     MAX_WBITS, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY,
                     ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        inflateEnd(&d_stream);
        gzclose_w(gz_out);
        gzclose(gz_in);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
    }

    // Also initialize with deflateInit_ for comparison
    z_stream c_stream2;
    memset(&c_stream2, 0, sizeof(z_stream));
    deflateInit_(&c_stream2, Z_BEST_COMPRESSION, ZLIB_VERSION, sizeof(z_stream));

    // Read decompressed data from gz_in and write to gz_out
    Bytef buffer[4096];
    uLong total_crc = 0;
    uLong partial_crc1 = 0, partial_crc2 = 0;
    off_t combined_offset = 0;

    while (true) {
        int bytes_read = gzread(gz_in, buffer, sizeof(buffer));
        if (bytes_read <= 0) {
            break;
        }

        // Update CRC32 for the read chunk
        uLong chunk_crc = crc32(0L, buffer, bytes_read);
        total_crc = crc32_combine(total_crc, chunk_crc, combined_offset);
        combined_offset += bytes_read;

        // Split CRC for demonstration of crc32_combine
        if (combined_offset == bytes_read) {
            partial_crc1 = chunk_crc;
        } else if (combined_offset == 2 * bytes_read) {
            partial_crc2 = chunk_crc;
        }

        // Write to gz_out
        gzwrite(gz_out, buffer, bytes_read);
    }

    // Get position in gz_out
    off64_t gz_pos = gztell64(gz_out);

    // Close gz files
    gzclose(gz_in);
    gzclose_w(gz_out);  // Use specific writer close

    // Now test compression and decompression using z_stream APIs
    // Reset input position by reopening
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        deflateEnd(&c_stream);
        deflateEnd(&c_stream2);
        inflateEnd(&d_stream);
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
    }

    // Read all input data
    Bytef *input_data = (Bytef *)malloc(size);
    if (!input_data) {
        assert_file_closed(&in_file);;
        deflateEnd(&c_stream);
        deflateEnd(&c_stream2);
        inflateEnd(&d_stream);
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
    }
    fread(input_data, 1, size, in_file);
    assert_file_closed(&in_file);;

    // Set up decompression stream with input data
    d_stream.next_in = input_data;
    d_stream.avail_in = size;

    // Create decompression output buffer
    Bytef decompressed[8192];
    d_stream.next_out = decompressed;
    d_stream.avail_out = sizeof(decompressed);

    // Perform decompression
    int inflate_result = inflate(&d_stream, Z_FINISH);

    // Mark position for potential reset
    long mark = inflateMark(&d_stream);

    // Check sync point
    int is_sync_point = inflateSyncPoint(&d_stream);

    // Reset decompression stream
    inflateReset(&d_stream);

    // Now compress the decompressed data
    uLongf compressed_size = compressBound(d_stream.total_out);
    z_size_t compressed_size_z = compressBound_z(d_stream.total_out);

    Bytef *compressed = (Bytef *)malloc(compressed_size);
    if (!compressed) {
        free(input_data);
        deflateEnd(&c_stream);
        deflateEnd(&c_stream2);
        inflateEnd(&d_stream);
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
    }

    // Use compress2
    uLongf destLen = compressed_size;
    compress2(compressed, &destLen, decompressed, d_stream.total_out, Z_BEST_SPEED);

    // Also use compress_z
    z_size_t destLen_z = compressed_size_z;
    Bytef *compressed2 = (Bytef *)malloc(compressed_size_z);
    if (compressed2) {
        compress_z(compressed2, &destLen_z, decompressed, d_stream.total_out);
        free(compressed2);
    }

    // Test deflate functions
    c_stream.next_in = decompressed;
    c_stream.avail_in = d_stream.total_out;
    c_stream.next_out = compressed;
    c_stream.avail_out = compressed_size;

    // Get deflate bound
    uLong def_bound = deflateBound(&c_stream, d_stream.total_out);
    z_size_t def_bound_z = deflateBound_z(&c_stream, d_stream.total_out);

    // Perform compression with deflate
    deflate(&c_stream, Z_FINISH);

    // Change parameters during compression
    deflateParams(&c_stream, Z_BEST_COMPRESSION, Z_HUFFMAN_ONLY);

    // Reset but keep settings
    deflateResetKeep(&c_stream);

    // Clean up deflate streams
    deflateEnd(&c_stream);
    deflateEnd(&c_stream2);

    // Clean up inflate stream
    inflateEnd(&d_stream);

    // Call inflateBackEnd (even though we didn't use inflateBack)
    // We need to initialize a stream for it
    z_stream back_stream;
    memset(&back_stream, 0, sizeof(z_stream));
    // Note: inflateBackEnd requires stream to be initialized with inflateBackInit,
    // but we call it anyway for coverage
    inflateBackEnd(&back_stream);

    // Clean up
    free(input_data);
    free(compressed);

    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
}