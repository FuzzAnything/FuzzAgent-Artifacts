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
//<ID> 314
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"deflateStateCheck":[[544,9,544,20,0,0,4,0]]},"library_calls":["gzdopen","gzfread","gzseek64","gzerror","gzseek64","inflateInit2_","gzclose","inflateGetHeader","gzread","inflate","inflateEnd","deflateBound","compress","deflateInit_","deflatePrime","deflateEnd","gzdopen","gzputc","gzclose","gzclose"],"critical_calls":["gzdopen","gzfread","gzseek64","gzerror","gzseek64","inflateInit2_","inflateGetHeader","gzread","inflate","inflateEnd","deflateBound","compress","deflateInit_","deflatePrime","deflateEnd","gzdopen","gzputc","gzclose","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_89(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if input is too small for meaningful processing
    if (size < 10) {
	return 0;
	}
	
    
    // Create input and output files
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Initialize gzFile for reading compressed data
    gzFile gz_in = gzdopen(in_fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 1. gzfread - read compressed data using gzfread
    const size_t read_chunk_size = 1024;
    Bytef read_buffer[read_chunk_size];
    z_size_t items_read = 0;
    z_size_t total_read = 0;
    
    do {
        items_read = gzfread(read_buffer, 1, read_chunk_size, gz_in);
        total_read += items_read;
    } while (items_read > 0);
    
    // 2. gzseek64 - seek to beginning of the file
    gzseek64(gz_in, 0, SEEK_SET);
    
    // 3. gzgetc - read a single character
    int first_char = gzgetc(gz_in);
    (void)first_char; // Avoid unused variable warning
    
    // 4. gzerror - check for errors
    int errnum = 0;
    const char *error_msg = gzerror(gz_in, &errnum);
    (void)error_msg; // Avoid unused variable warning
    
    // Reset gzFile position again
    gzseek64(gz_in, 0, SEEK_SET);
    
    // Prepare for decompression using inflate
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    
    // Initialize inflate with gzip header detection
    int ret = inflateInit2(&strm, 15 + 32); // 32 enables gzip decoding
    if (ret != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 5. inflateGetHeader - get gzip header
    gz_header header;
    memset(&header, 0, sizeof(header));
    inflateGetHeader(&strm, &header);
    
    // Decompress the data
    Bytef decomp_buffer[4096];
    size_t decompressed_size = 0;
    
    // Read compressed data from gzFile directly
    strm.avail_in = gzread(gz_in, strm.next_in = (Bytef*)read_buffer, read_chunk_size);
    
    do {
        strm.avail_out = sizeof(decomp_buffer);
        strm.next_out = decomp_buffer;
        
        ret = inflate(&strm, Z_NO_FLUSH);
        
        size_t have = sizeof(decomp_buffer) - strm.avail_out;
        decompressed_size += have;
        
        // Write decompressed data to output file
        if (have > 0) {
            fwrite(decomp_buffer, 1, have, out_file);
        }
        
        if (ret == Z_STREAM_END) break;
        if (ret != Z_OK) break;
        
    } while (strm.avail_out == 0);
    
    inflateEnd(&strm);
    
    // 6. deflateBound - estimate compression bound
    uLong compressed_bound = deflateBound(&strm, decompressed_size);
    (void)compressed_bound; // Avoid unused variable warning
    
    // 7. compress - compress the decompressed data
    Bytef *compressed_data = (Bytef*)malloc(compressed_bound);
    uLongf compressed_len = compressed_bound;
    
    if (compressed_data && decompressed_size > 0) {
        // Reset file pointer to read decompressed data
        fseek(out_file, 0, SEEK_SET);
        Bytef *decompressed_data = (Bytef*)malloc(decompressed_size);
        
        if (decompressed_data) {
            fread(decompressed_data, 1, decompressed_size, out_file);
            
            int compress_ret = compress(compressed_data, &compressed_len, 
                                       decompressed_data, decompressed_size);
            (void)compress_ret; // Avoid unused variable warning
            
            free(decompressed_data);
        }
    }
    
    // 8. deflatePrime - this requires a deflate stream
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    def_strm.zalloc = Z_NULL;
    def_strm.zfree = Z_NULL;
    def_strm.opaque = Z_NULL;
    
    if (deflateInit(&def_strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
        // Use deflatePrime with some bits from the input data
        if (size > 2) {
            int bits = data[0] % 8 + 1; // 1-8 bits
            int value = data[1] & ((1 << bits) - 1);
            deflatePrime(&def_strm, bits, value);
        }
        deflateEnd(&def_strm);
    }
    
    // 9. gzputc - write a character to gzFile
    gzFile gz_out = gzdopen(out_fd, "wb");
    if (gz_out) {
        if (size > 3) {
            gzputc(gz_out, data[2]);
        }
        gzclose(gz_out);
    }
    
    // Clean up resources
    if (compressed_data) free(compressed_data);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}