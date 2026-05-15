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
//<ID> 39
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["crc32","zlibVersion","gzdopen","inflateInit2_","zError","gzclose","inflateUndermine","inflateSetDictionary","inflate","inflateSetDictionary","inflate","zError","crc32","inflateEnd","gzclose","deflateInit2_","zError","deflateBound_z","compressBound","deflateSetDictionary","deflate","zError","deflateEnd","deflatePending","deflateResetKeep","deflate","deflateEnd","gzdopen","gzclose"],"critical_calls":["crc32","zlibVersion","gzdopen","inflateInit2_","inflateUndermine","inflateSetDictionary","inflate","inflateSetDictionary","inflate","zError","crc32","inflateEnd","gzclose","deflateInit2_","deflateBound_z","compressBound","deflateSetDictionary","deflate","deflatePending","deflateResetKeep","deflate","deflateEnd","gzdopen","gzclose"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified zlib APIs to implement a decompression and re-compression cycle with CRC32 checks:

*/


// Include zlib headers

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 50) {
	return 0;
	}
	 // Need minimum data for meaningful testing

    // Initialize variables
    z_stream strm_inflate, strm_deflate;
    int ret;
    uLong crc_val = crc32(0L, Z_NULL, 0);
    const char* zlib_ver = zlibVersion();
    
    // 1. Open input file using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Get file descriptor and create gzFile for reading
    int fd_in = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd_in, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 3. Initialize inflate stream for decompression
    memset(&strm_inflate, 0, sizeof(strm_inflate));
    ret = inflateInit2_(&strm_inflate, MAX_WBITS + 16, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        const char* err_msg = zError(ret);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 4. Process input in chunks
    const size_t CHUNK_SIZE = 4096;
    std::vector<Bytef> decompressed_data;
    Bytef in_buffer[CHUNK_SIZE];
    Bytef out_buffer[CHUNK_SIZE];
    
    // 5. Try to undermine inflate (test error conditions)
    if (size > 100) {
        inflateUndermine(&strm_inflate, data[0] & 1);
    }
    
    // 6. Set dictionary if we have enough data
    if (size > 256) {
        Bytef dict[256];
        memcpy(dict, data, 256);
        inflateSetDictionary(&strm_inflate, dict, 256);
    }
    
    // 7. Decompress the input
    do {
        size_t bytes_read = fread(in_buffer, 1, CHUNK_SIZE, in_file);
        if (bytes_read == 0) break;
        
        strm_inflate.next_in = in_buffer;
        strm_inflate.avail_in = bytes_read;
        
        do {
            strm_inflate.next_out = out_buffer;
            strm_inflate.avail_out = CHUNK_SIZE;
            
            ret = inflate(&strm_inflate, Z_NO_FLUSH);
            
            if (ret == Z_NEED_DICT && size > 256) {
                Bytef dict[256];
                memcpy(dict, data + 128, 256);
                inflateSetDictionary(&strm_inflate, dict, 256);
                ret = inflate(&strm_inflate, Z_NO_FLUSH);
            }
            
            if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR) {
                const char* err_msg = zError(ret);
                break;
            }
            
            size_t have = CHUNK_SIZE - strm_inflate.avail_out;
            decompressed_data.insert(decompressed_data.end(), out_buffer, out_buffer + have);
            
            // Update CRC during decompression
            crc_val = crc32(crc_val, out_buffer, have);
            
        } while (strm_inflate.avail_out == 0);
        
    } while (ret != Z_STREAM_END);
    
    // 8. End inflate stream
    inflateEnd(&strm_inflate);
    
    // 9. Close input files
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    
    if (decompressed_data.empty()) {
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
	}
	
    
    // 10. Initialize deflate stream for re-compression
    memset(&strm_deflate, 0, sizeof(strm_deflate));
    ret = deflateInit2_(&strm_deflate, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                       MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY,
                       ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        const char* err_msg = zError(ret);
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 11. Calculate compression bounds
    z_size_t deflate_bound = deflateBound_z(&strm_deflate, decompressed_data.size());
    uLong compress_bound = compressBound(decompressed_data.size());
    
    // 12. Set dictionary for compression
    if (decompressed_data.size() > 100) {
        Bytef dict[100];
        size_t dict_size = std::min((size_t)100, decompressed_data.size());
        memcpy(dict, decompressed_data.data(), dict_size);
        deflateSetDictionary(&strm_deflate, dict, dict_size);
    }
    
    // 13. Prepare output buffer
    std::vector<Bytef> compressed_data(deflate_bound);
    strm_deflate.next_in = decompressed_data.data();
    strm_deflate.avail_in = decompressed_data.size();
    strm_deflate.next_out = compressed_data.data();
    strm_deflate.avail_out = compressed_data.size();
    
    // 14. Perform compression
    ret = deflate(&strm_deflate, Z_FINISH);
    if (ret != Z_STREAM_END) {
        const char* err_msg = zError(ret);
        deflateEnd(&strm_deflate);
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 15. Check for pending data
    unsigned int pending;
    int bits;
    deflatePending(&strm_deflate, &pending, &bits);
    
    // 16. Reset deflate stream (keep internal state)
    deflateResetKeep(&strm_deflate);
    
    // 17. Try compressing again with reset stream
    std::vector<Bytef> recompressed_data(deflate_bound);
    strm_deflate.next_in = decompressed_data.data();
    strm_deflate.avail_in = decompressed_data.size();
    strm_deflate.next_out = recompressed_data.data();
    strm_deflate.avail_out = recompressed_data.size();
    
    ret = deflate(&strm_deflate, Z_FINISH);
    
    // 18. End deflate stream
    deflateEnd(&strm_deflate);
    
    // 19. Open output file and create gzFile
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
	}
	
    
    int fd_out = fuzz_fileno(out_file);
    gzFile gz_out = gzdopen(fd_out, "wb");
    if (!gz_out) {
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 20. Close gzFile (which will flush and close)
    gzclose(gz_out);
    
    // 21. Clean up output file
    assert_file_closed(&out_file);;
    
    // 22. Remove the output file we created
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}