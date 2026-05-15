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
//<ID> 104
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["gzdopen","inflateInit2_","gzclose","inflateGetDictionary","inflateValidate","gzread","inflate","inflateCodesUsed","inflateSync","gzungetc","deflateInit_","deflateSetDictionary","compress_z","compress2_z","crc32_z","deflateEnd","gzdopen","gzwrite","gzclose","inflateEnd","gzclose"],"critical_calls":["gzdopen","inflateInit2_","inflateGetDictionary","inflateValidate","gzread","inflate","inflateCodesUsed","inflateSync","gzungetc","deflateInit_","deflateSetDictionary","compress_z","compress2_z","crc32_z","deflateEnd","gzdopen","gzwrite","gzclose","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Create input FILE* from the fuzzing data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Get file descriptor for gzdopen
    int fd = fuzz_fileno(in_file);

    // Open gzFile for reading compressed data
    gzFile gz_in = gzdopen(fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Initialize inflation stream
    z_stream strm_inflate;
    memset(&strm_inflate, 0, sizeof(z_stream));
    
    // Use inflateInit2_ with negative windowBits for raw deflate data
    int ret = inflateInit2_(&strm_inflate, -MAX_WBITS, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Try to get dictionary from inflation stream
    Bytef dict_buffer[1024];
    uInt dict_len = sizeof(dict_buffer);
    inflateGetDictionary(&strm_inflate, dict_buffer, &dict_len);

    // Validate inflation stream
    inflateValidate(&strm_inflate, 1);

    // Create output buffer for decompressed data
    Bytef decompressed[4096];
    strm_inflate.next_out = decompressed;
    strm_inflate.avail_out = sizeof(decompressed);

    // Read some data from gzFile to inflate
    unsigned char gz_buffer[1024];
    int bytes_read = gzread(gz_in, gz_buffer, sizeof(gz_buffer));
    
    if (bytes_read > 0) {
        strm_inflate.next_in = gz_buffer;
        strm_inflate.avail_in = bytes_read;
        
        // Inflate the data
        ret = inflate(&strm_inflate, Z_NO_FLUSH);
        
        // Get codes used
        unsigned long codes_used = inflateCodesUsed(&strm_inflate);
        (void)codes_used; // Prevent unused variable warning
        
        // Try sync operation
        if (ret == Z_DATA_ERROR) {
            inflateSync(&strm_inflate);
        }
    }

    // Try to unget a character to gzFile
    if (bytes_read > 0) {
        gzungetc(gz_buffer[0], gz_in);
    }

    // Initialize deflation stream
    z_stream strm_deflate;
    memset(&strm_deflate, 0, sizeof(z_stream));
    
    ret = deflateInit_(&strm_deflate, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, sizeof(z_stream));
    if (ret == Z_OK) {
        // Set dictionary for deflation
        if (dict_len > 0) {
            deflateSetDictionary(&strm_deflate, dict_buffer, dict_len);
        }
        
        // Test compress_z and compress2_z with decompressed data
        Bytef compressed_z[4096];
        z_size_t dest_len_z = sizeof(compressed_z);
        
        if (strm_inflate.total_out > 0) {
            // Use compress_z
            compress_z(compressed_z, &dest_len_z, decompressed, strm_inflate.total_out);
            
            // Use compress2_z with different level
            z_size_t dest_len_z2 = sizeof(compressed_z);
            compress2_z(compressed_z, &dest_len_z2, decompressed, strm_inflate.total_out, Z_BEST_COMPRESSION);
        }
        
        // Calculate CRC32 of decompressed data
        uLong crc = crc32_z(0L, decompressed, strm_inflate.total_out);
        (void)crc; // Prevent unused variable warning
        
        // Clean up deflation stream
        deflateEnd(&strm_deflate);
    }

    // Create output FILE* for gzFile writing
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        int out_fd = fuzz_fileno(out_file);
        gzFile gz_out = gzdopen(out_fd, "wb");
        
        if (gz_out) {
            // Write decompressed data to gzFile
            if (strm_inflate.total_out > 0) {
                gzwrite(gz_out, decompressed, strm_inflate.total_out);
            }
            
            // Close gzFile
            gzclose(gz_out);
        }
        assert_file_closed(&out_file);;
    }

    // Clean up inflation stream
    inflateEnd(&strm_inflate);
    
    // Close gzFile and original FILE*
    gzclose(gz_in);
    assert_file_closed(&in_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}