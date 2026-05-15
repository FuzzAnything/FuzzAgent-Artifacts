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
//<ID> 150
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["zlibVersion","gzdopen","gzdirect","deflateInit_","gzclose","deflateReset","deflateEnd","gzclose","compressBound_z","inflateBackInit_","deflateEnd","gzclose","inflatePrime","inflateBack","inflateUndermine","inflateCodesUsed","inflateBackEnd","uncompress2_z","crc32","crc32","crc32","crc32_combine_op","deflatePending","deflateEnd","inflateInit_","inflateCopy","inflateEnd","inflateEnd","gzclose"],"critical_calls":["zlibVersion","gzdopen","gzdirect","deflateInit_","deflateReset","compressBound_z","inflateBackInit_","inflatePrime","inflateBack","inflateUndermine","inflateCodesUsed","inflateBackEnd","uncompress2_z","crc32","crc32","crc32","crc32_combine_op","deflatePending","deflateEnd","inflateInit_","inflateCopy","inflateEnd","inflateEnd","gzclose"],"visited":0}
/*To create a comprehensive fuzzing driver that exercises the specified zlib APIs, we'll implement a scenario that involves compression, decompression, CRC calculation, and stream manipulation. Here's the fuzz driver:

*/


// Custom in_func for inflateBack
static unsigned int  id_000057_my_in_func(void* desc, unsigned char** buf) {
    static unsigned char data[1024];
    FILE* f = (FILE*)desc;
    size_t n = fread(data, 1, 1024, f);
    if (n > 0) {
        *buf = data;
        return n;
    }
    return 0;
}

// Custom out_func for inflateBack
static int  id_000057_my_out_func(void* desc, unsigned char* buf, unsigned int len) {
    FILE* f = (FILE*)desc;
    return fwrite(buf, 1, len, f) == len ? 0 : 1;
}

extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for various operations
    
    // 1. Get zlib version
    const char* version = zlibVersion();
    if (version == nullptr) {
	return 0;
	}
	
    
    // Create input file from memory
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Open gzip file using file descriptor
    int fd = fuzz_fileno(in_file);
    gzFile gzfile = gzdopen(fd, "rb");
    if (!gzfile) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 3. Check if gzip file is direct
    int is_direct = gzdirect(gzfile);
    
    // 4. Initialize deflate stream
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int ret = deflateInit_(&def_strm, Z_DEFAULT_COMPRESSION, version, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gzfile);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 5. Reset deflate stream
    ret = deflateReset(&def_strm);
    if (ret != Z_OK) {
        deflateEnd(&def_strm);
        gzclose(gzfile);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 6. Calculate compressed bound
    z_size_t bound = compressBound_z(size);
    
    // 7. Initialize inflate back stream
    z_stream inf_back_strm;
    memset(&inf_back_strm, 0, sizeof(inf_back_strm));
    unsigned char window[32768];  // 32K window
    
    // Try different window bits based on input
    int windowBits = (data[0] % 10) + 8;  // 8-17 bits
    ret = inflateBackInit_(&inf_back_strm, windowBits, window, version, sizeof(z_stream));
    if (ret != Z_OK) {
        deflateEnd(&def_strm);
        gzclose(gzfile);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 8. Prime inflate stream with some bits from input
    int prime_bits = (data[1] % 16) + 1;  // 1-16 bits
    int prime_value = data[2];
    ret = inflatePrime(&inf_back_strm, prime_bits, prime_value);
    
    // 9. Perform inflate back operation
    rewind(in_file);  // Reset file pointer
    ret = inflateBack(&inf_back_strm,  id_000057_my_in_func, in_file,  id_000057_my_out_func, out_file);
    
    // 10. Try to undermine inflate stream
    int undermine_val = (data[3] % 2);  // 0 or 1
    ret = inflateUndermine(&inf_back_strm, undermine_val);
    
    // 11. Get codes used by inflate
    unsigned long codes_used = inflateCodesUsed(&inf_back_strm);
    
    // 12. End inflate back
    inflateBackEnd(&inf_back_strm);
    
    // 13. Uncompress data using uncompress2_z
    Bytef* uncompressed = (Bytef*)malloc(bound);
    z_size_t destLen = bound;
    z_size_t sourceLen = size;
    const Bytef* source = (const Bytef*)data;
    
    ret = uncompress2_z(uncompressed, &destLen, source, &sourceLen);
    free(uncompressed);
    
    // 14. Calculate CRC32
    uLong crc1 = crc32(0L, Z_NULL, 0);
    uInt crc_len = size < 1000 ? (uInt)size : 1000;
    crc1 = crc32(crc1, data, crc_len);
    
    // Calculate second CRC on remaining data
    uLong crc2 = 0;
    if (size > 1000) {
        crc2 = crc32(0L, data + 1000, (uInt)(size - 1000));
    }
    
    // 15. Combine CRC operations
    uLong combined_crc = crc32_combine_op(crc1, crc2, 0);
    
    // 16. Check for pending data in deflate stream
    unsigned int pending;
    int bits;
    ret = deflatePending(&def_strm, &pending, &bits);
    
    // 17. End deflate stream
    deflateEnd(&def_strm);
    
    // 18. Create and copy an inflate stream
    z_stream inf_strm1, inf_strm2;
    memset(&inf_strm1, 0, sizeof(inf_strm1));
    memset(&inf_strm2, 0, sizeof(inf_strm2));
    
    inflateInit_(&inf_strm1, version, sizeof(z_stream));
    
    // Copy the stream
    ret = inflateCopy(&inf_strm2, &inf_strm1);
    
    inflateEnd(&inf_strm1);
    inflateEnd(&inf_strm2);
    
    // 19. Read a byte from gzip file
    rewind(in_file);  // Reset for gzread
    int gz_byte = gzgetc(gzfile);
    
    // 20. Close gzip file
    gzclose(gzfile);
    
    // Clean up
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    // Use calculated values to avoid optimization
    volatile int use_is_direct = is_direct;
    volatile z_size_t use_bound = bound;
    volatile int use_prime_bits = prime_bits;
    volatile unsigned long use_codes = codes_used;
    volatile uLong use_crc = combined_crc;
    volatile unsigned int use_pending = pending;
    volatile int use_gz_byte = gz_byte;
    
    (void)use_is_direct;
    (void)use_bound;
    (void)use_prime_bits;
    (void)use_codes;
    (void)use_crc;
    (void)use_pending;
    (void)use_gz_byte;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}