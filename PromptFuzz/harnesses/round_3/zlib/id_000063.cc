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
//<ID> 129
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["zlibCompileFlags","gzopen64","inflateInit_","gzclose","deflateInit_","inflateEnd","gzclose","deflateEnd","inflateEnd","gzclose","crc32","inflateSync","inflateSyncPoint","inflateReset2","inflateReset","deflateBound_z","deflateParams","crc32_combine_gen64","gzread","deflateEnd","inflateEnd","gzclose"],"critical_calls":["zlibCompileFlags","gzopen64","inflateInit_","deflateInit_","crc32","inflateSync","inflateSyncPoint","inflateReset2","inflateReset","deflateBound_z","deflateParams","crc32_combine_gen64","gzread","deflateEnd","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t* f_data, size_t f_size) {
	if(f_size<59) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // 1. Call zlibCompileFlags at beginning
    uLong compile_flags = zlibCompileFlags();
    (void)compile_flags; // Avoid unused variable warning

    // 2. Create input file from memory buffer for gzopen64
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Open the file with gzopen64 (high-level API)
    gzFile gz = gzopen64("input_file", fuzz_str_1);
    if (!gz) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // 4. Set up inflate stream for low-level decompression
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    
    // Initialize inflate stream
    int ret = inflateInit_(&inflate_strm, fuzz_str_2, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // 5. Set up deflate stream for compression
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    
    // Initialize deflate stream
    ret = deflateInit_(&deflate_strm, fuzz_int32_t_4, fuzz_str_3, sizeof(z_stream));
    if (ret != Z_OK) {
        inflateEnd(&inflate_strm);
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // 6. Allocate buffers for processing
    const size_t CHUNK_SIZE = 4096;
    Bytef *in_buf = (Bytef *)malloc(CHUNK_SIZE);
    Bytef *out_buf = (Bytef *)malloc(CHUNK_SIZE * 2);
    Bytef *dict_buf = (Bytef *)malloc(1024);
    
    if (!in_buf || !out_buf || !dict_buf) {
        free(in_buf);
        free(out_buf);
        free(dict_buf);
        deflateEnd(&deflate_strm);
        inflateEnd(&inflate_strm);
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // 7. Process input data through multiple paths
    size_t bytes_processed = 0;
    uLong crc = 0;
    
    while (bytes_processed < size && bytes_processed < CHUNK_SIZE) {
        size_t to_copy = (size - bytes_processed) < CHUNK_SIZE ? 
                        (size - bytes_processed) : CHUNK_SIZE;
        
        memcpy(in_buf, data + bytes_processed, to_copy);
        bytes_processed += to_copy;

        // 8. Compute CRC32
        crc = crc32(fuzz_uint64_t_5, in_buf, (uInt)to_copy);

        // 9. Try inflateSync
        inflate_strm.avail_in = (uInt)to_copy;
        inflate_strm.next_in = in_buf;
        inflateSync(&inflate_strm);

        // 10. Check sync point
        inflateSyncPoint(&inflate_strm);

        // 11. Try inflateReset2 with different window bits
        inflateReset2(&inflate_strm, 15 + 16); // gzip mode

        // 12. Try inflateReset
        inflateReset(&inflate_strm);

        // 13. Use deflateBound_z to estimate compressed size
        z_size_t bound = deflateBound_z(&deflate_strm, (z_size_t)to_copy);
        (void)bound; // Avoid unused variable warning

        // 14. Try deflateParams
        deflateParams(&deflate_strm, fuzz_int32_t_6, fuzz_int32_t_7);

        // 15. Generate CRC combine function (even if not used)
        uLong combine_gen = crc32_combine_gen64((off64_t)to_copy);
        (void)combine_gen; // Avoid unused variable warning
    }

    // 16. Try to read from gzFile (simulate reading decompressed data)
    int bytes_read = gzread(gz, out_buf, 1024);
    (void)bytes_read; // Avoid unused variable warning

    // 17. Clean up all resources
    free(dict_buf);
    free(out_buf);
    free(in_buf);
    deflateEnd(&deflate_strm);
    inflateEnd(&inflate_strm);
    gzclose(gz);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}