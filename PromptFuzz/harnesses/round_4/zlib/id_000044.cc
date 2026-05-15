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
//<ID> 88
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 1
//<Quality> {"density":5,"unique_branches":{"uncompress2_z":[[42,22,42,36,0,0,4,1]]},"library_calls":["uncompress2_z","uncompress2_z","crc32_z","compressBound","compress","inflateInit_","inflate","inflateReset","inflateEnd","gzdopen","gzread","gztell","gzclose"],"critical_calls":["uncompress2_z","uncompress2_z","crc32_z","compressBound","compress","inflateInit_","inflate","inflateReset","inflateEnd","gzdopen","gzread","gztell","gzclose"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified zlib APIs to achieve the event of decompressing, validating, and recompressing data:

*/


extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
	if(f_size<17) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
	//fuzzer shim end}




    // Early return for small inputs
    if (size < 10) {
	return 0;
	}
	
    
    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file for compressed data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // 1. First, try to decompress the input data using uncompress2_z
    // The input should be zlib-compressed data
    z_size_t sourceLen = size;
    z_size_t destLen = 0;
    Bytef *uncompressed_data = nullptr;
    
    // First call to get required buffer size
    int uncompress_result = uncompress2_z(nullptr, &destLen, data, &sourceLen);
    if (uncompress_result == Z_BUF_ERROR || destLen > 0) {
        // Allocate buffer for uncompressed data
        uncompressed_data = new Bytef[destLen];
        if (uncompressed_data) {
            // Reset source length for second call
            sourceLen = size;
            z_size_t actualDestLen = destLen;
            uncompress_result = uncompress2_z(uncompressed_data, &actualDestLen, data, &sourceLen);
            
            if (uncompress_result == Z_OK) {
                // 2. Calculate CRC32 of uncompressed data using crc32_z
                uLong crc_value = crc32_z(fuzz_uint64_t_1, uncompressed_data, actualDestLen);
                
                // 3. Recompress the data using compress
                uLongf compressedLen = compressBound(actualDestLen);
                Bytef *recompressed_data = new Bytef[compressedLen];
                if (recompressed_data) {
                    int compress_result = compress(recompressed_data, &compressedLen, 
                                                  uncompressed_data, actualDestLen);
                    
                    if (compress_result == Z_OK) {
                        // Write compressed data to output file
                        fwrite(recompressed_data, 1, compressedLen, out_file);
                        fflush(out_file);
                        
                        // 4. Use inflateReset with a z_stream for inflation
                        z_stream strm;
                        memset(&strm, 0, sizeof(strm));
                        strm.zalloc = Z_NULL;
                        strm.zfree = Z_NULL;
                        strm.opaque = Z_NULL;
                        
                        // Initialize for inflation
                        if (inflateInit(&strm) == Z_OK) {
                            // Set up input for inflation
                            strm.next_in = recompressed_data;
                            strm.avail_in = compressedLen;
                            
                            // Allocate output buffer for inflation
                            Bytef *inflate_out = new Bytef[actualDestLen];
                            if (inflate_out) {
                                strm.next_out = inflate_out;
                                strm.avail_out = actualDestLen;
                                
                                // Inflate some data
                                int inflate_result = inflate(&strm, fuzz_int32_t_2);
                                
                                if (inflate_result == Z_OK || inflate_result == Z_STREAM_END) {
                                    // Use inflateReset to reset the stream state
                                    inflateReset(&strm);
                                }
                                
                                delete[] inflate_out;
                            }
                            
                            inflateEnd(&strm);
                        }
                        
                        // 5. Use gztell with a gzFile
                        // Create a gzFile from the output file
                        fseek(out_file, 0, SEEK_SET);
                        gzFile gz_out = gzdopen(out_fd, fuzz_str_3);
                        if (gz_out) {
                            // Read a small amount to move the position
                            char buf[10];
                            gzread(gz_out, buf, sizeof(buf));
                            
                            // Use gztell to get current position
                            off_t position = gztell(gz_out);
                            
                            gzclose(gz_out);
                        } else {
                            // Reopen the file descriptor since gzdopen might have consumed it
                            out_file = fdopen(out_fd, "wb");
                        }
                    }
                    
                    delete[] recompressed_data;
                }
            }
            
            delete[] uncompressed_data;
        }
    }
    
    // Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}