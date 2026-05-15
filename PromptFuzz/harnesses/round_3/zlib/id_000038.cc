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
//<ID> 72
//<Prompt> []
/*<Combination>: [
*/
//<score> 10.5, nr_unique_branch: 4
//<Quality> {"density":7,"unique_branches":{"gzgets":[[586,9,586,20,0,0,4,0],[586,24,586,44,0,0,4,1]],"inflateInit2_":[[183,9,183,38,0,0,4,1],[191,9,191,36,0,0,4,1]]},"library_calls":["gzdopen","gzbuffer","inflateInit_","gzclose","gzopen64","gzputc","gzclose","inflateMark","inflate","inflateEnd","inflateInit_","adler32","adler32","adler32","adler32","adler32_combine64","adler32_combine","gzseek","gzclearerr","gzgets","gzrewind","inflateEnd","gzclose"],"critical_calls":["gzdopen","gzbuffer","inflateInit_","gzopen64","gzputc","gzclose","inflateMark","inflate","inflateEnd","inflateInit_","adler32","adler32","adler32","adler32","adler32_combine64","adler32_combine","gzseek","gzclearerr","gzgets","gzrewind","inflateEnd","gzclose"],"visited":1}
/*Here's a fuzzer driver that uses the specified zlib APIs to process compressed data:

*/


extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<72) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need enough data for meaningful operations
    
    // Create input FILE* from the fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output FILE* for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    int fd = fuzz_fileno(in_file);
    gzFile gz_file = gzdopen(fd, fuzz_str_1);
    if (!gz_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Set buffer size for gzFile
    gzbuffer(gz_file, fuzz_uint32_t_2);
    
    // Initialize inflate stream
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    int ret = inflateInit_(&strm, fuzz_str_3, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Open another gzFile for writing using gzopen64
    gzFile gz_out = gzopen64("output_file.gz", fuzz_str_4);
    if (gz_out) {
        // Write a single character using gzputc
        gzputc(gz_out, 'X');
        gzclose(gz_out);
    }
    
    // Process compressed data with inflate
    Bytef buffer[4096];
    size_t bytes_read = 0;
    
    // Use inflateMark to mark position
    long mark_pos = inflateMark(&strm);
    
    // Read and decompress data
    while (bytes_read < size) {
        size_t chunk_size = (size - bytes_read > 4096) ? 4096 : size - bytes_read;
        strm.avail_in = chunk_size;
        strm.next_in = (Bytef *)(data + bytes_read);
        
        do {
            strm.avail_out = sizeof(buffer);
            strm.next_out = buffer;
            
            ret = inflate(&strm, fuzz_int32_t_5);
            
            if (ret == Z_OK || ret == Z_STREAM_END || ret == Z_BUF_ERROR) {
                size_t decompressed = sizeof(buffer) - strm.avail_out;
                if (out_file && decompressed > 0) {
                    fwrite(buffer, 1, decompressed, out_file);
                }
            }
        } while (strm.avail_out == 0);
        
        bytes_read += chunk_size;
        
        if (ret == Z_STREAM_END) {
            // Reset stream for potential re-use
            inflateEnd(&strm);
            inflateInit_(&strm, fuzz_str_6, sizeof(z_stream));
        } else if (ret != Z_OK && ret != Z_BUF_ERROR) {
            break;
        }
    }
    
    // Calculate Adler-32 checksums
    uLong adler1 = adler32(fuzz_uint64_t_7, Z_NULL, 0);
    uLong adler2 = adler32(fuzz_uint64_t_8, Z_NULL, 0);
    
    // Calculate checksum for first half of data
    size_t half1 = size / 2;
    if (half1 > 0) {
        adler1 = adler32(adler1, data, half1);
    }
    
    // Calculate checksum for second half of data
    size_t half2 = size - half1;
    if (half2 > 0) {
        adler2 = adler32(adler2, data + half1, half2);
    }
    
    // Combine checksums using both versions
    uLong combined64 = adler32_combine64(adler1, adler2, (off64_t)half2);
    uLong combined = adler32_combine(adler1, adler2, (off_t)half2);
    
    // Seek operations on gzFile
    if (gz_file) {
        // Try to seek to a position
        if (gzseek(gz_file, 50, fuzz_int32_t_9) >= 0) {
            // Clear any errors after seeking
            gzclearerr(gz_file);
            
            // Try to read from new position
            char buf[100];
            gzgets(gz_file, buf, sizeof(buf));
            
            // Rewind to beginning
            gzrewind(gz_file);
        }
    }
    
    // Cleanup
    inflateEnd(&strm);
    
    if (gz_file) {
        gzclose(gz_file);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    remove("output_file.gz");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file.gz");
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file.gz");
	assert_fd_closed(fd);
	return 0;
}