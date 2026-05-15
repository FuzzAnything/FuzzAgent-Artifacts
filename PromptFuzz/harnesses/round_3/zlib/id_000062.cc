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
//<ID> 126
//<Prompt> []
/*<Combination>: [
*/
//<score> 5.3333335, nr_unique_branch: 1
//<Quality> {"density":8,"unique_branches":{"deflate":[[995,9,995,29,0,0,4,0]]},"library_calls":["gzopen64","zlibCompileFlags","deflateInit_","gzclose","inflateInit_","deflateEnd","gzclose","gzread","crc32","inflateReset2","inflateSync","inflateSyncPoint","inflateReset","crc32_combine_gen64","deflateParams","deflateBound_z","inflateEnd","deflateEnd","gzclose","deflate","deflate","inflateEnd","deflateEnd","gzclose"],"critical_calls":["gzopen64","zlibCompileFlags","deflateInit_","inflateInit_","gzread","crc32","inflateReset2","inflateSync","inflateSyncPoint","inflateReset","crc32_combine_gen64","deflateParams","deflateBound_z","deflate","deflate","inflateEnd","deflateEnd","gzclose"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t* f_data, size_t f_size) {
	if(f_size<67) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // 1. First, write the input data to a file for gzopen64
    FILE *input_file = fopen("input_file", "wb");
    if (!input_file) {
	assert_file_closed(&input_file);
	return 0;
	}
	
    fwrite(data, 1, size, input_file);
    assert_file_closed(&input_file);;
    
    // 2. Open the compressed file using gzopen64
    gzFile gz_file = gzopen64("input_file", fuzz_str_1);
    if (!gz_file) {
        remove("input_file");
        assert_file_closed(&input_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 3. Get zlib compilation flags
    uLong compile_flags = zlibCompileFlags();
    (void)compile_flags; // Use it to avoid unused variable warning
    
    // 4. Initialize deflate stream for re-compression
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    int ret = deflateInit_(&deflate_strm, fuzz_int32_t_3, 
                          fuzz_str_2, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_file);
        remove("input_file");
        assert_file_closed(&input_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 5. Initialize inflate stream for decompression
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    ret = inflateInit_(&inflate_strm, fuzz_str_4, sizeof(z_stream));
    if (ret != Z_OK) {
        deflateEnd(&deflate_strm);
        gzclose(gz_file);
        remove("input_file");
        assert_file_closed(&input_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 6. Read decompressed data from gz file
    const size_t buffer_size = 4096;
    Bytef decompressed_buffer[buffer_size];
    uLong decompressed_total = 0;
    uLong crc_value = 0;
    
    while (true) {
        int bytes_read = gzread(gz_file, decompressed_buffer, buffer_size);
        if (bytes_read <= 0) break;
        
        // Update CRC32 of decompressed data
        crc_value = crc32(fuzz_uint64_t_5, decompressed_buffer, bytes_read);
        decompressed_total += bytes_read;
    }
    
    // 7. Reset inflate stream with different window bits
    ret = inflateReset2(&inflate_strm, 15 + 16); // Add 16 for gzip decoding
    (void)ret;
    
    // 8. Try to use inflateSync (even though we're not in a sync situation)
    // This will likely fail but we call it as required
    inflateSync(&inflate_strm);
    
    // 9. Check sync point
    inflateSyncPoint(&inflate_strm);
    
    // 10. Reset inflate stream again
    inflateReset(&inflate_strm);
    
    // 11. Create crc combine generator
    off64_t dummy_offset = 100;
    uLong combine_gen = crc32_combine_gen64(dummy_offset);
    (void)combine_gen;
    
    // 12. Set up deflate parameters
    ret = deflateParams(&deflate_strm, fuzz_int32_t_6, fuzz_int32_t_7);
    (void)ret;
    
    // 13. Calculate bound for deflated size
    z_size_t source_len = decompressed_total;
    z_size_t bound_size = deflateBound_z(&deflate_strm, source_len);
    
    // 14. Allocate output buffer and prepare for compression
    Bytef *compressed_buffer = (Bytef*)malloc(bound_size);
    if (!compressed_buffer) {
        inflateEnd(&inflate_strm);
        deflateEnd(&deflate_strm);
        gzclose(gz_file);
        remove("input_file");
        assert_file_closed(&input_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 15. Re-open input file to get data for compression
    FILE *reopen_file = fopen("input_file", "rb");
    if (reopen_file) {
        Bytef input_buffer[1024];
        size_t read_size;
        
        deflate_strm.next_out = compressed_buffer;
        deflate_strm.avail_out = bound_size;
        
        while ((read_size = fread(input_buffer, 1, sizeof(input_buffer), reopen_file)) > 0) {
            deflate_strm.next_in = input_buffer;
            deflate_strm.avail_in = read_size;
            
            ret = deflate(&deflate_strm, fuzz_int32_t_8);
            if (ret != Z_OK) break;
        }
        
        // Finish compression
        deflate_strm.avail_in = 0;
        ret = deflate(&deflate_strm, fuzz_int32_t_9);
        
        assert_file_closed(&reopen_file);;
    }
    
    // 16. Clean up all resources
    inflateEnd(&inflate_strm);
    deflateEnd(&deflate_strm);
    gzclose(gz_file);
    free(compressed_buffer);
    
    // 17. Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&reopen_file);
	assert_file_closed(&input_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}