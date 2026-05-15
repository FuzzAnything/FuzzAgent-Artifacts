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
//<ID> 38
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 2
//<Quality> {"density":10,"unique_branches":{"adler32_z":[[75,13,75,25,0,0,4,0],[90,13,90,26,0,0,4,0]]},"library_calls":["gzdopen","gzdirect","gzoffset","gzseek64","gzerror","inflateInit_","gzclose","inflatePrime","deflateInit_","inflateEnd","gzclose","deflateSetDictionary","deflateGetDictionary","gzread","gzoffset","gzclose","inflateEnd","deflateEnd","inflate","inflateEnd","deflateEnd"],"critical_calls":["gzdopen","gzdirect","gzoffset","gzseek64","gzerror","inflateInit_","inflatePrime","deflateInit_","deflateSetDictionary","deflateGetDictionary","gzread","gzoffset","gzclose","inflate","inflateEnd","deflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<21) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need enough data for meaningful operations

    // 1. Create a temporary file with compressed data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Open as gzFile for reading
    int fd = fuzz_fileno(in_file);
    gzFile gz = gzdopen(fd, fuzz_str_1);
    if (!gz) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 3. Check if file is in direct (uncompressed) mode
    int direct = gzdirect(gz);
    
    // 4. Get current offset (before seeking)
    off_t offset1 = gzoffset(gz);
    
    // 5. Seek to a position within the compressed data
    // Use 64-bit seek to handle large files
    off64_t offset2 = gzseek64(gz, size/4, fuzz_int32_t_2);
    
    // 6. Get error status after seeking
    int errnum = 0;
    const char *errmsg = gzerror(gz, &errnum);
    
    // 7. Set up inflate stream for decompression
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    
    int ret = inflateInit(&strm);
    if (ret != Z_OK) {
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 8. Use inflatePrime to insert bits into the bit buffer
    // Use first 16 bits of data as prime value
    uint16_t prime_bits = 0;
    if (size >= 2) {
        prime_bits = (data[0] << 8) | data[1];
    }
    inflatePrime(&strm, fuzz_int32_t_3, fuzz_int32_t_4);
    
    // 9. Set up deflate stream for compression
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    def_strm.zalloc = Z_NULL;
    def_strm.zfree = Z_NULL;
    def_strm.opaque = Z_NULL;
    
    ret = deflateInit(&def_strm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        inflateEnd(&strm);
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 10. Set dictionary for deflate stream
    // Use first 32 bytes of input as dictionary (if available)
    uInt dict_len = (size > 32) ? 32 : (uInt)size;
    deflateSetDictionary(&def_strm, data, dict_len);
    
    // 11. Get dictionary back from deflate stream
    Bytef dict_buffer[32];
    uInt retrieved_dict_len = 32;
    deflateGetDictionary(&def_strm, dict_buffer, &retrieved_dict_len);
    
    // 12. Read some data from gzFile to consume input
    char read_buffer[1024];
    int bytes_read = gzread(gz, read_buffer, sizeof(read_buffer));
    
    // 13. Get offset after reading
    off_t offset3 = gzoffset(gz);
    
    // 14. Clean up gzFile resources
    gzclose(gz);
    assert_file_closed(&in_file);;
    
    // 15. Create output file for compressed data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        inflateEnd(&strm);
        deflateEnd(&def_strm);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 16. Process remaining input data with inflate stream
    size_t processed = 0;
    while (processed < size && strm.avail_in == 0) {
        size_t chunk_size = (size - processed > 1024) ? 1024 : size - processed;
        strm.avail_in = (uInt)chunk_size;
        strm.next_in = (Bytef*)(data + processed);
        
        char out_buffer[2048];
        strm.avail_out = sizeof(out_buffer);
        strm.next_out = (Bytef*)out_buffer;
        
        ret = inflate(&strm, fuzz_int32_t_5);
        
        size_t have = sizeof(out_buffer) - strm.avail_out;
        if (have > 0 && out_file) {
            fwrite(out_buffer, 1, have, out_file);
        }
        
        processed += chunk_size;
        
        if (ret == Z_STREAM_END || ret != Z_OK) {
            break;
        }
    }
    
    // 17. Clean up all resources
    inflateEnd(&strm);
    deflateEnd(&def_strm);
    assert_file_closed(&out_file);;
    
    // 18. Remove temporary output file
    unlink("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}