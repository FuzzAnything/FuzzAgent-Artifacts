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
//<ID> 116
//<Prompt> []
/*<Combination>: [
*/
//<score> 52, nr_unique_branch: 3
//<Quality> {"density":13,"unique_branches":{"deflateSetDictionary":[[567,9,567,32,0,0,4,0]],"inflate":[[1111,9,1111,18,0,0,4,0]],"gzbuffer":[[339,9,339,17,0,0,4,0]]},"library_calls":["gzdopen","gzbuffer","gzopen64","gzclose","inflateInit_","deflateSetDictionary","inflate","inflateMark","inflate","inflateEnd","adler32","adler32","adler32","adler32","adler32_combine","adler32_combine64","gzgets","gzeof","gzclearerr","gzrewind","gzseek","gzputc","uncompress2_z","gzclose","gzclose"],"critical_calls":["gzdopen","gzbuffer","gzopen64","inflateInit_","deflateSetDictionary","inflate","inflateMark","inflate","inflateEnd","adler32","adler32","adler32","adler32","adler32_combine","adler32_combine64","gzgets","gzeof","gzclearerr","gzrewind","gzseek","gzputc","uncompress2_z","gzclose","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t* f_data, size_t f_size) {
	if(f_size<59) return 0;

	
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
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create input FILE* from the fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor for gzdopen
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 1. Open compressed file using gzdopen
    gzFile gz_input = gzdopen(fd, fuzz_str_1);
    if (!gz_input) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 2. Set buffer size
    unsigned int buffer_size = 8192;
    if (size > 100) {
        buffer_size = (data[0] % 65536) + 1;
    }
    gzbuffer(gz_input, fuzz_uint32_t_2);

    // 3. Open another gzFile for writing using gzopen64
    gzFile gz_output = gzopen64("output_file", fuzz_str_3);
    if (!gz_output) {
        gzclose(gz_input);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
    }

    // 4. Initialize z_stream for inflation
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Allocate buffers for inflation
    Bytef *inflate_buffer = nullptr;
    uLongf inflate_buffer_size = 0;
    
    if (size > 0) {
        // Try to estimate decompressed size (minimum 1024 bytes)
        inflate_buffer_size = size * 2 + 100;
        if (inflate_buffer_size < 1024) {
            inflate_buffer_size = 1024;
        }
        inflate_buffer = (Bytef *)malloc(inflate_buffer_size);
    }

    if (inflate_buffer) {
        // 5. Initialize inflation
        int ret = inflateInit_(&strm, fuzz_str_4, sizeof(z_stream));
        if (ret == Z_OK) {
            // Set input data for inflation
            strm.next_in = (Bytef *)data;
            strm.avail_in = (uInt)(size > UINT_MAX ? UINT_MAX : size);
            strm.total_in = 0;
            
            strm.next_out = inflate_buffer;
            strm.avail_out = (uInt)(inflate_buffer_size > UINT_MAX ? UINT_MAX : inflate_buffer_size);
            strm.total_out = 0;
            
            // 6. Try to set dictionary (use first few bytes as dictionary if available)
            if (size > 10) {
                uInt dict_len = (data[0] % 10) + 1;
                if (dict_len > size) dict_len = (uInt)size;
                deflateSetDictionary(&strm, data, dict_len);
            }
            
            // 7. Perform inflation
            ret = inflate(&strm, fuzz_int32_t_5);
            
            // 8. Mark position in stream
            long mark_pos = inflateMark(&strm);
            
            // 9. Try to get more data if available
            if (ret == Z_OK || ret == Z_STREAM_END) {
                // Reset output buffer for more inflation
                strm.next_out = inflate_buffer + strm.total_out;
                strm.avail_out = (uInt)(inflate_buffer_size - strm.total_out > UINT_MAX ? 
                                       UINT_MAX : inflate_buffer_size - strm.total_out);
                ret = inflate(&strm, fuzz_int32_t_6);
            }
            
            // 10. End inflation
            inflateEnd(&strm);
            
            // Use inflated data for Adler-32 calculation
            if (strm.total_out > 0) {
                // 11. Compute Adler-32 checksum
                uLong adler1 = adler32(fuzz_uint64_t_7, Z_NULL, 0);
                uLong adler2 = adler32(fuzz_uint64_t_8, Z_NULL, 0);
                
                // Split data into two parts for Adler-32 combine demonstration
                size_t half = strm.total_out / 2;
                if (half > 0) {
                    adler1 = adler32(adler1, inflate_buffer, (uInt)half);
                    if (strm.total_out - half > 0) {
                        adler2 = adler32(adler2, inflate_buffer + half, 
                                       (uInt)(strm.total_out - half));
                        
                        // 12. Combine Adler-32 checksums using both combine functions
                        uLong combined1 = adler32_combine(adler1, adler2, (off_t)half);
                        uLong combined2 = adler32_combine64(adler1, adler2, (off64_t)half);
                        (void)combined1; // Suppress unused warning
                        (void)combined2; // Suppress unused warning
                    }
                }
            }
        }
    }

    // 13. Read from gz_input file using gzgets
    char line_buffer[256];
    int line_len = 0;
    if (gzgets(gz_input, line_buffer, sizeof(line_buffer))) {
        line_len = (int)strlen(line_buffer);
    }
    
    // 14. Check for EOF
    int eof_check = gzeof(gz_input);
    (void)eof_check; // Suppress unused warning
    
    // 15. Clear any errors
    gzclearerr(gz_input);
    
    // 16. Rewind the gz file
    gzrewind(gz_input);
    
    // 17. Seek in the gz file
    off_t seek_pos = gzseek(gz_input, 0, fuzz_int32_t_9);
    (void)seek_pos; // Suppress unused warning
    
    // 18. Write to gz_output using gzputc
    if (line_len > 0) {
        for (int i = 0; i < line_len && i < 10; i++) {
            gzputc(gz_output, line_buffer[i]);
        }
    }
    
    // 19. Try uncompress2_z with the original data
    if (size > 0 && inflate_buffer) {
        z_size_t dest_len = inflate_buffer_size;
        z_size_t source_len = size;
        Bytef *uncompress_buffer = (Bytef *)malloc(dest_len);
        
        if (uncompress_buffer) {
            int uncompress_ret = uncompress2_z(uncompress_buffer, &dest_len, 
                                               data, &source_len);
            (void)uncompress_ret; // Suppress unused warning
            free(uncompress_buffer);
        }
    }
    
    // 20. Clean up
    if (inflate_buffer) {
        free(inflate_buffer);
    }
    
    // Close gz files
    gzclose(gz_input);  // This also closes the underlying file descriptor
    gzclose(gz_output);
    
    // Note: in_file is closed automatically by gzclose(gz_input)
    // because gzdopen takes ownership of the file descriptor
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}