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
//<ID> 74
//<Prompt> []
/*<Combination>: [
*/
//<score> 27, nr_unique_branch: 2
//<Quality> {"density":18,"unique_branches":{"adler32_combine_":[[150,9,150,21,0,0,4,1]],"deflateBound_z":[[888,24,888,35,0,0,4,0]]},"library_calls":["zlibCompileFlags","compressBound","deflateInit_","deflate","deflateBound","deflateBound_z","deflateParams","deflateResetKeep","inflateInit_","deflateEnd","inflateMark","inflate","inflateReset","adler32","crc32","adler32","adler32","adler32_combine","crc32","crc32","crc32_combine","gzdopen","gzsetparams","gzread","gztell64","gzdopen","gzwrite","gzclose","gzclose","deflateEnd","inflateEnd","compress2"],"critical_calls":["zlibCompileFlags","compressBound","deflateInit_","deflate","deflateBound","deflateBound_z","deflateParams","deflateResetKeep","inflateInit_","inflateMark","inflate","inflateReset","adler32","crc32","adler32","adler32","adler32_combine","crc32","crc32","crc32_combine","gzdopen","gzsetparams","gzread","gztell64","gzdopen","gzwrite","gzclose","gzclose","deflateEnd","inflateEnd","compress2"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize compression and decompression streams
    z_stream compress_stream;
    z_stream decompress_stream;
    int ret;
    
    // 1. Get zlib compile flags
    uLong flags = zlibCompileFlags();
    
    // 2. Process input data through compression/decompression cycle
    if (size > 0) {
        // Allocate buffers for compressed and decompressed data
        uLong max_compressed = compressBound(size);
        Bytef* compressed_data = (Bytef*)malloc(max_compressed);
        Bytef* decompressed_data = (Bytef*)malloc(size);
        
        if (!compressed_data || !decompressed_data) {
            free(compressed_data);
            free(decompressed_data);
            return 0;
        }
        
        // 3. Initialize deflation stream
        ret = deflateInit_(&compress_stream, Z_DEFAULT_COMPRESSION, 
                          ZLIB_VERSION, sizeof(z_stream));
        if (ret != Z_OK) {
            free(compressed_data);
            free(decompressed_data);
            return 0;
        }
        
        // 4. Set up compression stream
        compress_stream.next_in = (Bytef*)data;
        compress_stream.avail_in = size;
        compress_stream.next_out = compressed_data;
        compress_stream.avail_out = max_compressed;
        
        // 5. Perform compression
        ret = deflate(&compress_stream, Z_FINISH);
        
        // 6. Get compressed size
        uLong compressed_size = max_compressed - compress_stream.avail_out;
        
        // 7. Use deflateBound and deflateBound_z
        uLong bound1 = deflateBound(&compress_stream, size);
        z_size_t bound2 = deflateBound_z(&compress_stream, size);
        
        // 8. Try to change compression parameters
        deflateParams(&compress_stream, Z_BEST_COMPRESSION, Z_DEFAULT_STRATEGY);
        
        // 9. Reset compression stream keeping state
        deflateResetKeep(&compress_stream);
        
        // 10. Initialize inflation stream
        ret = inflateInit_(&decompress_stream, ZLIB_VERSION, sizeof(z_stream));
        if (ret != Z_OK) {
            deflateEnd(&compress_stream);
            free(compressed_data);
            free(decompressed_data);
            return 0;
        }
        
        // 11. Set up decompression stream
        decompress_stream.next_in = compressed_data;
        decompress_stream.avail_in = compressed_size;
        decompress_stream.next_out = decompressed_data;
        decompress_stream.avail_out = size;
        
        // 12. Perform decompression with mark/reset cycle
        long mark = inflateMark(&decompress_stream);
        ret = inflate(&decompress_stream, Z_FINISH);
        
        // 13. Reset decompression stream
        inflateReset(&decompress_stream);
        
        // 14. Calculate checksums on original data
        uLong adler_checksum = adler32(0L, data, size);
        uLong crc_checksum = crc32(0L, data, size);
        
        // 15. Combine checksums (dummy combination)
        if (size > 1) {
            size_t half = size / 2;
            uLong adler1 = adler32(0L, data, half);
            uLong adler2 = adler32(0L, data + half, size - half);
            uLong combined_adler = adler32_combine(adler1, adler2, size - half);
            
            uLong crc1 = crc32(0L, data, half);
            uLong crc2 = crc32(0L, data + half, size - half);
            uLong combined_crc = crc32_combine(crc1, crc2, size - half);
        }
        
        // 16. Use gzFile API with input data
        FILE* in_file = fmemopen((void*)data, size, "rb");
        if (in_file) {
            int fd = fuzz_fileno(in_file);
            gzFile gz = gzdopen(fd, "rb");
            if (gz) {
                // 17. Set gz parameters
                gzsetparams(gz, Z_DEFAULT_COMPRESSION, Z_DEFAULT_STRATEGY);
                
                // Read some data
                char buffer[1024];
                gzread(gz, buffer, sizeof(buffer));
                
                // 18. Get position
                off64_t pos = gztell64(gz);
                
                // 19. Write to another gzFile
                FILE* out_file = fopen("output_file", "wb");
                if (out_file) {
                    int out_fd = fuzz_fileno(out_file);
                    gzFile gz_out = gzdopen(out_fd, "wb");
                    if (gz_out) {
                        gzwrite(gz_out, buffer, sizeof(buffer));
                        gzclose(gz_out);
                    } else {
                        assert_file_closed(&out_file);;
                    }
                }
                
                // 20. Close gz file
                gzclose(gz);
            }
            assert_file_closed(&in_file);;
        }
        
        // 21. Clean up compression/decompression streams
        deflateEnd(&compress_stream);
        inflateEnd(&decompress_stream);
        
        // 22. Try compress2 API
        uLongf destLen = max_compressed;
        compress2(compressed_data, &destLen, data, size, Z_BEST_COMPRESSION);
        
        // 23. Clean up allocated memory
        free(compressed_data);
        free(decompressed_data);
        
        // Remove temporary output file
        remove("output_file");
    }
    
    return 0;
}