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
//<ID> 129
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["inflateInit2_","zError","inflateSetDictionary","inflate","zError","crc32","deflateInit2_","inflateEnd","deflateSetDictionary","deflateGetDictionary","deflateBound_z","compressBound","deflate","zError","deflatePending","deflateResetKeep","gzdopen","gzdopen","gzsetparams","gzread","gzclose","gzclose","inflateEnd","deflateEnd"],"critical_calls":["inflateInit2_","inflateSetDictionary","inflate","zError","crc32","deflateInit2_","deflateSetDictionary","deflateGetDictionary","deflateBound_z","compressBound","deflate","zError","deflatePending","deflateResetKeep","gzdopen","gzdopen","gzsetparams","gzread","gzclose","gzclose","inflateEnd","deflateEnd"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	 // Need minimal data for operations

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
    
    // Variables for operations
    z_stream strm_inflate, strm_deflate;
    gzFile gzfile = NULL;
    int ret;
    uLong crc = 0;
    unsigned int pending;
    int bits;
    uInt dict_len;
    Bytef dictionary[32768]; // Max dictionary size
    
    // 1. Initialize inflation
    strm_inflate.zalloc = Z_NULL;
    strm_inflate.zfree = Z_NULL;
    strm_inflate.opaque = Z_NULL;
    ret = inflateInit2_(&strm_inflate, MAX_WBITS, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        const char *err = zError(ret);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 2. Set dictionary for inflation (use first 256 bytes as dictionary if available)
    uInt dict_size = size > 256 ? 256 : (uInt)size;
    ret = inflateSetDictionary(&strm_inflate, data, dict_size);
    if (ret != Z_OK && ret != Z_NEED_DICT) {
        // Continue anyway
    }
    
    // 3. Prepare for decompression
    std::vector<Bytef> in_buffer(data, data + size);
    std::vector<Bytef> out_buffer(1024 * 1024); // 1MB buffer
    
    strm_inflate.next_in = in_buffer.data();
    strm_inflate.avail_in = (uInt)size;
    strm_inflate.next_out = out_buffer.data();
    strm_inflate.avail_out = (uInt)out_buffer.size();
    
    // 4. Decompress
    ret = inflate(&strm_inflate, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK) {
        const char *err = zError(ret);
        // Continue anyway
    }
    
    size_t decompressed_size = out_buffer.size() - strm_inflate.avail_out;
    
    // 5. Compute CRC32 of decompressed data
    if (decompressed_size > 0) {
        crc = crc32(0L, out_buffer.data(), (uInt)decompressed_size);
    }
    
    // 6. Initialize deflation
    strm_deflate.zalloc = Z_NULL;
    strm_deflate.zfree = Z_NULL;
    strm_deflate.opaque = Z_NULL;
    ret = deflateInit2_(&strm_deflate, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                       MAX_WBITS, 8, Z_DEFAULT_STRATEGY,
                       ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        inflateEnd(&strm_inflate);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 7. Set dictionary for deflation (same as inflation dictionary)
    ret = deflateSetDictionary(&strm_deflate, data, dict_size);
    if (ret != Z_OK) {
        // Continue anyway
    }
    
    // 8. Get dictionary from deflate stream
    dict_len = sizeof(dictionary);
    ret = deflateGetDictionary(&strm_deflate, dictionary, &dict_len);
    
    // 9. Get size bounds
    z_size_t bound_z = deflateBound_z(&strm_deflate, (z_size_t)decompressed_size);
    uLong bound = compressBound((uLong)decompressed_size);
    
    // 10. Prepare for compression
    std::vector<Bytef> compressed_buffer(bound_z);
    
    strm_deflate.next_in = out_buffer.data();
    strm_deflate.avail_in = (uInt)decompressed_size;
    strm_deflate.next_out = compressed_buffer.data();
    strm_deflate.avail_out = (uInt)compressed_buffer.size();
    
    // 11. Compress data
    ret = deflate(&strm_deflate, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK) {
        const char *err = zError(ret);
        // Continue anyway
    }
    
    // 12. Check pending data
    ret = deflatePending(&strm_deflate, &pending, &bits);
    
    // 13. Reset deflate stream keeping dictionary
    ret = deflateResetKeep(&strm_deflate);
    if (ret != Z_OK) {
        // Continue anyway
    }
    
    // 14. Use gzdopen for input file
    lseek(fd_in, 0, SEEK_SET);
    gzfile = gzdopen(fd_in, "rb");
    if (gzfile) {
        // 15. Set params on gzfile (using output file descriptor)
        gzFile gzfile_out = gzdopen(fd_out, "wb");
        if (gzfile_out) {
            ret = gzsetparams(gzfile_out, Z_DEFAULT_COMPRESSION, Z_DEFAULT_STRATEGY);
            
            // 16. Read from gzfile and write to output
            char buf[4096];
            int bytes_read;
            while ((bytes_read = gzread(gzfile, buf, sizeof(buf))) > 0) {
                // Process data if needed
            }
            
            // 17. Close gz files
            gzclose(gzfile_out);
        }
        gzclose(gzfile);
    }
    
    // 18. Clean up resources
    inflateEnd(&strm_inflate);
    deflateEnd(&strm_deflate);
    
    // Close files (already closed if gzfile was used)
    if (!gzfile) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
    }
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}