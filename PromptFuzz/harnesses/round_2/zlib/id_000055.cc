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
//<ID> 140
//<Prompt> []
/*<Combination>: [
*/
//<score> 4, nr_unique_branch: 0
//<Quality> {"density":4,"unique_branches":{},"library_calls":["gzopen64","gzfread","gzflush","gzclose","uncompress","uncompress2","inflateInit2_","inflate","inflateReset","inflateEnd","crc32_combine_gen","gzopen64","gzfwrite","gzflush","gzclose"],"critical_calls":["gzopen64","gzfread","gzflush","gzclose","uncompress","uncompress2","inflateInit2_","inflate","inflateReset","inflateEnd","crc32_combine_gen","gzopen64","gzfwrite","gzflush","gzclose"],"visited":0}
/**/



extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimum data for meaningful operations

    // 1. Create a gzip file from input data and read it back
    const char* filename = "temp_gzip_file.gz";
    
    // Write compressed data to file
    FILE* file = fopen(filename, "wb");
    if (!file) {
	assert_file_closed(&file);
	return 0;
	}
	
    fwrite(data, 1, size, file);
    assert_file_closed(&file);;

    // Open with gzopen64
    gzFile gzfile = gzopen64(filename, "rb");
    if (!gzfile) {
        remove(filename);
        assert_file_closed(&file);
	return 0;
    }

    // Read using gzfread
    char read_buffer[1024];
    z_size_t items_read = gzfread(read_buffer, 1, sizeof(read_buffer), gzfile);
    
    // Try gzflush (though reading mode, will likely fail but we call it)
    gzflush(gzfile, Z_SYNC_FLUSH);

    gzclose(gzfile);
    remove(filename);

    // 2. Prepare for uncompress operations
    uLongf dest_len = size * 10;  // Estimated maximum uncompressed size
    Bytef* dest = (Bytef*)malloc(dest_len);
    if (!dest) {
	assert_file_closed(&file);
	return 0;
	}
	

    uLongf dest_len2 = size * 10;
    Bytef* dest2 = (Bytef*)malloc(dest_len2);
    if (!dest2) {
        free(dest);
        assert_file_closed(&file);
	return 0;
    }

    // 3. First try regular uncompress
    int uncompress_result = uncompress(dest, &dest_len, data, size);
    
    // 4. Then try uncompress2 with source length pointer
    uLong source_len = size;
    int uncompress2_result = uncompress2(dest2, &dest_len2, data, &source_len);

    // 5. Set up z_stream for inflateReset
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    
    // Initialize for inflation
    int inflate_init_result = inflateInit2(&strm, 15 + 16);  // 15 window bits + 16 for gzip
    
    if (inflate_init_result == Z_OK) {
        // Set input data
        strm.avail_in = size;
        strm.next_in = (Bytef*)data;
        
        // Prepare output buffer
        Bytef out_buffer[1024];
        strm.avail_out = sizeof(out_buffer);
        strm.next_out = out_buffer;
        
        // Try to inflate a bit
        inflate(&strm, Z_NO_FLUSH);
        
        // Call inflateReset
        inflateReset(&strm);
        
        inflateEnd(&strm);
    }

    // 6. Use crc32_combine_gen with some dummy offset
    off_t dummy_offset = 1024;
    uLong combine_gen = crc32_combine_gen(dummy_offset);

    // 7. Create another gzip file for writing
    gzFile gzfile_write = gzopen64("temp_write.gz", "wb");
    if (gzfile_write) {
        // Write some data
        const char* test_data = "test data";
        gzfwrite(test_data, 1, strlen(test_data), gzfile_write);
        
        // Flush and close
        gzflush(gzfile_write, Z_FINISH);
        gzclose(gzfile_write);
        remove("temp_write.gz");
    }

    // Cleanup
    free(dest);
    free(dest2);

    assert_file_closed(&file);
	return 0;
}