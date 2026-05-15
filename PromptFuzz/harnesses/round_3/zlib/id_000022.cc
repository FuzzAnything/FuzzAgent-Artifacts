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
//<ID> 43
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 9
//<Quality> {"density":12,"unique_branches":{"updatewindow":[[284,13,284,17,0,0,4,0]],"inflateCopy":[[1344,9,1344,32,0,0,4,0],[1347,13,1347,29,0,0,4,1],[1357,9,1357,39,0,0,4,1],[1363,9,1363,25,0,0,4,0]],"inflate_fast":[[217,33,217,44,0,0,4,0],[222,42,222,46,0,0,4,0],[222,42,222,46,0,0,4,1]],"inflate":[[700,9,700,18,0,0,4,0]]},"library_calls":["gzdopen","gzbuffer","gzclose","compressBound","gzclose","inflateInit_","gzclose","gzseek","gzclearerr","inflateMark","gzrewind","inflate","adler32","adler32","adler32_combine","inflateCopy","inflateEnd","compressBound","compress","inflateEnd","gzclose"],"critical_calls":["gzdopen","gzbuffer","compressBound","inflateInit_","gzseek","gzclearerr","inflateMark","gzrewind","inflate","adler32","adler32_combine","inflateCopy","inflateEnd","compressBound","compress","inflateEnd","gzclose"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<49) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
	//fuzzer shim end}




    // Create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a gzFile from the file descriptor
    int fd = fuzz_fileno(in_file);
    gzFile gz_file = gzdopen(fd, fuzz_str_1);
    if (!gz_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Set buffer size for gzFile
    gzbuffer(gz_file, fuzz_uint32_t_2);

    // Create output file for compressed data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Initialize zlib streams for decompression
    z_stream strm1, strm2;
    memset(&strm1, 0, sizeof(z_stream));
    memset(&strm2, 0, sizeof(z_stream));

    // Allocate buffers
    const size_t CHUNK = 16384;
    Bytef *in_buf = (Bytef *)malloc(CHUNK);
    Bytef *out_buf = (Bytef *)malloc(CHUNK);
    Bytef *compressed_buf = (Bytef *)malloc(compressBound(CHUNK));
    
    if (!in_buf || !out_buf || !compressed_buf) {
        free(in_buf);
        free(out_buf);
        free(compressed_buf);
        assert_file_closed(&out_file);;
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Initialize decompression stream
    if (inflateInit(&strm1) != Z_OK) {
        free(in_buf);
        free(out_buf);
        free(compressed_buf);
        assert_file_closed(&out_file);;
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    uLong adler1 = 0, adler2 = 0;
    int ret = Z_OK;
    off_t offset = 0;
    long mark_pos = 0;
    int use_compress = 0;

    // Process data using gzFile APIs
    while (true) {
        // Read a byte using gzgetc
        int c = gzgetc(gz_file);
        if (c == -1) {
            break;
        }

        // Store byte in input buffer
        if (offset < CHUNK) {
            in_buf[offset] = (Bytef)c;
            offset++;
        }

        // Seek randomly within bounds
        if (offset > 10) {
            gzseek(gz_file, offset/2, fuzz_int32_t_3);
        }

        // Clear any errors
        gzclearerr(gz_file);

        // Mark position in inflate stream
        mark_pos = inflateMark(&strm1);
    }

    // Reset file position
    gzrewind(gz_file);

    // Process data for decompression
    strm1.avail_in = 0;
    strm1.next_in = Z_NULL;
    
    do {
        // Read raw data for decompression
        strm1.avail_in = fread(in_buf, 1, CHUNK, in_file);
        if (ferror(in_file)) {
            break;
        }
        if (strm1.avail_in == 0) {
            break;
        }
        strm1.next_in = in_buf;

        // Decompress chunk
        do {
            strm1.avail_out = CHUNK;
            strm1.next_out = out_buf;
            
            ret = inflate(&strm1, fuzz_int32_t_4);
            
            if (ret == Z_STREAM_ERROR || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
                break;
            }

            size_t have = CHUNK - strm1.avail_out;
            
            // Calculate Adler-32 checksums
            if (adler1 == 0) {
                adler1 = adler32(fuzz_uint64_t_5, out_buf, have);
            } else {
                adler2 = adler32(fuzz_uint64_t_6, out_buf, have);
            }

            // Write decompressed data to output file
            if (have > 0) {
                fwrite(out_buf, 1, have, out_file);
            }

        } while (strm1.avail_out == 0);

    } while (ret != Z_STREAM_END);

    // Combine Adler-32 checksums
    if (adler1 != 0 && adler2 != 0) {
        uLong combined = adler32_combine(fuzz_uint64_t_7, fuzz_uint64_t_8, offset);
        (void)combined; // Prevent unused variable warning
    }

    // Copy inflate stream state
    if (inflateCopy(&strm2, &strm1) == Z_OK) {
        // Use the copied stream
        inflateEnd(&strm2);
    }

    // Compress some data using compress() API
    if (offset > 0) {
        uLongf destLen = compressBound(offset);
        if (destLen > 0 && compressed_buf) {
            int compress_ret = compress(compressed_buf, &destLen, in_buf, offset);
            (void)compress_ret; // Prevent unused variable warning
            use_compress = 1;
        }
    }

    // Cleanup
    inflateEnd(&strm1);
    free(in_buf);
    free(out_buf);
    free(compressed_buf);
    assert_file_closed(&out_file);;
    gzclose(gz_file);
    assert_file_closed(&in_file);;

    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}