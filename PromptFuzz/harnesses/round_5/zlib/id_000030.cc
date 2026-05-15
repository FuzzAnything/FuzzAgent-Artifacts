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
//<ID> 36
//<Prompt> []
/*<Combination>: [
*/
//<score> 16.5, nr_unique_branch: 2
//<Quality> {"density":11,"unique_branches":{"inflate":[[542,29,542,47,0,0,4,0]],"inflateInit2_":[[207,9,207,20,0,0,4,0]]},"library_calls":["adler32","uncompress2","uncompress2","adler32","gzopen","gzwrite","gzclose_w","inflateInit2_","inflate","inflateGetDictionary","adler32","inflateSyncPoint","inflateReset","inflateEnd","inflateInit_","inflateSetDictionary","inflate","inflateGetDictionary","inflateEnd"],"critical_calls":["adler32","uncompress2","uncompress2","adler32","gzopen","gzwrite","gzclose_w","inflateInit2_","inflate","inflateGetDictionary","adler32","inflateSyncPoint","inflateReset","inflateEnd","inflateInit_","inflateSetDictionary","inflate","inflateGetDictionary","inflateEnd"],"visited":1}
/*Here's a C++ fuzzing driver that uses all the specified zlib APIs to achieve a complete decompression workflow with dictionary handling and checksum verification:

*/


extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
        return 0;
    }

    // Extract parameters from fuzzing input
    size_t offset = 0;
    
    // Get dictionary size from first 2 bytes (max 64KB)
    uint16_t dict_size = 0;
    if (offset + sizeof(uint16_t) <= size) {
        memcpy(&dict_size, data + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);
        dict_size = dict_size % 65536; // Limit to reasonable size
    }
    
    // Get windowBits from next byte
    int windowBits = 15; // default
    if (offset + 1 <= size) {
        windowBits = (data[offset] % 16) + 8; // range 8-23
        offset++;
    }
    
    // Remaining data is the compressed input
    const Bytef* compressed_data = data + offset;
    size_t compressed_size = size - offset;
    
    // Initialize Adler-32 checksum
    uLong adler = adler32(0L, Z_NULL, 0);
    
    // Step 1: Try to decompress using uncompress2
    uLongf destLen = 0;
    uLong sourceLen = compressed_size;
    Bytef* dest_buffer = nullptr;
    int uncompress_result = Z_OK;
    
    // First call to get required buffer size
    int ret = uncompress2(nullptr, &destLen, compressed_data, &sourceLen);
    
    if (ret == Z_OK && destLen > 0) {
        dest_buffer = (Bytef*)malloc(destLen);
        if (dest_buffer) {
            sourceLen = compressed_size;
            uncompress_result = uncompress2(dest_buffer, &destLen, compressed_data, &sourceLen);
            
            if (uncompress_result == Z_OK) {
                // Calculate Adler-32 checksum of decompressed data
                adler = adler32(adler, dest_buffer, destLen);
            }
        }
    }
    
    // Step 2: Create a temporary gzip file for writing
    gzFile gz_out = gzopen("output_file.gz", "wb");
    if (gz_out) {
        if (dest_buffer && destLen > 0 && uncompress_result == Z_OK) {
            // Write decompressed data to gzip file
            gzwrite(gz_out, dest_buffer, destLen);
        }
        
        // Close the gzip file for writing
        gzclose_w(gz_out);
    }
    
    // Step 3: Inflate stream with dictionary support
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize inflate with windowBits
    if (inflateInit2(&strm, windowBits) == Z_OK) {
        strm.avail_in = compressed_size;
        strm.next_in = (Bytef*)compressed_data;
        
        // Allocate output buffer for inflation
        Bytef out_buffer[4096];
        strm.avail_out = sizeof(out_buffer);
        strm.next_out = out_buffer;
        
        // Perform inflation
        int inflate_ret = inflate(&strm, Z_NO_FLUSH);
        
        if (inflate_ret == Z_OK || inflate_ret == Z_STREAM_END) {
            // Try to get dictionary if one was used
            if (dict_size > 0) {
                Bytef* dictionary = (Bytef*)malloc(dict_size);
                uInt dictLength = dict_size;
                
                if (dictionary) {
                    inflateGetDictionary(&strm, dictionary, &dictLength);
                    
                    // Calculate Adler-32 of dictionary
                    if (dictLength > 0) {
                        adler = adler32(adler, dictionary, dictLength);
                    }
                    
                    free(dictionary);
                }
            }
            
            // Check if we're at a synchronization point
            if (inflateSyncPoint(&strm) == 1) {
                // Reset stream if at sync point
                inflateReset(&strm);
            }
        }
        
        inflateEnd(&strm);
    }
    
    // Step 4: Try alternate decompression with dictionary
    if (dict_size > 0 && dict_size <= compressed_size) {
        z_stream strm2;
        memset(&strm2, 0, sizeof(strm2));
        
        if (inflateInit(&strm2) == Z_OK) {
            // Set dictionary from compressed data
            inflateSetDictionary(&strm2, compressed_data, 
                                dict_size < compressed_size ? dict_size : (uInt)compressed_size);
            
            // Inflate with dictionary
            Bytef* dict_buffer = (Bytef*)malloc(4096);
            if (dict_buffer) {
                strm2.avail_in = compressed_size - dict_size;
                strm2.next_in = (Bytef*)compressed_data + dict_size;
                strm2.avail_out = 4096;
                strm2.next_out = dict_buffer;
                
                inflate(&strm2, Z_FINISH);
                free(dict_buffer);
            }
            
            // Get dictionary back
            if (dict_size > 0) {
                Bytef* dict_out = (Bytef*)malloc(dict_size);
                uInt dict_out_len = dict_size;
                
                if (dict_out) {
                    inflateGetDictionary(&strm2, dict_out, &dict_out_len);
                    free(dict_out);
                }
            }
            
            inflateEnd(&strm2);
        }
    }
    
    // Clean up
    if (dest_buffer) {
        free(dest_buffer);
    }
    
    // Open and close a file descriptor to demonstrate FILE* usage
    if (size > 0) {
        FILE* in_file = fmemopen((void *)data, size, "rb");
        if (in_file) {
            FILE* out_file = fopen("output_file", "wb");
            if (out_file) {
                // Use file descriptors if needed
                int in_fd = fuzz_fileno(in_file);
                int out_fd = fuzz_fileno(out_file);
                
                // Simple copy (just to use the file descriptors)
                uint8_t buffer[1024];
                size_t bytes_read;
                while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
                    fwrite(buffer, 1, bytes_read, out_file);
                }
                
                assert_file_closed(&out_file);;
            }
            assert_file_closed(&in_file);;
        }
    }
    
    // Remove temporary files
    remove("output_file.gz");
    remove("output_file");
    
    assert_file_name_closed("output_file");
	assert_file_name_closed("output_file.gz");
	assert_file_name_closed("output_file.gz");
	return 0;
}