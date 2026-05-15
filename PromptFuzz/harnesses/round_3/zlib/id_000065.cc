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
//<ID> 131
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 3
//<Quality> {"density":5,"unique_branches":{"deflateSetDictionary":[[580,9,580,32,0,0,4,0],[581,13,581,22,0,0,4,1]],"inflateUndermine":[[1373,9,1373,32,0,0,4,0]]},"library_calls":["gzdopen","gztell","gztell64","inflateBackInit_","inflateUndermine","inflateBackEnd","deflateInit_","deflateSetDictionary","deflateBound","compress","deflateEnd","crc32_combine_gen","gzclose"],"critical_calls":["gzdopen","gztell","gztell64","inflateBackInit_","inflateUndermine","inflateBackEnd","deflateInit_","deflateSetDictionary","deflateBound","compress","deflateEnd","crc32_combine_gen","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t* f_data, size_t f_size) {
	if(f_size<26) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	
    
    // 1. Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Get file descriptor and open as gzFile
    int fd = fuzz_fileno(in_file);
    gzFile gz_file = gzdopen(fd, fuzz_str_1);
    if (!gz_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 3. Read compressed data using gzgetc and track position
    int bytes_read = 0;
    off_t tell_result = 0;
    off64_t tell64_result = 0;
    
    // Read first few bytes with gzgetc
    for (int i = 0; i < 10 && bytes_read < (int)size; i++) {
        int ch = gzgetc(gz_file);
        if (ch == -1) break;
        bytes_read++;
        
        // Get position with both gztell functions
        if (i == 5) {
            tell_result = gztell(gz_file);
            tell64_result = gztell64(gz_file);
        }
    }
    
    // 4. Prepare for inflateBack by setting up z_stream
    z_stream strm;
    unsigned char window[32768]; // 32K window as required by inflateBack
    memset(&strm, 0, sizeof(strm));
    
    // Initialize for inflateBack
    int back_init_result = inflateBackInit_(&strm, 15, window, 
                                           fuzz_str_2, sizeof(z_stream));
    
    if (back_init_result == Z_OK) {
        // 5. Try to undermine inflation (though we won't actually use inflateBack fully)
        int undermine_result = inflateUndermine(&strm, fuzz_int32_t_3);
        (void)undermine_result; // Result may be error, that's OK for fuzzing
        
        // Clean up inflateBack
        inflateBackEnd(&strm);
    }
    
    // 6. Create a dictionary from part of input data
    Bytef dictionary[32768];
    uInt dict_len = size > 32768 ? 32768 : (uInt)size;
    if (dict_len > 10) {
        memcpy(dictionary, data, dict_len);
        
        // 7. Setup deflate stream for compression
        z_stream def_strm;
        memset(&def_strm, 0, sizeof(def_strm));
        deflateInit(&def_strm, Z_DEFAULT_COMPRESSION);
        
        // Set dictionary
        int dict_result = deflateSetDictionary(&def_strm, dictionary, dict_len);
        (void)dict_result;
        
        // 8. Get bound for compressed size
        uLong source_len = size > 10000 ? 10000 : (uLong)size;
        uLong bound_result = deflateBound(&def_strm, source_len);
        (void)bound_result;
        
        // 9. Actually compress some data
        Bytef *compressed = (Bytef*)malloc(bound_result);
        uLongf compressed_len = bound_result;
        
        if (compressed) {
            // Use a subset of input data for compression
            uLong compress_size = size > 1000 ? 1000 : (uLong)size;
            int compress_result = compress(compressed, &compressed_len, 
                                          data, compress_size);
            (void)compress_result;
            
            free(compressed);
        }
        
        deflateEnd(&def_strm);
    }
    
    // 10. Generate CRC combine value using position
    if (tell_result > 0) {
        uLong crc_gen = crc32_combine_gen(tell_result);
        (void)crc_gen;
    }
    
    // 11. Clean up
    gzclose(gz_file);  // This also closes the fd
    // Note: in_file is already closed by gzclose, don't double-close
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}