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
//<ID> 107
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 5
//<Quality> {"density":21,"unique_branches":{"deflate":[[1029,36,1029,48,0,0,4,0]],"deflateSetDictionary":[[571,23,571,32,0,0,4,1],[575,9,575,18,0,0,4,1],[581,13,581,22,0,0,4,0]],"deflateBound_z":[[884,5,884,11,0,0,4,0]]},"library_calls":["gzdopen","gzdirect","gzungetc","inflateInit2_","gzclose","inflateEnd","gzclose","inflateEnd","gzclose","inflate","inflateSyncPoint","inflateCopy","inflateReset2","inflateReset","gzerror","deflateInit2_","inflateEnd","inflateEnd","gzclose","deflateSetDictionary","deflateGetDictionary","deflateBound","deflateEnd","inflateEnd","inflateEnd","gzclose","deflate","deflateEnd","inflateEnd","inflateEnd","gzclose"],"critical_calls":["gzdopen","gzdirect","gzungetc","inflateInit2_","inflate","inflateSyncPoint","inflateCopy","inflateReset2","inflateReset","gzerror","deflateInit2_","deflateSetDictionary","deflateGetDictionary","deflateBound","deflate","deflateEnd","inflateEnd","inflateEnd","gzclose"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Create input FILE* from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Get file descriptor and open as gzFile
    int fd = fuzz_fileno(in_file);
    const char *mode = "rb";
    gzFile gz = gzdopen(fd, mode);
    if (!gz) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Check if file is in gzip format
    int is_direct = gzdirect(gz);

    // Push back a character to test gzungetc
    if (size > 0) {
        int first_byte = gzgetc(gz);
        if (first_byte != -1) {
            gzungetc(first_byte, gz);
        }
    }

    // Initialize inflate stream for decompression
    z_stream strm_inflate;
    memset(&strm_inflate, 0, sizeof(z_stream));
    
    // Use appropriate windowBits based on gzdirect result
    int windowBits = is_direct ? -MAX_WBITS : MAX_WBITS + 16;
    int ret = inflateInit2_(&strm_inflate, windowBits, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Allocate input buffer for compressed data
    Bytef *compressed = (Bytef*)malloc(size);
    if (!compressed) {
        inflateEnd(&strm_inflate);
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    memcpy(compressed, data, size);
    
    // Set up inflate stream
    strm_inflate.next_in = compressed;
    strm_inflate.avail_in = (uInt)size;

    // Allocate output buffer for decompressed data
    size_t decompressed_size = size * 4; // Initial guess
    Bytef *decompressed = (Bytef*)malloc(decompressed_size);
    if (!decompressed) {
        free(compressed);
        inflateEnd(&strm_inflate);
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Decompress the data
    strm_inflate.next_out = decompressed;
    strm_inflate.avail_out = (uInt)decompressed_size;
    
    ret = inflate(&strm_inflate, Z_FINISH);
    
    // Check sync point
    inflateSyncPoint(&strm_inflate);

    // Create copy of inflate stream
    z_stream strm_inflate_copy;
    memset(&strm_inflate_copy, 0, sizeof(z_stream));
    inflateCopy(&strm_inflate_copy, &strm_inflate);

    // Reset the copied stream
    inflateReset2(&strm_inflate_copy, windowBits);
    inflateReset(&strm_inflate_copy);

    // Get error from gzFile
    int gz_errnum;
    const char *gz_err = gzerror(gz, &gz_errnum);

    // Initialize deflate stream for recompression
    z_stream strm_deflate;
    memset(&strm_deflate, 0, sizeof(z_stream));
    
    ret = deflateInit2_(&strm_deflate, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                       windowBits, 8, Z_DEFAULT_STRATEGY,
                       ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        free(decompressed);
        free(compressed);
        inflateEnd(&strm_inflate);
        inflateEnd(&strm_inflate_copy);
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Set dictionary for deflate (using first part of decompressed data as dictionary)
    uInt dict_len = 0;
    if (strm_inflate.total_out > 0) {
        dict_len = (uInt)(strm_inflate.total_out < 32768 ? strm_inflate.total_out : 32768);
        deflateSetDictionary(&strm_deflate, decompressed, dict_len);
    }

    // Get dictionary back
    Bytef *dict_buf = (Bytef*)malloc(dict_len);
    if (dict_buf) {
        uInt dict_len_out = dict_len;
        deflateGetDictionary(&strm_deflate, dict_buf, &dict_len_out);
        free(dict_buf);
    }

    // Calculate bound for compressed output
    uLong bound = deflateBound(&strm_deflate, strm_inflate.total_out);

    // Allocate buffer for recompressed data
    Bytef *recompressed = (Bytef*)malloc(bound);
    if (!recompressed) {
        deflateEnd(&strm_deflate);
        free(decompressed);
        free(compressed);
        inflateEnd(&strm_inflate);
        inflateEnd(&strm_inflate_copy);
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Recompress the decompressed data
    strm_deflate.next_in = decompressed;
    strm_deflate.avail_in = (uInt)strm_inflate.total_out;
    strm_deflate.next_out = recompressed;
    strm_deflate.avail_out = (uInt)bound;
    
    ret = deflate(&strm_deflate, Z_FINISH);

    // Write recompressed data to output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        fwrite(recompressed, 1, strm_deflate.total_out, out_file);
        assert_file_closed(&out_file);;
    }

    // Cleanup
    deflateEnd(&strm_deflate);
    inflateEnd(&strm_inflate);
    inflateEnd(&strm_inflate_copy);
    gzclose(gz);
    assert_file_closed(&in_file);;
    
    free(compressed);
    free(decompressed);
    free(recompressed);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}