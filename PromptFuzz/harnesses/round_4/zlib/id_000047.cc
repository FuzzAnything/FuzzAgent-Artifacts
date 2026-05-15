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
//<ID> 100
//<Prompt> []
/*<Combination>: [
*/
//<score> 39, nr_unique_branch: 3
//<Quality> {"density":13,"unique_branches":{"gzgets":[[586,9,586,20,0,0,4,0],[586,24,586,44,0,0,4,1]],"adler32_combine_":[[150,9,150,21,0,0,4,1]]},"library_calls":["compressBound_z","compress_z","gzdopen","gzbuffer","gzclose","gzdopen","gzclose","inflateInit_","gzclose","gzclose","inflateEnd","gzclose","gzclose","inflate","inflateMark","inflateUndermine","inflateReset","gzseek","gztell64","gzputs","gzgets","adler32","adler32","adler32_combine64","inflateEnd","gzclose","gzclose"],"critical_calls":["compressBound_z","compress_z","gzdopen","gzbuffer","gzdopen","inflateInit_","inflate","inflateMark","inflateUndermine","inflateReset","gzseek","gztell64","gzputs","gzgets","adler32","adler32","adler32_combine64","inflateEnd","gzclose","gzclose"],"visited":0}
/**/


// For fmemopen
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* f_data, size_t f_size) {
	if(f_size<60) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
	//fuzzer shim end}




    // Early return if input is too small for meaningful operations
    if (size < 10) {
	return 0;
	}
	
    
    // 1. First, compress some data using compress_z
    // Use first half of input as source data for compression
    size_t source_len = size / 2;
    const Bytef* source = reinterpret_cast<const Bytef*>(data);
    
    // Calculate bound for compressed data
    z_size_t dest_len = compressBound_z(source_len);
    Bytef* compressed = static_cast<Bytef*>(malloc(dest_len));
    if (!compressed) {
	return 0;
	}
	
    
    int compress_result = compress_z(compressed, &dest_len, source, source_len);
    
    // 2. Create a FILE* for reading compressed data (simulating zlib output)
    FILE* in_file = fmemopen((void*)compressed, dest_len, "rb");
    if (!in_file) {
        free(compressed);
        assert_file_closed(&in_file);
	return 0;
    }
	int fuzz_fd_1 = fuzz_fileno(in_file);

    
    // 3. Open gzip file for reading
    gzFile gz_in = gzdopen(fuzz_fd_1, fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        free(compressed);
        assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_1);
	return 0;
    }
    
    // 4. Set buffer size for gzip file
    unsigned int buffer_size = (size % 8192) + 1024; // Vary buffer size based on input
    gzbuffer(gz_in, buffer_size);
    
    // 5. Create output file for writing
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        gzclose(gz_in);
        free(compressed);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_1);
	return 0;
    }
	int fuzz_fd_0 = fuzz_fileno(out_file);

    
    // 6. Open gzip file for writing
    gzFile gz_out = gzdopen(fuzz_fd_0, fuzz_str_2);
    if (!gz_out) {
        assert_file_closed(&out_file);;
        gzclose(gz_in);
        free(compressed);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	assert_fd_closed(fuzz_fd_1);
	return 0;
    }
    
    // 7. Set up z_stream for inflation
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize inflation with default window bits
    if (inflateInit(&strm) != Z_OK) {
        gzclose(gz_out);
        gzclose(gz_in);
        free(compressed);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	assert_fd_closed(fuzz_fd_1);
	return 0;
    }
    
    // 8. Create a buffer for inflation
    Bytef* inflate_buffer = static_cast<Bytef*>(malloc(4096));
    if (!inflate_buffer) {
        inflateEnd(&strm);
        gzclose(gz_out);
        gzclose(gz_in);
        free(compressed);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	assert_fd_closed(fuzz_fd_1);
	return 0;
    }
    
    // 9. Attempt to read from gzip input and inflate
    strm.avail_in = static_cast<uInt>(dest_len);
    strm.next_in = compressed;
    strm.avail_out = 4096;
    strm.next_out = inflate_buffer;
    
    // Perform inflation
    int inflate_result = inflate(&strm, fuzz_int32_t_3);
    
    // 10. Use inflateMark to get current position
    long mark_position = inflateMark(&strm);
    
    // 11. Use inflateUndermine (this is a testing function)
    // Note: This function may not be available in all zlib versions
    // We'll call it but ignore the result
    inflateUndermine(&strm, (size % 2));
    
    // 12. Reset inflation stream
    inflateReset(&strm);
    
    // 13. Seek in gzip input file
    // Use a small offset to avoid going beyond file boundaries
    off_t seek_offset = (size % 100);
    gzseek(gz_in, seek_offset, fuzz_int32_t_4);
    
    // 14. Get current position in gzip file
    off64_t current_pos = gztell64(gz_in);
    
    // 15. Write to gzip output file using gzputs
    const char* test_string = "Test string for gzputs\n";
    gzputs(gz_out, fuzz_str_5);
    
    // 16. Read a line from gzip input (if any data left)
    char read_buffer[256];
    if (current_pos < dest_len) {
        gzgets(gz_in, read_buffer, sizeof(read_buffer));
    }
    
    // 17. Compute Adler-32 checksums for combination
    // Use second half of original input for checksum computation
    const Bytef* data_for_checksum = reinterpret_cast<const Bytef*>(data + size/2);
    size_t checksum_len = size - size/2;
    
    uLong adler1 = adler32(fuzz_uint64_t_6, data_for_checksum, checksum_len/2);
    uLong adler2 = adler32(fuzz_uint64_t_7, data_for_checksum + checksum_len/2, 
                          checksum_len - checksum_len/2);
    
    // 18. Combine Adler-32 checksums
    // Use current_pos as offset for combination
    uLong combined_adler = adler32_combine64(adler1, adler2, current_pos);
    
    // 19. Clean up resources in reverse order
    inflateEnd(&strm);
    gzclose(gz_out);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    free(inflate_buffer);
    free(compressed);
    
    // Use the combined Adler-32 value to prevent optimization removal
    if (combined_adler == 0) {
        // This should never happen, but prevents optimization
        volatile int dummy = 0;
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	assert_fd_closed(fuzz_fd_1);
	return 0;
}