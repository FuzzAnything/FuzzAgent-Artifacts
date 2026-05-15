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
//<ID> 74
//<Prompt> []
/*<Combination>: [
*/
//<score> 32.5, nr_unique_branch: 9
//<Quality> {"density":13,"unique_branches":{"gzseek64":[[376,9,376,31,0,0,4,0],[376,35,376,58,0,0,4,1],[396,9,396,31,0,0,4,1],[423,9,423,31,0,0,4,1]],"gzrewind":[[355,9,355,31,0,0,4,0]],"gzread":[[403,9,403,31,0,0,4,0]],"gzclearerr":[[538,9,538,31,0,0,4,0],[538,35,538,58,0,0,4,1],[542,9,542,31,0,0,4,1]]},"library_calls":["gzdopen","gzbuffer","inflateInit_","gzclose","gzopen64","inflateEnd","gzclose","adler32","adler32","gzread","inflate","gzputc","adler32","inflateMark","gzseek","gzclearerr","gzrewind","gzread","adler32","adler32_combine","adler32_combine64","inflateEnd","gzclose","gzclose"],"critical_calls":["gzdopen","gzbuffer","inflateInit_","gzopen64","adler32","adler32","gzread","inflate","gzputc","adler32","inflateMark","gzseek","gzclearerr","gzrewind","gzread","adler32","adler32_combine","adler32_combine64","inflateEnd","gzclose","gzclose"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified zlib APIs to decompress and process zlib-compressed data:

*/


extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<55) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need enough data for meaningful testing
    
    // Create a temporary file with the compressed data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Get file descriptor for gzdopen
    int fd = fuzz_fileno(in_file);
    
    // Open the compressed file using gzdopen
    gzFile gz = gzdopen(fd, fuzz_str_1);
    if (!gz) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Set buffer size for gzFile
    gzbuffer(gz, fuzz_uint32_t_2);
    
    // Initialize inflate stream
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    
    int ret = inflateInit_(&strm, fuzz_str_3, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Open another gzFile for writing using gzopen64
    gzFile gz_out = gzopen64("output_file", fuzz_str_4);
    if (!gz_out) {
        inflateEnd(&strm);
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
    }
    
    // Prepare input buffer for inflate
    Bytef in_buf[4096];
    Bytef out_buf[4096];
    uLong adler1 = adler32(fuzz_uint64_t_5, Z_NULL, 0);
    uLong adler2 = adler32(fuzz_uint64_t_6, Z_NULL, 0);
    
    // Read and decompress data
    int bytes_read;
    while ((bytes_read = gzread(gz, in_buf, sizeof(in_buf))) > 0) {
        strm.avail_in = bytes_read;
        strm.next_in = in_buf;
        
        // Decompress the data
        do {
            strm.avail_out = sizeof(out_buf);
            strm.next_out = out_buf;
            
            ret = inflate(&strm, fuzz_int32_t_7);
            if (ret == Z_STREAM_ERROR || ret == Z_DATA_ERROR || 
                ret == Z_MEM_ERROR || ret == Z_NEED_DICT) {
                break;
            }
            
            size_t have = sizeof(out_buf) - strm.avail_out;
            
            // Write decompressed data to output gzFile
            for (size_t i = 0; i < have; i++) {
                gzputc(gz_out, out_buf[i]);
            }
            
            // Calculate Adler-32 checksum
            adler1 = adler32(adler1, out_buf, have);
            
        } while (strm.avail_out == 0);
        
        // Mark current position in inflate stream
        long mark = inflateMark(&strm);
        
        if (ret != Z_OK && ret != Z_STREAM_END) {
            break;
        }
    }
    
    // Seek in the output file
    gzseek(gz_out, 0, fuzz_int32_t_8);
    
    // Clear any errors
    gzclearerr(gz_out);
    
    // Calculate second Adler-32 checksum from output file
    gzrewind(gz_out);
    Bytef read_buf[2048];
    int read_bytes;
    while ((read_bytes = gzread(gz_out, read_buf, sizeof(read_buf))) > 0) {
        adler2 = adler32(adler2, read_buf, read_bytes);
    }
    
    // Combine Adler-32 checksums using both functions
    off_t offset = 100;
    off64_t offset64 = 100;
    uLong combined1 = adler32_combine(adler1, adler2, offset);
    uLong combined64 = adler32_combine64(adler1, adler2, offset64);
    
    // Clean up
    inflateEnd(&strm);
    gzclose(gz);
    gzclose(gz_out);
    assert_file_closed(&in_file);;
    
    // Remove temporary output file
    remove("output_file");
    
    // Use the combined values to prevent optimization
    if (combined1 == 0 && combined64 == 0) {
        // This should never happen, but prevents dead code elimination
        printf("Checksums: %lu %lu\n", combined1, combined64);
    }
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}