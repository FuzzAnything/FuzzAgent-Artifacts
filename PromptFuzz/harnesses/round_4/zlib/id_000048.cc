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
//<ID> 106
//<Prompt> []
/*<Combination>: [
*/
//<score> 242, nr_unique_branch: 18
//<Quality> {"density":22,"unique_branches":{"inflateInit2_":[[207,9,207,20,0,0,4,0]],"gzseek64":[[384,9,384,27,0,0,4,0],[384,31,384,49,0,0,4,1],[388,9,388,27,0,0,4,1],[391,19,391,30,0,0,4,1]],"gzbuffer":[[339,9,339,17,0,0,4,0]],"deflateInit2_":[[435,9,435,23,0,0,4,0],[436,48,436,63,0,0,4,0],[436,67,436,76,0,0,4,0]],"deflateGetDictionary":[[630,9,630,32,0,0,4,0]],"deflateStateCheck":[[544,9,544,20,0,0,4,0]],"inflate":[[514,21,514,38,0,0,4,0]],"deflateBound_z":[[876,9,876,32,0,0,4,0],[877,17,877,36,0,0,4,1],[878,16,878,34,0,0,4,1],[917,9,917,24,0,0,4,0]],"deflateSetDictionary":[[567,9,567,32,0,0,4,0]],"crc32_combine_op":[[970,9,970,16,0,0,4,0]]},"library_calls":["gzdopen","gzbuffer","gzseek64","inflateInit2_","gzclose","inflateEnd","gzclose","inflate","inflateSync","deflateInit2_","deflateSetDictionary","deflateGetDictionary","inflateResetKeep","inflateCodesUsed","adler32","adler32","adler32","adler32","adler32_combine64","crc32","crc32","crc32","crc32","crc32_combine_op","deflateBound_z","deflateEnd","inflateEnd","gzclose"],"critical_calls":["gzdopen","gzbuffer","gzseek64","inflateInit2_","inflate","inflateSync","deflateInit2_","deflateSetDictionary","deflateGetDictionary","inflateResetKeep","inflateCodesUsed","adler32","adler32","adler32","adler32","adler32_combine64","crc32","crc32","crc32","crc32","crc32_combine_op","deflateBound_z","deflateEnd","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<45) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
	//fuzzer shim end}




    // Early return if input is too small for meaningful operations
    if (size < 10) {
	return 0;
	}
	

    // 1. Create a FILE* from input data for gzFile operations
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Get file descriptor and create gzFile
    int fd = fuzz_fileno(in_file);
    gzFile gz_file = gzdopen(fd, fuzz_str_1);
    if (!gz_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 3. Set buffer size for gzFile using gzbuffer
    unsigned int buffer_size = 8192;
    if (size > 100) {
        buffer_size = (data[0] % 65536) + 1; // Use input to vary buffer size
    }
    gzbuffer(gz_file, fuzz_uint32_t_2);
    
    // 4. Seek within the gzFile using gzseek64
    // Use input data to determine seek parameters
    off64_t seek_offset = (data[1] % (size/2)) & ~1; // Ensure even offset
    int seek_whence = (data[2] % 2) ? SEEK_CUR : SEEK_SET;
    gzseek64(gz_file, seek_offset, seek_whence);
    
    // 5. Initialize z_stream for inflation
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Use input data to determine windowBits
    int windowBits = 15 + (data[3] % 16); // 15-30 range
    if (inflateInit2(&strm, windowBits) != Z_OK) {
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 6. Set up input/output buffers for inflation
    Bytef *in_buf = (Bytef*)malloc(size);
    Bytef *out_buf = (Bytef*)malloc(size * 2);
    if (!in_buf || !out_buf) {
        free(in_buf);
        free(out_buf);
        inflateEnd(&strm);
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    memcpy(in_buf, data, size);
    strm.next_in = in_buf;
    strm.avail_in = size;
    strm.next_out = out_buf;
    strm.avail_out = size * 2;
    
    // 7. Perform inflation with potential sync
    int inflate_result = inflate(&strm, fuzz_int32_t_3);
    if (inflate_result == Z_DATA_ERROR) {
        // Try to sync if we hit data error
        inflateSync(&strm);
    }
    
    // 8. Get dictionary from deflation stream (create a dummy deflate stream)
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    deflateInit2(&def_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                 windowBits, 8, Z_DEFAULT_STRATEGY);
    
    // Set a dictionary using part of input data
    uInt dict_len = (size > 100) ? 50 : size/2;
    if (dict_len > 0) {
        deflateSetDictionary(&def_strm, in_buf, dict_len);
    }
    
    // Get the dictionary back
    Bytef *dict_buf = (Bytef*)malloc(32768);
    uInt retrieved_dict_len = 0;
    if (dict_buf) {
        deflateGetDictionary(&def_strm, dict_buf, &retrieved_dict_len);
        free(dict_buf);
    }
    
    // 9. Use inflateResetKeep to reset inflation stream
    inflateResetKeep(&strm);
    
    // 10. Get inflation codes usage
    unsigned long codes_used = inflateCodesUsed(&strm);
    (void)codes_used; // Use to avoid unused variable warning
    
    // 11. Calculate Adler-32 checksums and combine
    uLong adler1 = adler32(fuzz_uint64_t_4, Z_NULL, 0);
    uLong adler2 = adler32(fuzz_uint64_t_5, Z_NULL, 0);
    
    size_t half_size = size / 2;
    if (half_size > 0) {
        adler1 = adler32(adler1, in_buf, half_size);
        adler2 = adler32(adler2, in_buf + half_size, size - half_size);
        
        // Combine Adler-32 checksums
        off64_t len2 = size - half_size;
        uLong combined_adler = adler32_combine64(adler1, adler2, len2);
        (void)combined_adler; // Use to avoid unused variable warning
    }
    
    // 12. Calculate CRC-32 checksums and combine with operation
    uLong crc1 = crc32(fuzz_uint64_t_6, Z_NULL, 0);
    uLong crc2 = crc32(fuzz_uint64_t_7, Z_NULL, 0);
    
    if (half_size > 0) {
        crc1 = crc32(crc1, in_buf, half_size);
        crc2 = crc32(crc2, in_buf + half_size, size - half_size);
        
        // Generate operation for CRC combination
        // Using a simple XOR operation (0 = CRC32_COMBINE_OP_XOR)
        uLong op = 0;
        uLong combined_crc = crc32_combine_op(crc1, crc2, op);
        (void)combined_crc; // Use to avoid unused variable warning
    }
    
    // 13. Calculate deflate bound for potential recompression
    z_size_t bound = deflateBound_z(&def_strm, strm.total_out);
    (void)bound; // Use to avoid unused variable warning
    
    // 14. Clean up deflate stream
    deflateEnd(&def_strm);
    
    // 15. Clean up inflate stream
    inflateEnd(&strm);
    
    // 16. Clean up gzFile
    gzclose(gz_file);
    
    // 17. Clean up FILE*
    assert_file_closed(&in_file);;
    
    // 18. Free allocated buffers
    free(in_buf);
    free(out_buf);
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}