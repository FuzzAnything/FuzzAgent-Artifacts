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
//<ID> 11
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 6
//<Quality> {"density":14,"unique_branches":{"gzgets":[[597,13,597,31,0,0,4,1],[603,13,603,33,0,0,4,1],[605,13,605,24,0,0,4,1],[615,14,615,18,0,0,4,0],[615,22,615,33,0,0,4,0],[620,9,620,19,0,0,4,1]]},"library_calls":["gzdopen","gzopen","gzclose","deflateInit2_","gzclose","gzclose","deflateSetHeader","gzread","crc32","deflateBound","deflate","deflatePending","gzwrite","crc32","deflate","gzwrite","crc32","deflateEnd","gzclose","gzopen","gzoffset64","gztell","gzgets","gzclose","inflateInit2_","inflateUndermine","inflateEnd","crc32_combine","gzclose"],"critical_calls":["gzdopen","gzopen","deflateInit2_","deflateSetHeader","gzread","crc32","deflateBound","deflate","deflatePending","gzwrite","crc32","deflate","gzwrite","crc32","deflateEnd","gzclose","gzopen","gzoffset64","gztell","gzgets","gzclose","inflateInit2_","inflateUndermine","inflateEnd","crc32_combine","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 1: Create a FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 2: Create a gzFile for reading compressed data
    int fd_in = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd_in, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Step 3: Create a gzFile for writing compressed data
    gzFile gz_out = gzopen("output_file", "wb");
    if (!gz_out) {
        gzclose(gz_in);  // This will also close fd_in and in_file
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_in);
	return 0;
    }

    // Step 4: Initialize z_stream for compression
    z_stream strm_comp;
    strm_comp.zalloc = Z_NULL;
    strm_comp.zfree = Z_NULL;
    strm_comp.opaque = Z_NULL;
    
    int ret = deflateInit2(&strm_comp, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                           MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK) {
        gzclose(gz_out);
        gzclose(gz_in);
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_in);
	return 0;
    }

    // Step 5: Set gzip header using deflateSetHeader
    gz_header header;
    memset(&header, 0, sizeof(header));
    header.text = 1;
    header.time = 0;
    header.os = 255;  // Unknown OS
    deflateSetHeader(&strm_comp, &header);

    // Step 6: Read from input gzFile and compress/write to output gzFile
    char buffer[1024];
    char out_buffer[2048];
    int bytes_read;
    uLong source_crc = 0;
    uLong dest_crc = 0;
    
    while ((bytes_read = gzread(gz_in, buffer, sizeof(buffer))) > 0) {
        // Compute CRC of source data
        source_crc = crc32(source_crc, (const Bytef*)buffer, bytes_read);
        
        // Use deflateBound to estimate output size
        uLong bound = deflateBound(&strm_comp, bytes_read);
        
        // Set up compression stream
        strm_comp.next_in = (Bytef*)buffer;
        strm_comp.avail_in = bytes_read;
        strm_comp.next_out = (Bytef*)out_buffer;
        strm_comp.avail_out = sizeof(out_buffer);
        
        // Compress the data
        ret = deflate(&strm_comp, Z_NO_FLUSH);
        if (ret != Z_OK) {
            break;
        }
        
        // Check for pending bits using deflatePending
        unsigned int pending;
        int bits;
        deflatePending(&strm_comp, &pending, &bits);
        
        // Write compressed data to output gzFile
        int compressed_size = sizeof(out_buffer) - strm_comp.avail_out;
        if (compressed_size > 0) {
            gzwrite(gz_out, out_buffer, compressed_size);
            dest_crc = crc32(dest_crc, (const Bytef*)out_buffer, compressed_size);
        }
    }

    // Step 7: Finish compression
    strm_comp.next_in = Z_NULL;
    strm_comp.avail_in = 0;
    strm_comp.next_out = (Bytef*)out_buffer;
    strm_comp.avail_out = sizeof(out_buffer);
    
    ret = deflate(&strm_comp, Z_FINISH);
    if (ret == Z_STREAM_END) {
        int compressed_size = sizeof(out_buffer) - strm_comp.avail_out;
        if (compressed_size > 0) {
            gzwrite(gz_out, out_buffer, compressed_size);
            dest_crc = crc32(dest_crc, (const Bytef*)out_buffer, compressed_size);
        }
    }

    // Step 8: Clean up compression stream
    deflateEnd(&strm_comp);
    gzclose(gz_out);

    // Step 9: Reopen output file for reading
    gzFile gz_out_read = gzopen("output_file", "rb");
    if (gz_out_read) {
        // Get file offset using gzoffset64 (for 64-bit systems)
        off64_t offset = gzoffset64(gz_out_read);
        
        // Get current position using gztell
        off_t pos = gztell(gz_out_read);
        
        // Try to read a line using gzgets
        char line[256];
        gzgets(gz_out_read, line, sizeof(line));
        
        gzclose(gz_out_read);
    }

    // Step 10: Initialize z_stream for decompression
    z_stream strm_decomp;
    strm_decomp.zalloc = Z_NULL;
    strm_decomp.zfree = Z_NULL;
    strm_decomp.opaque = Z_NULL;
    
    ret = inflateInit2(&strm_decomp, MAX_WBITS + 16);
    if (ret == Z_OK) {
        // Step 11: Use inflateUndermine to test error handling
        inflateUndermine(&strm_decomp, 1);
        
        inflateEnd(&strm_decomp);
    }

    // Step 12: Combine CRCs using crc32_combine
    // We'll combine the source CRC with itself at offset 0 (just for demonstration)
    uLong combined_crc = crc32_combine(source_crc, source_crc, 0);

    // Step 13: Clean up remaining resources
    gzclose(gz_in);  // This will close the underlying FILE* and fd
    
    // Step 14: Remove temporary output file
    remove("output_file");

    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_in);
	return 0;
}