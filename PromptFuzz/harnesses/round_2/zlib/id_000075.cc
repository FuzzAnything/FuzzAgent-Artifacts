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
//<ID> 246
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 2
//<Quality> {"density":10,"unique_branches":{"inflateSync":[[1299,9,1299,27,0,0,4,1]],"inflate":[[1100,21,1100,38,0,0,4,1]]},"library_calls":["gzdopen","gzbuffer","inflateInit2_","gzclose","inflateValidate","gzrewind","inflate","inflateSync","inflate","inflateEnd","deflateInit2_","deflateTune","deflateBound","deflate","deflatePrime","deflate","deflateCopy","deflateResetKeep","deflateEnd","deflateEnd","gzclose"],"critical_calls":["gzdopen","gzbuffer","inflateInit2_","inflateValidate","gzrewind","inflate","inflateSync","inflate","inflateEnd","deflateInit2_","deflateTune","deflateBound","deflate","deflatePrime","deflate","deflateCopy","deflateResetKeep","deflateEnd","deflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_75(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Check minimum size for meaningful operations
    if (size < 10) {
	return 0;
	}
	
    
    // Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create gzFile from file descriptor
    int fd_in = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd_in, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Set buffer size for gzFile (API: gzbuffer)
    gzbuffer(gz_in, 4096);
    
    // Allocate buffers for compression/decompression
    Bytef *decompressed = nullptr;
    Bytef *recompressed = nullptr;
    uLongf decompressed_len = 0;
    uLongf recompressed_len = 0;
    
    z_stream strm_inflate, strm_deflate, strm_deflate_copy;
    int ret = Z_OK;
    
    // Initialize inflate stream for decompression
    memset(&strm_inflate, 0, sizeof(z_stream));
    ret = inflateInit2(&strm_inflate, 15 + 16); // 15 window bits + 16 for gzip header
    if (ret != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Validate inflation (API: inflateValidate)
    inflateValidate(&strm_inflate, 1);
    
    // Try to read from gzFile using gzgetc (API: gzgetc)
    int gz_char = gzgetc(gz_in);
    if (gz_char == -1) {
        // No valid gzip data, try different approach
    }
    
    // Reset gz file position
    gzrewind(gz_in);
    
    // Try to decompress using inflateSync if needed
    // First attempt normal inflation
    Bytef inbuf[4096];
    Bytef outbuf[4096];
    strm_inflate.next_in = inbuf;
    strm_inflate.avail_in = 0;
    strm_inflate.next_out = outbuf;
    strm_inflate.avail_out = sizeof(outbuf);
    
    int flush = Z_NO_FLUSH;
    do {
        if (strm_inflate.avail_in == 0) {
            size_t read_size = fread(inbuf, 1, sizeof(inbuf), in_file);
            strm_inflate.next_in = inbuf;
            strm_inflate.avail_in = read_size;
            if (read_size < sizeof(inbuf)) {
                flush = Z_FINISH;
            }
        }
        
        ret = inflate(&strm_inflate, flush);
        
        // Try inflateSync if we encounter data error (API: inflateSync)
        if (ret == Z_DATA_ERROR) {
            inflateSync(&strm_inflate);
            ret = inflate(&strm_inflate, flush);
        }
        
        // Write decompressed data
        size_t have = sizeof(outbuf) - strm_inflate.avail_out;
        if (have > 0) {
            decompressed_len += have;
            decompressed = (Bytef*)realloc(decompressed, decompressed_len);
            memcpy(decompressed + decompressed_len - have, outbuf, have);
            strm_inflate.next_out = outbuf;
            strm_inflate.avail_out = sizeof(outbuf);
        }
    } while (ret == Z_OK);
    
    inflateEnd(&strm_inflate);
    
    // If we have decompressed data, try recompression with special features
    if (decompressed_len > 0 && decompressed != nullptr) {
        // Initialize deflate stream (API: deflateInit2_)
        memset(&strm_deflate, 0, sizeof(z_stream));
        ret = deflateInit2(&strm_deflate, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                          15 + 16, 8, Z_DEFAULT_STRATEGY);
        
        if (ret == Z_OK) {
            // Tune deflate parameters (API: deflateTune)
            deflateTune(&strm_deflate, 8, 9, 8, 4096);
            
            // Set up input for deflation
            strm_deflate.next_in = decompressed;
            strm_deflate.avail_in = decompressed_len;
            
            // Allocate output buffer
            recompressed_len = deflateBound(&strm_deflate, decompressed_len);
            recompressed = (Bytef*)malloc(recompressed_len);
            strm_deflate.next_out = recompressed;
            strm_deflate.avail_out = recompressed_len;
            
            // Start deflation
            ret = deflate(&strm_deflate, Z_NO_FLUSH);
            
            // Insert some bits using deflatePrime (API: deflatePrime)
            if (ret == Z_OK && strm_deflate.avail_in > 100) {
                // Insert 5 bits with value 0x0A
                deflatePrime(&strm_deflate, 5, 0x0A);
            }
            
            // Continue deflation
            ret = deflate(&strm_deflate, Z_FINISH);
            
            if (ret == Z_STREAM_END) {
                recompressed_len = recompressed_len - strm_deflate.avail_out;
                
                // Create a copy of the deflate stream (API: deflateCopy)
                memset(&strm_deflate_copy, 0, sizeof(z_stream));
                ret = deflateCopy(&strm_deflate_copy, &strm_deflate);
                
                if (ret == Z_OK) {
                    // Reset the original stream while keeping internal state (API: deflateResetKeep)
                    deflateResetKeep(&strm_deflate);
                    
                    // Write recompressed data
                    fwrite(recompressed, 1, recompressed_len, out_file);
                    
                    // Clean up copied stream
                    deflateEnd(&strm_deflate_copy);
                }
            }
            
            deflateEnd(&strm_deflate);
        }
    }
    
    // Clean up
    if (decompressed) free(decompressed);
    if (recompressed) free(recompressed);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_in);
	return 0;
}