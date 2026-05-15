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
//<ID> 97
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 3
//<Quality> {"density":16,"unique_branches":{"gzseek64":[[396,35,396,53,0,0,4,0],[397,13,397,39,0,0,4,0],[399,13,399,22,0,0,4,1]]},"library_calls":["inflateInit_","inflate","inflateMark","inflateEnd","compressBound","compress2","compressBound","compress2","gzopen64","gzclose","gzdopen","gzclose","gzread","adler32","adler32","gzread","adler32","adler32","adler32_combine","adler32_combine64","gzseek","gzclearerr","gzrewind","gzopen64","gzputc","gzclose","gzclose","gzclose"],"critical_calls":["inflateInit_","inflate","inflateMark","inflateEnd","compressBound","compress2","gzopen64","gzdopen","gzread","adler32","adler32","gzread","adler32","adler32","adler32_combine","adler32_combine64","gzseek","gzclearerr","gzrewind","gzopen64","gzputc","gzclose","gzclose","gzclose"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified zlib APIs to process zlib-compressed data:

*/


extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* f_data, size_t f_size) {
	if(f_size<47) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // 1. First, try to decompress the input data (which should be zlib output)
    z_stream strm;
    Bytef decompressed[4096];
    int ret;
    
    // Initialize zlib stream for decompression
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    
    ret = inflateInit(&strm);
    if (ret != Z_OK) {
	return 0;
	}
	
    
    // Set input data for decompression
    strm.avail_in = size;
    strm.next_in = (Bytef *)data;
    strm.avail_out = sizeof(decompressed);
    strm.next_out = decompressed;
    
    // Decompress the input
    ret = inflate(&strm, fuzz_int32_t_1);
    
    // Use inflateMark to get current position
    long mark = inflateMark(&strm);
    
    // End inflation
    inflateEnd(&strm);
    
    // 2. Now compress the decompressed data (or original data if decompression failed)
    size_t compressed_size;
    Bytef *compressed = nullptr;
    
    if (ret == Z_OK || ret == Z_STREAM_END) {
        // Use the decompressed data
        size_t decompressed_len = sizeof(decompressed) - strm.avail_out;
        
        // Calculate maximum compressed size
        uLong bound = compressBound(decompressed_len);
        compressed = (Bytef *)malloc(bound);
        compressed_size = bound;
        
        // Compress the data with different levels
        for (int level = 0; level <= 9; level += 3) {
            uLongf destLen = compressed_size;
            ret = compress2(compressed, &destLen, decompressed, decompressed_len, fuzz_int32_t_2);
            // Just test the API, don't care about result
        }
    } else {
        // Use original data for compression
        uLong bound = compressBound(size);
        compressed = (Bytef *)malloc(bound);
        compressed_size = bound;
        
        uLongf destLen = compressed_size;
        ret = compress2(compressed, &destLen, data, size, fuzz_int32_t_3);
    }
    
    if (!compressed) {
	return 0;
	}
	
    
    // 3. Write compressed data to a file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        free(compressed);
        assert_file_closed(&out_file);
	return 0;
    }
    
    // Write compressed data to file
    fwrite(compressed, 1, compressed_size, out_file);
    assert_file_closed(&out_file);;
    
    // 4. Open the output file with gzopen64
    gzFile gz_out = gzopen64("output_file", fuzz_str_4);
    if (!gz_out) {
        free(compressed);
        assert_file_closed(&out_file);
	assert_file_name_closed("output_file");
	return 0;
    }
    
    // 5. Get file descriptor and open with gzdopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        gzclose(gz_out);
        free(compressed);
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_file_name_closed("output_file");
	return 0;
    }
    
    int fd = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd, fuzz_str_5);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        gzclose(gz_out);
        free(compressed);
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
    }
    
    // 6. Read from gz file
    char buffer[1024];
    int bytes_read = gzread(gz_in, buffer, sizeof(buffer));
    
    // 7. Compute Adler-32 checksums
    uLong adler1 = adler32(fuzz_uint64_t_6, Z_NULL, 0);
    if (bytes_read > 0) {
        adler1 = adler32(adler1, (const Bytef *)buffer, bytes_read);
    }
    
    // Read from output file and compute another checksum
    char buffer2[1024];
    int bytes_read2 = gzread(gz_out, buffer2, sizeof(buffer2));
    uLong adler2 = adler32(fuzz_uint64_t_7, Z_NULL, 0);
    if (bytes_read2 > 0) {
        adler2 = adler32(adler2, (const Bytef *)buffer2, bytes_read2);
    }
    
    // 8. Combine Adler-32 checksums
    uLong combined_adler = adler32_combine(adler1, adler2, 100);
    uLong combined_adler64 = adler32_combine64(adler1, adler2, 100);
    
    // 9. Test gzseek and gzrewind
    if (gz_out) {
        // Try to seek
        off_t seek_pos = gzseek(gz_out, 0, fuzz_int32_t_8);
        
        // Clear any errors
        gzclearerr(gz_out);
        
        // Try to rewind
        gzrewind(gz_out);
    }
    
    // 10. Write a character to gz file (requires write mode)
    // We'll open a new file for writing
    gzFile gz_write = gzopen64("temp_output", fuzz_str_9);
    if (gz_write) {
        gzputc(gz_write, 'A');
        gzclose(gz_write);
    }
    
    // 11. Clean up all resources
    if (gz_in) gzclose(gz_in);
    if (gz_out) gzclose(gz_out);
    if (in_file) assert_file_closed(&in_file);;
    free(compressed);
    
    // Remove temporary files
    remove("output_file");
    remove("temp_output");
    
    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}