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
//<ID> 38
//<Prompt> []
/*<Combination>: [
*/
//<score> 5.25, nr_unique_branch: 0
//<Quality> {"density":21,"unique_branches":{},"library_calls":["inflateInit2_","zError","inflateEnd","inflate","zError","inflateSetDictionary","inflateUndermine","inflateGetDictionary","inflateEnd","crc32","crc32","deflateInit2_","deflateSetDictionary","deflateGetDictionary","deflateBound_z","compressBound","deflateEnd","deflate","deflatePending","deflateResetKeep","deflateEnd","gzdopen","gzclose"],"critical_calls":["inflateInit2_","inflate","zError","inflateSetDictionary","inflateUndermine","inflateGetDictionary","inflateEnd","crc32","crc32","deflateInit2_","deflateSetDictionary","deflateGetDictionary","deflateBound_z","compressBound","deflate","deflatePending","deflateResetKeep","deflateEnd","gzdopen","gzclose"],"visited":3}
/**/


extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
        // Too small for meaningful testing
        return 0;
    }

    // 1. First, try to decompress the input data (assuming it's zlib-compressed)
    z_stream d_stream;
    memset(&d_stream, 0, sizeof(d_stream));

    // Initialize decompression with windowBits for automatic header detection
    int ret = inflateInit2_(&d_stream, 15 + 32, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        const char* error_msg = zError(ret);
        (void)error_msg; // Use error message
        return 0;
    }

    // Allocate buffer for decompressed data
    size_t decompressed_size = size * 4; // Estimate
    Bytef* decompressed = (Bytef*)malloc(decompressed_size);
    if (!decompressed) {
        inflateEnd(&d_stream);
        return 0;
    }

    d_stream.next_in = (Bytef*)data;
    d_stream.avail_in = (uInt)size;
    d_stream.next_out = decompressed;
    d_stream.avail_out = (uInt)decompressed_size;

    // Try to decompress
    ret = inflate(&d_stream, Z_FINISH);
    const char* decompress_error = zError(ret);
    (void)decompress_error; // Use error message

    // Try to set dictionary during decompression (use part of input as dictionary)
    if (size > 50) {
        uInt dict_len = (uInt)(size / 4);
        if (dict_len > 32768) dict_len = 32768; // Reasonable limit
        inflateSetDictionary(&d_stream, data + 10, dict_len);
    }

    // Test inflateUndermine - should only be called after inflateInit2
    // This is a testing function that may cause errors
    inflateUndermine(&d_stream, 1);

    // Get dictionary used (if any)
    uInt dict_length = 0;
    Bytef dict_buffer[32768];
    inflateGetDictionary(&d_stream, dict_buffer, &dict_length);

    // End decompression
    inflateEnd(&d_stream);

    // 2. Compute CRC32 of the decompressed data (or input if decompression failed)
    uLong crc = crc32(0L, Z_NULL, 0);
    size_t data_to_crc = d_stream.total_out > 0 ? d_stream.total_out : size;
    Bytef* crc_data = d_stream.total_out > 0 ? decompressed : (Bytef*)data;
    crc = crc32(crc, crc_data, (uInt)data_to_crc);

    // 3. Now re-compress the data
    z_stream c_stream;
    memset(&c_stream, 0, sizeof(c_stream));

    // Initialize compression with specific parameters
    ret = deflateInit2_(&c_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                        15 + 16, // windowBits with gzip header
                        8, Z_DEFAULT_STRATEGY,
                        ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        free(decompressed);
        return 0;
    }

    // Set compression dictionary (use part of original input)
    if (size > 30) {
        uInt comp_dict_len = (uInt)(size / 3);
        if (comp_dict_len > 32768) comp_dict_len = 32768;
        deflateSetDictionary(&c_stream, data + 5, comp_dict_len);
    }

    // Get dictionary used
    uInt comp_dict_length = 0;
    Bytef comp_dict_buffer[32768];
    deflateGetDictionary(&c_stream, comp_dict_buffer, &comp_dict_length);

    // Use deflateBound_z and compressBound to estimate output size
    z_size_t bound_z = deflateBound_z(&c_stream, (z_size_t)data_to_crc);
    uLong bound = compressBound((uLong)data_to_crc);

    // Allocate compression buffer
    size_t compressed_buf_size = (size_t)(bound_z > bound ? bound_z : bound);
    Bytef* compressed = (Bytef*)malloc(compressed_buf_size);
    if (!compressed) {
        deflateEnd(&c_stream);
        free(decompressed);
        return 0;
    }

    c_stream.next_in = crc_data;
    c_stream.avail_in = (uInt)data_to_crc;
    c_stream.next_out = compressed;
    c_stream.avail_out = (uInt)compressed_buf_size;

    // Compress the data
    ret = deflate(&c_stream, Z_FINISH);

    // Check for pending data
    unsigned int pending = 0;
    int bits = 0;
    deflatePending(&c_stream, &pending, &bits);

    // Reset compression stream while keeping allocation
    deflateResetKeep(&c_stream);

    // End compression
    deflateEnd(&c_stream);

    // 4. Use gzFile operations with file descriptors
    // Create a temporary file with compressed data
    FILE* tmp_out = fopen("output_file", "wb");
    if (tmp_out) {
        fwrite(compressed, 1, c_stream.total_out, tmp_out);
        assert_file_closed(&tmp_out);;
    }

    // Re-open the file for reading
    FILE* in_file = fopen("output_file", "rb");
    if (in_file) {
        // Get file descriptor
        int fd = fuzz_fileno(in_file);
        
        // Open as gzFile
        gzFile gz = gzdopen(fd, "rb");
        if (gz) {
            // We would normally read from gz here, but gzread is not in the required list
            // Just open and close to use the API
            gzclose(gz);
        } else {
            // If gzdopen failed, we still need to close the file
            assert_file_closed(&in_file);;
        }
    }

    // 5. Clean up
    free(decompressed);
    free(compressed);
    
    // Remove temporary file
    remove("output_file");

    assert_file_closed(&in_file);
	assert_file_closed(&tmp_out);
	assert_file_name_closed("output_file");
	return 0;
}