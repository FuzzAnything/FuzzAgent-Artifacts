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
//<ID> 40
//<Prompt> []
/*<Combination>: [
*/
//<score> 34, nr_unique_branch: 1
//<Quality> {"density":17,"unique_branches":{"deflateInit2_":[[435,27,435,42,0,0,4,0]]},"library_calls":["gzdopen","gzclearerr","inflateInit2_","gzclose","deflateInit2_","inflateEnd","gzclose","deflateSetHeader","deflateEnd","inflateEnd","gzclose","crc32","crc32","gzread","crc32","inflate","inflateGetDictionary","inflateReset","inflateUndermine","deflateBound","deflate","deflatePending","gzdopen","gzwrite","gztell","gzoffset64","gzclose","crc32","crc32_combine","compress_z","deflateEnd","inflateEnd","gzclose"],"critical_calls":["gzdopen","gzclearerr","inflateInit2_","deflateInit2_","deflateSetHeader","crc32","crc32","gzread","crc32","inflate","inflateGetDictionary","inflateReset","inflateUndermine","deflateBound","deflate","deflatePending","gzdopen","gzwrite","gztell","gzoffset64","gzclose","crc32","crc32_combine","compress_z","deflateEnd","inflateEnd","gzclose"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified zlib APIs to process zlib-compressed data:

*/


extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	 // Need minimum data for meaningful operations

    // Create input and output files
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);

    // 1. Open gzFile for reading compressed data
    gzFile gz_in = gzdopen(fd_in, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Clear any previous errors on gzFile
    gzclearerr(gz_in);

    // 2. Initialize inflate stream for decompression
    z_stream strm_inflate;
    memset(&strm_inflate, 0, sizeof(z_stream));
    
    // Use inflateInit2_ with windowBits for zlib format
    int windowBits = 15 + 32; // 32 enables zlib/gzip auto-detection
    if (inflateInit2_(&strm_inflate, windowBits, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 3. Initialize deflate stream for recompression
    z_stream strm_deflate;
    memset(&strm_deflate, 0, sizeof(z_stream));
    
    // Use deflateInit2_ with various parameters
    int level = Z_DEFAULT_COMPRESSION;
    int method = Z_DEFLATED;
    int memLevel = 8;
    int strategy = Z_DEFAULT_STRATEGY;
    if (deflateInit2_(&strm_deflate, level, method, windowBits, memLevel, 
                      strategy, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        inflateEnd(&strm_inflate);
        gzclose(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 4. Set gzip header for deflate stream
    gz_header header;
    memset(&header, 0, sizeof(gz_header));
    header.text = 0;
    header.time = 0;
    header.os = 255; // Unknown OS
    if (deflateSetHeader(&strm_deflate, &header) != Z_OK) {
        // Continue even if header setting fails
    }

    // 5. Read from gzFile and decompress
    const size_t buffer_size = 4096;
    uint8_t *read_buffer = (uint8_t *)malloc(buffer_size);
    uint8_t *decomp_buffer = (uint8_t *)malloc(buffer_size * 2);
    uint8_t *comp_buffer = (uint8_t *)malloc(buffer_size * 2);
    
    if (!read_buffer || !decomp_buffer || !comp_buffer) {
        free(read_buffer);
        free(decomp_buffer);
        free(comp_buffer);
        deflateEnd(&strm_deflate);
        inflateEnd(&strm_inflate);
        gzclose(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 6. Process data through multiple stages
    uLong crc1 = crc32(0L, Z_NULL, 0);
    uLong crc2 = crc32(0L, Z_NULL, 0);
    
    // Read from gzFile
    int bytes_read = gzread(gz_in, read_buffer, buffer_size);
    if (bytes_read > 0) {
        // Update CRC of read data
        crc1 = crc32(crc1, read_buffer, bytes_read);
        
        // Set up inflate stream with the read data
        strm_inflate.next_in = read_buffer;
        strm_inflate.avail_in = bytes_read;
        strm_inflate.next_out = decomp_buffer;
        strm_inflate.avail_out = buffer_size * 2;
        
        // Decompress
        int inflate_result = inflate(&strm_inflate, Z_NO_FLUSH);
        if (inflate_result == Z_OK || inflate_result == Z_STREAM_END) {
            size_t decomp_size = buffer_size * 2 - strm_inflate.avail_out;
            
            // Get dictionary from inflate stream
            Bytef dict_buffer[256];
            uInt dict_len = 256;
            inflateGetDictionary(&strm_inflate, dict_buffer, &dict_len);
            
            // Reset inflate stream
            inflateReset(&strm_inflate);
            
            // Try to undermine (for testing error paths)
            inflateUndermine(&strm_inflate, 0);
            
            // Set up deflate stream with decompressed data
            strm_deflate.next_in = decomp_buffer;
            strm_deflate.avail_in = decomp_size;
            strm_deflate.next_out = comp_buffer;
            strm_deflate.avail_out = buffer_size * 2;
            
            // Calculate bound for compressed size
            uLong bound = deflateBound(&strm_deflate, decomp_size);
            
            // Compress with deflate
            int deflate_result = deflate(&strm_deflate, Z_FINISH);
            if (deflate_result == Z_OK || deflate_result == Z_STREAM_END) {
                size_t comp_size = buffer_size * 2 - strm_deflate.avail_out;
                
                // Check pending bits
                unsigned int pending;
                int bits;
                deflatePending(&strm_deflate, &pending, &bits);
                
                // Write compressed data to gzFile output
                gzFile gz_out = gzdopen(fd_out, "wb");
                if (gz_out) {
                    gzwrite(gz_out, comp_buffer, comp_size);
                    
                    // Get file position
                    off_t pos = gztell(gz_out);
                    (void)pos; // Use to avoid unused warning
                    
                    // Get 64-bit offset
                    off64_t offset = gzoffset64(gz_out);
                    (void)offset; // Use to avoid unused warning
                    
                    gzclose(gz_out);
                }
                
                // Update second CRC
                crc2 = crc32(crc2, comp_buffer, comp_size);
                
                // Combine CRCs
                uLong combined_crc = crc32_combine(crc1, crc2, comp_size);
                (void)combined_crc; // Use to avoid unused warning
                
                // Also try compress_z API
                Bytef *dest = (Bytef *)malloc(buffer_size * 2);
                z_size_t dest_len = buffer_size * 2;
                if (dest) {
                    compress_z(dest, &dest_len, decomp_buffer, decomp_size);
                    free(dest);
                }
            }
        }
    }

    // 7. Clean up
    free(read_buffer);
    free(decomp_buffer);
    free(comp_buffer);
    
    deflateEnd(&strm_deflate);
    inflateEnd(&strm_inflate);
    
    gzclose(gz_in);
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}