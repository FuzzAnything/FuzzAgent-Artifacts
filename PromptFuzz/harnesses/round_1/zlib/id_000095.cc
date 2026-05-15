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
//<ID> 291
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 2
//<Quality> {"density":15,"unique_branches":{"inflate":[[575,9,575,18,0,0,4,0],[584,9,584,16,0,0,4,0]]},"library_calls":["zlibVersion","inflateInit2_","inflateValidate","inflateEnd","inflate","adler32_z","adler32_z","adler32_combine","inflateEnd","deflateInit2_","deflateTune","deflateEnd","deflateSetHeader","deflateEnd","deflateGetDictionary","deflateBound","deflate","deflateEnd","gzdopen","gzdopen","gzread","gzwrite","gzungetc","gzclose","gzclose"],"critical_calls":["zlibVersion","inflateInit2_","inflateValidate","inflate","adler32_z","adler32_combine","inflateEnd","deflateInit2_","deflateTune","deflateSetHeader","deflateGetDictionary","deflateBound","deflate","deflateEnd","gzdopen","gzdopen","gzread","gzwrite","gzungetc","gzclose","gzclose"],"visited":0}
/**/


// Custom FILE* creation functions as specified
FILE*  id_000095_create_input_file(const uint8_t* data, size_t size) {
    return fmemopen((void*)data, size, "rb");
}

FILE*  id_000095_create_output_file() {
    return fopen("output_file", "wb");
}

extern "C" int LLVMFuzzerTestOneInput_95(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create input FILE* from fuzzer data
    FILE* in_file =  id_000095_create_input_file(data, size);
    if (!in_file) {
	return 0;
	}
	
    
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_fd_closed(fd);
	return 0;
    }
    
    // 1. Initialize decompression with inflateInit2_
    z_stream decomp_strm = {};
    decomp_strm.zalloc = Z_NULL;
    decomp_strm.zfree = Z_NULL;
    decomp_strm.opaque = Z_NULL;
    
    // Using 32 for automatic header detection, MAX_WBITS = 15
    int windowBits = 15 + 32;
    const char* version = zlibVersion();
    int stream_size = sizeof(z_stream);
    int ret = inflateInit2_(&decomp_strm, windowBits, version, stream_size);
    if (ret != Z_OK) {
        assert_file_closed(&in_file);;
        assert_fd_closed(fd);
	return 0;
    }
    
    // 2. Enable validation during decompression
    ret = inflateValidate(&decomp_strm, 1);
    if (ret != Z_OK) {
        inflateEnd(&decomp_strm);
        assert_file_closed(&in_file);;
        assert_fd_closed(fd);
	return 0;
    }
    
    // 3. Decompress the input data
    Bytef decomp_buf[4096];
    decomp_strm.next_in = (Bytef*)data;
    decomp_strm.avail_in = size;
    
    uLong decomp_adler1 = 0, decomp_adler2 = 0;
    size_t decomp_total = 0;
    off_t combined_offset = 0;
    
    do {
        decomp_strm.next_out = decomp_buf;
        decomp_strm.avail_out = sizeof(decomp_buf);
        
        ret = inflate(&decomp_strm, Z_NO_FLUSH);
        
        size_t have = sizeof(decomp_buf) - decomp_strm.avail_out;
        if (have > 0) {
            // 4. Compute Adler-32 checksums in parts
            if (decomp_total < 2048) {
                decomp_adler1 = adler32_z(decomp_adler1, decomp_buf, have);
            } else {
                decomp_adler2 = adler32_z(decomp_adler2, decomp_buf, have);
            }
            decomp_total += have;
        }
    } while (ret == Z_OK);
    
    // 5. Combine Adler-32 checksums if we have two parts
    if (decomp_total >= 2048) {
        combined_offset = 2048;
        uLong combined = adler32_combine(decomp_adler1, decomp_adler2, combined_offset);
        (void)combined; // Prevent unused variable warning
    }
    
    // Clean up decompression
    inflateEnd(&decomp_strm);
    
    // 6. Initialize compression with deflateInit2_
    z_stream comp_strm = {};
    comp_strm.zalloc = Z_NULL;
    comp_strm.zfree = Z_NULL;
    comp_strm.opaque = Z_NULL;
    
    int level = Z_DEFAULT_COMPRESSION;
    int method = Z_DEFLATED;
    int memLevel = 8;
    int strategy = Z_DEFAULT_STRATEGY;
    
    // Use gzip format with MAX_WBITS + 16
    windowBits = 15 + 16;
    ret = deflateInit2_(&comp_strm, level, method, windowBits, memLevel, 
                       strategy, version, stream_size);
    if (ret != Z_OK) {
        assert_file_closed(&in_file);;
        assert_fd_closed(fd);
	return 0;
    }
    
    // 7. Tune compression parameters
    int good_length = 8;
    int max_lazy = 8;
    int nice_length = 32;
    int max_chain = 256;
    ret = deflateTune(&comp_strm, good_length, max_lazy, nice_length, max_chain);
    if (ret != Z_OK) {
        deflateEnd(&comp_strm);
        assert_file_closed(&in_file);;
        assert_fd_closed(fd);
	return 0;
    }
    
    // 8. Set gzip header
    gz_header gzip_header = {};
    gzip_header.text = 0;
    gzip_header.time = 0;
    gzip_header.os = 255; // Unknown OS
    ret = deflateSetHeader(&comp_strm, &gzip_header);
    if (ret != Z_OK) {
        deflateEnd(&comp_strm);
        assert_file_closed(&in_file);;
        assert_fd_closed(fd);
	return 0;
    }
    
    // 9. Get dictionary (none in this case, but call API)
    Bytef dict_buf[256];
    uInt dict_len = 256;
    ret = deflateGetDictionary(&comp_strm, dict_buf, &dict_len);
    // ret may be Z_OK or Z_STREAM_ERROR, continue anyway
    
    // 10. Estimate maximum compressed size
    uLong source_len = decomp_total;
    uLong bound_size = deflateBound(&comp_strm, source_len);
    (void)bound_size; // Prevent unused variable warning
    
    // 11. Compress the decompressed data
    // We don't have the actual decompressed data stored, so we'll compress dummy data
    // This is a limitation since we didn't store decompressed output
    Bytef dummy_data[] = "test data for compression";
    uLong dummy_len = sizeof(dummy_data) - 1;
    
    comp_strm.next_in = dummy_data;
    comp_strm.avail_in = dummy_len;
    
    Bytef comp_output[4096];
    comp_strm.next_out = comp_output;
    comp_strm.avail_out = sizeof(comp_output);
    
    ret = deflate(&comp_strm, Z_FINISH);
    
    // Clean up compression
    deflateEnd(&comp_strm);
    
    // 12. Create gzFile for reading using gzdopen
    rewind(in_file);
    gzFile gz_in = gzdopen(fd, "rb");
    if (gz_in) {
        // 13. Create output FILE* and gzFile for writing
        FILE* out_file =  id_000095_create_output_file();
        if (out_file) {
            int out_fd = fuzz_fileno(out_file);
            if (out_fd >= 0) {
                gzFile gz_out = gzdopen(out_fd, "wb");
                if (gz_out) {
                    // Read from gz_in and write to gz_out
                    char buffer[1024];
                    int bytes_read;
                    while ((bytes_read = gzread(gz_in, buffer, sizeof(buffer))) > 0) {
                        // Write to gz_out (in reality we should check return value)
                        (void)gzwrite(gz_out, buffer, bytes_read);
                    }
                    
                    // 14. Use gzungetc
                    // First read a character
                    int ch = gzgetc(gz_in);
                    if (ch != -1) {
                        // Push it back
                        ret = gzungetc(ch, gz_in);
                        (void)ret; // Prevent unused variable warning
                    }
                    
                    // 15. Close gzFile
                    gzclose(gz_out);
                }
                assert_file_closed(&out_file);;
            }
        }
        
        // 16. Close gzFile
        gzclose(gz_in);
    }
    
    // 17. Close input FILE*
    assert_file_closed(&in_file);;
    
    // Remove output file if created
    remove("output_file");
    
    assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}