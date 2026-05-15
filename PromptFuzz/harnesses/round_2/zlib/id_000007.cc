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
//<ID> 10
//<Prompt> []
/*<Combination>: [
*/
//<score> 2.5, nr_unique_branch: 0
//<Quality> {"density":5,"unique_branches":{},"library_calls":["compressBound","uncompress_z","inflateInit2_","inflate","inflateResetKeep","inflateValidate","inflateEnd","deflateInit_","deflate","deflatePending","deflateEnd","gzdopen","gztell","gzclose","adler32","adler32","adler32_combine","get_crc_table"],"critical_calls":["compressBound","uncompress_z","inflateInit2_","inflate","inflateResetKeep","inflateValidate","inflateEnd","deflateInit_","deflate","deflatePending","deflateEnd","gzdopen","gztell","gzclose","adler32","adler32","adler32_combine","get_crc_table"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Variables for various operations
    int ret;
    z_stream strm_inflate, strm_deflate;
    Bytef *dest = nullptr;
    z_size_t destLen;
    gzFile file = nullptr;
    unsigned int pending;
    int bits;
    off_t tell_pos;
    
    // 1. First, try to uncompress the input data using uncompress_z
    // We need to estimate the output buffer size
    uLong bound = compressBound(static_cast<uLong>(size));
    destLen = static_cast<z_size_t>(bound);
    dest = static_cast<Bytef*>(malloc(destLen));
    if (!dest) {
	return 0;
	}
	
    
    ret = uncompress_z(dest, &destLen, data, static_cast<z_size_t>(size));
    // We'll continue even if uncompress fails, as we want to test other APIs
    
    // 2. Setup inflate stream for validation
    memset(&strm_inflate, 0, sizeof(strm_inflate));
    strm_inflate.next_in = const_cast<Bytef*>(data);
    strm_inflate.avail_in = static_cast<uInt>(size);
    
    ret = inflateInit2(&strm_inflate, MAX_WBITS + 16); // Auto-detect gzip/zlib
    if (ret == Z_OK) {
        // Try to inflate a little
        Bytef out_buf[1024];
        strm_inflate.next_out = out_buf;
        strm_inflate.avail_out = sizeof(out_buf);
        inflate(&strm_inflate, Z_SYNC_FLUSH);
        
        // Reset and keep the dictionary
        inflateResetKeep(&strm_inflate);
        
        // Validate the stream
        inflateValidate(&strm_inflate, 1); // Enable validation
        
        // Clean up inflate stream
        inflateEnd(&strm_inflate);
    }
    
    // 3. Setup deflate stream to check pending bits
    memset(&strm_deflate, 0, sizeof(strm_deflate));
    deflateInit(&strm_deflate, Z_DEFAULT_COMPRESSION);
    
    // Use some data for deflation (use uncompressed data if available, otherwise use original)
    Bytef* src_for_deflate;
    uInt src_len;
    if (destLen > 0 && ret == Z_OK) {
        src_for_deflate = dest;
        src_len = static_cast<uInt>(destLen > 65536 ? 65536 : destLen);
    } else {
        src_for_deflate = const_cast<Bytef*>(data);
        src_len = static_cast<uInt>(size > 65536 ? 65536 : size);
    }
    
    strm_deflate.next_in = src_for_deflate;
    strm_deflate.avail_in = src_len;
    
    // Deflate some data
    Bytef deflate_out[1024];
    strm_deflate.next_out = deflate_out;
    strm_deflate.avail_out = sizeof(deflate_out);
    deflate(&strm_deflate, Z_SYNC_FLUSH);
    
    // Check for pending bits
    deflatePending(&strm_deflate, &pending, &bits);
    
    // Clean up deflate stream
    deflateEnd(&strm_deflate);
    
    // 4. Create a gzFile from the input data and get position
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
	int fuzz_fd_0 = fuzz_fileno(in_file);

        file = gzdopen(fuzz_fd_0, "rb");
        if (file) {
            // Read a byte to move position
            gzgetc(file);
            tell_pos = gztell(file);
            gzclose(file);
        }
        assert_file_closed(&in_file);;
    }
    
    // 5. Use adler32_combine if we have at least two checksums
    // Create two dummy Adler-32 checksums and combine them
    uLong adler1 = adler32(0L, Z_NULL, 0);
    uLong adler2 = adler32(0L, data, static_cast<uInt>(size > 10 ? 10 : size));
    uLong combined = adler32_combine(adler1, adler2, 100);
    (void)combined; // Prevent unused variable warning
    
    // 6. Get CRC table (mostly for coverage)
    const z_crc_t *crc_table = get_crc_table();
    (void)crc_table; // Prevent unused variable warning
    
    // 7. Clean up
    if (dest) free(dest);
    
    assert_file_closed(&in_file);
	return 0;
}