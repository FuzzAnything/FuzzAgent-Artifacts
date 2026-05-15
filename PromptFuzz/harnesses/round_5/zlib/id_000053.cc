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
//<ID> 113
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 2
//<Quality> {"density":14,"unique_branches":{"deflateReset":[[708,9,708,20,0,0,4,1]],"deflateResetKeep":[[647,9,647,32,0,0,4,0]]},"library_calls":["zlibCompileFlags","inflateInit_","deflateBound_z","inflateEnd","inflate","inflateReset","adler32","adler32","adler32","adler32","adler32_combine","adler32_combine64","adler32","adler32","deflateReset","deflateBound_z","inflateEnd"],"critical_calls":["zlibCompileFlags","inflateInit_","deflateBound_z","inflate","inflateReset","adler32","adler32","adler32","adler32","adler32_combine","adler32_combine64","adler32","adler32","deflateReset","deflateBound_z","inflateEnd"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified zlib APIs to achieve a scenario where we decompress zlib output data, compute Adler-32 checksums on different parts of it, and combine them:

*/


extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. First, get zlib compilation flags
    uLong compile_flags = zlibCompileFlags();
    (void)compile_flags; // Use variable to avoid unused warning
    
    // 2. Initialize inflation stream for decompressing the input data
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // We need to use inflateInit_ with the correct parameters
    if (inflateInit_(&strm, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        return 0; // Initialization failed
    }
    
    // 3. Set up input buffer with the fuzzer data (compressed data)
    strm.next_in = (Bytef *)data;
    strm.avail_in = size;
    
    // 4. Create output buffer for decompressed data
    // Use deflateBound_z to estimate maximum decompressed size
    // Note: deflateBound_z expects compressed size, but we use it as worst-case estimate
    z_size_t max_decompressed_size = deflateBound_z(&strm, size);
    if (max_decompressed_size == 0 || max_decompressed_size > 10 * 1024 * 1024) {
        // Limit to 10MB to avoid excessive memory usage
        max_decompressed_size = 10 * 1024 * 1024;
    }
    
    Bytef *decompressed_data = (Bytef *)malloc(max_decompressed_size);
    if (!decompressed_data) {
        inflateEnd(&strm);
        return 0;
    }
    
    strm.next_out = decompressed_data;
    strm.avail_out = max_decompressed_size;
    
    // 5. Decompress the data
    int ret = inflate(&strm, Z_FINISH);
    
    // 6. Check if decompression was successful or partially successful
    if (ret != Z_STREAM_END && ret != Z_OK) {
        // Decompression failed, but we'll still try to use what we have
        // Reset stream for potential further operations
        inflateReset(&strm);
    }
    
    // 7. Get the actual decompressed size
    size_t decompressed_bytes = max_decompressed_size - strm.avail_out;
    
    // 8. Compute Adler-32 checksums on different parts of the decompressed data
    if (decompressed_bytes > 0) {
        // Split the data into two parts for checksum combination
        size_t split_point = decompressed_bytes / 2;
        
        // Compute Adler-32 for first half
        uLong adler1 = adler32(0L, Z_NULL, 0);
        if (split_point > 0) {
            adler1 = adler32(adler1, decompressed_data, split_point);
        }
        
        // Compute Adler-32 for second half
        uLong adler2 = adler32(0L, Z_NULL, 0);
        if (decompressed_bytes - split_point > 0) {
            adler2 = adler32(adler2, decompressed_data + split_point, 
                           decompressed_bytes - split_point);
        }
        
        // 9. Combine the checksums using both 32-bit and 64-bit versions
        if (decompressed_bytes > 0) {
            // Use the 32-bit version
            off_t len1 = (off_t)split_point;
            off_t len2 = (off_t)(decompressed_bytes - split_point);
            
            uLong combined_32 = adler32_combine(adler1, adler2, len2);
            (void)combined_32; // Use variable
            
            // 10. Also use the 64-bit version for comparison
            // Convert to off64_t for the 64-bit version
            off64_t len64_1 = (off64_t)split_point;
            off64_t len64_2 = (off64_t)(decompressed_bytes - split_point);
            
            uLong combined_64 = adler32_combine64(adler1, adler2, len64_2);
            (void)combined_64; // Use variable
            
            // For verification: compute checksum of entire data
            uLong full_adler = adler32(0L, Z_NULL, 0);
            full_adler = adler32(full_adler, decompressed_data, decompressed_bytes);
            
            // Note: combined_32/combined_64 should equal full_adler in theory
            // This is the main event we're testing
        }
        
        // 11. Another test: try to estimate compression bound for the decompressed data
        // Reset stream for deflation
        deflateReset(&strm);
        
        // Use deflateBound_z to estimate compressed size from decompressed data
        z_size_t estimated_compressed = deflateBound_z(&strm, decompressed_bytes);
        (void)estimated_compressed; // Use variable
    }
    
    // 12. Cleanup
    free(decompressed_data);
    inflateEnd(&strm);
    
    return 0;
}