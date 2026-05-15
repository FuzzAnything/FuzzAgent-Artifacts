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
//<ID> 115
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":7,"unique_branches":{"gzvprintf":[[423,9,423,32,0,0,4,0]]},"library_calls":["gzdopen","inflateInit2_","gzclose_r","deflateInit2_","inflateEnd","gzclose_r","gzprintf","deflateEnd","inflateEnd","gzclose_r","inflate","inflateSync","inflate","deflateSetDictionary","adler32","adler32","adler32","adler32","adler32_combine","deflateEnd","inflateEnd","gzclose_r"],"critical_calls":["gzdopen","inflateInit2_","deflateInit2_","gzprintf","inflate","inflateSync","inflate","deflateSetDictionary","adler32","adler32","adler32","adler32","adler32_combine","deflateEnd","inflateEnd","gzclose_r"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimum data for operations

    // Create input FILE* from the fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Open a gzFile for reading from the input file descriptor
    int fd_in = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd_in, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Initialize decompression stream
    z_stream inf_stream;
    memset(&inf_stream, 0, sizeof(inf_stream));
    inf_stream.zalloc = Z_NULL;
    inf_stream.zfree = Z_NULL;
    inf_stream.opaque = Z_NULL;
    
    // Use deflateInit2_ for compression initialization (required API)
    z_stream def_stream;
    memset(&def_stream, 0, sizeof(def_stream));
    def_stream.zalloc = Z_NULL;
    def_stream.zfree = Z_NULL;
    def_stream.opaque = Z_NULL;
    
    // Initialize decompression with deflateInit2_ (windowBits negative for raw decompression)
    int ret = inflateInit2(&inf_stream, -MAX_WBITS);
    if (ret != Z_OK) {
        gzclose_r(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Initialize compression with deflateInit2_ (required API)
    ret = deflateInit2_(&def_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                       MAX_WBITS, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY,
                       ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        inflateEnd(&inf_stream);
        gzclose_r(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Read some data from gzFile using gzprintf to write format string (required API)
    // Note: gzprintf writes to file, we'll use it with a dummy format
    gzprintf(gz_in, "Reading %lu bytes\n", (unsigned long)size);
    
    // Prepare buffers for decompression
    size_t out_buf_size = size * 2;
    Bytef *out_buf = (Bytef*)malloc(out_buf_size);
    Bytef *in_buf = (Bytef*)malloc(size);
    
    if (!out_buf || !in_buf) {
        free(out_buf);
        free(in_buf);
        deflateEnd(&def_stream);
        inflateEnd(&inf_stream);
        gzclose_r(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Read data from gzFile for processing
    size_t bytes_read = fread(in_buf, 1, size, in_file);
    
    // Setup decompression stream
    inf_stream.next_in = in_buf;
    inf_stream.avail_in = (uInt)bytes_read;
    inf_stream.next_out = out_buf;
    inf_stream.avail_out = (uInt)out_buf_size;
    
    // Perform decompression
    ret = inflate(&inf_stream, Z_NO_FLUSH);
    
    // Try inflateSync if decompression had issues (required API)
    if (ret == Z_DATA_ERROR || ret == Z_STREAM_ERROR) {
        inflateSync(&inf_stream);
        // Try decompression again
        ret = inflate(&inf_stream, Z_NO_FLUSH);
    }
    
    // Set dictionary for compression using decompressed data (required API)
    if (inf_stream.total_out > 0) {
        // Use first 256 bytes or whatever's available as dictionary
        uInt dict_len = (inf_stream.total_out < 256) ? (uInt)inf_stream.total_out : 256;
        deflateSetDictionary(&def_stream, out_buf, dict_len);
    }
    
    // Calculate Adler-32 checksums and combine them (required API)
    uLong adler1 = adler32(0L, Z_NULL, 0);
    uLong adler2 = adler32(0L, Z_NULL, 0);
    
    if (bytes_read > 0) {
        // Split data into two parts for checksum calculation
        size_t split = bytes_read / 2;
        if (split > 0) {
            adler1 = adler32(adler1, in_buf, (uInt)split);
            if (bytes_read - split > 0) {
                adler2 = adler32(adler2, in_buf + split, (uInt)(bytes_read - split));
            }
        }
    }
    
    // Combine the two Adler-32 checksums (required API)
    uLong combined_adler = adler32_combine(adler1, adler2, (off_t)bytes_read);
    
    // Use the combined Adler value in the compression stream
    def_stream.adler = combined_adler;
    
    // Clean up all resources
    deflateEnd(&def_stream);
    inflateEnd(&inf_stream);
    
    // Close gzFile with gzclose_r (required API)
    gzclose_r(gz_in);
    
    // Note: gzclose_r already closes the underlying file descriptor,
    // so we don't need to close it again with fclose
    assert_file_closed(&in_file);;
    
    free(out_buf);
    free(in_buf);
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
}