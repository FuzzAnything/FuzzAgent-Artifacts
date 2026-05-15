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
//<ID> 237
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 4
//<Quality> {"density":5,"unique_branches":{"inflateBack":[[491,17,491,38,0,0,4,0],[496,25,496,66,0,0,4,0],[496,25,496,66,0,0,4,1],[101,13,101,22,86,0,4,1]]},"library_calls":["inflateInit_","inflateEnd","compressBound","inflateEnd","uncompress2_z","inflateValidate","inflateBackInit_","inflateBack","inflateBackEnd","crc32","crc32","crc32_combine64","inflateEnd"],"critical_calls":["inflateInit_","compressBound","uncompress2_z","inflateValidate","inflateBackInit_","inflateBack","inflateBackEnd","crc32","crc32","crc32_combine64","inflateEnd"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified zlib APIs to achieve decompression with validation and CRC combination:

*/


// Callback functions for inflateBack
static unsigned int  id_000083_inf_in(void* desc, unsigned char** buf) {
    // Return 0 to indicate no more input
    return 0;
}

static int  id_000083_inf_out(void* desc, unsigned char* buf, unsigned int size) {
    // Just discard output for fuzzing
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_83(const uint8_t* f_data, size_t f_size) {
	if(f_size<54) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimum data for meaningful operations

    // Split data for different operations
    size_t split_point = size / 2;
    const Bytef* first_half = data;
    size_t first_half_size = split_point;
    const Bytef* second_half = data + split_point;
    size_t second_half_size = size - split_point;

    // 1. Initialize inflation stream with inflateInit_
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    int ret = inflateInit_(&strm, fuzz_str_1, sizeof(z_stream));
    if (ret != Z_OK) {
        // Cleanup and return
        inflateEnd(&strm);
        return 0;
    }

    // 2. Use uncompress2_z on first half of data
    // First, try to decompress assuming it's zlib-compressed data
    Bytef* dest = nullptr;
    z_size_t destLen = 0;
    z_size_t sourceLen = first_half_size;
    
    // Allocate destination buffer (use compressBound for worst case)
    uLong bound = compressBound(first_half_size);
    dest = (Bytef*)malloc(bound);
    if (!dest) {
        inflateEnd(&strm);
        return 0;
    }
    
    // Try to uncompress
    int uncompress_ret = uncompress2_z(dest, &destLen, first_half, &sourceLen);
    // We don't care about success/failure for fuzzing, just exercise the API
    
    // 3. Use inflateValidate to set validation option
    // Enable validation (non-zero enables validation, 0 disables)
    int validate_ret = inflateValidate(&strm, fuzz_int32_t_2);
    
    // 4. Use inflateBack with callback functions
    // Initialize for inflateBack
    z_stream back_strm;
    memset(&back_strm, 0, sizeof(back_strm));
    
    unsigned char window[32768];  // Maximum window size
    
    ret = inflateBackInit_(&back_strm, 15, window, fuzz_str_3, sizeof(z_stream));
    if (ret == Z_OK) {
        // Set up input data for inflateBack
        struct {
            const Bytef* data;
            size_t remaining;
        } input_info = {second_half, second_half_size};
        
        // Custom input function that reads from our buffer
        auto custom_in = [](void* desc, unsigned char** buf) -> unsigned int {
            auto* info = static_cast<decltype(input_info)*>(desc);
            if (info->remaining == 0) return 0;
            
            *buf = (unsigned char*)info->data;
            unsigned int to_read = (info->remaining > 65535) ? 65535 : info->remaining;
            info->data += to_read;
            info->remaining -= to_read;
            return to_read;
        };
        
        // Use inflateBack with our custom input function and discard output
        ret = inflateBack(&back_strm, custom_in, &input_info,  id_000083_inf_out, nullptr);
        
        inflateBackEnd(&back_strm);
    }
    
    // 5. Use crc32_combine64 to combine CRCs
    // Compute CRCs for both halves
    uLong crc1 = crc32(fuzz_uint64_t_4, first_half, first_half_size);
    uLong crc2 = crc32(fuzz_uint64_t_5, second_half, second_half_size);
    
    // Combine them using crc32_combine64
    // off64_t is 64-bit offset, we use the second half size
    off64_t len2 = second_half_size;
    uLong combined_crc = crc32_combine64(crc1, crc2, len2);
    (void)combined_crc;  // Mark as used
    
    // Clean up inflate stream
    inflateEnd(&strm);
    
    // Clean up allocated resources
    free(dest);

    return 0;
}