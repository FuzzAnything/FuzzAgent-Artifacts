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
//<ID> 64
//<Prompt> []
/*<Combination>: [
*/
//<score> 9.5, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["gzdopen","gzdopen","gzclose","gzclose_w","gzdirect","inflateInit_","gzclose","gzclose_w","inflateValidate","deflateInit_","inflateEnd","gzclose","gzclose_w","inflateBackInit_","deflateEnd","inflateEnd","gzclose","gzclose_w","compressBound","compress2_z","uncompress2_z","gzread","gztell64","gzfwrite","gzflush","inflate","deflate","deflateUsed","crc32","crc32","crc32","crc32","crc32_combine64","gztell64","gzdirect","inflateBackEnd","inflateEnd","deflateEnd","gzclose","gzclose_w"],"critical_calls":["gzdopen","gzdopen","gzdirect","inflateInit_","inflateValidate","deflateInit_","inflateBackInit_","compressBound","compress2_z","uncompress2_z","gzread","gztell64","gzfwrite","gzflush","inflate","deflate","deflateUsed","crc32","crc32","crc32","crc32","crc32_combine64","gztell64","gzdirect","inflateBackEnd","inflateEnd","deflateEnd","gzclose","gzclose_w"],"visited":1}
/*Here's a comprehensive fuzz driver that uses all the specified zlib APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<96) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_12, fdp);
	//fuzzer shim end}




    // 1. Create input file from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Create output files
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Get file descriptors
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    
    // 4. Initialize compression and decompression streams
    z_stream comp_strm, decomp_strm, back_strm;
    memset(&comp_strm, 0, sizeof(z_stream));
    memset(&decomp_strm, 0, sizeof(z_stream));
    memset(&back_strm, 0, sizeof(z_stream));
    
    // 5. Open gzFile for reading and writing
    gzFile gz_read = gzdopen(fd_in, fuzz_str_1);
    gzFile gz_write = gzdopen(fd_out, "wb");
    if (!gz_read || !gz_write) {
        if (gz_read) gzclose(gz_read);
        if (gz_write) gzclose_w(gz_write);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 6. Check if gzFile is in direct mode
    int is_direct = gzdirect(gz_read);
    
    // 7. Initialize inflate for decompression
    if (inflateInit_(&decomp_strm, fuzz_str_2, sizeof(z_stream)) != Z_OK) {
        gzclose(gz_read);
        gzclose_w(gz_write);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 8. Enable validation during inflation
    inflateValidate(&decomp_strm, fuzz_int32_t_3);
    
    // 9. Initialize deflate for compression
    if (deflateInit_(&comp_strm, fuzz_int32_t_5, fuzz_str_4, sizeof(z_stream)) != Z_OK) {
        inflateEnd(&decomp_strm);
        gzclose(gz_read);
        gzclose_w(gz_write);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 10. Initialize inflateBack for backward decompression
    unsigned char window[32768];  // 32KB window
    if (inflateBackInit_(&back_strm, 15, window, fuzz_str_6, sizeof(z_stream)) != Z_OK) {
        deflateEnd(&comp_strm);
        inflateEnd(&decomp_strm);
        gzclose(gz_read);
        gzclose_w(gz_write);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 11. Create buffers for processing
    std::vector<Bytef> input_buffer(size);
    fread(input_buffer.data(), 1, size, in_file);
    rewind(in_file);
    
    // 12. Get compression bound for input
    uLong bound = compressBound(size);
    std::vector<Bytef> compressed(bound);
    z_size_t compressed_len = bound;
    
    // 13. Compress using compress2_z
    int compress_result = compress2_z(compressed.data(), &compressed_len, 
                                     input_buffer.data(), size, fuzz_int32_t_7);
    
    // 14. Prepare for uncompress2_z
    std::vector<Bytef> decompressed(size * 2);  // Double size for safety
    z_size_t decompressed_len = decompressed.size();
    z_size_t source_len = compressed_len;
    
    // 15. Decompress using uncompress2_z
    int uncompress_result = uncompress2_z(decompressed.data(), &decompressed_len,
                                         compressed.data(), &source_len);
    
    // 16. Process data through gzFile interface
    // Read from gzFile
    std::vector<Bytef> gz_read_buffer(1024);
    int bytes_read = gzread(gz_read, gz_read_buffer.data(), 1024);
    
    // Get current position
    off64_t pos = gztell64(gz_read);
    
    // Write to gzFile using gzfwrite
    if (bytes_read > 0) {
        z_size_t items_written = gzfwrite(gz_read_buffer.data(), 1, bytes_read, gz_write);
        
        // Flush the output
        gzflush(gz_write, fuzz_int32_t_8);
    }
    
    // 17. Process data through z_stream interface
    if (uncompress_result == Z_OK) {
        // Set up decompression stream with input
        decomp_strm.next_in = compressed.data();
        decomp_strm.avail_in = compressed_len;
        
        std::vector<Bytef> inflate_output(size);
        decomp_strm.next_out = inflate_output.data();
        decomp_strm.avail_out = inflate_output.size();
        
        // Perform inflation
        int inflate_result = inflate(&decomp_strm, fuzz_int32_t_9);
        
        // Set up compression stream
        comp_strm.next_in = decompressed.data();
        comp_strm.avail_in = decompressed_len;
        
        std::vector<Bytef> deflate_output(bound);
        comp_strm.next_out = deflate_output.data();
        comp_strm.avail_out = deflate_output.size();
        
        // Perform deflation
        int deflate_result = deflate(&comp_strm, fuzz_int32_t_10);
        
        // Check deflate used bits
        int bits_used;
        deflateUsed(&comp_strm, &bits_used);
    }
    
    // 18. Calculate CRC32 checksums
    uLong crc1 = crc32(fuzz_uint64_t_11, Z_NULL, 0);
    if (decompressed_len > 0) {
        crc1 = crc32(crc1, decompressed.data(), decompressed_len);
    }
    
    uLong crc2 = crc32(fuzz_uint64_t_12, Z_NULL, 0);
    if (compressed_len > 0) {
        crc2 = crc32(crc2, compressed.data(), compressed_len);
    }
    
    // 19. Combine CRC checksums
    uLong combined_crc = crc32_combine64(crc1, crc2, size);
    
    // 20. Check gzFile position again
    off64_t final_pos = gztell64(gz_write);
    
    // 21. Check if write file is in direct mode
    int write_is_direct = gzdirect(gz_write);
    
    // 22. Cleanup - release all resources
    inflateBackEnd(&back_strm);
    inflateEnd(&decomp_strm);
    deflateEnd(&comp_strm);
    
    gzclose(gz_read);
    gzclose_w(gz_write);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}