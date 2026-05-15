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
//<ID> 41
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 13
//<Quality> {"density":6,"unique_branches":{"gz_skip":[[298,18,298,28,0,0,4,1],[304,17,304,38,0,0,4,1],[307,14,307,25,0,0,4,0]],"inflateCopy":[[1334,9,1334,34,0,0,4,1],[1334,38,1334,52,0,0,4,1],[1341,9,1341,23,0,0,4,1],[1344,9,1344,32,0,0,4,1],[1357,9,1357,39,0,0,4,0],[1358,9,1358,52,0,0,4,0],[1363,9,1363,25,0,0,4,1]],"inflateMark":[[1400,9,1400,32,0,0,4,1],[1404,10,1404,29,0,0,4,1],[1405,14,1405,34,0,0,4,1]]},"library_calls":["compressBound","compress","gzdopen","gzbuffer","gzseek","gzclearerr","inflateInit_","gzclose","inflateMark","inflateCopy","inflateEnd","adler32","adler32","adler32","adler32","adler32_combine","inflateEnd","gzclose"],"critical_calls":["compressBound","compress","gzdopen","gzbuffer","gzseek","gzclearerr","inflateInit_","inflateMark","inflateCopy","inflateEnd","adler32","adler32","adler32","adler32","adler32_combine","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<29) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful fuzzing
    
    // 1. Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Compress some data using compress() API
    Bytef *compressed_data = nullptr;
    uLongf compressed_size = 0;
    
    // Use first half of input as source for compression
    size_t compress_source_len = size / 2;
    Bytef *compress_source = (Bytef *)malloc(compress_source_len);
    if (!compress_source) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    memcpy(compress_source, data, compress_source_len);
    
    // Get maximum bound for compressed data
    compressed_size = compressBound(compress_source_len);
    compressed_data = (Bytef *)malloc(compressed_size);
    if (!compressed_data) {
        free(compress_source);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Call compress() API
    if (compress(compressed_data, &compressed_size, compress_source, compress_source_len) != Z_OK) {
        free(compressed_data);
        free(compress_source);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Create gzFile from file descriptor
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        free(compressed_data);
        free(compress_source);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    gzFile gz = gzdopen(fd, fuzz_str_1);
    if (!gz) {
        free(compressed_data);
        free(compress_source);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 4. Set buffer size using gzbuffer() API
    gzbuffer(gz, fuzz_uint32_t_2); // 8KB buffer
    
    // 5. Seek in the gzFile using gzseek() API
    // Seek to middle of file (if possible)
    off_t seek_pos = (off_t)(size / 4);
    gzseek(gz, seek_pos, fuzz_int32_t_3);
    
    // 6. Read a character using gzgetc() API
    int ch = gzgetc(gz);
    (void)ch; // Use result to avoid unused variable warning
    
    // 7. Clear any errors using gzclearerr() API
    gzclearerr(gz);
    
    // 8. Set up inflate stream for decompression
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize inflate stream
    if (inflateInit(&strm) != Z_OK) {
        gzclose(gz);
        free(compressed_data);
        free(compress_source);
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 9. Use inflateMark() API
    long mark_position = inflateMark(&strm);
    (void)mark_position; // Use result
    
    // 10. Create another inflate stream and use inflateCopy() API
    z_stream strm_copy;
    memset(&strm_copy, 0, sizeof(strm_copy));
    
    if (inflateCopy(&strm_copy, &strm) == Z_OK) {
        // Clean up the copied stream
        inflateEnd(&strm_copy);
    }
    
    // 11. Compute Adler-32 checksums and combine them
    // Use second half of input data
    size_t adler_data_len = size - compress_source_len;
    const Bytef *adler_data = data + compress_source_len;
    
    // Compute two Adler-32 checksums
    uLong adler1 = adler32(fuzz_uint64_t_4, nullptr, 0);
    uLong adler2 = adler32(fuzz_uint64_t_5, nullptr, 0);
    
    // Split the data for two checksums
    size_t half_len = adler_data_len / 2;
    if (half_len > 0) {
        adler1 = adler32(adler1, adler_data, half_len);
        adler2 = adler32(adler2, adler_data + half_len, adler_data_len - half_len);
        
        // Combine the checksums using adler32_combine() API
        uLong combined_adler = adler32_combine(adler1, adler2, adler_data_len);
        (void)combined_adler; // Use result
    }
    
    // 12. Clean up all resources
    inflateEnd(&strm);
    gzclose(gz);
    free(compressed_data);
    free(compress_source);
    // in_file is closed by gzclose() since we used gzdopen()
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}