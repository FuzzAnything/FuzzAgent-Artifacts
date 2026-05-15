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
//<ID> 8
//<Prompt> []
/*<Combination>: [
*/
//<score> 40, nr_unique_branch: 2
//<Quality> {"density":20,"unique_branches":{"deflate_huff":[[2163,21,2163,40,0,0,4,0],[1644,8,1644,31,3,0,4,0]]},"library_calls":["gzopen","gzbuffer","deflateInit2_","gzclose","adler32","adler32","adler32","adler32","deflateBound","deflateParams","deflate","deflatePending","gzprintf","deflate","gzprintf","adler32_combine","gzprintf","gzclearerr","deflateEnd","gzclose"],"critical_calls":["gzopen","gzbuffer","deflateInit2_","adler32","adler32","adler32","deflateBound","deflateParams","deflate","deflatePending","gzprintf","deflate","gzprintf","adler32_combine","gzprintf","gzclearerr","deflateEnd","gzclose"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified zlib APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<55) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need enough data for meaningful operations

    // 1. Open input file for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Open output gzip file for writing
    gzFile gzout = gzopen(fuzz_str_1, fuzz_str_2);
    if (!gzout) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Set buffer size for gzFile
    gzbuffer(gzout, fuzz_uint32_t_3);
    
    // 4. Initialize deflate stream
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    int ret = deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                           MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK) {
        gzclose(gzout);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 5. Read input and compress in chunks
    unsigned char inbuf[1024];
    unsigned char outbuf[2048];
    uLong total_compressed = 0;
    uLong adler1 = adler32(fuzz_uint64_t_4, Z_NULL, 0);
    uLong adler2 = adler32(fuzz_uint64_t_5, Z_NULL, 0);
    size_t half_size = size / 2;
    
    while (!feof(in_file) && !ferror(in_file)) {
        size_t bytes_read = fread(inbuf, 1, sizeof(inbuf), in_file);
        if (bytes_read == 0) break;
        
        // 6. Calculate Adler-32 checksums for two halves
        if (total_compressed < half_size) {
            adler1 = adler32(adler1, inbuf, bytes_read);
        } else {
            adler2 = adler32(adler2, inbuf, bytes_read);
        }
        
        // 7. Use deflateBound to get worst-case compressed size
        uLong bound_size = deflateBound(&strm, bytes_read);
        if (bound_size > sizeof(outbuf)) {
            bound_size = sizeof(outbuf);
        }
        
        strm.avail_in = bytes_read;
        strm.next_in = inbuf;
        
        do {
            strm.avail_out = sizeof(outbuf);
            strm.next_out = outbuf;
            
            // 8. Use deflateParams to change compression parameters mid-stream
            if (total_compressed > size / 3) {
                deflateParams(&strm, fuzz_int32_t_6, fuzz_int32_t_7);
            }
            
            ret = deflate(&strm, fuzz_int32_t_8);
            if (ret == Z_STREAM_ERROR) break;
            
            uLong have = sizeof(outbuf) - strm.avail_out;
            total_compressed += have;
            
            // 9. Check pending output with deflatePending
            unsigned int pending;
            int bits;
            deflatePending(&strm, &pending, &bits);
            
            // 10. Write compressed data to gzFile using gzprintf
            for (uLong i = 0; i < have; i++) {
                gzprintf(gzout, "%02x", outbuf[i]);
            }
            
        } while (strm.avail_out == 0);
    }
    
    // 11. Finish compression
    do {
        strm.avail_out = sizeof(outbuf);
        strm.next_out = outbuf;
        ret = deflate(&strm, fuzz_int32_t_9);
        uLong have = sizeof(outbuf) - strm.avail_out;
        
        for (uLong i = 0; i < have; i++) {
            gzprintf(gzout, "%02x", outbuf[i]);
        }
    } while (ret == Z_OK);
    
    // 12. Combine Adler-32 checksums
    off_t len = total_compressed;
    uLong combined_adler = adler32_combine(adler1, adler2, len);
    
    // Write combined checksum to file
    gzprintf(gzout, "\nCombined Adler-32: %lu\n", combined_adler);
    
    // 13. Clear any gzFile errors
    gzclearerr(gzout);
    
    // 14. Clean up
    deflateEnd(&strm);
    gzclose(gzout);
    assert_file_closed(&in_file);;
    
    // Clean up output file
    remove("output_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}