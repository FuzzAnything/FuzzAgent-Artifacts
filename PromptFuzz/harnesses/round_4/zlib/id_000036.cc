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
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":7,"unique_branches":{"inflatePrime":[[223,9,223,18,0,0,4,0]]},"library_calls":["gzdopen","deflateInit2_","deflateBound_z","uncompress2","gzseek","gzread","inflateInit2_","inflateGetHeader","inflateGetDictionary","deflatePrime","inflatePrime","crc32","crc32","crc32_combine","inflateEnd","deflateEnd","gzclose"],"critical_calls":["gzdopen","deflateInit2_","deflateBound_z","uncompress2","gzseek","gzread","inflateInit2_","inflateGetHeader","inflateGetDictionary","deflatePrime","inflatePrime","crc32","crc32","crc32_combine","inflateEnd","deflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<25) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
	//fuzzer shim end}




    // Early return if input is too small for meaningful operations
    if (size < 20) {
	return 0;
	}
	

    // 1. Create FILE* for input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // 2. Get file descriptor and open as gzFile
    int fd = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd, fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 3. Prepare for uncompression using uncompress2
    // Use first half of data as compressed source
    size_t comp_len = size / 2;
    const Bytef* source = data + comp_len;
    uLong sourceLen = size - comp_len;
    
    // Allocate destination buffer using deflateBound_z for safety
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                 MAX_WBITS, 8, Z_DEFAULT_STRATEGY);
    
    // Use deflateBound_z to estimate buffer size
    z_size_t bound_size = deflateBound_z(&strm, comp_len);
    Bytef* dest = (Bytef*)malloc(bound_size);
    uLongf destLen = bound_size;
    
    // 4. Perform uncompress2
    uLong actualSourceLen = sourceLen;
    int uncompress_result = uncompress2(dest, &destLen, source, &actualSourceLen);
    
    // 5. Use gzseek on the gzFile
    // Seek to middle of file and read some data
    if (gzseek(gz_in, size/4, fuzz_int32_t_2) != -1) {
        char buf[100];
        gzread(gz_in, buf, sizeof(buf));
    }

    // 6. Setup inflation stream for header and dictionary operations
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    inflateInit2(&inflate_strm, MAX_WBITS);
    
    // 7. Use inflateGetHeader
    gz_header head;
    memset(&head, 0, sizeof(head));
    inflateGetHeader(&inflate_strm, &head);
    
    // 8. Use inflateGetDictionary
    Bytef dict_buf[256];
    uInt dict_len = sizeof(dict_buf);
    inflateGetDictionary(&inflate_strm, dict_buf, &dict_len);
    
    // 9. Use deflatePrime (add some bits from input)
    if (size > 10) {
        int bits = data[0] % 16;  // 0-15 bits
        int value = data[1] & ((1 << bits) - 1);
        deflatePrime(&strm, bits, value);
    }
    
    // 10. Use inflatePrime (add some bits from input)
    if (size > 20) {
        int bits = data[2] % 16;  // 0-15 bits
        int value = data[3] & ((1 << bits) - 1);
        inflatePrime(&inflate_strm, bits, value);
    }
    
    // 11. Use crc32_combine
    // Compute two CRC32 values from different parts of input
    uLong crc1 = crc32(fuzz_uint64_t_3, data, size/3);
    uLong crc2 = crc32(fuzz_uint64_t_4, data + size/3, size/3);
    off_t len2 = size/3;
    uLong combined_crc = crc32_combine(crc1, crc2, len2);
    
    // 12. Clean up resources
    inflateEnd(&inflate_strm);
    deflateEnd(&strm);
    free(dest);
    gzclose(gz_in);
    // Note: gzclose already closes the underlying fd, so we don't need to fclose in_file
    // But we need to ensure the FILE* is properly closed
    assert_file_closed(&in_file);;
    
    // Use the combined_crc to prevent optimization
    if (combined_crc == 0x12345678) {
        // This should never happen, just to use the variable
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 1;
    }
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}