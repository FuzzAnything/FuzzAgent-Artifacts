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
//<ID> 67
//<Prompt> []
/*<Combination>: [
*/
//<score> 16.666666, nr_unique_branch: 1
//<Quality> {"density":25,"unique_branches":{"deflateParams":[[784,9,784,39,0,0,4,0]]},"library_calls":["inflateInit_","inflateEnd","inflate","inflateMark","inflateReset","inflateEnd","compressBound","compress2","compressBound_z","compressBound_z","compress_z","gzdopen","gzsetparams","gzwrite","gztell64","gzclose","deflateInit_","deflateBound","deflateBound_z","deflate","deflateParams","deflateResetKeep","deflateEnd","crc32","crc32","crc32_combine","zlibCompileFlags","inflateBackEnd"],"critical_calls":["inflateInit_","inflate","inflateMark","inflateReset","inflateEnd","compressBound","compress2","compressBound_z","compressBound_z","compress_z","gzdopen","gzsetparams","gzwrite","gztell64","gzclose","deflateInit_","deflateBound","deflateBound_z","deflate","deflateParams","deflateResetKeep","deflateEnd","crc32","crc32","crc32_combine","zlibCompileFlags","inflateBackEnd"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Initialize structures
    z_stream inflate_strm;
    z_stream deflate_strm;
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    memset(&deflate_strm, 0, sizeof(deflate_strm));

    // 1. Decompress input using inflate APIs
    int ret = inflateInit_(&inflate_strm, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }

    // Set up input buffer for inflation
    inflate_strm.next_in = (Bytef *)data;
    inflate_strm.avail_in = (uInt)size;

    // Create output buffer for decompressed data
    size_t decompressed_size = size * 4; // Initial guess
    Bytef *decompressed = (Bytef *)malloc(decompressed_size);
    if (!decompressed) {
        inflateEnd(&inflate_strm);
        return 0;
    }

    inflate_strm.next_out = decompressed;
    inflate_strm.avail_out = (uInt)decompressed_size;

    // Perform inflation
    ret = inflate(&inflate_strm, Z_FINISH);
    
    // Mark position
    long mark_pos = inflateMark(&inflate_strm);
    
    // Reset inflate stream
    inflateReset(&inflate_strm);
    
    // End inflation
    inflateEnd(&inflate_strm);

    // 2. Compress the decompressed data using compress APIs
    uLongf compressed_size = compressBound(inflate_strm.total_out);
    Bytef *compressed = (Bytef *)malloc(compressed_size);
    if (!compressed) {
        free(decompressed);
        return 0;
    }

    ret = compress2(compressed, &compressed_size, decompressed, 
                    inflate_strm.total_out, Z_DEFAULT_COMPRESSION);

    // Also use _z version
    z_size_t compressed_z_size;
    z_size_t source_z_len = (z_size_t)inflate_strm.total_out;
    Bytef *compressed_z = (Bytef *)malloc(compressBound_z(source_z_len));
    if (compressed_z) {
        z_size_t dest_z_len = compressBound_z(source_z_len);
        compress_z(compressed_z, &dest_z_len, decompressed, source_z_len);
        free(compressed_z);
    }

    // 3. Use gzFile APIs
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        int fd = fuzz_fileno(out_file);
        gzFile gz = gzdopen(fd, "wb");
        if (gz) {
            // Set compression parameters
            gzsetparams(gz, Z_BEST_COMPRESSION, Z_DEFAULT_STRATEGY);
            
            // Write data
            gzwrite(gz, decompressed, (unsigned int)inflate_strm.total_out);
            
            // Get position
            off64_t pos = gztell64(gz);
            
            // Close
            gzclose(gz);
        }
        assert_file_closed(&out_file);;
    }

    // 4. Use deflate APIs for stream compression
    ret = deflateInit_(&deflate_strm, Z_BEST_COMPRESSION, 
                       ZLIB_VERSION, sizeof(z_stream));
    if (ret == Z_OK) {
        // Set up deflate stream
        deflate_strm.next_in = decompressed;
        deflate_strm.avail_in = (uInt)inflate_strm.total_out;
        
        // Calculate bound
        uLong deflate_bound = deflateBound(&deflate_strm, inflate_strm.total_out);
        z_size_t deflate_bound_z = deflateBound_z(&deflate_strm, source_z_len);
        
        Bytef *deflate_output = (Bytef *)malloc(deflate_bound);
        if (deflate_output) {
            deflate_strm.next_out = deflate_output;
            deflate_strm.avail_out = (uInt)deflate_bound;
            
            // Perform deflation
            ret = deflate(&deflate_strm, Z_FINISH);
            
            // Change parameters
            deflateParams(&deflate_strm, Z_DEFAULT_COMPRESSION, Z_HUFFMAN_ONLY);
            
            // Reset keeping dictionary
            deflateResetKeep(&deflate_strm);
            
            // Clean up deflate
            deflateEnd(&deflate_strm);
            free(deflate_output);
        }
    }

    // 5. Use CRC combine
    uLong crc1 = crc32(0L, decompressed, (uInt)(inflate_strm.total_out / 2));
    uLong crc2 = crc32(0L, decompressed + inflate_strm.total_out / 2, 
                       (uInt)(inflate_strm.total_out - inflate_strm.total_out / 2));
    uLong combined_crc = crc32_combine(crc1, crc2, inflate_strm.total_out / 2);

    // 6. Get zlib compile flags
    uLong flags = zlibCompileFlags();

    // 7. Clean up inflateBack stream (initialize and end immediately)
    z_stream back_strm;
    memset(&back_strm, 0, sizeof(back_strm));
    // Note: inflateBackInit_ would normally require window buffer
    // We initialize it minimally since we're just calling the end function
    inflateBackEnd(&back_strm);

    // Clean up allocated memory
    free(decompressed);
    free(compressed);

    assert_file_closed(&out_file);
	return 0;
}