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
//<ID> 86
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 10
//<Quality> {"density":6,"unique_branches":{"gzungetc":[[516,9,516,27,0,0,4,1],[520,9,520,27,0,0,4,0],[520,31,520,56,0,0,4,0],[520,31,520,56,0,0,4,1],[520,60,520,73,0,0,4,0],[533,9,533,27,0,0,4,1],[543,9,543,44,0,0,4,1],[549,9,549,36,0,0,4,1]],"inflateInit2_":[[183,9,183,38,0,0,4,1],[191,9,191,36,0,0,4,1]]},"library_calls":["compressBound","uncompress","compressBound","uncompress2","inflateBackInit_","inflateBackEnd","inflateInit2_","inflate","inflatePrime","inflateSetDictionary","inflateEnd","gzdopen","gzbuffer","gztell","gzungetc","gzdopen","gzprintf","gzclose","gzclose"],"critical_calls":["compressBound","uncompress","compressBound","uncompress2","inflateBackInit_","inflateBackEnd","inflateInit2_","inflate","inflatePrime","inflateSetDictionary","inflateEnd","gzdopen","gzbuffer","gztell","gzungetc","gzdopen","gzprintf","gzclose","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Ensure minimum input size for operations
    if (size < 10) {
        return 0;
    }

    // Variables for decompression operations
    Bytef* decompressed = nullptr;
    uLongf destLen = 0;
    uLong sourceLen = size;
    int ret = 0;
    
    // First, try uncompress on the input data
    // Calculate maximum possible decompressed size
    destLen = compressBound(sourceLen);
    decompressed = (Bytef*)malloc(destLen);
    if (!decompressed) {
	return 0;
	}
	
    
    // Try uncompress
    ret = uncompress(decompressed, &destLen, data, sourceLen);
    // We don't care if it fails - it's fuzzing
    
    // Now try uncompress2 with the same data
    uLongf destLen2 = 0;
    uLong sourceLen2 = size;
    Bytef* decompressed2 = (Bytef*)malloc(compressBound(sourceLen2));
    if (decompressed2) {
        ret = uncompress2(decompressed2, &destLen2, data, &sourceLen2);
        free(decompressed2);
    }
    
    // Create a temporary file to write decompressed data for gz operations
    FILE* tmp_file = fopen("tmp_output", "wb");
    if (tmp_file) {
        if (destLen > 0 && decompressed) {
            fwrite(decompressed, 1, destLen, tmp_file);
        }
        assert_file_closed(&tmp_file);;
    }
    
    // Initialize zlib stream for inflation
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Try inflateBackInit_ with a custom window
    unsigned char window[32768];  // 32KB window
    ret = inflateBackInit_(&strm, 15, window, ZLIB_VERSION, sizeof(z_stream));
    if (ret == Z_OK) {
        // We won't actually use inflateBack since we don't have callbacks,
        // but we initialized it as required
        inflateBackEnd(&strm);
    }
    
    // Initialize inflate stream for dictionary operations
    ret = inflateInit2(&strm, 15 + 32);  // 15 window bits + 32 for gzip decoding
    if (ret == Z_OK) {
        // Set up input data for inflation
        Bytef* in_data = (Bytef*)malloc(size);
        if (in_data) {
            memcpy(in_data, data, size);
            strm.next_in = in_data;
            strm.avail_in = size;
            
            // Allocate output buffer
            Bytef* out_buf = (Bytef*)malloc(1024);
            if (out_buf) {
                strm.next_out = out_buf;
                strm.avail_out = 1024;
                
                // Try to inflate a bit
                ret = inflate(&strm, Z_NO_FLUSH);
                
                // Try inflatePrime with some bits from input
                if (size > 2) {
                    inflatePrime(&strm, 8, data[0]);  // Prime with first byte
                }
                
                // Try inflateSetDictionary if we have enough data
                if (size > 100) {
                    Bytef dict[100];
                    memcpy(dict, data, 100);
                    inflateSetDictionary(&strm, dict, 100);
                }
                
                free(out_buf);
            }
            free(in_data);
        }
        inflateEnd(&strm);
    }
    
    // Now work with gzFile operations
    // Open the temporary file we created earlier as a gzFile
    FILE* in_file = fopen("tmp_output", "rb");
    if (in_file) {
        int fd = fuzz_fileno(in_file);
        if (fd >= 0) {
            // Create gzFile from file descriptor
            gzFile gz = gzdopen(fd, "rb");
            if (gz) {
                // Set buffer size
                gzbuffer(gz, 8192);  // 8KB buffer
                
                // Try to get current position
                off_t pos = gztell(gz);
                
                // Read a byte and unget it
                int byte_read = gzgetc(gz);
                if (byte_read != -1) {
                    gzungetc(byte_read, gz);
                }
                
                // Write to another gzFile
                FILE* out_file = fopen("output_file", "wb");
                if (out_file) {
                    int out_fd = fuzz_fileno(out_file);
                    if (out_fd >= 0) {
                        gzFile gz_out = gzdopen(out_fd, "wb");
                        if (gz_out) {
                            // Use gzprintf with format string
                            if (decompressed && destLen > 0) {
                                // Create a null-terminated string from decompressed data
                                // Use min of destLen and 100 to avoid huge strings
                                size_t str_len = destLen < 100 ? destLen : 100;
                                char* str_buf = (char*)malloc(str_len + 1);
                                if (str_buf) {
                                    memcpy(str_buf, decompressed, str_len);
                                    str_buf[str_len] = '\0';
                                    gzprintf(gz_out, "Decompressed: %s\n", str_buf);
                                    free(str_buf);
                                }
                            }
                            
                            gzclose(gz_out);
                        } else {
                            assert_file_closed(&out_file);;
                        }
                    }
                }
                
                gzclose(gz);
            } else {
                assert_file_closed(&in_file);;
            }
        }
        
        // Clean up temporary file
        remove("tmp_output");
    }
    
    // Clean up decompressed buffer
    if (decompressed) {
        free(decompressed);
    }
    
    // Clean up output file if it exists
    remove("output_file");
    
    assert_file_closed(&in_file);
	assert_file_closed(&tmp_file);
	assert_file_name_closed("output_file");
	return 0;
}