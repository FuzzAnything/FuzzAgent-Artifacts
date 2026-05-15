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
//<ID> 13
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 3
//<Quality> {"density":8,"unique_branches":{"deflateInit2_":[[422,9,422,23,0,0,4,0],[424,13,424,29,0,0,4,1]],"deflate":[[1029,36,1029,48,0,0,4,0]]},"library_calls":["get_crc_table","gzdopen","gztell","gzread","gzclose","inflateInit2_","gzclose","inflateValidate","inflate","inflateResetKeep","inflateEnd","gzclose","deflateInit2_","inflateEnd","gzclose","deflate","deflatePending","compressBound","uncompress_z","adler32","adler32","adler32","adler32","adler32_combine","deflateEnd","inflateEnd","gzclose"],"critical_calls":["get_crc_table","gzdopen","gztell","gzread","inflateInit2_","inflateValidate","inflate","inflateResetKeep","deflateInit2_","deflate","deflatePending","compressBound","uncompress_z","adler32","adler32","adler32","adler32","adler32_combine","deflateEnd","inflateEnd","gzclose"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Event: Validate and partially decompress zlib-compressed data,
    // check compression pending state, compute combined checksums

    if (size < 10) {
	return 0;
	}
	 // Need minimum data for meaningful test

    // 1. Get CRC table (API: get_crc_table)
    const z_crc_t *crc_table = get_crc_table();
    if (!crc_table) {
	return 0;
	}
	

    // 2. Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 3. Create gzFile handle
    int fd = fuzz_fileno(in_file);
    gzFile gz = gzdopen(fd, "rb");
    if (!gz) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 4. Get current position in gzFile (API: gztell)
    off_t pos = gztell(gz);
    (void)pos; // Use pos to avoid unused warning

    // 5. Read some data through gz interface
    char buf[1024];
    int read_bytes = gzread(gz, buf, sizeof(buf));
    if (read_bytes < 0) {
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 6. Set up inflation stream for compressed data
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    inflate_strm.zalloc = Z_NULL;
    inflate_strm.zfree = Z_NULL;
    inflate_strm.opaque = Z_NULL;
    
    // Initialize inflation (we'll use raw deflate format for maximum compatibility)
    if (inflateInit2(&inflate_strm, -15) != Z_OK) {
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 7. Enable validation (API: inflateValidate)
    inflateValidate(&inflate_strm, 1);

    // 8. Set input for inflation
    inflate_strm.next_in = (Bytef *)data;
    inflate_strm.avail_in = (uInt)(size > UINT_MAX ? UINT_MAX : size);
    
    // 9. Prepare output buffer
    Bytef out_buf[4096];
    inflate_strm.next_out = out_buf;
    inflate_strm.avail_out = sizeof(out_buf);

    // 10. Perform partial inflation
    int inflate_result = inflate(&inflate_strm, Z_SYNC_FLUSH);
    
    // 11. Keep the stream state (API: inflateResetKeep)
    if (inflateResetKeep(&inflate_strm) != Z_OK) {
        inflateEnd(&inflate_strm);
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 12. Set up deflation stream to test pending state
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    deflate_strm.zalloc = Z_NULL;
    deflate_strm.zfree = Z_NULL;
    deflate_strm.opaque = Z_NULL;
    
    if (deflateInit2(&deflate_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                     -15, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        inflateEnd(&inflate_strm);
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 13. Compress some test data
    const char *test_data = "Test data for compression";
    uLong test_len = strlen(test_data);
    deflate_strm.next_in = (Bytef *)test_data;
    deflate_strm.avail_in = (uInt)test_len;
    
    Bytef comp_buf[1024];
    deflate_strm.next_out = comp_buf;
    deflate_strm.avail_out = sizeof(comp_buf);
    
    deflate(&deflate_strm, Z_FINISH);

    // 14. Check pending data (API: deflatePending)
    unsigned int pending;
    int bits;
    deflatePending(&deflate_strm, &pending, &bits);

    // 15. Get compression bound (API: compressBound)
    uLong bound = compressBound(test_len);
    (void)bound; // Use bound to avoid unused warning

    // 16. Test uncompress_z with our compressed data
    // First, get actual compressed size
    uLong comp_size = sizeof(comp_buf) - deflate_strm.avail_out;
    if (comp_size > 0) {
        Bytef uncomp_buf[1024];
        z_size_t uncomp_len = sizeof(uncomp_buf);
        z_size_t source_len = comp_size;
        
        // API: uncompress_z
        int uncompress_result = uncompress_z(uncomp_buf, &uncomp_len, 
                                             comp_buf, source_len);
        (void)uncompress_result; // Use result to avoid unused warning
    }

    // 17. Compute Adler-32 checksums and combine them
    uLong adler1 = adler32(0L, Z_NULL, 0);
    uLong adler2 = adler32(0L, Z_NULL, 0);
    
    if (size > 0) {
        // Split data into two parts for combining
        size_t half = size / 2;
        adler1 = adler32(adler1, data, (uInt)half);
        adler2 = adler32(adler2, data + half, (uInt)(size - half));
        
        // API: adler32_combine
        uLong combined = adler32_combine(adler1, adler2, (off_t)(size - half));
        (void)combined; // Use combined to avoid unused warning
    }

    // 18. Clean up all resources
    deflateEnd(&deflate_strm);
    inflateEnd(&inflate_strm);  // API: inflateEnd
    gzclose(gz);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}