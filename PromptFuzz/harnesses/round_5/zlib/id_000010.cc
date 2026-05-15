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
//<ID> 12
//<Prompt> []
/*<Combination>: [
*/
//<score> 12.666667, nr_unique_branch: 1
//<Quality> {"density":19,"unique_branches":{"gzoffset64":[[484,9,484,31,0,0,4,1]]},"library_calls":["gzdopen","gzdopen","gzclose","gzclose","inflateInit2_","gzclose","gzclose","deflateInit2_","inflateEnd","gzclose","gzclose","deflateSetHeader","gzrewind","gzeof","gzgets","gztell","gzoffset64","deflateBound","deflate","deflatePending","inflateReset","inflateUndermine","crc32","crc32","crc32","crc32","crc32_combine","gzwrite","gztell","gzoffset64","inflateEnd","deflateEnd","gzclose","gzclose"],"critical_calls":["gzdopen","gzdopen","inflateInit2_","deflateInit2_","deflateSetHeader","gzrewind","gzeof","gzgets","gztell","gzoffset64","deflateBound","deflate","deflatePending","inflateReset","inflateUndermine","crc32","crc32","crc32","crc32","crc32_combine","gzwrite","gztell","gzoffset64","inflateEnd","deflateEnd","gzclose","gzclose"],"visited":2}
/**/



extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Create output file for compressed data
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
    
    // 4. Open gzip files
    gzFile gz_in = gzdopen(fd_in, "rb");
    gzFile gz_out = gzdopen(fd_out, "wb");
    if (!gz_in || !gz_out) {
        if (gz_in) gzclose(gz_in);
        if (gz_out) gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 5. Initialize zlib streams for compression and decompression
    z_stream strm_inflate, strm_deflate;
    memset(&strm_inflate, 0, sizeof(z_stream));
    memset(&strm_deflate, 0, sizeof(z_stream));
    
    // 6. Initialize inflation stream
    if (inflateInit2(&strm_inflate, 15 + 16) != Z_OK) {
        gzclose(gz_in);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 7. Initialize deflation stream
    if (deflateInit2(&strm_deflate, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                     15 + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        inflateEnd(&strm_inflate);
        gzclose(gz_in);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 8. Set gzip header for deflation
    gz_header header;
    memset(&header, 0, sizeof(gz_header));
    header.text = 1;
    header.time = 0;
    header.os = 255;
    deflateSetHeader(&strm_deflate, &header);
    
    // 9. Read decompressed data using gzgets
    std::vector<char> decompressed_buffer(4096);
    char line_buf[256];
    size_t total_decompressed = 0;
    
    // Read all data through gzFile
    gzrewind(gz_in);
    while (!gzeof(gz_in)) {
        // Read line by line (or chunks if not text)
        if (gzgets(gz_in, line_buf, sizeof(line_buf))) {
            size_t len = strlen(line_buf);
            if (total_decompressed + len < decompressed_buffer.size()) {
                decompressed_buffer.resize(total_decompressed + len + 4096);
            }
            memcpy(decompressed_buffer.data() + total_decompressed, line_buf, len);
            total_decompressed += len;
        }
    }
    
    // 10. Get file positions
    off_t pos = gztell(gz_in);
    off64_t pos64 = gzoffset64(gz_in);
    
    // 11. Use deflateBound to estimate compressed size
    uLong bound = deflateBound(&strm_deflate, total_decompressed);
    std::vector<Bytef> compressed_buffer(bound);
    
    // 12. Set up deflation input/output
    strm_deflate.next_in = (Bytef*)decompressed_buffer.data();
    strm_deflate.avail_in = total_decompressed;
    strm_deflate.next_out = compressed_buffer.data();
    strm_deflate.avail_out = bound;
    
    // 13. Perform deflation with pending check
    unsigned int pending;
    int bits;
    int flush = Z_FINISH;
    
    // Deflate in one go
    deflate(&strm_deflate, flush);
    
    // Check pending bits
    deflatePending(&strm_deflate, &pending, &bits);
    
    // 14. Use inflateUndermine to test error handling
    // First reset the inflate stream
    inflateReset(&strm_inflate);
    
    // Set up inflation with some data
    std::vector<Bytef> test_data(100);
    strm_inflate.next_in = test_data.data();
    strm_inflate.avail_in = test_data.size();
    
    // Try to undermine the stream (this may cause an error)
    inflateUndermine(&strm_inflate, 1);
    
    // 15. Calculate CRC32 values and combine them
    uLong crc1 = crc32(0L, Z_NULL, 0);
    uLong crc2 = crc32(0L, Z_NULL, 0);
    
    if (total_decompressed > 0) {
        // Split data into two parts for CRC calculation
        size_t split = total_decompressed / 2;
        crc1 = crc32(crc1, (Bytef*)decompressed_buffer.data(), split);
        crc2 = crc32(crc2, (Bytef*)decompressed_buffer.data() + split, 
                    total_decompressed - split);
        
        // Combine the two CRC values
        uLong combined_crc = crc32_combine(crc1, crc2, total_decompressed - split);
        (void)combined_crc; // Prevent unused variable warning
    }
    
    // 16. Write compressed data to gzFile
    gzwrite(gz_out, compressed_buffer.data(), 
            bound - strm_deflate.avail_out);
    
    // 17. Get position in output file
    off_t out_pos = gztell(gz_out);
    off64_t out_pos64 = gzoffset64(gz_out);
    (void)out_pos;
    (void)out_pos64;
    
    // 18. Clean up
    inflateEnd(&strm_inflate);
    deflateEnd(&strm_deflate);
    gzclose(gz_in);
    gzclose(gz_out);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // 19. Remove temporary output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}