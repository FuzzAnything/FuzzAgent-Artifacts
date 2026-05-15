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
//<ID> 41
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 5
//<Quality> {"density":9,"unique_branches":{"gzoffset64":[[482,9,482,21,0,0,4,0]],"gz_comp":[[116,21,116,29,0,0,4,0],[117,25,117,40,0,0,4,1],[117,44,117,64,0,0,4,1]],"gzclose_w":[[696,9,696,31,0,0,4,0]]},"library_calls":["inflateInit2_","deflateInit2_","inflateEnd","deflateSetHeader","deflateBound","crc32","crc32","inflate","inflateGetDictionary","inflateSetDictionary","inflate","inflateUndermine","deflate","deflatePending","inflateReset","deflate","gzdopen","gzwrite","gztell","gzoffset64","gzclearerr","gzclose","compressBound_z","compress_z","crc32","crc32","crc32","crc32","crc32_combine","inflateEnd","deflateEnd"],"critical_calls":["inflateInit2_","deflateInit2_","deflateSetHeader","deflateBound","crc32","crc32","inflate","inflateGetDictionary","inflateSetDictionary","inflate","inflateUndermine","deflate","deflatePending","inflateReset","deflate","gzdopen","gzwrite","gztell","gzoffset64","gzclearerr","gzclose","compressBound_z","compress_z","crc32","crc32","crc32","crc32","crc32_combine","inflateEnd","deflateEnd"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create input file from the fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file for compressed data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    
    // Initialize zlib structures
    z_stream strm_inflate, strm_deflate;
    gz_header head;
    Bytef *dict = nullptr;
    uInt dict_len = 0;
    unsigned int pending;
    int bits;
    int ret;
    
    // Initialize inflate stream for decompression
    memset(&strm_inflate, 0, sizeof(strm_inflate));
    ret = inflateInit2_(&strm_inflate, MAX_WBITS + 16, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Initialize deflate stream for recompression
    memset(&strm_deflate, 0, sizeof(strm_deflate));
    ret = deflateInit2_(&strm_deflate, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                       MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY,
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
    
    // Setup gz_header for deflate
    memset(&head, 0, sizeof(head));
    head.os = 255; // Unknown OS
    deflateSetHeader(&strm_deflate, &head);
    
    // Allocate buffers
    const size_t CHUNK = 64 * 1024;
    Bytef *in_buf = new Bytef[CHUNK];
    Bytef *out_buf = new Bytef[CHUNK];
    Bytef *deflate_out = new Bytef[deflateBound(&strm_deflate, CHUNK)];
    
    // Read and process data in chunks
    while (!feof(in_file) && !ferror(in_file)) {
        size_t bytes_read = fread(in_buf, 1, CHUNK, in_file);
        if (bytes_read == 0) break;
        
        // Calculate CRC of input chunk
        uLong crc_input = crc32(0L, Z_NULL, 0);
        crc_input = crc32(crc_input, in_buf, bytes_read);
        
        // Setup inflate input
        strm_inflate.avail_in = bytes_read;
        strm_inflate.next_in = in_buf;
        
        // Inflate (decompress) the data
        do {
            strm_inflate.avail_out = CHUNK;
            strm_inflate.next_out = out_buf;
            
            ret = inflate(&strm_inflate, Z_NO_FLUSH);
            if (ret == Z_NEED_DICT) {
                // Try to get dictionary if needed
                if (dict == nullptr) {
                    dict = new Bytef[32768];
                    dict_len = 32768;
                }
                inflateGetDictionary(&strm_inflate, dict, &dict_len);
                ret = inflateSetDictionary(&strm_inflate, dict, dict_len);
                if (ret != Z_OK) break;
                ret = inflate(&strm_inflate, Z_NO_FLUSH);
            }
            
            if (ret < Z_OK) {
                // Try undermine on error
                inflateUndermine(&strm_inflate, 1);
                break;
            }
            
            size_t have = CHUNK - strm_inflate.avail_out;
            
            // Setup deflate with decompressed data
            strm_deflate.avail_in = have;
            strm_deflate.next_in = out_buf;
            
            // Deflate (recompress) the data
            do {
                strm_deflate.avail_out = CHUNK;
                strm_deflate.next_out = deflate_out;
                
                ret = deflate(&strm_deflate, Z_NO_FLUSH);
                if (ret < Z_OK) break;
                
                size_t deflate_have = CHUNK - strm_deflate.avail_out;
                
                // Write compressed data to file
                fwrite(deflate_out, 1, deflate_have, out_file);
                
            } while (strm_deflate.avail_out == 0);
            
        } while (strm_inflate.avail_out == 0);
        
        // Check pending bits in deflate
        deflatePending(&strm_deflate, &pending, &bits);
        
        // Reset inflate for next chunk
        inflateReset(&strm_inflate);
    }
    
    // Finalize deflate
    do {
        strm_deflate.avail_out = CHUNK;
        strm_deflate.next_out = deflate_out;
        ret = deflate(&strm_deflate, Z_FINISH);
        size_t have = CHUNK - strm_deflate.avail_out;
        fwrite(deflate_out, 1, have, out_file);
    } while (strm_deflate.avail_out == 0);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Open output file for gz operations
    gzFile gz_out = gzdopen(fd_out, "wb");
    if (gz_out) {
        // Write some data using gzwrite
        Bytef test_data[] = "test";
        gzwrite(gz_out, test_data, sizeof(test_data) - 1);
        
        // Get position information
        off_t pos = gztell(gz_out);
        off64_t offset = gzoffset64(gz_out);
        
        // Clear any errors
        gzclearerr(gz_out);
        
        // Close gz file
        gzclose(gz_out);
    }
    
    // Use compress_z API
    if (size > 0) {
        z_size_t dest_len = compressBound_z(size);
        Bytef *compressed = new Bytef[dest_len];
        compress_z(compressed, &dest_len, data, size);
        delete[] compressed;
    }
    
    // Combine CRC values (example)
    uLong crc1 = crc32(0L, Z_NULL, 0);
    uLong crc2 = crc32(0L, Z_NULL, 0);
    if (size > 10) {
        crc1 = crc32(crc1, data, 5);
        crc2 = crc32(crc2, data + 5, 5);
        uLong combined = crc32_combine(crc1, crc2, 10);
    }
    
    // Clean up
    delete[] in_buf;
    delete[] out_buf;
    delete[] deflate_out;
    delete[] dict;
    
    inflateEnd(&strm_inflate);
    deflateEnd(&strm_deflate);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}