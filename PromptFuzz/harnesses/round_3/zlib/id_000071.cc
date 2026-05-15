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
//<ID> 178
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 1
//<Quality> {"density":10,"unique_branches":{"deflateReset":[[708,9,708,20,0,0,4,1]]},"library_calls":["compressBound_z","uncompress_z","gzdopen","gzclose","inflateInit2_","gzclose","inflate","inflatePrime","inflateCopy","inflateEnd","deflateReset","gzgetc_","gzrewind","gzoffset","gzoffset64","gzprintf","gzerror","inflateEnd","deflateEnd","gzclose"],"critical_calls":["compressBound_z","uncompress_z","gzdopen","inflateInit2_","inflate","inflatePrime","inflateCopy","inflateEnd","deflateReset","gzgetc_","gzrewind","gzoffset","gzoffset64","gzprintf","gzerror","inflateEnd","deflateEnd","gzclose"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified zlib APIs to achieve decompression and file operations on zlib-compressed data:

*/


extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t* f_data, size_t f_size) {
	if(f_size<34) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	 // Need minimum data for meaningful operations

    // Event: Decompress zlib-compressed data using various methods (stream and file APIs),
    //        then perform file operations on the decompressed data

    // 1. First, try to decompress using uncompress_z (if data appears to be zlib compressed)
    // Allocate buffer for decompressed data
    z_size_t destLen = compressBound_z(size) * 2; // Estimate decompressed size
    std::vector<Bytef> decompressed(destLen);
    
    // Try uncompress_z - this might fail if input isn't valid zlib data
    int uncompress_result = uncompress_z(decompressed.data(), &destLen, 
                                         data, size);
    
    // 2. Create FILE* for input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 3. Create gzFile from file descriptor
    int fd = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd, fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 4. Create output FILE for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 5. Setup z_stream for inflation
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    
    // Initialize inflation with windowBits parameter
    int init_result = inflateInit2_(&strm, fuzz_int32_t_3, fuzz_str_2, sizeof(z_stream));
    if (init_result != Z_OK) {
        assert_file_closed(&out_file);;
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 6. Set input for inflation
    strm.avail_in = size;
    strm.next_in = (Bytef *)data;
    
    // 7. Prepare output buffer for inflation
    std::vector<Bytef> inflate_out(4096);
    strm.avail_out = inflate_out.size();
    strm.next_out = inflate_out.data();
    
    // 8. Perform inflation (decompression)
    int inflate_result = inflate(&strm, fuzz_int32_t_4);
    
    // 9. Use inflatePrime to add bits to the stream (simulate partial data)
    if (size > 20) {
        inflatePrime(&strm, fuzz_int32_t_5, data[0]); // Add 8 bits from first byte
    }
    
    // 10. Create another stream for copy operation
    z_stream strm_copy;
    memset(&strm_copy, 0, sizeof(z_stream));
    
    // 11. Copy the inflation stream state
    int copy_result = inflateCopy(&strm_copy, &strm);
    if (copy_result == Z_OK) {
        // Process with copied stream
        inflateEnd(&strm_copy);
    }
    
    // 12. Reset a deflation stream (even though we're mainly inflating)
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(z_stream));
    deflateReset(&def_strm); // This will reset an uninitialized stream
    
    // 13. Read from gzFile using gzgetc_
    rewind(in_file); // Reset file pointer
    int gz_char = gzgetc_(gz_in);
    
    // 14. Rewind gzFile
    gzrewind(gz_in);
    
    // 15. Get file offset using both gzoffset variants
    off_t offset32 = gzoffset(gz_in);
    off64_t offset64 = gzoffset64(gz_in);
    
    // 16. Write to gzFile using gzprintf (though opened for reading, this may fail)
    // We'll try it but ignore errors since file is read-only
    gzprintf(gz_in, "Test format string %d", 123);
    
    // 17. Check for errors in gzFile operations
    int gz_err = 0;
    const char *gz_err_str = gzerror(gz_in, &gz_err);
    
    // 18. Write decompressed data to output file if uncompress succeeded
    if (uncompress_result == Z_OK && destLen > 0) {
        fwrite(decompressed.data(), 1, destLen, out_file);
    }
    
    // 19. Write inflated data to output file
    if (inflate_result == Z_OK || inflate_result == Z_STREAM_END) {
        size_t inflated_bytes = inflate_out.size() - strm.avail_out;
        if (inflated_bytes > 0) {
            fwrite(inflate_out.data(), 1, inflated_bytes, out_file);
        }
    }
    
    // 20. Cleanup all resources
    inflateEnd(&strm);
    deflateEnd(&def_strm); // End deflation stream
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove temporary output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}