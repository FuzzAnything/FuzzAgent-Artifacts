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
//<ID> 72
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["zlibCompileFlags","inflateInit_","compressBound","inflateEnd","inflate","inflateMark","inflateReset","inflateEnd","adler32","crc32","adler32","crc32","adler32","crc32","adler32_combine","crc32_combine","compressBound","compress2","deflateInit_","deflateBound","deflateBound_z","deflate","deflateParams","deflateResetKeep","deflateEnd","deflateEnd","gzdopen","gzsetparams","gzwrite","gztell64","gzclose","inflateBackInit_","inflateBackEnd"],"critical_calls":["zlibCompileFlags","inflateInit_","compressBound","inflate","inflateMark","inflateReset","inflateEnd","adler32","crc32","adler32","crc32","adler32","crc32","adler32_combine","crc32_combine","compressBound","compress2","deflateInit_","deflateBound","deflateBound_z","deflate","deflateParams","deflateResetKeep","deflateEnd","gzdopen","gzsetparams","gzwrite","gztell64","gzclose","inflateBackInit_","inflateBackEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
        return 0; // Too small for meaningful testing
    }

    // 1. Check zlib compile flags
    uLong compile_flags = zlibCompileFlags();

    // 2. First decompress input (assuming it's zlib-compressed data)
    z_stream decompress_stream = {};
    decompress_stream.next_in = (Bytef *)data;
    decompress_stream.avail_in = (uInt)(size > UINT_MAX ? UINT_MAX : size);
    
    // Initialize decompression
    if (inflateInit_(&decompress_stream, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        return 0;
    }

    // Allocate output buffer for decompression
    uLong decompressed_bound = compressBound((uLong)size);
    Bytef *decompressed_data = (Bytef *)malloc(decompressed_bound);
    if (!decompressed_data) {
        inflateEnd(&decompress_stream);
        return 0;
    }
    
    decompress_stream.next_out = decompressed_data;
    decompress_stream.avail_out = (uInt)decompressed_bound;
    
    // Decompress
    int decompress_result = inflate(&decompress_stream, Z_FINISH);
    
    // Get decompressed size
    uLong decompressed_size = decompress_stream.total_out;
    
    // Use inflateMark and inflateReset if decompression succeeded partially
    if (decompress_result == Z_BUF_ERROR || decompress_result == Z_DATA_ERROR) {
        long mark_position = inflateMark(&decompress_stream);
        if (mark_position >= 0) {
            inflateReset(&decompress_stream);
        }
    }
    
    // End decompression
    inflateEnd(&decompress_stream);

    // 3. Compute checksums on decompressed data
    uLong adler1 = adler32(0L, Z_NULL, 0);
    uLong crc1 = crc32(0L, Z_NULL, 0);
    
    if (decompressed_size > 0) {
        // Split data into two parts for combining checksums
        uInt half_len = (uInt)(decompressed_size / 2);
        if (half_len > 0) {
            adler1 = adler32(adler1, decompressed_data, half_len);
            crc1 = crc32(crc1, decompressed_data, half_len);
            
            uLong adler2 = adler32(0L, decompressed_data + half_len, 
                                  (uInt)(decompressed_size - half_len));
            uLong crc2 = crc32(0L, decompressed_data + half_len, 
                              (uInt)(decompressed_size - half_len));
            
            // Combine checksums
            uLong combined_adler = adler32_combine(adler1, adler2, (off_t)(decompressed_size - half_len));
            uLong combined_crc = crc32_combine(crc1, crc2, (off_t)(decompressed_size - half_len));
            (void)combined_adler; // Use to avoid unused warning
            (void)combined_crc;   // Use to avoid unused warning
        }
    }

    // 4. Recompress the decompressed data
    if (decompressed_size > 0) {
        // First using compress2 (one-shot compression)
        uLongf compressed_size1 = compressBound(decompressed_size);
        Bytef *compressed_data1 = (Bytef *)malloc(compressed_size1);
        if (compressed_data1) {
            compress2(compressed_data1, &compressed_size1, 
                     decompressed_data, (uLong)decompressed_size, 
                     Z_DEFAULT_COMPRESSION);
            free(compressed_data1);
        }

        // Second using streaming compression
        z_stream compress_stream = {};
        if (deflateInit_(&compress_stream, Z_DEFAULT_COMPRESSION, 
                        ZLIB_VERSION, sizeof(z_stream)) == Z_OK) {
            
            // Get deflate bound using both versions
            uLong bound1 = deflateBound(&compress_stream, (uLong)decompressed_size);
            z_size_t bound2 = deflateBound_z(&compress_stream, (z_size_t)decompressed_size);
            (void)bound1; // Use to avoid unused warning
            (void)bound2; // Use to avoid unused warning

            Bytef *compressed_data2 = (Bytef *)malloc(bound1);
            if (compressed_data2) {
                compress_stream.next_in = decompressed_data;
                compress_stream.avail_in = (uInt)(decompressed_size > UINT_MAX ? UINT_MAX : decompressed_size);
                compress_stream.next_out = compressed_data2;
                compress_stream.avail_out = (uInt)bound1;
                
                // Compress with deflate
                deflate(&compress_stream, Z_FINISH);
                
                // Change parameters during compression
                deflateParams(&compress_stream, Z_BEST_COMPRESSION, Z_DEFAULT_STRATEGY);
                
                // Reset keeping history
                deflateResetKeep(&compress_stream);
                
                // End compression
                deflateEnd(&compress_stream);
                
                free(compressed_data2);
            } else {
                deflateEnd(&compress_stream);
            }
        }
    }

    // 5. Test gzFile APIs
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Get file descriptor
        int fd = fuzz_fileno(in_file);
        (void)fd;
	int fuzz_fd_0 = fuzz_fileno(in_file);
 // Use to avoid unused warning
        
        // Open gzFile for reading (though we'll write to it)
        gzFile gz = gzdopen(fuzz_fd_0, "wb");
        if (gz) {
            // Set compression parameters
            gzsetparams(gz, Z_BEST_SPEED, Z_DEFAULT_STRATEGY);
            
            // Write decompressed data if available
            if (decompressed_size > 0) {
                gzwrite(gz, decompressed_data, 
                       (unsigned int)(decompressed_size > UINT_MAX ? UINT_MAX : decompressed_size));
            }
            
            // Get position
            off64_t pos = gztell64(gz);
            (void)pos; // Use to avoid unused warning
            
            // Close gzFile
            gzclose(gz);
        }
        assert_file_closed(&in_file);;
    }

    // 6. Test inflateBackEnd (requires inflateBackInit first)
    z_stream back_stream = {};
    unsigned char window[32768]; // 32K window
    if (inflateBackInit_(&back_stream, MAX_WBITS, window, 
                        ZLIB_VERSION, sizeof(z_stream)) == Z_OK) {
        // Note: We don't actually use inflateBack as it requires callbacks
        // Just initialize and end it
        inflateBackEnd(&back_stream);
    }

    // 7. Cleanup
    if (decompressed_data) {
        free(decompressed_data);
    }

    assert_file_closed(&in_file);
	return 0;
}