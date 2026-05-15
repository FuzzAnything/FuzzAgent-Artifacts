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
//<ID> 198
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 1
//<Quality> {"density":13,"unique_branches":{"gzgetc":[[481,9,481,31,0,0,4,0]]},"library_calls":["gzdopen","gzdirect","gzgetc_","gzungetc","gzclearerr","inflateInit2_","gzclose","inflateEnd","gzclose","inflate","inflateSyncPoint","inflateCopy","inflateReset2","inflateEnd","gzopen","gzwrite","gzgetc_","gzclearerr","gzclose","deflateInit2_","deflateSetDictionary","deflateGetDictionary","deflateBound","deflate","deflateEnd","inflateEnd","gzclose"],"critical_calls":["gzdopen","gzdirect","gzgetc_","gzungetc","gzclearerr","inflateInit2_","inflate","inflateSyncPoint","inflateCopy","inflateReset2","inflateEnd","gzopen","gzwrite","gzgetc_","gzclearerr","gzclose","deflateInit2_","deflateSetDictionary","deflateGetDictionary","deflateBound","deflate","deflateEnd","inflateEnd","gzclose"],"visited":0}
/*Here's a fuzz driver that uses the specified zlib APIs to handle zlib-compressed data:

*/


extern "C" int LLVMFuzzerTestOneInput_73(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
        // Too small to be meaningful zlib data
        return 0;
    }

    // Create input FILE* from memory buffer
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

    int fd = fuzz_fileno(in_file);
    
    // 1. Open compressed file with gzdopen
    gzFile gz_in = gzdopen(fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 2. Check if file is in gzip format
    int is_direct = gzdirect(gz_in);

    // 3. Try to unget a character
    int first_char = gzgetc_(gz_in);
    if (first_char != -1) {
        gzungetc(first_char, gz_in);
    }

    // 4. Clear any errors on gzFile
    gzclearerr(gz_in);

    // Set up zlib streams for inflation
    z_stream strm_inflate1, strm_inflate2;
    memset(&strm_inflate1, 0, sizeof(strm_inflate1));
    memset(&strm_inflate2, 0, sizeof(strm_inflate2));

    // 5. Initialize inflation stream
    int ret = inflateInit2_(&strm_inflate1, MAX_WBITS + 16, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Set up input for inflation
    strm_inflate1.next_in = (Bytef *)data;
    strm_inflate1.avail_in = (uInt)(size > UINT_MAX ? UINT_MAX : size);
    
    // Allocate output buffer for inflation
    size_t out_buf_size = size * 4; // Reasonable expansion factor
    if (out_buf_size > 10 * 1024 * 1024) { // Cap at 10MB
        out_buf_size = 10 * 1024 * 1024;
    }
    Bytef *out_buf = (Bytef *)malloc(out_buf_size);
    if (!out_buf) {
        inflateEnd(&strm_inflate1);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    strm_inflate1.next_out = out_buf;
    strm_inflate1.avail_out = (uInt)(out_buf_size > UINT_MAX ? UINT_MAX : out_buf_size);

    // 6. Perform inflation
    ret = inflate(&strm_inflate1, Z_SYNC_FLUSH);
    
    // 7. Check sync point (even if inflation failed)
    inflateSyncPoint(&strm_inflate1);

    // 8. Copy inflation stream
    ret = inflateCopy(&strm_inflate2, &strm_inflate1);
    
    if (ret == Z_OK) {
        // 9. Reset the copied stream with different window bits
        inflateReset2(&strm_inflate2, MAX_WBITS);
        inflateEnd(&strm_inflate2);
    }

    // 10. Also try to open a regular gzFile
    gzFile gz_out = gzopen("output_file.gz", "wb");
    if (gz_out) {
        // Write inflated data if we have any
        if (strm_inflate1.total_out > 0) {
            gzwrite(gz_out, out_buf, (unsigned int)strm_inflate1.total_out);
        }
        
        // 11. Try to get a character
        gzgetc_(gz_out);
        
        // 12. Clear errors
        gzclearerr(gz_out);
        
        gzclose(gz_out);
    }

    // Set up deflation stream
    z_stream strm_deflate;
    memset(&strm_deflate, 0, sizeof(strm_deflate));
    
    deflateInit2(&strm_deflate, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                 MAX_WBITS, 8, Z_DEFAULT_STRATEGY);
    
    // 13. Set dictionary for deflation (use part of input as dictionary)
    if (size > 32) {
        uInt dict_len = 32;
        Bytef dictionary[32];
        memcpy(dictionary, data, 32);
        deflateSetDictionary(&strm_deflate, dictionary, dict_len);
        
        // 14. Get dictionary back
        Bytef retrieved_dict[32];
        uInt retrieved_len = 32;
        deflateGetDictionary(&strm_deflate, retrieved_dict, &retrieved_len);
    }
    
    // 15. Calculate bound for deflated data
    uLong bound = deflateBound(&strm_deflate, (uLong)size);
    
    // Prepare deflation
    Bytef *deflated_buf = (Bytef *)malloc(bound);
    if (deflated_buf) {
        strm_deflate.next_in = (Bytef *)data;
        strm_deflate.avail_in = (uInt)(size > UINT_MAX ? UINT_MAX : size);
        strm_deflate.next_out = deflated_buf;
        strm_deflate.avail_out = (uInt)(bound > UINT_MAX ? UINT_MAX : bound);
        
        // 16. Perform deflation
        deflate(&strm_deflate, Z_FINISH);
        
        // Write deflated data to file
        if (strm_deflate.total_out > 0) {
            fwrite(deflated_buf, 1, strm_deflate.total_out, out_file);
        }
        
        free(deflated_buf);
    }
    
    // 17. End deflation
    deflateEnd(&strm_deflate);
    
    // 18. End inflation
    inflateEnd(&strm_inflate1);
    
    // 19. Close gzFile
    gzclose(gz_in);
    
    // Clean up
    free(out_buf);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove temporary output files
    remove("output_file");
    remove("output_file.gz");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file.gz");
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file.gz");
	assert_fd_closed(fd);
	return 0;
}