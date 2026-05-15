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
//<ID> 25
//<Prompt> []
/*<Combination>: [
*/
//<score> 153, nr_unique_branch: 46
//<Quality> {"density":17,"unique_branches":{"deflateSetHeader":[[715,9,715,32,0,0,4,1],[715,36,715,58,0,0,4,1]],"deflateBound":[[931,12,931,33,0,0,4,1]],"zError":[[65,31,65,41,1,0,4,1],[65,45,65,54,1,0,4,1]],"deflateBound_z":[[887,5,887,11,0,0,4,1],[891,5,891,11,0,0,4,0],[893,13,893,32,0,0,4,0],[895,17,895,43,0,0,4,1],[898,17,898,30,0,0,4,0],[901,26,901,32,0,0,4,0],[901,26,901,32,0,0,4,1],[903,17,903,30,0,0,4,1],[907,17,907,32,0,0,4,1]],"deflate":[[1072,13,1072,32,0,0,4,1],[1106,17,1106,43,0,0,4,1],[1110,17,1110,32,0,0,4,1],[1117,9,1117,33,0,0,4,0],[1118,13,1118,39,0,0,4,1],[1144,9,1144,32,0,0,4,0],[1145,13,1145,38,0,0,4,0],[1149,21,1149,54,0,0,4,1],[1160,22,1160,30,0,0,4,0],[1160,22,1160,30,0,0,4,1],[1166,9,1166,35,0,0,4,0],[1167,13,1167,41,0,0,4,1],[1187,9,1187,32,0,0,4,0],[1188,13,1188,28,0,0,4,1],[1204,13,1204,28,0,0,4,1],[293,64,293,65,40,0,4,1],[293,64,293,65,40,0,4,1],[293,64,293,65,40,0,4,0],[293,64,293,65,40,0,4,1],[293,64,293,65,40,0,4,0],[293,64,293,65,45,0,4,1],[293,64,293,65,45,0,4,1],[293,64,293,65,45,0,4,1],[975,13,975,28,64,0,4,1]],"deflateTune":[[823,9,823,32,0,0,4,1]],"gzwrite":[[259,9,259,21,0,0,4,1],[264,9,264,32,0,0,4,1],[264,37,264,55,0,0,4,1],[270,9,270,21,0,0,4,1]],"inflateGetDictionary":[[1172,9,1172,32,0,0,4,1],[1176,9,1176,21,0,0,4,1],[1182,9,1182,29,0,0,4,0]]},"library_calls":["inflateInit2_","zError","deflateInit2_","zError","inflateEnd","deflateSetHeader","zError","deflateTune","zError","inflate","inflateGetDictionary","deflateSetDictionary","crc32","deflate","crc32","zError","inflateGetDictionary","deflateBound","crc32_combine","crc32_combine64","deflate","gzdopen","gzwrite","gzclose_w","inflateEnd","deflateEnd"],"critical_calls":["inflateInit2_","deflateInit2_","deflateSetHeader","zError","deflateTune","zError","inflate","inflateGetDictionary","deflateSetDictionary","crc32","deflate","crc32","zError","inflateGetDictionary","deflateBound","crc32_combine","crc32_combine64","deflate","gzdopen","gzwrite","gzclose_w","inflateEnd","deflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<83) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_14, fuzz_str_sz_14, fdp);
	//fuzzer shim end}




    // Early return for insufficient data
    if (size < 100) {
	return 0;
	}
	
    
    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    
    // Initialize zlib structures
    z_stream strm_inflate, strm_deflate;
    gz_header header;
    Bytef dictionary[32768];
    uInt dictLength = 0;
    int ret;
    const char *err_msg;
    
    // 1. Setup inflation stream
    memset(&strm_inflate, 0, sizeof(z_stream));
    strm_inflate.zalloc = Z_NULL;
    strm_inflate.zfree = Z_NULL;
    strm_inflate.opaque = Z_NULL;
    strm_inflate.avail_in = 0;
    strm_inflate.next_in = Z_NULL;
    
    ret = inflateInit2(&strm_inflate, 15 + 16); // 15 window bits + 16 for gzip
    if (ret != Z_OK) {
        err_msg = zError(ret); // Use zError API
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 2. Setup deflation stream
    memset(&strm_deflate, 0, sizeof(z_stream));
    strm_deflate.zalloc = Z_NULL;
    strm_deflate.zfree = Z_NULL;
    strm_deflate.opaque = Z_NULL;
    
    ret = deflateInit2(&strm_deflate, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                       15 + 16, 8, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK) {
        err_msg = zError(ret); // Use zError API
        inflateEnd(&strm_inflate);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 3. Setup gzip header
    memset(&header, 0, sizeof(gz_header));
    header.os = 255; // Unknown OS
    header.time = 0;
    header.xflags = 0;
    header.extra = Z_NULL;
    header.extra_len = 0;
    header.name = (Bytef*)"fuzzed";
    header.comment = Z_NULL;
    header.hcrc = 0;
    header.done = 0;
    
    ret = deflateSetHeader(&strm_deflate, &header); // Use deflateSetHeader API
    if (ret != Z_OK) {
        err_msg = zError(ret);
    }
    
    // 4. Tune deflate parameters
    ret = deflateTune(&strm_deflate, fuzz_int32_t_1, fuzz_int32_t_2, fuzz_int32_t_3, fuzz_int32_t_4); // Use deflateTune API
    if (ret != Z_OK) {
        err_msg = zError(ret);
    }
    
    // 5. Process input data in chunks
    Bytef in_buffer[4096];
    Bytef out_buffer[8192];
    uLong crc1 = 0, crc2 = 0;
    off_t combined_offset = 0;
    off64_t combined_offset64 = 0;
    
    // Read and inflate input
    while (!feof(in_file) && !ferror(in_file)) {
        size_t bytes_read = fread(in_buffer, 1, sizeof(in_buffer), in_file);
        if (bytes_read == 0) break;
        
        strm_inflate.avail_in = (uInt)bytes_read;
        strm_inflate.next_in = in_buffer;
        
        do {
            strm_inflate.avail_out = sizeof(out_buffer);
            strm_inflate.next_out = out_buffer;
            
            ret = inflate(&strm_inflate, fuzz_int32_t_5);
            
            if (ret == Z_NEED_DICT) {
                // Try to get dictionary from inflate stream
                ret = inflateGetDictionary(&strm_inflate, dictionary, &dictLength);
                if (ret == Z_OK && dictLength > 0) {
                    // Set dictionary for deflate
                    deflateSetDictionary(&strm_deflate, dictionary, dictLength);
                }
                continue;
            }
            
            size_t have = sizeof(out_buffer) - strm_inflate.avail_out;
            
            if (have > 0) {
                // Calculate CRC for this chunk
                crc1 = crc32(fuzz_uint64_t_6, out_buffer, (uInt)have);
                
                // Deflate the inflated data
                strm_deflate.avail_in = (uInt)have;
                strm_deflate.next_in = out_buffer;
                
                do {
                    strm_deflate.avail_out = sizeof(in_buffer);
                    strm_deflate.next_out = in_buffer;
                    
                    ret = deflate(&strm_deflate, fuzz_int32_t_7);
                    
                    size_t deflated = sizeof(in_buffer) - strm_deflate.avail_out;
                    if (deflated > 0) {
                        fwrite(in_buffer, 1, deflated, out_file);
                        crc2 = crc32(fuzz_uint64_t_8, in_buffer, (uInt)deflated);
                    }
                } while (strm_deflate.avail_out == 0);
            }
        } while (strm_inflate.avail_out == 0);
        
        if (ret == Z_STREAM_END) break;
        if (ret != Z_OK) {
            err_msg = zError(ret);
            break;
        }
    }
    
    // 6. Get dictionary from inflate stream (even if not needed)
    ret = inflateGetDictionary(&strm_inflate, dictionary, &dictLength); // Use inflateGetDictionary API
    
    // 7. Calculate deflate bound for remaining data
    uLong bound = deflateBound(&strm_deflate, strm_deflate.avail_in); // Use deflateBound API
    
    // 8. Combine CRC values
    uLong combined_crc = crc32_combine(fuzz_uint64_t_9, fuzz_uint64_t_10, combined_offset); // Use crc32_combine API
    uLong combined_crc64 = crc32_combine64(fuzz_uint64_t_11, fuzz_uint64_t_12, combined_offset64); // Use crc32_combine64 API
    
    // 9. Finish deflation
    do {
        strm_deflate.avail_out = sizeof(out_buffer);
        strm_deflate.next_out = out_buffer;
        
        ret = deflate(&strm_deflate, fuzz_int32_t_13);
        
        size_t have = sizeof(out_buffer) - strm_deflate.avail_out;
        if (have > 0) {
            fwrite(out_buffer, 1, have, out_file);
        }
    } while (strm_deflate.avail_out == 0);
    
    // 10. Create gzFile for writing and close it
    gzFile gz_out = gzdopen(fd_out, "wb");
    if (gz_out) {
        // Write some data to trigger gzclose_w
        const char *test_data = "test";
        gzwrite(gz_out, fuzz_str_14, static_cast<unsigned int>(fuzz_str_sz_14));
        gzclose_w(gz_out); // Use gzclose_w API
    } else {
        assert_file_closed(&out_file);;
    }
    
    // Cleanup
    inflateEnd(&strm_inflate);
    deflateEnd(&strm_deflate);
    assert_file_closed(&in_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}