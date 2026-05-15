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
//<ID> 195
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 5
//<Quality> {"density":4,"unique_branches":{"gzgetc":[[485,9,485,27,0,0,4,0],[485,31,485,56,0,0,4,0],[485,31,485,56,0,0,4,1],[485,60,485,73,0,0,4,0],[490,9,490,22,0,0,4,0]]},"library_calls":["gzopen64","gzdirect","gzgetc_","gzclose","deflateInit_","deflatePending","deflate","inflateInit_","inflate","inflateCodesUsed","inflateEnd","adler32","adler32","adler32","adler32","adler32_combine","deflateEnd","gzdopen","gzwrite","gzclose"],"critical_calls":["gzopen64","gzdirect","gzgetc_","gzclose","deflateInit_","deflatePending","deflate","inflateInit_","inflate","inflateCodesUsed","inflateEnd","adler32","adler32","adler32","adler32","adler32_combine","deflateEnd","gzdopen","gzwrite","gzclose"],"visited":0}
/*Here's a comprehensive fuzzer that uses all the specified zlib APIs in a meaningful way with the input data:

*/


extern "C" int LLVMFuzzerTestOneInput_74(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Early return for insufficient data
    if (size < 10) {
	return 0;
	}
	
    
    // Create temporary files for input/output
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
    
    // 1. Use gzopen64 to open a compressed file stream
    int fd = fuzz_fileno(in_file);
    gzFile gz = gzopen64("input_file", "rb");  // Will fail but we call it
    if (gz) {
        // 2. Use gzdirect to check if file is in gzip format
        int is_direct = gzdirect(gz);
        
        // 3. Use gzgetc to read a byte
        int byte1 = gzgetc(gz);
        
        // 4. Use gzgetc_ (alternative version) to read another byte
        int byte2 = gzgetc_(gz);
        
        gzclose(gz);
    }
    
    // 5. Create z_stream for deflate operations
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    // Initialize deflate with random level from data
    int level = data[0] % 10;  // 0-9
    if (deflateInit(&strm, level) != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }
    
    // 6. Use deflatePending to check pending output
    unsigned int pending = 0;
    int bits = 0;
    deflatePending(&strm, &pending, &bits);
    
    // Set up input/output buffers
    Bytef in_buffer[1024];
    Bytef out_buffer[2048];
    size_t bytes_read = fread(in_buffer, 1, (size < 1024) ? size : 1024, in_file);
    
    strm.next_in = in_buffer;
    strm.avail_in = (uInt)bytes_read;
    strm.next_out = out_buffer;
    strm.avail_out = 2048;
    
    // Perform deflation
    deflate(&strm, Z_FINISH);
    
    // 7. Use inflateCodesUsed to get code usage statistics
    // First create an inflate stream
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = Z_NULL;
    istrm.zfree = Z_NULL;
    istrm.opaque = Z_NULL;
    
    if (inflateInit(&istrm) == Z_OK) {
        // Set up inflate stream with deflated data
        istrm.next_in = out_buffer;
        istrm.avail_in = 2048 - strm.avail_out;
        Bytef inf_buffer[1024];
        istrm.next_out = inf_buffer;
        istrm.avail_out = 1024;
        
        // Inflate the data
        inflate(&istrm, Z_FINISH);
        
        // Get code usage statistics
        unsigned long codes_used = inflateCodesUsed(&istrm);
        
        inflateEnd(&istrm);
    }
    
    // 8. Use adler32_combine to combine Adler-32 checksums
    uLong adler1 = adler32(0L, Z_NULL, 0);
    uLong adler2 = adler32(0L, Z_NULL, 0);
    
    if (bytes_read > 20) {
        // Split data into two parts for Adler-32 combination
        size_t split = bytes_read / 2;
        adler1 = adler32(0L, in_buffer, (uInt)split);
        adler2 = adler32(0L, in_buffer + split, (uInt)(bytes_read - split));
        
        // Combine the two Adler-32 checksums
        uLong combined = adler32_combine(adler1, adler2, (off_t)bytes_read);
        (void)combined;  // Use result to avoid compiler warning
    }
    
    // Clean up deflate stream
    deflateEnd(&strm);
	int fuzz_fd_0 = fuzz_fileno(out_file);

    
    // Create another gzFile for reading the compressed output
    gzFile gz_out = gzdopen(fuzz_fd_0, "wb");
    if (gz_out) {
        // Write some data to the gzip file
        gzwrite(gz_out, data, (unsigned int)(size > 100 ? 100 : size));
        gzclose(gz_out);
    }
    
    // Clean up files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove temporary output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fuzz_fd_0);
	assert_fd_closed(fd);
	return 0;
}