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
//<ID> 71
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 2
//<Quality> {"density":12,"unique_branches":{"inflateMark":[[1404,10,1404,29,0,0,4,0],[1405,14,1405,34,0,0,4,0]]},"library_calls":["gzdopen","gzbuffer","gzopen64","gzclose","inflateInit_","gzclose","gzclose","inflateSetDictionary","inflateEnd","gzclose","gzclose","inflate","inflateMark","gzclearerr","gzseek","gzrewind","gzputc","adler32","adler32","adler32","adler32","adler32_combine","adler32_combine64","inflateEnd","gzclose","gzclose"],"critical_calls":["gzdopen","gzbuffer","gzopen64","inflateInit_","inflateSetDictionary","inflate","inflateMark","gzclearerr","gzseek","gzrewind","gzputc","adler32","adler32","adler32","adler32","adler32_combine","adler32_combine64","inflateEnd","gzclose","gzclose"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified zlib APIs to achieve decompression and validation of zlib-compressed data:

*/


extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<55) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
	//fuzzer shim end}




    // Return early if input is too small for meaningful processing
    if (size < 10) {
        return 0;
    }

    // Create a temporary file for reading compressed data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor and open as gzFile for reading
    int fd = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd, fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Set buffer size for gzFile
    gzbuffer(gz_in, fuzz_uint32_t_2);

    // Open another gzFile for writing (to test gzputc)
    gzFile gz_out = gzopen64("output_file", fuzz_str_3);
    if (!gz_out) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
    }

    // Initialize inflate stream for decompression
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Use a small portion of data for dictionary if available
    uInt dict_size = size > 100 ? 100 : (uInt)size;
    Bytef *dictionary = (Bytef *)malloc(dict_size);
    if (dictionary) {
        memcpy(dictionary, data, dict_size);
    }

    int ret = inflateInit_(&strm, fuzz_str_4, sizeof(z_stream));
    if (ret != Z_OK) {
        free(dictionary);
        gzclose(gz_out);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
    }

    // Set dictionary if we have one
    if (dictionary) {
        inflateSetDictionary(&strm, dictionary, dict_size);
    }

    // Allocate buffers for decompression
    size_t out_buf_size = size * 2;
    Bytef *out_buf = (Bytef *)malloc(out_buf_size);
    Bytef *in_buf = (Bytef *)malloc(size);
    
    if (!out_buf || !in_buf) {
        free(out_buf);
        free(in_buf);
        free(dictionary);
        inflateEnd(&strm);
        gzclose(gz_out);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
    }

    // Copy input data to buffer
    memcpy(in_buf, data, size);

    // Setup inflate stream
    strm.next_in = in_buf;
    strm.avail_in = (uInt)size;
    strm.next_out = out_buf;
    strm.avail_out = (uInt)out_buf_size;

    // Perform decompression
    ret = inflate(&strm, fuzz_int32_t_5);
    
    // Mark current position in stream
    long mark = inflateMark(&strm);

    // Clear any errors on gzFile
    gzclearerr(gz_in);

    // Try to seek in gzFile (will likely fail with fmemopen, but we call it)
    off_t seek_pos = gzseek(gz_in, 0, fuzz_int32_t_6);
    
    // Rewind gzFile
    gzrewind(gz_in);

    // Write a byte to output gzFile
    gzputc(gz_out, 'X');

    // Calculate Adler-32 checksums
    uLong adler1 = adler32(fuzz_uint64_t_7, Z_NULL, 0);
    if (strm.total_out > 0) {
        adler1 = adler32(adler1, out_buf, (uInt)(strm.total_out > 65535 ? 65535 : strm.total_out));
    }

    // Calculate another Adler-32 for combination
    uLong adler2 = adler32(fuzz_uint64_t_8, Z_NULL, 0);
    if (size > 100) {
        adler2 = adler32(adler2, in_buf + 50, 50);
    }

    // Combine checksums using both versions
    uLong combined1 = adler32_combine(adler1, adler2, 100);
    uLong combined2 = adler32_combine64(adler1, adler2, 100);

    // Cleanup
    inflateEnd(&strm);
    gzclose(gz_out);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    
    free(out_buf);
    free(in_buf);
    free(dictionary);

    // Remove temporary output file
    remove("output_file");

    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}