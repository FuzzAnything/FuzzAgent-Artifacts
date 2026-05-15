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
//<ID> 52
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 9
//<Quality> {"density":12,"unique_branches":{"gzvprintf":[[428,9,428,25,0,0,4,1],[450,9,450,28,0,0,4,1]],"gz_write":[[205,9,205,26,0,0,4,1],[230,13,230,33,0,0,4,1],[238,17,238,24,0,0,4,0],[245,17,245,26,0,0,4,1],[247,18,247,21,0,0,4,1]],"gz_vacate":[[386,9,386,66,0,0,4,1],[389,9,389,28,0,0,4,0]]},"library_calls":["gzdopen","gzclose","gzdopen","gzclose","inflateInit2_","gzclose","gzclose","gzread","inflate","inflateMark","inflateResetKeep","compressBound","compress2","gzwrite","gzprintf","crc32","crc32","crc32","crc32","crc32_combine64","inflateEnd","gzclose","gzclose"],"critical_calls":["gzdopen","gzdopen","inflateInit2_","gzread","inflate","inflateMark","inflateResetKeep","compressBound","compress2","gzwrite","gzprintf","crc32","crc32","crc32","crc32","crc32_combine64","inflateEnd","gzclose","gzclose"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<34) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
	//fuzzer shim end}




    // 1. Create FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Create gzFile from file descriptor for reading compressed data
    int fd_in = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd_in, fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 3. Create output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 4. Create gzFile for writing compressed output
    int fd_out = fuzz_fileno(out_file);
    gzFile gz_out = gzdopen(fd_out, fuzz_str_2);
    if (!gz_out) {
        assert_file_closed(&out_file);;
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 5. Initialize z_stream for inflation (decompression)
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    
    int ret = inflateInit2(&strm, 15 + 16); // 15 window bits + 16 for gzip header
    if (ret != Z_OK) {
        gzclose(gz_out);
        gzclose(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 6. Read compressed data from gzFile and inflate it
    Bytef in_buffer[4096];
    Bytef out_buffer[8192];
    
    // Read some data from gz_in to use as source for inflation
    int bytes_read = gzread(gz_in, in_buffer, sizeof(in_buffer));
    if (bytes_read > 0) {
        strm.avail_in = bytes_read;
        strm.next_in = in_buffer;
        
        // Inflate the data
        strm.avail_out = sizeof(out_buffer);
        strm.next_out = out_buffer;
        ret = inflate(&strm, fuzz_int32_t_3);
        
        // Use inflateMark to get current decompression state
        long mark = inflateMark(&strm);
        
        // Use inflateResetKeep to reset decompression state while keeping history
        if (mark != -1) {
            inflateResetKeep(&strm);
        }
    }
    
    // 7. Compress the inflated data (or original data if inflation failed)
    uLongf compressed_len = compressBound(size);
    Bytef *compressed_data = (Bytef *)malloc(compressed_len);
    if (compressed_data) {
        // Use compress2 with default compression level
        ret = compress2(compressed_data, &compressed_len, 
                       data, (uLong)size, fuzz_int32_t_4);
        
        if (ret == Z_OK && compressed_len > 0) {
            // Write compressed data to gzFile using gzwrite
            gzwrite(gz_out, compressed_data, compressed_len);
            
            // Use gzprintf to write metadata
            gzprintf(gz_out, "Compressed size: %lu\n", (unsigned long)compressed_len);
        }
        free(compressed_data);
    }
    
    // 8. Calculate CRC32 checksums and combine them
    uLong crc1 = crc32(fuzz_uint64_t_5, Z_NULL, 0);
    uLong crc2 = crc32(fuzz_uint64_t_6, Z_NULL, 0);
    
    if (size > 0) {
        // Calculate CRC for first half
        size_t half1 = size / 2;
        if (half1 > 0) {
            crc1 = crc32(crc1, data, half1);
        }
        
        // Calculate CRC for second half
        size_t half2 = size - half1;
        if (half2 > 0) {
            crc2 = crc32(crc2, data + half1, half2);
        }
        
        // Combine the two CRC checksums using crc32_combine64
        uLong combined_crc = crc32_combine64(crc1, crc2, (off64_t)size);
        (void)combined_crc; // Prevent unused variable warning
    }
    
    // 9. Clean up all resources
    inflateEnd(&strm);
    gzclose(gz_out);
    gzclose(gz_in);
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    // Remove the output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}