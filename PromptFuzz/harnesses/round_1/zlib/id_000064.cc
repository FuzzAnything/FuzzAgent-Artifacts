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
//<ID> 156
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 1
//<Quality> {"density":20,"unique_branches":{"uncompress2_z":[[36,31,36,45,0,0,4,1]]},"library_calls":["deflateInit_","inflateInit_","deflateInit2_","inflateInit2_","compressBound","deflateBound_z","deflateSetDictionary","deflate","deflateResetKeep","inflateSetDictionary","inflateUndermine","inflate","inflateGetDictionary","inflateResetKeep","uncompress_z","crc32","crc32","crc32_combine","gzdopen","gzfread","gzrewind","gzgets","gzclose","gzdopen","gzclose","deflateEnd","inflateEnd","deflateEnd","inflateEnd"],"critical_calls":["deflateInit_","inflateInit_","deflateInit2_","inflateInit2_","compressBound","deflateBound_z","deflateSetDictionary","deflate","deflateResetKeep","inflateSetDictionary","inflateUndermine","inflate","inflateGetDictionary","inflateResetKeep","uncompress_z","crc32","crc32","crc32_combine","gzdopen","gzfread","gzrewind","gzgets","gzclose","gzdopen","gzclose","deflateEnd","inflateEnd","deflateEnd","inflateEnd"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create FILE* from input data for gz functions
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Create output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Get file descriptors
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    
    // 4. Initialize streams
    z_stream deflate_stream = {0};
    z_stream inflate_stream = {0};
    z_stream deflate_old_stream = {0};
    z_stream inflate_old_stream = {0};
    
    // 5. Use old init APIs (deflateInit_ and inflateInit_)
    int level = Z_DEFAULT_COMPRESSION;
    deflateInit_(&deflate_old_stream, level, ZLIB_VERSION, sizeof(z_stream));
    inflateInit_(&inflate_old_stream, ZLIB_VERSION, sizeof(z_stream));
    
    // 6. Initialize deflate stream with deflateInit2_
    int windowBits = 15; // 32 for gzip format
    int memLevel = 8;
    int strategy = Z_DEFAULT_STRATEGY;
    deflateInit2_(&deflate_stream, level, Z_DEFLATED, windowBits, memLevel, strategy,
                  ZLIB_VERSION, sizeof(z_stream));
    
    // 7. Initialize inflate stream with inflateInit2_
    inflateInit2_(&inflate_stream, windowBits, ZLIB_VERSION, sizeof(z_stream));
    
    // 8. Use compressBound and deflateBound_z for buffer sizing
    uLong comp_bound = compressBound(size);
    z_size_t deflate_bound = deflateBound_z(&deflate_stream, size);
    
    // Allocate buffers
    Bytef *compressed_buf = (Bytef *)malloc(deflate_bound);
    Bytef *decompressed_buf = (Bytef *)malloc(size * 2);
    z_size_t destLen = deflate_bound;
    z_size_t srcLen = size;
    
    // 9. Compress using deflate with dictionary
    // Set up dictionary from first part of data
    uInt dictSize = (size > 1024) ? 1024 : (uInt)size;
    if (dictSize > 0) {
        deflateSetDictionary(&deflate_stream, data, dictSize);
    }
    
    deflate_stream.avail_in = (uInt)size;
    deflate_stream.next_in = (Bytef *)data;
    deflate_stream.avail_out = (uInt)deflate_bound;
    deflate_stream.next_out = compressed_buf;
    
    deflate(&deflate_stream, Z_FINISH);
    
    // 10. Use deflateResetKeep
    deflateResetKeep(&deflate_stream);
    
    // 11. Decompress using inflate with dictionary
    inflate_stream.avail_in = (uInt)deflate_stream.total_out;
    inflate_stream.next_in = compressed_buf;
    inflate_stream.avail_out = (uInt)(size * 2);
    inflate_stream.next_out = decompressed_buf;
    
    // Set same dictionary for decompression
    if (dictSize > 0) {
        inflateSetDictionary(&inflate_stream, data, dictSize);
    }
    
    // 12. Use inflateUndermine (try to undermine inflate)
    inflateUndermine(&inflate_stream, 1);
    
    inflate(&inflate_stream, Z_FINISH);
    
    // 13. Get dictionary back from inflate
    Bytef retrieved_dict[1024];
    uInt retrieved_dict_len = 1024;
    inflateGetDictionary(&inflate_stream, retrieved_dict, &retrieved_dict_len);
    
    // 14. Use inflateResetKeep
    inflateResetKeep(&inflate_stream);
    
    // 15. Use uncompress_z
    uncompress_z(decompressed_buf, &destLen, compressed_buf, deflate_stream.total_out);
    
    // 16. CRC32 operations
    uLong crc1 = crc32(0L, data, (uInt)(size > 100 ? 100 : size));
    uLong crc2 = crc32(0L, data + 100, (uInt)(size > 200 ? 100 : (size > 100 ? size - 100 : 0)));
    uLong combined_crc = crc32_combine(crc1, crc2, size > 100 ? 100 : 0);
    
    // 17. gzFile operations
    gzFile gz_in = gzdopen(fd_in, "rb");
    if (gz_in) {
        char gz_buf[1024];
        // Use gzfread
        gzfread(gz_buf, 1, sizeof(gz_buf), gz_in);
        
        // Use gzgets
        gzrewind(gz_in);
        gzgets(gz_in, gz_buf, sizeof(gz_buf));
        
        gzclose(gz_in);
    }
    
    // 18. Use gzdopen for output (even though we won't write with it)
    gzFile gz_out = gzdopen(fd_out, "wb");
    if (gz_out) {
        gzclose(gz_out);
    }
    
    // 19. Clean up streams
    deflateEnd(&deflate_stream);
    inflateEnd(&inflate_stream);
    deflateEnd(&deflate_old_stream);
    inflateEnd(&inflate_old_stream);
    
    // 20. Free allocated memory
    free(compressed_buf);
    free(decompressed_buf);
    
    // 21. Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // 22. Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}