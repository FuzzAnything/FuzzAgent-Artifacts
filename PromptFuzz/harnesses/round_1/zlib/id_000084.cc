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
//<ID> 228
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["gzopen64","gzdopen","gzbuffer","gzoffset","gztell64","gzclose","gzread","gzclose","gzclose","inflateInit_","gzclose","inflatePrime","inflateEnd","uncompress2","gzclose"],"critical_calls":["gzopen64","gzdopen","gzbuffer","gzoffset","gztell64","gzread","inflateInit_","inflatePrime","inflateEnd","uncompress2","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_84(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // 1. Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Get file descriptor for reading
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 3. Open gzip file using file descriptor
    gzFile gz_file = gzopen64("input_file", "rb"); // Using file name variant
    if (!gz_file) {
        // Fallback: Use gzdopen with file descriptor
        gz_file = gzdopen(fd, "rb");
        if (!gz_file) {
            assert_file_closed(&in_file);;
            assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
        }
    }
    
    // 4. Set buffer size for gzip file
    unsigned int buffer_size = 8192; // Default buffer size
    if (size > 0) {
        buffer_size = (unsigned int)(data[0] % 65536 + 1024); // Use first byte for buffer size
    }
    gzbuffer(gz_file, buffer_size);
    
    // 5. Get current position in gzip file (using both offset functions)
    off_t offset1 = gzoffset(gz_file);
    off64_t offset2 = gztell64(gz_file);
    
    // 6. Read compressed data from gzip file
    size_t read_size = size > 4096 ? 4096 : size;
    Bytef *compressed_data = (Bytef *)malloc(read_size);
    if (!compressed_data) {
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }
    
    int bytes_read = gzread(gz_file, compressed_data, read_size);
    if (bytes_read <= 0) {
        free(compressed_data);
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }
    
    // 7. Prepare for decompression
    uLongf destLen = size * 2; // Estimated decompressed size
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) {
        free(compressed_data);
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }
    
    // 8. Create z_stream for inflatePrime
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize inflate with default settings
    if (inflateInit(&strm) != Z_OK) {
        free(dest);
        free(compressed_data);
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }
    
    // 9. Use inflatePrime to insert bits (if we have enough data)
    if (size >= 2) {
        int bits = data[0] % 8 + 1; // 1-8 bits
        int value = data[1] & ((1 << bits) - 1); // Value with specified bits
        inflatePrime(&strm, bits, value);
    }
    
    // Clean up inflate stream
    inflateEnd(&strm);
    
    // 10. Use uncompress2 to decompress the data
    uLong sourceLen = bytes_read;
    int result = uncompress2(dest, &destLen, compressed_data, &sourceLen);
    
    // 11. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write decompressed data if successful
        if (result == Z_OK) {
            fwrite(dest, 1, destLen, out_file);
        }
        assert_file_closed(&out_file);;
    }
    
    // 12. Clean up resources
    free(dest);
    free(compressed_data);
    
    // Close gzip file
    gzclose(gz_file);
    
    // Close input file
    assert_file_closed(&in_file);;
    
    // Remove output file to avoid accumulation
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}