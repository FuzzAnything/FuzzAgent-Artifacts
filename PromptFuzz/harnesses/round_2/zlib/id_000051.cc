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
//<ID> 122
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["zlibVersion","gzdopen","gzdirect","deflateInit_","gzclose","deflateReset","compressBound_z","deflateEnd","gzclose","inflateBackInit_","deflateEnd","gzclose","inflatePrime","inflateBack","inflateUndermine","inflateCodesUsed","inflateBackEnd","uncompress2_z","crc32","crc32","crc32","crc32_combine_op","deflateEnd","gzclose"],"critical_calls":["zlibVersion","gzdopen","gzdirect","deflateInit_","deflateReset","compressBound_z","inflateBackInit_","inflatePrime","inflateBack","inflateUndermine","inflateCodesUsed","inflateBackEnd","uncompress2_z","crc32","crc32","crc32","crc32_combine_op","deflateEnd","gzclose"],"visited":1}
/**/


// Custom input function for inflateBack
static unsigned int  id_000051_custom_in_func(void* desc, unsigned char** buf) {
    // We'll use the input data directly from memory
    static size_t pos = 0;
    size_t* desc_ptr = (size_t*)desc;
    size_t size = desc_ptr[0];
    const uint8_t* data = (const uint8_t*)desc_ptr[1];
    
    if (pos >= size) {
        return 0; // No more data
    }
    
    // Point to the remaining data
    *buf = (unsigned char*)data + pos;
    
    // Calculate how much to return (at most 1024 bytes at a time)
    size_t remaining = size - pos;
    size_t to_return = remaining > 1024 ? 1024 : remaining;
    
    pos += to_return;
    return (unsigned int)to_return;
}

// Custom output function for inflateBack
static int  id_000051_custom_out_func(void* desc, unsigned char* buf, unsigned int size) {
    // We'll just discard the output for fuzzing
    (void)desc;
    (void)buf;
    (void)size;
    return 0; // Success
}

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
        return 0; // Need at least some data
    }

    // 1. Get zlib version
    const char* version = zlibVersion();
    if (version == nullptr) {
	return 0;
	}
	

    // 2. Create FILE* for input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file == nullptr) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // 3. Open gzFile from file descriptor
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    gzFile gz_file = gzdopen(fd, "rb");
    if (gz_file == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 4. Check if gzFile is in direct mode
    int is_direct = gzdirect(gz_file);

    // 5. Initialize deflate stream
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(z_stream));
    
    int deflate_init_ret = deflateInit_(&deflate_strm, Z_DEFAULT_COMPRESSION, 
                                        version, sizeof(z_stream));
    if (deflate_init_ret != Z_OK) {
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 6. Reset deflate stream
    int deflate_reset_ret = deflateReset(&deflate_strm);
    
    // 7. Get compressed size bound for decompression
    z_size_t bound = compressBound_z(size);

    // 8. Initialize inflateBack stream
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(z_stream));
    
    // Allocate window buffer for inflateBack
    unsigned char* window = (unsigned char*)malloc(32768);
    if (window == nullptr) {
        deflateEnd(&deflate_strm);
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    int inflate_back_init_ret = inflateBackInit_(&inflate_strm, 15, window, 
                                                 version, sizeof(z_stream));
    if (inflate_back_init_ret != Z_OK) {
        free(window);
        deflateEnd(&deflate_strm);
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 9. Prime inflate stream with some bits
    int inflate_prime_ret = inflatePrime(&inflate_strm, 4, data[0] & 0x0F);

    // 10. Setup descriptor for input function
    size_t desc[2];
    desc[0] = size;
    desc[1] = (size_t)data;

    // 11. Perform inflateBack
    int inflate_back_ret = inflateBack(&inflate_strm, 
                                        id_000051_custom_in_func, (void*)desc,
                                        id_000051_custom_out_func, nullptr);

    // 12. Try to undermine inflate (corrupt state)
    int undermine_ret = inflateUndermine(&inflate_strm, 1);

    // 13. Get code usage
    unsigned long codes_used = inflateCodesUsed(&inflate_strm);

    // 14. End inflateBack
    int inflate_back_end_ret = inflateBackEnd(&inflate_strm);
    free(window);

    // 15. Try to decompress with uncompress2_z
    // Allocate output buffer
    z_size_t dest_len = bound;
    Bytef* dest = (Bytef*)malloc(dest_len);
    if (dest != nullptr) {
        z_size_t source_len = size;
        int uncompress_ret = uncompress2_z(dest, &dest_len, 
                                          data, &source_len);
        free(dest);
    }

    // 16. Compute CRC32 checksums
    uLong crc1 = crc32(0L, nullptr, 0);
    if (size > 0) {
        crc1 = crc32(crc1, data, (uInt)(size > 100 ? 100 : size));
    }
    
    // Compute another CRC if we have enough data
    uLong crc2 = 0;
    if (size > 100) {
        crc2 = crc32(0L, data + 100, (uInt)(size - 100 > 100 ? 100 : size - 100));
    }
    
    // Combine CRC operations
    uLong combined_crc = crc32_combine_op(crc1, crc2, 0);

    // 17. End deflate stream
    int deflate_end_ret = deflateEnd(&deflate_strm);

    // 18. Close gzFile
    int gzclose_ret = gzclose(gz_file);
    
    // Clean up
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}