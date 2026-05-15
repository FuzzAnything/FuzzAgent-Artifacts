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
//<ID> 57
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":7,"unique_branches":{"inflate":[[1119,9,1119,18,0,0,4,0]]},"library_calls":["gzdopen","inflateInit2_","inflateSync","inflate","inflateEnd","gzrewind","gzgetc_","gzclearerr","crc32","crc32","crc32","crc32","crc32_combine","crc32_combine_gen","crc32_combine_op","gzclose"],"critical_calls":["gzdopen","inflateInit2_","inflateSync","inflate","inflateEnd","gzrewind","gzgetc_","gzclearerr","crc32","crc32","crc32","crc32","crc32_combine","crc32_combine_gen","crc32_combine_op","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Get file descriptor and open as gzFile
    int fd = fuzz_fileno(in_file);
    if (fd == -1) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    gzFile gz = gzdopen(fd, "rb");
    if (!gz) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 3. Set up z_stream for inflation
    z_stream strm;
    Bytef *out_buffer = nullptr;
    uLongf out_len = 0;
    int ret = Z_OK;
    
    // Initialize z_stream
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    
    // 4. Try to inflate the data
    ret = inflateInit2(&strm, 15 + 32); // 15 window bits + 32 for auto header detection
    if (ret == Z_OK) {
        // Prepare input buffer
        strm.avail_in = size;
        strm.next_in = (Bytef *)data;
        
        // Prepare output buffer
        out_len = size * 2; // Estimate
        if (out_len < 100) out_len = 100;
        out_buffer = (Bytef *)malloc(out_len);
        if (out_buffer) {
            strm.avail_out = out_len;
            strm.next_out = out_buffer;
            
            // Call inflateSync (this API must be used)
            inflateSync(&strm);
            
            // Try to inflate
            ret = inflate(&strm, Z_FINISH);
            
            inflateEnd(&strm);
        }
    }
    
    // 5. Read from gzFile using gzgetc_ (this API must be used)
    gzrewind(gz); // Rewind to start
    int gz_char;
    while ((gz_char = gzgetc_(gz)) != -1) {
        // Just read through the file
    }
    
    // 6. Clear any errors on gzFile (this API must be used)
    gzclearerr(gz);
    
    // 7. Compute and combine CRC32 values (these APIs must be used)
    uLong crc1 = crc32(0L, Z_NULL, 0);
    uLong crc2 = crc32(0L, Z_NULL, 0);
    
    // Compute CRC32 for first half of data
    if (size > 0) {
        size_t half1 = size / 2;
        if (half1 > 0) {
            crc1 = crc32(crc1, data, half1);
        }
        
        // Compute CRC32 for second half of data
        size_t half2 = size - half1;
        if (half2 > 0) {
            crc2 = crc32(0L, data + half1, half2);
        }
        
        // Combine CRCs using crc32_combine (this API must be used)
        off_t len2 = half2;
        uLong combined_crc = crc32_combine(crc1, crc2, len2);
        
        // Also use crc32_combine_op (this API must be used)
        uLong op = crc32_combine_gen(len2);
        uLong combined_crc_op = crc32_combine_op(crc1, crc2, op);
        
        // The results should be identical
        // For fuzzing, we just call the APIs, don't need to verify
        (void)combined_crc;
        (void)combined_crc_op;
    }
    
    // 8. Cleanup
    if (out_buffer) {
        free(out_buffer);
    }
    
    gzclose(gz);  // This will also close the file descriptor
    // Note: gzclose closes the underlying fd, so we don't need to assert_file_closed(&in_file);
    // But we should still check if it needs closing
    assert_file_closed(&in_file);;  // Safe to call even if gzclose closed it
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}