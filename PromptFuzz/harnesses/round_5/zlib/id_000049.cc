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
//<ID> 98
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 1
//<Quality> {"density":16,"unique_branches":{"inflate":[[494,36,494,60,0,0,4,0]]},"library_calls":["gzdopen","gzclearerr","inflateInit2_","gzclose","deflateInit2_","inflateEnd","gzclose","deflateSetHeader","crc32","crc32","gzread","gztell","gzoffset64","crc32","crc32","inflate","inflateGetDictionary","inflateUndermine","deflateBound","deflate","deflatePending","gzwrite","gzdopen","crc32_combine","compressBound_z","inflate","inflateGetDictionary","inflateCodesUsed","inflateEnd","deflateEnd","gzclose"],"critical_calls":["gzdopen","gzclearerr","inflateInit2_","deflateInit2_","deflateSetHeader","crc32","crc32","gzread","gztell","gzoffset64","crc32","crc32","inflate","inflateGetDictionary","inflateUndermine","deflateBound","deflate","deflatePending","gzwrite","gzdopen","crc32_combine","compressBound_z","inflate","inflateGetDictionary","inflateCodesUsed","inflateEnd","deflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Create input file from memory buffer
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

    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);

    // 1. Open compressed stream for reading
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

    // 2. Clear any existing errors
    gzclearerr(gz_in);

    // 3. Initialize decompression stream
    z_stream strm_inflate;
    memset(&strm_inflate, 0, sizeof(strm_inflate));
    int ret = inflateInit2_(&strm_inflate, MAX_WBITS + 16, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 4. Initialize compression stream
    z_stream strm_deflate;
    memset(&strm_deflate, 0, sizeof(strm_deflate));
    ret = deflateInit2_(&strm_deflate, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                       MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY,
                       ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        inflateEnd(&strm_inflate);
        gzclose(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 5. Set gzip header for compression
    gz_header gzheader;
    memset(&gzheader, 0, sizeof(gzheader));
    gzheader.os = 255; // Unknown OS
    deflateSetHeader(&strm_deflate, &gzheader);

    // Read buffer
    const size_t BUF_SIZE = 4096;
    Bytef in_buf[BUF_SIZE];
    Bytef out_buf[BUF_SIZE];
    
    // CRC32 values
    uLong crc1 = crc32(0L, Z_NULL, 0);
    uLong crc2 = crc32(0L, Z_NULL, 0);
    
    // Dictionary
    Bytef dictionary[32768];
    uInt dictLength = 0;
    
    // Variables for pending bits
    unsigned int pending;
    int bits;
    
    // Variables for tracking
    off_t tell_pos;
    off64_t offset64_pos;
    
    // Read from gzFile and process
    int bytes_read;
    while ((bytes_read = gzread(gz_in, in_buf, BUF_SIZE)) > 0) {
        // 6. Get current position
        tell_pos = gztell(gz_in);
        
        // 7. Get offset (64-bit)
        offset64_pos = gzoffset64(gz_in);
        
        // 8. Calculate CRC32 of first half
        if (bytes_read > 1) {
            crc1 = crc32(crc1, in_buf, bytes_read / 2);
            crc2 = crc32(crc2, in_buf + bytes_read / 2, bytes_read - bytes_read / 2);
        }
        
        // 9. Set input for inflation
        strm_inflate.next_in = in_buf;
        strm_inflate.avail_in = bytes_read;
        
        // 10. Inflate the data
        do {
            strm_inflate.next_out = out_buf;
            strm_inflate.avail_out = BUF_SIZE;
            ret = inflate(&strm_inflate, Z_NO_FLUSH);
            
            // 11. Get dictionary if available
            inflateGetDictionary(&strm_inflate, dictionary, &dictLength);
            
            // 12. Try to undermine (may fail, that's OK)
            inflateUndermine(&strm_inflate, data[0] & 1);
            
        } while (strm_inflate.avail_out == 0 && ret == Z_OK);
        
        // 13. Get bound for compressed size
        uLong bound = deflateBound(&strm_deflate, strm_inflate.total_out);
        
        // 14. Set input for deflation (use inflated data if available)
        if (strm_inflate.total_out > 0) {
            strm_deflate.next_in = out_buf;
            strm_deflate.avail_in = BUF_SIZE - strm_inflate.avail_out;
            
            // 15. Deflate the data
            do {
                strm_deflate.next_out = in_buf; // Reuse buffer
                strm_deflate.avail_out = BUF_SIZE;
                ret = deflate(&strm_deflate, Z_NO_FLUSH);
                
                // 16. Check pending bits
                deflatePending(&strm_deflate, &pending, &bits);
                
            } while (strm_deflate.avail_out == 0 && ret == Z_OK);
            
            // 17. Write compressed data to gzFile
            gzwrite(gzdopen(fd_out, "wb"), in_buf, BUF_SIZE - strm_deflate.avail_out);
        }
    }
    
    // 18. Combine CRC32 values
    uLong combined_crc = crc32_combine(crc1, crc2, size);
    
    // 19. Get compression bound using z_size_t version
    z_size_t zsize = size;
    z_size_t compress_bound = compressBound_z(zsize);
    
    // 20. Second inflate call (with flush)
    strm_inflate.next_in = Z_NULL;
    strm_inflate.avail_in = 0;
    inflate(&strm_inflate, Z_FINISH);
    
    // 21. Get dictionary again
    inflateGetDictionary(&strm_inflate, dictionary, &dictLength);
    
    // 22. Get codes used
    unsigned long codes_used = inflateCodesUsed(&strm_inflate);
    
    // Cleanup
    inflateEnd(&strm_inflate);
    deflateEnd(&strm_deflate);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Use variables to avoid unused warnings
    (void)tell_pos;
    (void)offset64_pos;
    (void)combined_crc;
    (void)compress_bound;
    (void)codes_used;
    (void)pending;
    (void)bits;
    (void)dictLength;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}