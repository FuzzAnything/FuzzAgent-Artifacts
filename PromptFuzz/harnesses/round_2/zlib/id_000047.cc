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
//<ID> 113
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 1
//<Quality> {"density":12,"unique_branches":{"inflateStateCheck":[[91,9,91,38,0,0,4,0]]},"library_calls":["gzdopen","deflateInit_","gzclose","gzgets","crc32_z","deflate","deflatePending","deflate","deflateEnd","gzclose","gzdopen","gzclose","gzgets","crc32_z","crc32_combine64","inflateValidate","gzdopen","gzputs","gzclose","gzclose","gzclose"],"critical_calls":["gzdopen","deflateInit_","gzgets","crc32_z","deflate","deflatePending","deflate","deflateEnd","gzdopen","gzgets","crc32_z","crc32_combine64","inflateValidate","gzdopen","gzputs","gzclose","gzclose","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	 // Too small to be meaningful

    // Create a FILE* from the input data for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Create a FILE* for writing compressed data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Event: Compress input data with zlib, write to gzip file,
    // read it back while computing CRC checksums, and validate inflation
    
    int fd = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Set up deflation stream for compression
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    def_strm.zalloc = Z_NULL;
    def_strm.zfree = Z_NULL;
    def_strm.opaque = Z_NULL;
    
    // Initialize deflation with default parameters
    int ret = deflateInit_(&def_strm, Z_DEFAULT_COMPRESSION, 
                          ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Read data from gzFile, compress it, and write to output
    char read_buf[1024];
    char write_buf[1024];
    uLong crc1 = 0;
    uLong crc2 = 0;
    
    while (true) {
        // Read from gzip file
        char *line = gzgets(gz_in, read_buf, sizeof(read_buf));
        if (!line) break;
        
        size_t line_len = strlen(read_buf);
        
        // Compute CRC32 of the read data
        crc1 = crc32_z(crc1, (const Bytef *)read_buf, line_len);
        
        // Set up deflation input
        def_strm.next_in = (Bytef *)read_buf;
        def_strm.avail_in = line_len;
        
        // Compress and write to output file
        do {
            def_strm.next_out = (Bytef *)write_buf;
            def_strm.avail_out = sizeof(write_buf);
            ret = deflate(&def_strm, Z_NO_FLUSH);
            if (ret == Z_STREAM_ERROR) break;
            
            size_t have = sizeof(write_buf) - def_strm.avail_out;
            if (have > 0) {
                fwrite(write_buf, 1, have, out_file);
            }
        } while (def_strm.avail_out == 0);
        
        // Check for pending data in deflation stream
        unsigned int pending;
        int bits;
        deflatePending(&def_strm, &pending, &bits);
    }
    
    // Finish deflation
    def_strm.next_in = Z_NULL;
    def_strm.avail_in = 0;
    do {
        def_strm.next_out = (Bytef *)write_buf;
        def_strm.avail_out = sizeof(write_buf);
        ret = deflate(&def_strm, Z_FINISH);
        size_t have = sizeof(write_buf) - def_strm.avail_out;
        if (have > 0) {
            fwrite(write_buf, 1, have, out_file);
        }
    } while (def_strm.avail_out == 0);
    
    deflateEnd(&def_strm);
    assert_file_closed(&out_file);;
    
    // Now open the output file as gzFile for reading
    FILE *compressed_file = fopen("output_file", "rb");
    if (!compressed_file) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&compressed_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    int fd_out = fuzz_fileno(compressed_file);
    gzFile gz_out = gzdopen(fd_out, "rb");
    if (!gz_out) {
        assert_file_closed(&compressed_file);;
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&compressed_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Read back and compute another CRC
    char buf2[1024];
    while (true) {
        char *line2 = gzgets(gz_out, buf2, sizeof(buf2));
        if (!line2) break;
        size_t len2 = strlen(buf2);
        crc2 = crc32_z(crc2, (const Bytef *)buf2, len2);
    }
    
    // Combine the two CRCs
    uLong combined_crc = crc32_combine64(crc1, crc2, 0);
    (void)combined_crc; // Prevent unused variable warning
    
    // Set up inflation stream for validation
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    inf_strm.zalloc = Z_NULL;
    inf_strm.zfree = Z_NULL;
    inf_strm.opaque = Z_NULL;
    
    // Re-read the compressed output file
    fseek(compressed_file, 0, SEEK_END);
    long comp_size = ftell(compressed_file);
    fseek(compressed_file, 0, SEEK_SET);
    
    Bytef *comp_data = (Bytef *)malloc(comp_size);
    if (comp_data) {
        fread(comp_data, 1, comp_size, compressed_file);
        
        inf_strm.next_in = comp_data;
        inf_strm.avail_in = comp_size;
        
        // Enable validation during inflation
        inflateValidate(&inf_strm, 1);
        
        free(comp_data);
    }
	int fuzz_fd_0 = fuzz_fileno(stdout);

    
    // Write something to the gzFile using gzputs
    gzFile gz_write = gzdopen(fuzz_fd_0, "wb");
    if (gz_write) {
        gzputs(gz_write, "Fuzzing completed\n");
        gzclose(gz_write);
    }
    
    // Clean up
    gzclose(gz_out);
    gzclose(gz_in);
    assert_file_closed(&compressed_file);;
    assert_file_closed(&in_file);;
    
    // Remove temporary output file
    remove("output_file");
    
    assert_file_closed(&compressed_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fuzz_fd_0);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd);
	return 0;
}