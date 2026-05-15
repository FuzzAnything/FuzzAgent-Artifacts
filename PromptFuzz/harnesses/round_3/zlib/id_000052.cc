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
//<ID> 102
//<Prompt> []
/*<Combination>: [
*/
//<score> 38, nr_unique_branch: 1
//<Quality> {"density":19,"unique_branches":{"adler32_combine_":[[150,9,150,21,0,0,4,1]]},"library_calls":["gzdopen","gzdopen","gzclose","gzclose","gzsetparams","inflateInit2_","gzclose","gzclose","gzread","inflate","crc32","adler32","inflateMark","gzwrite","inflateResetKeep","gzread","crc32","adler32","deflateUsed","crc32_combine64","adler32_combine","compress2","inflateEnd","gzclose","gzclose"],"critical_calls":["gzdopen","gzdopen","gzsetparams","inflateInit2_","gzread","inflate","crc32","adler32","inflateMark","gzwrite","inflateResetKeep","gzread","crc32","adler32","deflateUsed","crc32_combine64","adler32_combine","compress2","inflateEnd","gzclose","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t* f_data, size_t f_size) {
	if(f_size<111) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_12, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_13, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_14, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_15, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_16, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create temporary files for reading and writing
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    
    // 1. Use gzdopen to open file descriptors as gzFile
    gzFile gz_in = gzdopen(fd_in, fuzz_str_1);
    gzFile gz_out = gzdopen(fd_out, fuzz_str_2);
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
    
    // 2. Use gzsetparams on output file
    gzsetparams(gz_out, fuzz_int32_t_3, fuzz_int32_t_4);
    
    // 3. Initialize inflate stream for decompression
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int ret = inflateInit2_(&strm, fuzz_int32_t_6, fuzz_str_5, sizeof(z_stream));
    if (ret != Z_OK) {
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
    
    // 4. Read compressed data using gzread
    const size_t BUFFER_SIZE = 4096;
    Bytef buffer_in[BUFFER_SIZE];
    Bytef buffer_out[BUFFER_SIZE];
    uLong crc1 = 0, crc2 = 0;
    uLong adler1 = 0, adler2 = 0;
    long mark_pos = 0;
    int bits_used = 0;
    bool marked = false;
    
    while (true) {
        // Read from gzFile
        int bytes_read = gzread(gz_in, buffer_in, BUFFER_SIZE);
        if (bytes_read <= 0) break;
        
        // Set up input for inflate
        strm.avail_in = bytes_read;
        strm.next_in = buffer_in;
        
        do {
            // Set up output buffer
            strm.avail_out = BUFFER_SIZE;
            strm.next_out = buffer_out;
            
            // 5. Use inflate to decompress
            ret = inflate(&strm, fuzz_int32_t_7);
            
            // Process decompressed data
            size_t decompressed = BUFFER_SIZE - strm.avail_out;
            if (decompressed > 0) {
                // 6. Use crc32 on decompressed data
                crc1 = crc32(fuzz_uint64_t_8, buffer_out, decompressed);
                adler1 = adler32(fuzz_uint64_t_9, buffer_out, decompressed);
                
                // 7. Use inflateMark at first opportunity
                if (!marked && strm.avail_in > 0) {
                    mark_pos = inflateMark(&strm);
                    marked = true;
                }
                
                // Write to gzFile for testing
                gzwrite(gz_out, buffer_out, decompressed);
            }
        } while (strm.avail_out == 0 && ret == Z_OK);
        
        if (ret != Z_OK && ret != Z_STREAM_END) break;
        
        if (ret == Z_STREAM_END) {
            // 8. Use inflateResetKeep to reset stream
            inflateResetKeep(&strm);
            
            // Read remaining data for second CRC calculation
            while (true) {
                bytes_read = gzread(gz_in, buffer_in, BUFFER_SIZE);
                if (bytes_read <= 0) break;
                crc2 = crc32(fuzz_uint64_t_10, buffer_in, bytes_read);
                adler2 = adler32(fuzz_uint64_t_11, buffer_in, bytes_read);
            }
            break;
        }
    }
    
    // 9. Use deflateUsed (even though we're inflating, we can still call it)
    deflateUsed(&strm, &bits_used);
    
    // 10. Use crc32_combine64 and adler32_combine
    uLong combined_crc = crc32_combine64(fuzz_uint64_t_12, fuzz_uint64_t_13, size);
    uLong combined_adler = adler32_combine(fuzz_uint64_t_14, fuzz_uint64_t_15, size);
    
    // 11. Compress some data using compress2
    Bytef compressed[BUFFER_SIZE * 2];
    uLongf compressed_len = sizeof(compressed);
    if (size > 0) {
        compress2(compressed, &compressed_len, data, 
                  size > BUFFER_SIZE ? BUFFER_SIZE : (uLong)size, 
                  fuzz_int32_t_16);
    }
    
    // 12. Clean up
    inflateEnd(&strm);
    gzclose(gz_in);
    gzclose(gz_out);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove temporary output file
    remove("output_file");
    
    // Use the computed values to avoid optimization
    volatile uLong dummy = combined_crc + combined_adler + mark_pos + bits_used;
    (void)dummy;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}