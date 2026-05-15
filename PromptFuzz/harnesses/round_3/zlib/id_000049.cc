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
//<ID> 96
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 1
//<Quality> {"density":13,"unique_branches":{"gzputc":[[320,9,320,32,0,0,4,0]]},"library_calls":["gzdopen","gzopen64","compressBound","gzclose","gzclose","compress2","inflateInit_","inflate","inflateMark","gzread","gzputc","gzseek","gzclearerr","gzrewind","adler32","adler32","adler32","adler32","adler32_combine","adler32_combine64","inflateEnd","gzclose","gzclose"],"critical_calls":["gzdopen","gzopen64","compressBound","compress2","inflateInit_","inflate","inflateMark","gzread","gzputc","gzseek","gzclearerr","gzrewind","adler32","adler32","adler32","adler32","adler32_combine","adler32_combine64","inflateEnd","gzclose","gzclose"],"visited":1}
/*Here's a C++ fuzz driver that uses all the specified zlib APIs to achieve a comprehensive decompression and checksum verification event:

*/


extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
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
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 1) {
	return 0;
	}
	
    
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
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // 1. Open compressed file with gzdopen
    gzFile gz_in = gzdopen(in_fd, fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 2. Also open with gzopen64 for comparison
    gzFile gz_in2 = gzopen64("input_file", fuzz_str_2);
    if (gz_in2) {
        // Write input data to a temporary file for gzopen64
        FILE *tmp = fopen("input_file", "wb");
        if (tmp) {
            fwrite(data, 1, size, tmp);
            assert_file_closed(&tmp);;
        }
    }
    
    // 3. Allocate buffers for decompression
    uLongf destLen = compressBound(size);  // Maximum compressed size
    Bytef *compressed = (Bytef *)malloc(destLen);
    Bytef *decompressed = (Bytef *)malloc(size * 2);  // Extra space
    
    if (!compressed || !decompressed) {
        free(compressed);
        free(decompressed);
        gzclose(gz_in);
        if (gz_in2) gzclose(gz_in2);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 4. Recompress the data using compress2
    uLongf compLen = destLen;
    int compress_level = data[0] % 10;  // Use first byte for compression level
    int compress_result = compress2(compressed, &compLen, data, size, compress_level);
    
    // 5. Set up z_stream for inflation
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.next_in = compressed;
    strm.avail_in = compLen;
    strm.next_out = decompressed;
    strm.avail_out = size * 2;
    
    // 6. Initialize inflation
    if (inflateInit(&strm) == Z_OK) {
        // 7. Inflate the data
        int inflate_result = inflate(&strm, fuzz_int32_t_3);
        
        // 8. Get mark position
        long mark_pos = inflateMark(&strm);
        
        // 9. Read from gzFile
        unsigned int read_len = size > 100 ? 100 : size;
        Bytef read_buf[100];
        int gzread_result = 0;
        if (gz_in) {
            gzread_result = gzread(gz_in, read_buf, read_len);
        }
        
        // 10. Write a character to gzFile
        if (gz_in) {
            gzputc(gz_in, 'X');
        }
        
        // 11. Seek in gzFile
        if (gz_in) {
            gzseek(gz_in, 0, fuzz_int32_t_4);
        }
        
        // 12. Clear errors in gzFile
        if (gz_in) {
            gzclearerr(gz_in);
        }
        
        // 13. Rewind gzFile
        if (gz_in) {
            gzrewind(gz_in);
        }
        
        // 14. Compute Adler-32 checksums
        uLong adler1 = adler32(fuzz_uint64_t_5, Z_NULL, 0);
        if (strm.total_out > 0) {
            adler1 = adler32(adler1, decompressed, strm.total_out);
        }
        
        uLong adler2 = adler32(fuzz_uint64_t_6, Z_NULL, 0);
        if (gzread_result > 0) {
            adler2 = adler32(adler2, read_buf, gzread_result);
        }
        
        // 15. Combine Adler-32 checksums
        off_t combine_len = 100;
        uLong combined_adler = adler32_combine(adler1, adler2, combine_len);
        
        // 16. Also use 64-bit version
        off64_t combine_len64 = 100;
        uLong combined_adler64 = adler32_combine64(adler1, adler2, combine_len64);
        
        // 17. End inflation
        inflateEnd(&strm);
    }
    
    // 18. Close gzFiles
    if (gz_in) {
        gzclose(gz_in);
    }
    if (gz_in2) {
        gzclose(gz_in2);
    }
    
    // Clean up
    free(compressed);
    free(decompressed);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}