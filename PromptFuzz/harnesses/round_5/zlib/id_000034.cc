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
//<ID> 44
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 2
//<Quality> {"density":18,"unique_branches":{"inflate":[[495,10,495,33,0,0,4,0],[495,37,495,56,0,0,4,1]]},"library_calls":["gzdopen","gzclearerr","inflateInit2_","gzclose","deflateInit2_","inflateEnd","gzclose","deflateSetHeader","gzread","gztell","gzoffset64","crc32","crc32","inflate","inflateGetDictionary","inflateUndermine","deflateBound","deflate","deflatePending","gzdopen","gzwrite","crc32","crc32_combine","compressBound_z","compressBound_z","compress_z","gzclose","gzread","inflateReset","inflateEnd","deflateEnd","gzclose"],"critical_calls":["gzdopen","gzclearerr","inflateInit2_","deflateInit2_","deflateSetHeader","gzread","gztell","gzoffset64","crc32","crc32","inflate","inflateGetDictionary","inflateUndermine","deflateBound","deflate","deflatePending","gzdopen","gzwrite","crc32","crc32_combine","compressBound_z","compressBound_z","compress_z","gzclose","gzread","inflateReset","inflateEnd","deflateEnd","gzclose"],"visited":1}
/*Here's a C++ fuzz driver that uses all the specified zlib APIs to achieve the event of decompressing input zlib data, validating it, then recompressing it with various operations:

*/


extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return for insufficient data
    if (size < 10) {
	return 0;
	}
	
    
    // Variables for various operations
    int ret;
    unsigned int pending;
    int bits;
    uLong crc_val, crc_val2;
    off_t offset_result;
    off64_t offset64_result;
    uLong bound_result;
    z_size_t destLen;
    uInt dictLength;
    
    // Create a FILE* for the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Get file descriptor for gzdopen
    int fd = fuzz_fileno(in_file);
    
    // 1. Open the input as a gzFile
    gzFile gz_in = gzdopen(fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 2. Clear any errors on the gzFile
    gzclearerr(gz_in);
    
    // 3. Initialize decompression stream
    z_stream strm_inflate;
    strm_inflate.zalloc = Z_NULL;
    strm_inflate.zfree = Z_NULL;
    strm_inflate.opaque = Z_NULL;
    strm_inflate.avail_in = 0;
    strm_inflate.next_in = Z_NULL;
    
    ret = inflateInit2_(&strm_inflate, 15 + 32, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 4. Initialize compression stream
    z_stream strm_deflate;
    strm_deflate.zalloc = Z_NULL;
    strm_deflate.zfree = Z_NULL;
    strm_deflate.opaque = Z_NULL;
    
    ret = deflateInit2_(&strm_deflate, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                        15 + 16, 8, Z_DEFAULT_STRATEGY,
                        ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        inflateEnd(&strm_inflate);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 5. Set gzip header for compression
    gz_header header;
    memset(&header, 0, sizeof(header));
    header.text = 1;
    header.time = time(NULL);
    header.os = 255;
    ret = deflateSetHeader(&strm_deflate, &header);
    
    // 6. Prepare decompression input buffer
    const size_t chunk_size = 4096;
    Bytef *in_buffer = new Bytef[chunk_size];
    Bytef *out_buffer = new Bytef[chunk_size];
    Bytef *dict_buffer = new Bytef[1024];
    
    // 7. Read from gzFile and decompress
    size_t bytes_read;
    strm_inflate.avail_in = 0;
    
    // Read first chunk from gzFile
    bytes_read = gzread(gz_in, in_buffer, chunk_size);
    if (bytes_read > 0) {
        strm_inflate.avail_in = bytes_read;
        strm_inflate.next_in = in_buffer;
    }
    
    // 8. Get current position in gzFile
    offset_result = gztell(gz_in);
    
    // 9. Get offset64 (for large files)
    offset64_result = gzoffset64(gz_in);
    
    // 10. Calculate CRC32 of input data
    crc_val = crc32(0L, Z_NULL, 0);
    crc_val = crc32(crc_val, data, size > 1000 ? 1000 : size);
    
    // 11. Decompress the data
    do {
        strm_inflate.avail_out = chunk_size;
        strm_inflate.next_out = out_buffer;
        
        ret = inflate(&strm_inflate, Z_NO_FLUSH);
        
        if (ret == Z_NEED_DICT) {
            // Try to get dictionary
            inflateGetDictionary(&strm_inflate, dict_buffer, &dictLength);
        }
        
        if (ret < 0) {
            // Try to undermine the stream (testing invalid input)
            inflateUndermine(&strm_inflate, 1);
            break;
        }
        
        // 12. If we got output, compress it back
        size_t have = chunk_size - strm_inflate.avail_out;
        if (have > 0) {
            // Set up deflate stream with the decompressed data
            strm_deflate.avail_in = have;
            strm_deflate.next_in = out_buffer;
            
            // 13. Get bound for compressed size
            bound_result = deflateBound(&strm_deflate, have);
            
            // Compress the data
            strm_deflate.avail_out = chunk_size;
            strm_deflate.next_out = in_buffer;  // Reuse in_buffer for output
            
            ret = deflate(&strm_deflate, Z_FINISH);
            
            // 14. Check for pending bits
            deflatePending(&strm_deflate, &pending, &bits);
            
            size_t compressed_size = chunk_size - strm_deflate.avail_out;
            if (compressed_size > 0) {
                // 15. Write compressed data to a new gzFile
                FILE *out_file = fopen("output_file", "wb");
                if (out_file) {
                    int out_fd = fuzz_fileno(out_file);
                    gzFile gz_out = gzdopen(out_fd, "wb");
                    if (gz_out) {
                        gzwrite(gz_out, in_buffer, compressed_size);
                        
                        // 16. Combine CRC values
                        crc_val2 = crc32(0L, in_buffer, compressed_size);
                        uLong combined_crc = crc32_combine(crc_val, crc_val2, compressed_size);
                        
                        // 17. Use compress_z API
                        Bytef *compress_z_dest = new Bytef[compressBound_z(size)];
                        z_size_t compress_z_destLen = compressBound_z(size);
                        if (compress_z(compress_z_dest, &compress_z_destLen, 
                                      data, size > 10000 ? 10000 : size) == Z_OK) {
                            // Successfully used compress_z
                        }
                        delete[] compress_z_dest;
                        
                        // 18. Clean up gzFile
                        gzclose(gz_out);
                    }
                    assert_file_closed(&out_file);;
                }
            }
        }
        
        // Read more data if needed
        if (strm_inflate.avail_in == 0 && !feof((FILE*)gz_in)) {
            bytes_read = gzread(gz_in, in_buffer, chunk_size);
            if (bytes_read > 0) {
                strm_inflate.avail_in = bytes_read;
                strm_inflate.next_in = in_buffer;
            }
        }
        
    } while (ret != Z_STREAM_END);
    
    // 19. Reset inflate stream
    inflateReset(&strm_inflate);
    
    // 20. Clean up all resources
    inflateEnd(&strm_inflate);
    deflateEnd(&strm_deflate);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    
    // Remove output file
    remove("output_file");
    
    delete[] in_buffer;
    delete[] out_buffer;
    delete[] dict_buffer;
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}