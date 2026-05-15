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
//<ID> 131
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 1
//<Quality> {"density":8,"unique_branches":{"gzsetparams":[[641,9,641,32,0,0,4,0]]},"library_calls":["gzdopen","gzdirect","gzoffset","gzsetparams","gzclose_r","gzdopen","gzclose_r","inflateInit_","gzclose","gzclose_r","adler32","adler32","inflate","inflateSync","gzwrite","inflateReset","inflatePrime","gzputs","inflateEnd","gzclose","gzclose_r"],"critical_calls":["gzdopen","gzdirect","gzoffset","gzsetparams","gzdopen","inflateInit_","adler32","adler32","inflate","inflateSync","gzwrite","inflateReset","inflatePrime","gzputs","inflateEnd","gzclose","gzclose_r"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimal data for meaningful operations

    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Get file descriptor and open as gzFile
    int fd = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Check if file is in direct mode
    int is_direct = gzdirect(gz_in);

    // Get current offset
    off_t offset = gzoffset(gz_in);

    // Try to set compression parameters (may fail for read-only files)
    // We'll attempt but ignore return value since it's read-only
    gzsetparams(gz_in, Z_DEFAULT_COMPRESSION, Z_DEFAULT_STRATEGY);

    // Create output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        gzclose_r(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Get output file descriptor and open as gzFile for writing
    int out_fd = fuzz_fileno(out_file);
    gzFile gz_out = gzdopen(out_fd, "wb");
    if (!gz_out) {
        assert_file_closed(&out_file);;
        gzclose_r(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
    }

    // Initialize inflate stream for decompression
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    
    int ret = inflateInit(&strm);
    if (ret != Z_OK) {
        gzclose(gz_out);
        assert_file_closed(&out_file);;
        gzclose_r(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
    }

    // Reset file position to beginning
    fseek(in_file, 0, SEEK_SET);
    
    // Prepare buffer for decompression
    uint8_t in_buf[1024];
    uint8_t out_buf[1024];
    uLong adler = adler32(0L, Z_NULL, 0);
    
    // Process data in chunks
    size_t total_read = 0;
    while (total_read < size && !feof(in_file)) {
        size_t to_read = (size - total_read < sizeof(in_buf)) ? 
                         size - total_read : sizeof(in_buf);
        size_t read_count = fread(in_buf, 1, to_read, in_file);
        if (read_count == 0) break;
        
        total_read += read_count;
        
        // Update Adler-32 checksum
        adler = adler32(adler, in_buf, read_count);
        
        // Set up inflate stream
        strm.avail_in = read_count;
        strm.next_in = in_buf;
        
        // Inflate the data
        do {
            strm.avail_out = sizeof(out_buf);
            strm.next_out = out_buf;
            
            ret = inflate(&strm, Z_NO_FLUSH);
            if (ret == Z_STREAM_ERROR || ret == Z_DATA_ERROR || 
                ret == Z_MEM_ERROR) {
                // Try to sync if we get an error
                inflateSync(&strm);
                break;
            }
            
            // Write decompressed data to output
            size_t have = sizeof(out_buf) - strm.avail_out;
            if (have > 0) {
                gzwrite(gz_out, out_buf, have);
            }
        } while (strm.avail_out == 0);
        
        // Reset inflate stream for next iteration
        inflateReset(&strm);
    }

    // Try to prime inflate with some bits (using first byte of input)
    if (size > 0) {
        inflatePrime(&strm, 8, data[0] & 0xFF);
    }

    // Write a test string to output
    gzputs(gz_out, "Fuzzer output");

    // Clean up inflate stream
    inflateEnd(&strm);

    // Close all files
    gzclose(gz_out);      // Closes both gz_out and underlying out_file
    gzclose_r(gz_in);     // Closes gz_in for reading
    // in_file is already closed by gzclose_r
    
    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
}