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
//<ID> 121
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 6
//<Quality> {"density":12,"unique_branches":{"inflateBack":[[563,13,563,62,0,0,4,0],[564,13,564,32,0,0,4,0],[564,13,564,32,0,0,4,1],[157,17,157,41,16,0,4,0],[157,17,157,41,51,0,4,0],[157,17,157,41,64,0,4,0]]},"library_calls":["zlibVersion","gzdopen","gzdirect","inflateBackInit_","gzclose","compressBound_z","inflateBackEnd","gzclose","inflatePrime","inflatePrime","inflateUndermine","inflateBack","inflateCodesUsed","uncompress2_z","crc32","crc32","crc32","crc32_combine_op","crc32_combine_gen","deflateInit_","deflateReset","deflateEnd","inflateBackEnd","gzclose"],"critical_calls":["zlibVersion","gzdopen","gzdirect","inflateBackInit_","compressBound_z","inflatePrime","inflatePrime","inflateUndermine","inflateBack","inflateCodesUsed","uncompress2_z","crc32","crc32","crc32","crc32_combine_op","crc32_combine_gen","deflateInit_","deflateReset","deflateEnd","inflateBackEnd","gzclose"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Get zlib version
    const char *version = zlibVersion();
    
    // 2. Create a FILE* from input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Get file descriptor for gzdopen
    int fd = fuzz_fileno(in_file);
    
    // 4. Open gzFile for reading
    gzFile gz = gzdopen(fd, "rb");
    if (!gz) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 5. Check if file is in direct mode
    int direct = gzdirect(gz);
    
    // 6. Prepare for decompression using inflateBack
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = Z_NULL;
    strm.avail_in = 0;
    
    unsigned char window[32768]; // 32KB window as recommended
    
    // 7. Initialize inflateBack
    int ret = inflateBackInit_(&strm, 15, window, version, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 8. Prepare input descriptor for inflateBack callbacks
    struct back_input_desc {
        const uint8_t *data;
        size_t size;
        size_t pos;
    } in_desc;
    
    in_desc.data = data;
    in_desc.size = size;
    in_desc.pos = 0;
    
    // Input callback for inflateBack
    auto in_func = [](void *desc, unsigned char **buf) -> unsigned int {
        back_input_desc *d = (back_input_desc *)desc;
        
        if (d->pos >= d->size) {
            return 0; // No more data
        }
        
        *buf = (unsigned char *)(d->data + d->pos);
        unsigned int available = d->size - d->pos;
        
        // Return at most 4096 bytes at a time
        unsigned int to_read = (available > 4096) ? 4096 : (unsigned int)available;
        d->pos += to_read;
        
        return to_read;
    };
    
    // Prepare output buffer and descriptor
    z_size_t max_output = compressBound_z(size); // Get maximum possible output size
    Bytef *output_buf = (Bytef *)malloc(max_output);
    if (!output_buf) {
        inflateBackEnd(&strm);
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    struct back_output_desc {
        Bytef *buf;
        z_size_t pos;
        z_size_t max_size;
    } out_desc;
    
    out_desc.buf = output_buf;
    out_desc.pos = 0;
    out_desc.max_size = max_output;
    
    // Output callback for inflateBack
    auto out_func = [](void *desc, unsigned char *buf, unsigned int len) -> int {
        back_output_desc *d = (back_output_desc *)desc;
        
        if (d->pos + len > d->max_size) {
            return 1; // Buffer overflow
        }
        
        memcpy(d->buf + d->pos, buf, len);
        d->pos += len;
        return 0;
    };
    
    // 9. Prime inflate with some bits from the beginning of input
    if (size >= 2) {
        inflatePrime(&strm, 8, data[0]);
        if (size >= 3) {
            inflatePrime(&strm, 8, data[1]);
        }
    }
    
    // 10. Try to undermine the stream (for testing error conditions)
    inflateUndermine(&strm, 1);
    
    // 11. Perform decompression using inflateBack
    ret = inflateBack(&strm, in_func, &in_desc, out_func, &out_desc);
    
    // 12. Get information about codes used
    unsigned long codes_used = inflateCodesUsed(&strm);
    
    // 13. Also try uncompress2_z as alternative decompression method
    z_size_t destLen = max_output;
    z_size_t sourceLen = size;
    Bytef *dest2 = (Bytef *)malloc(max_output);
    if (dest2) {
        int uncompress_ret = uncompress2_z(dest2, &destLen, data, &sourceLen);
        free(dest2);
    }
    
    // 14. Calculate CRC32 of the original data
    uLong crc1 = crc32(0L, Z_NULL, 0);
    if (size > 0) {
        // Calculate CRC in two parts to demonstrate crc32_combine_op
        size_t half = size / 2;
        uLong crc2 = crc32(0L, data, (uInt)half);
        uLong crc3 = crc32(0L, data + half, (uInt)(size - half));
        
        // Combine the two CRCs
        uLong combined = crc32_combine_op(crc2, crc3, crc32_combine_gen((off_t)(size - half)));
    }
    
    // 15. Initialize a deflate stream for demonstration
    z_stream deflate_strm;
    deflate_strm.zalloc = Z_NULL;
    deflate_strm.zfree = Z_NULL;
    deflate_strm.opaque = Z_NULL;
    
    ret = deflateInit_(&deflate_strm, Z_DEFAULT_COMPRESSION, version, sizeof(z_stream));
    if (ret == Z_OK) {
        // Reset deflate stream
        deflateReset(&deflate_strm);
        // Clean up deflate stream
        deflateEnd(&deflate_strm);
    }
    
    // 16. Clean up inflateBack resources
    inflateBackEnd(&strm);
    
    // 17. Close gzFile
    gzclose(gz);
    
    // 18. Close the memory file
    assert_file_closed(&in_file);;
    
    // 19. Free allocated output buffer
    free(output_buf);
    
    // 20. Create and close an output file for demonstration
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        assert_file_closed(&out_file);;
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}