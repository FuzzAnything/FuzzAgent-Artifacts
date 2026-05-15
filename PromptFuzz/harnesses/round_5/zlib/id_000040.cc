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
//<ID> 59
//<Prompt> []
/*<Combination>: [
*/
//<score> 69, nr_unique_branch: 3
//<Quality> {"density":23,"unique_branches":{"deflate":[[1256,17,1256,37,0,0,4,0]],"deflate_slow":[[1644,8,1644,31,27,0,4,0],[1631,24,1631,44,44,0,4,1]]},"library_calls":["deflateInit2_","inflateInit2_","deflateEnd","gzdopen","inflateEnd","deflateEnd","inflateGetHeader","deflateSetDictionary","deflateGetDictionary","inflateSetDictionary","crc32_z","gzwrite","gzflush","deflate","deflatePending","inflate","deflate","inflate","inflateReset","deflateResetKeep","gzopen64","gzread","gzclose","gzclose","inflateEnd","deflateEnd"],"critical_calls":["deflateInit2_","inflateInit2_","gzdopen","inflateGetHeader","deflateSetDictionary","deflateGetDictionary","inflateSetDictionary","crc32_z","gzwrite","gzflush","deflate","deflatePending","inflate","deflate","inflate","inflateReset","deflateResetKeep","gzopen64","gzread","gzclose","gzclose","inflateEnd","deflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 1) {
        return 0;
    }

    // Create a FILE* for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a FILE* for writing output data
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

    // Variables for zlib operations
    z_stream strm_inflate, strm_deflate;
    gz_header header;
    Bytef dictionary[32768];
    uInt dictLength;
    unsigned int pending;
    int bits;
    int ret;
    uLong crc = 0;

    // Initialize deflate stream with advanced parameters
    memset(&strm_deflate, 0, sizeof(strm_deflate));
    ret = deflateInit2_(&strm_deflate, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                        15 + 16, 8, Z_DEFAULT_STRATEGY,
                        ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Initialize inflate stream with automatic header detection
    memset(&strm_inflate, 0, sizeof(strm_inflate));
    ret = inflateInit2_(&strm_inflate, 15 + 32, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        deflateEnd(&strm_deflate);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Open gzFile for writing using file descriptor
    gzFile gz_out = gzdopen(out_fd, "wb");
    if (!gz_out) {
        inflateEnd(&strm_inflate);
        deflateEnd(&strm_deflate);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Set gzip header for inflate
    memset(&header, 0, sizeof(header));
    header.os = 255; // Unknown OS
    inflateGetHeader(&strm_inflate, &header);

    // Read and process data in chunks
    Bytef in_buffer[4096];
    Bytef out_buffer[8192];
    Bytef gz_buffer[4096];
    size_t bytes_read;

    // First, read some data to use as dictionary
    bytes_read = fread(in_buffer, 1, sizeof(in_buffer), in_file);
    if (bytes_read > 0) {
        // Set dictionary for deflate
        deflateSetDictionary(&strm_deflate, in_buffer, 
                            bytes_read > 32768 ? 32768 : (uInt)bytes_read);
        
        // Get dictionary back
        dictLength = sizeof(dictionary);
        deflateGetDictionary(&strm_deflate, dictionary, &dictLength);
        
        // Use the dictionary for inflate as well
        inflateSetDictionary(&strm_inflate, dictionary, dictLength);
    }

    // Reset file position
    rewind(in_file);

    // Process the entire input
    while (!feof(in_file)) {
        bytes_read = fread(in_buffer, 1, sizeof(in_buffer), in_file);
        if (bytes_read == 0) {
            break;
        }

        // Calculate CRC
        crc = crc32_z(crc, in_buffer, bytes_read);

        // Write to gzFile
        gzwrite(gz_out, in_buffer, bytes_read);

        // Flush gzFile periodically
        if (rand() % 4 == 0) {
            gzflush(gz_out, Z_SYNC_FLUSH);
        }

        // Deflate some data
        strm_deflate.next_in = in_buffer;
        strm_deflate.avail_in = bytes_read;
        
        do {
            strm_deflate.next_out = out_buffer;
            strm_deflate.avail_out = sizeof(out_buffer);
            ret = deflate(&strm_deflate, Z_NO_FLUSH);
        } while (strm_deflate.avail_out == 0);

        // Get pending bits
        deflatePending(&strm_deflate, &pending, &bits);

        // Inflate the deflated data
        strm_inflate.next_in = out_buffer;
        strm_inflate.avail_in = sizeof(out_buffer) - strm_deflate.avail_out;
        
        do {
            strm_inflate.next_out = gz_buffer;
            strm_inflate.avail_out = sizeof(gz_buffer);
            ret = inflate(&strm_inflate, Z_NO_FLUSH);
        } while (strm_inflate.avail_out == 0 && ret == Z_OK);
    }

    // Finalize deflate
    strm_deflate.next_in = Z_NULL;
    strm_deflate.avail_in = 0;
    
    do {
        strm_deflate.next_out = out_buffer;
        strm_deflate.avail_out = sizeof(out_buffer);
        ret = deflate(&strm_deflate, Z_FINISH);
    } while (strm_deflate.avail_out == 0);

    // Finalize inflate
    strm_inflate.next_in = out_buffer;
    strm_inflate.avail_in = sizeof(out_buffer) - strm_deflate.avail_out;
    
    do {
        strm_inflate.next_out = gz_buffer;
        strm_inflate.avail_out = sizeof(gz_buffer);
        ret = inflate(&strm_inflate, Z_FINISH);
    } while (strm_inflate.avail_out == 0 && ret == Z_OK);

    // Reset inflate stream
    inflateReset(&strm_inflate);

    // Reset deflate stream while keeping state
    deflateResetKeep(&strm_deflate);

    // Open another gzFile for reading using gzopen64
    gzFile gz_in = gzopen64("input_file", "rb");
    if (gz_in) {
        // Try to read from it (though it won't have our data)
        Bytef read_buffer[1024];
        gzread(gz_in, read_buffer, sizeof(read_buffer));
        gzclose(gz_in);
    }

    // Cleanup
    gzclose(gz_out);  // This will also close the underlying file descriptor
    inflateEnd(&strm_inflate);
    deflateEnd(&strm_deflate);
    
    // Close the files (gzclose already closed out_file)
    assert_file_closed(&in_file);;
    
    // Note: out_file was closed by gzclose, so we don't need to close it again
    // But we should remove the output file we created
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}