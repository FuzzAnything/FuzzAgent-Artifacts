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
//<ID> 47
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 6
//<Quality> {"density":7,"unique_branches":{"gzgetc":[[481,9,481,31,0,0,4,0]],"gzsetparams":[[641,9,641,32,0,0,4,1],[641,37,641,55,0,0,4,1],[642,13,642,26,0,0,4,1],[647,9,647,30,0,0,4,0],[647,34,647,61,0,0,4,0]]},"library_calls":["zlibVersion","inflateInit2_","inflate","inflateCodesUsed","inflateEnd","gzdopen","gzsetparams","deflateInit_","gzclose","deflateCopy","deflateEnd","gzclose","deflateUsed","gzgetc_","gzputs","crc32_z","crc32_z","compressBound_z","compress_z","deflateEnd","deflateEnd","gzclose"],"critical_calls":["zlibVersion","inflateInit2_","inflate","inflateCodesUsed","inflateEnd","gzdopen","gzsetparams","deflateInit_","deflateCopy","deflateUsed","gzgetc_","gzputs","crc32_z","crc32_z","compressBound_z","compress_z","deflateEnd","deflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<32) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
	//fuzzer shim end}




    // 1. Get zlib version
    const char* version = zlibVersion();
    (void)version; // Mark as used
    
    // 2. Prepare decompression stream for input data
    z_stream decomp_stream;
    memset(&decomp_stream, 0, sizeof(decomp_stream));
    
    // Initialize decompression with auto header detection
    if (inflateInit2(&decomp_stream, 15 + 32) != Z_OK) {
        return 0;
    }
    
    // Set input for decompression
    decomp_stream.next_in = (Bytef*)data;
    decomp_stream.avail_in = (uInt)size;
    
    // Allocate output buffer for decompressed data
    std::vector<Bytef> decompressed_buf(64 * 1024); // 64KB buffer
    decomp_stream.next_out = decompressed_buf.data();
    decomp_stream.avail_out = (uInt)decompressed_buf.size();
    
    // 3. Decompress the input data
    int decomp_result = inflate(&decomp_stream, fuzz_int32_t_1);
    size_t decompressed_size = decompressed_buf.size() - decomp_stream.avail_out;
    
    // 4. Get inflate codes usage information
    unsigned long codes_used = inflateCodesUsed(&decomp_stream);
    (void)codes_used; // Mark as used
    
    // 5. Clean up decompression stream
    inflateEnd(&decomp_stream);
    
    if (decomp_result != Z_STREAM_END && decomp_result != Z_OK) {
        // Decompression failed, but continue to use other APIs
        decompressed_size = 0;
    }
    
    // 6. Create FILE* for reading decompressed data
    FILE* in_file = fmemopen((void*)decompressed_buf.data(), decompressed_size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 7. Open gzFile for writing with compression
    int fd_out = fuzz_fileno(fopen("output_file", "wb"));
    if (fd_out < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	return 0;
    }
    
    gzFile gz_out = gzdopen(fd_out, fuzz_str_2);
    if (!gz_out) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	return 0;
    }
    
    // 8. Set compression parameters on gzFile
    gzsetparams(gz_out, fuzz_int32_t_3, fuzz_int32_t_4);
    
    // 9. Prepare compression stream for copying
    z_stream comp_stream_src, comp_stream_dest;
    memset(&comp_stream_src, 0, sizeof(comp_stream_src));
    memset(&comp_stream_dest, 0, sizeof(comp_stream_dest));
    
    // Initialize source compression stream
    if (deflateInit(&comp_stream_src, Z_DEFAULT_COMPRESSION) != Z_OK) {
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	return 0;
    }
    
    // 10. Copy compression stream state
    if (deflateCopy(&comp_stream_dest, &comp_stream_src) != Z_OK) {
        deflateEnd(&comp_stream_src);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	return 0;
    }
    
    // 11. Get deflate used bits information
    int bits_used;
    deflateUsed(&comp_stream_src, &bits_used);
    (void)bits_used; // Mark as used
    
    // 12. Read from decompressed data and write to gzFile
    int c;
    while ((c = gzgetc_(gz_out)) != -1) {
        // Just reading to use gzgetc_, not actually using the value
    }
    
    // 13. Write a test string to gzFile
    gzputs(gz_out, fuzz_str_5);
    
    // 14. Compute CRC32 of decompressed data
    uLong crc = crc32_z(fuzz_uint64_t_6, Z_NULL, 0);
    if (decompressed_size > 0) {
        crc = crc32_z(crc, decompressed_buf.data(), decompressed_size);
    }
    (void)crc; // Mark as used
    
    // 15. Compress the decompressed data using compress_z
    z_size_t comp_dest_len = compressBound_z(decompressed_size);
    std::vector<Bytef> comp_dest(comp_dest_len);
    
    if (decompressed_size > 0) {
        int comp_result = compress_z(comp_dest.data(), &comp_dest_len, 
                                    decompressed_buf.data(), decompressed_size);
        (void)comp_result; // Mark as used
    }
    
    // 16. Clean up all resources
    deflateEnd(&comp_stream_src);
    deflateEnd(&comp_stream_dest);
    gzclose(gz_out);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	return 0;
}