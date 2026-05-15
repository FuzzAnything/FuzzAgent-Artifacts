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
//<ID> 211
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"deflateParams":[[786,51,786,69,0,0,4,0]]},"library_calls":["zlibVersion","compressBound_z","compress2_z","uncompress2_z","deflateInit_","deflateSetDictionary","deflateBound","deflateEnd","deflateBound","deflate","deflateParams","deflateEnd","inflateInit_","inflateEnd","inflate","inflateMark","inflateCopy","inflateReset2","inflateCodesUsed","inflateEnd","inflateEnd","gzdopen","gzbuffer","gzwrite","gzclose"],"critical_calls":["zlibVersion","compressBound_z","compress2_z","uncompress2_z","deflateInit_","deflateSetDictionary","deflateBound","deflateBound","deflate","deflateParams","deflateEnd","inflateInit_","inflate","inflateMark","inflateCopy","inflateReset2","inflateCodesUsed","inflateEnd","inflateEnd","gzdopen","gzbuffer","gzwrite","gzclose"],"visited":1}
/*Here's a comprehensive fuzz driver that uses all the specified zlib APIs to perform compression, decompression, and stream manipulation:

*/


extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // 1. Get zlib version
    const char *version = zlibVersion();
    (void)version; // Use version to avoid unused variable warning

    // 2. Create a temporary file with input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Prepare compressed buffer using compress2_z
    z_size_t comprLen = compressBound_z(size);
    Bytef *compr = (Bytef *)malloc(comprLen);
    if (!compr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    z_size_t sourceLen = size;
    int level = data[0] % 10; // Random level from 0-9
    int ret = compress2_z(compr, &comprLen, data, sourceLen, level);
    if (ret != Z_OK) {
        free(compr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 4. Decompress using uncompress2_z
    z_size_t uncomprLen = size * 2; // Estimate
    Bytef *uncompr = (Bytef *)malloc(uncomprLen);
    if (!uncompr) {
        free(compr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    z_size_t sourceLenForUncompress = comprLen;
    ret = uncompress2_z(uncompr, &uncomprLen, compr, &sourceLenForUncompress);
    
    // Even if this fails, continue with other APIs

    // 5. Set up deflate stream
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(z_stream));
    def_strm.zalloc = Z_NULL;
    def_strm.zfree = Z_NULL;
    def_strm.opaque = Z_NULL;
    
    ret = deflateInit_(&def_strm, level, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        free(uncompr);
        free(compr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 6. Set up deflate dictionary
    if (size > 10) {
        deflateSetDictionary(&def_strm, data, (uInt)(size % 10 + 1));
    }

    // 7. Compress using deflate
    Bytef *deflateOut = (Bytef *)malloc(deflateBound(&def_strm, uncomprLen));
    if (!deflateOut) {
        deflateEnd(&def_strm);
        free(uncompr);
        free(compr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    def_strm.next_in = uncompr;
    def_strm.avail_in = (uInt)uncomprLen;
    def_strm.next_out = deflateOut;
    def_strm.avail_out = (uInt)deflateBound(&def_strm, uncomprLen);
    
    ret = deflate(&def_strm, Z_FINISH);
    
    // 8. Change deflate parameters
    int strategy = (data[0] % 5) + 1; // Random strategy
    deflateParams(&def_strm, level, strategy);
    
    // 9. End deflate stream
    deflateEnd(&def_strm);
    free(deflateOut);

    // 10. Set up inflate stream
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(z_stream));
    inf_strm.zalloc = Z_NULL;
    inf_strm.zfree = Z_NULL;
    inf_strm.opaque = Z_NULL;
    inf_strm.next_in = compr;
    inf_strm.avail_in = (uInt)comprLen;
    
    ret = inflateInit_(&inf_strm, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        free(uncompr);
        free(compr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 11. Create output buffer for inflation
    Bytef *inflateOut = (Bytef *)malloc(size * 2);
    if (!inflateOut) {
        inflateEnd(&inf_strm);
        free(uncompr);
        free(compr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    inf_strm.next_out = inflateOut;
    inf_strm.avail_out = (uInt)(size * 2);

    // 12. Use inflate
    ret = inflate(&inf_strm, Z_FINISH);
    
    // 13. Mark current position
    long mark = inflateMark(&inf_strm);
    (void)mark; // Use mark to avoid unused variable warning

    // 14. Create copy of inflate stream
    z_stream inf_strm_copy;
    memset(&inf_strm_copy, 0, sizeof(z_stream));
    ret = inflateCopy(&inf_strm_copy, &inf_strm);
    
    // 15. Reset inflate stream with different window bits
    int windowBits = (data[0] % 15) + 8; // Random window bits
    inflateReset2(&inf_strm, windowBits);

    // 16. Get codes used
    unsigned long codes_used = inflateCodesUsed(&inf_strm);
    (void)codes_used; // Use codes_used to avoid unused variable warning

    // 17. End inflate streams
    inflateEnd(&inf_strm);
    if (ret == Z_OK) {
        inflateEnd(&inf_strm_copy);
    }

    // 18. Create output file for gz operations
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write compressed data to output file
        fwrite(compr, 1, comprLen, out_file);
        fflush(out_file);
        
        // 19. Reopen output file for gz operations
        int fd = fuzz_fileno(out_file);
        gzFile gz_out = gzdopen(fd, "wb");
        if (gz_out) {
            // 20. Set buffer size
            unsigned int buffer_size = (data[0] % 1024) + 1024;
            gzbuffer(gz_out, buffer_size);
            
            // Write some data
            if (uncompr && uncomprLen > 0) {
                gzwrite(gz_out, uncompr, (unsigned int)uncomprLen);
            }
            
            gzclose(gz_out);
        } else {
            assert_file_closed(&out_file);;
        }
    }

    // 21. Clean up
    if (uncompr) free(uncompr);
    if (compr) free(compr);
    if (inflateOut) free(inflateOut);
    if (in_file) assert_file_closed(&in_file);;
    
    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}