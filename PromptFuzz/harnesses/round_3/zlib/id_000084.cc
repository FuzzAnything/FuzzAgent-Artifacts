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
//<ID> 240
//<Prompt> []
/*<Combination>: [
*/
//<score> 39, nr_unique_branch: 2
//<Quality> {"density":13,"unique_branches":{"deflatePending":[[723,9,723,32,0,0,4,0]],"deflateTune":[[823,9,823,32,0,0,4,0]]},"library_calls":["gzdopen","gzdopen","gzclose","gzclose","gzbuffer","gzbuffer","inflateInit2_","gzclose","gzclose","deflateTune","adler32","adler32","gzeof","gzfread","inflate","inflateSync","gzwrite","adler32","adler32","gzputs","deflatePending","gzseek64","adler32_combine64","gzflush","inflateEnd","gzclose","gzclose"],"critical_calls":["gzdopen","gzdopen","gzbuffer","gzbuffer","inflateInit2_","deflateTune","adler32","adler32","gzeof","gzfread","inflate","inflateSync","gzwrite","adler32","adler32","gzputs","deflatePending","gzseek64","adler32_combine64","gzflush","inflateEnd","gzclose","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_84(const uint8_t* f_data, size_t f_size) {
	if(f_size<98) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_12, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_15, fuzz_str_sz_15, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_16, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_17, fdp);
	//fuzzer shim end}




    // Create input FILE* from the fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output FILE* for compressed data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Open gzFile handles for reading and writing
    gzFile gz_in = gzdopen(in_fd, fuzz_str_1);
    gzFile gz_out = gzdopen(out_fd, fuzz_str_2);
    
    if (!gz_in || !gz_out) {
        if (gz_in) gzclose(gz_in);
        if (gz_out) gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Set buffer size for gzFile
    gzbuffer(gz_in, fuzz_uint32_t_3);
    gzbuffer(gz_out, fuzz_uint32_t_4);

    // Initialize z_stream for inflation
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Use negative windowBits for raw deflate stream
    int ret = inflateInit2_(&strm, fuzz_int32_t_6, fuzz_str_5, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_in);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Tune deflate parameters (even though we're inflating)
    // This demonstrates deflateTune usage
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    deflateTune(&deflate_strm, fuzz_int32_t_7, fuzz_int32_t_8, fuzz_int32_t_9, fuzz_int32_t_10);

    // Read compressed data from gz_in
    const size_t CHUNK_SIZE = 4096;
    Bytef in_buf[CHUNK_SIZE];
    Bytef out_buf[CHUNK_SIZE];
    
    uLong adler1 = adler32(fuzz_uint64_t_11, Z_NULL, 0);
    uLong adler2 = adler32(fuzz_uint64_t_12, Z_NULL, 0);
    off64_t combined_offset = 0;

    while (!gzeof(gz_in)) {
        // Read compressed data
        z_size_t bytes_read = gzfread(in_buf, fuzz_uint64_t_13, CHUNK_SIZE, gz_in);
        if (bytes_read == 0) {
            break;
        }

        // Set up inflation stream
        strm.avail_in = (uInt)bytes_read;
        strm.next_in = in_buf;
        
        do {
            strm.avail_out = CHUNK_SIZE;
            strm.next_out = out_buf;
            
            ret = inflate(&strm, fuzz_int32_t_14);
            if (ret == Z_STREAM_ERROR || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
                // Try to recover with inflateSync
                inflateSync(&strm);
                break;
            }
            
            size_t have = CHUNK_SIZE - strm.avail_out;
            
            // Write decompressed data to gz_out
            if (have > 0) {
                gzwrite(gz_out, out_buf, (unsigned int)have);
                
                // Compute Adler-32 checksums
                adler1 = adler32(adler1, out_buf, (uInt)have);
                adler2 = adler32(adler2, out_buf, (uInt)have/2);
                
                // Write a test string
                gzputs(gz_out, fuzz_str_15);
            }
        } while (strm.avail_out == 0);
    }

    // Check for pending bits in deflate stream (demonstrating deflatePending)
    unsigned int pending;
    int bits;
    deflatePending(&deflate_strm, &pending, &bits);

    // Seek in the input gzFile
    gzseek64(gz_in, 0, fuzz_int32_t_16);

    // Combine Adler-32 checksums
    uLong combined_adler = adler32_combine64(adler1, adler2, combined_offset);

    // Flush output
    gzflush(gz_out, fuzz_int32_t_17);

    // Clean up
    inflateEnd(&strm);
    gzclose(gz_in);
    gzclose(gz_out);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}