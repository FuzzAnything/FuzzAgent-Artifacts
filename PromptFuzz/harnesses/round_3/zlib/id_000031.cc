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
//<ID> 55
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 5
//<Quality> {"density":5,"unique_branches":{"deflate_slow":[[2073,9,2073,20,0,0,4,0],[1644,8,1644,31,27,0,4,1],[1631,24,1631,44,44,0,4,0]],"inflateSyncPoint":[[1323,9,1323,32,0,0,4,1],[1325,12,1325,33,0,0,4,1]]},"library_calls":["gzdopen","deflateInit_","gzclose","gzgets","gztell64","compress_z","inflateInit2_","inflate","inflateSyncPoint","inflateEnd","deflate","deflateResetKeep","deflateEnd","gzclose"],"critical_calls":["gzdopen","deflateInit_","gzgets","gztell64","compress_z","inflateInit2_","inflate","inflateSyncPoint","inflateEnd","deflate","deflateResetKeep","deflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<51) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}




    // 1. Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Create gzFile from FILE descriptor
    int fd = fuzz_fileno(in_file);
    gzFile gz = gzdopen(fd, fuzz_str_1);
    if (!gz) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 3. Initialize deflate stream
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    // Use deflateInit_ with ZLIB_VERSION
    int ret = deflateInit_(&strm, fuzz_int32_t_3, fuzz_str_2, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 4. Read from gzFile using gzgets
    char buffer[1024];
    if (gzgets(gz, buffer, sizeof(buffer))) {
        // 5. Get current position in gzFile using gztell64
        off64_t pos = gztell64(gz);
        (void)pos; // Use pos to avoid unused variable warning
    }
    
    // 6. Use compress_z to compress some data
    // We'll compress the buffer we read (or a default string if buffer is empty)
    Bytef dest[2048];
    z_size_t destLen = sizeof(dest);
    const char *source = (strlen(buffer) > 0) ? buffer : "test data";
    z_size_t sourceLen = strlen(source);
    
    // Note: compress_z expects z_size_t for lengths
    compress_z(dest, &destLen, (const Bytef *)source, sourceLen);
    
    // 7. Check sync point in inflate stream
    // First create an inflate stream
    z_stream inflate_strm;
    inflate_strm.zalloc = Z_NULL;
    inflate_strm.zfree = Z_NULL;
    inflate_strm.opaque = Z_NULL;
    inflate_strm.next_in = Z_NULL;
    inflate_strm.avail_in = 0;
    
    if (inflateInit2_(&inflate_strm, 15 + 16, fuzz_str_4, sizeof(z_stream)) == Z_OK) {
        // Feed some compressed data to inflate stream
        inflate_strm.next_in = dest;
        inflate_strm.avail_in = (uInt)destLen;
        
        // Try to inflate a bit
        Bytef out[1024];
        inflate_strm.next_out = out;
        inflate_strm.avail_out = sizeof(out);
        inflate(&inflate_strm, fuzz_int32_t_5);
        
        // Check sync point
        int sync = inflateSyncPoint(&inflate_strm);
        (void)sync; // Use sync to avoid unused variable warning
        
        inflateEnd(&inflate_strm);
    }
    
    // 8. Use deflateResetKeep on the deflate stream
    // First feed some data to the deflate stream
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)sourceLen;
    
    Bytef deflate_out[1024];
    strm.next_out = deflate_out;
    strm.avail_out = sizeof(deflate_out);
    deflate(&strm, fuzz_int32_t_6);
    
    // Now reset keep
    deflateResetKeep(&strm);
    
    // 9. Clean up
    deflateEnd(&strm);
    gzclose(gz);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}