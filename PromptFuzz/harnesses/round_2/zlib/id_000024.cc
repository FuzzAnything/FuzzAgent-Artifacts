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
//<ID> 46
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 1
//<Quality> {"density":12,"unique_branches":{"uncompress2_z":[[42,22,42,36,0,0,4,1]]},"library_calls":["compressBound_z","uncompress2_z","inflateBackInit_","inflatePrime","inflateUndermine","inflateBack","inflateCodesUsed","inflateBackEnd","deflateInit_","deflateReset","deflateEnd","gzdopen","gzungetc","gzclose","crc32","crc32","crc32_combine_gen","crc32_combine_op"],"critical_calls":["compressBound_z","uncompress2_z","inflateBackInit_","inflatePrime","inflateUndermine","inflateBack","inflateCodesUsed","inflateBackEnd","deflateInit_","deflateReset","deflateEnd","gzdopen","gzungetc","gzclose","crc32","crc32","crc32_combine_gen","crc32_combine_op"],"visited":1}
/**/


// Callback functions for inflateBack
unsigned int  id_000024_inflateBack_in_func(void* desc, unsigned char** buf) {
    // Simple callback that returns no data to test error handling
    return 0;
}

int  id_000024_inflateBack_out_func(void* desc, unsigned char* buf, unsigned int size) {
    // Simple output callback that discards data
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. First use uncompress2_z to decompress the input (assuming it's zlib compressed data)
    z_size_t destLen = 0;
    z_size_t sourceLen = size;
    Bytef* dest = nullptr;
    
    // Try to decompress - but first need to check if we have enough data
    if (size < 2) {
        return 0;
    }
    
    // Allocate destination buffer using compressBound_z for worst-case size
    z_size_t bound = compressBound_z(size);
    if (bound == 0 || bound > 1024*1024*10) { // Limit to 10MB to avoid OOM
        bound = 1024*1024*10;
    }
    
    dest = (Bytef*)malloc(bound);
    if (!dest) {
        return 0;
    }
    
    int uncompress_ret = uncompress2_z(dest, &destLen, data, &sourceLen);
    
    // 2. Setup for inflateBack operations
    z_stream strm;
    unsigned char window[32768]; // 32K window as required by inflateBackInit_
    
    // Initialize for inflateBack
    int back_init_ret = inflateBackInit_(&strm, 15, window, ZLIB_VERSION, sizeof(z_stream));
    
    if (back_init_ret == Z_OK) {
        // 3. Try inflatePrime with some bits from the input
        int prime_bits = data[0] & 7; // Use 0-7 bits
        int prime_value = data[1] & 0xFF;
        inflatePrime(&strm, prime_bits, prime_value);
        
        // 4. Try inflateUndermine - this may fail but we try it
        if (size > 10) {
            inflateUndermine(&strm, data[2] & 1);
        }
        
        // 5. Attempt inflateBack with our callbacks
        inflateBack(&strm,  id_000024_inflateBack_in_func, nullptr,  id_000024_inflateBack_out_func, nullptr);
        
        // 6. Get inflation codes usage statistics
        unsigned long codes_used = inflateCodesUsed(&strm);
        
        // 7. Clean up inflateBack
        inflateBackEnd(&strm);
    }
    
    // 8. Now test compression APIs
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    
    // Initialize deflate
    int deflate_init_ret = deflateInit_(&def_strm, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, sizeof(z_stream));
    
    if (deflate_init_ret == Z_OK) {
        // 9. Test deflateReset
        deflateReset(&def_strm);
        
        // 10. Clean up deflate
        deflateEnd(&def_strm);
    }
    
    // 11. Test gzFile operations
    // First write decompressed data to a file
    FILE* out_file = fopen("output_file", "wb");
    if (out_file && destLen > 0) {
        fwrite(dest, 1, destLen, out_file);
        assert_file_closed(&out_file);;
    }
    
    // 12. Open the file with gzdopen
    FILE* in_file = fopen("output_file", "rb");
    if (in_file) {
        int fd = fuzz_fileno(in_file);
        gzFile gz = gzdopen(fd, "rb");
        
        if (gz) {
            // 13. Try gzgetc
            if (destLen > 0) {
                gzgetc(gz);
            }
            
            // 14. Try gzungetc with a character from input
            if (size > 3) {
                gzungetc(data[3], gz);
            }
            
            // 15. Close gz file
            gzclose(gz);
        }
        assert_file_closed(&in_file);;
    }
    
    // 16. Test crc32_combine_op with some dummy values
    uLong crc1 = crc32(0L, data, size > 100 ? 100 : (uInt)size);
    uLong crc2 = 0;
    if (size > 100) {
        crc2 = crc32(0L, data + 100, size - 100 > 100 ? 100 : (uInt)(size - 100));
    }
    
    // Use the combine op (op is crc32_combine_gen's output)
    uLong op = crc32_combine_gen(100);
    uLong combined = crc32_combine_op(crc1, crc2, op);
    
    // Clean up
    free(dest);
    
    // Remove temporary file
    remove("output_file");
    
    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_file_name_closed("output_file");
	return 0;
}