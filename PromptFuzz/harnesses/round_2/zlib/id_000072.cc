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
//<ID> 219
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"deflateSetDictionary":[[571,9,571,18,0,0,4,0]]},"library_calls":["zlibCompileFlags","deflateInit2_","deflateSetDictionary","inflateInit2_","deflateEnd","inflateGetHeader","inflateCopy","gzdopen","inflateEnd","inflateEnd","deflateEnd","gzdopen","gzclose","inflateEnd","inflateEnd","deflateEnd","gzprintf","gzeof","gzseek64","adler32","adler32","adler32","adler32","adler32_combine","inflate","gzclose","gzclose","inflateEnd","inflateEnd","deflateEnd"],"critical_calls":["zlibCompileFlags","deflateInit2_","deflateSetDictionary","inflateInit2_","inflateGetHeader","inflateCopy","gzdopen","gzdopen","gzprintf","gzeof","gzseek64","adler32","adler32","adler32","adler32","adler32_combine","inflate","gzclose","gzclose","inflateEnd","inflateEnd","deflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 32) {
        return 0;  // Need minimum data
    }
    
    // First, let's get zlib compilation flags
    uLong compile_flags = zlibCompileFlags();
    (void)compile_flags;  // Prevent unused warning
    
    // Create input FILE* using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output FILE*
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize compression stream
    z_stream comp_stream;
    memset(&comp_stream, 0, sizeof(comp_stream));
    
    // Initialize with deflate using deflateInit2_ (since we need to use inflateInit2_ later)
    int ret = deflateInit2_(&comp_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                           15 + 16, 8, Z_DEFAULT_STRATEGY,
                           ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set dictionary for compression
    // Use first 256 bytes of data or size whichever is smaller
    uInt dict_len = (size < 256) ? (uInt)size : 256;
    int dict_ret = deflateSetDictionary(&comp_stream, data, dict_len);
    (void)dict_ret;  // Use result to prevent warning
    
    // Initialize decompression stream for processing zlib output
    z_stream decomp_stream;
    memset(&decomp_stream, 0, sizeof(decomp_stream));
    
    // Use inflateInit2_ as specified
    ret = inflateInit2_(&decomp_stream, 15 + 16, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        deflateEnd(&comp_stream);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Setup gz_header for inflateGetHeader
    gz_header header;
    memset(&header, 0, sizeof(header));
    header.extra = Z_NULL;
    header.name = Z_NULL;
    header.comment = Z_NULL;
    
    // Request header during inflation
    inflateGetHeader(&decomp_stream, &header);
    
    // Copy the decompression stream using inflateCopy
    z_stream decomp_stream_copy;
    memset(&decomp_stream_copy, 0, sizeof(decomp_stream_copy));
    int copy_ret = inflateCopy(&decomp_stream_copy, &decomp_stream);
    (void)copy_ret;
    
    // Create gzFile from file descriptor for input
    int fd_in = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd_in, "rb");
    if (!gz_in) {
        inflateEnd(&decomp_stream);
        inflateEnd(&decomp_stream_copy);
        deflateEnd(&comp_stream);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Create gzFile from file descriptor for output
    int fd_out = fuzz_fileno(out_file);
    gzFile gz_out = gzdopen(fd_out, "wb");
    if (!gz_out) {
        gzclose(gz_in);
        inflateEnd(&decomp_stream);
        inflateEnd(&decomp_stream_copy);
        deflateEnd(&comp_stream);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Use gzprintf to write to output gzFile
    gzprintf(gz_out, "Processing %zu bytes of data\n", size);
    
    // Read from input gzFile using gzgetc
    // Read first 100 bytes or until EOF
    for (int i = 0; i < 100 && !gzeof(gz_in); i++) {
        int ch = gzgetc(gz_in);
        (void)ch;  // Use result
    }
    
    // Use gzseek64 to seek to beginning
    off64_t seek_pos = gzseek64(gz_in, 0, SEEK_SET);
    (void)seek_pos;
    
    // Calculate Adler-32 checksums for two halves of data
    uLong adler1 = adler32(0L, Z_NULL, 0);
    uLong adler2 = adler32(0L, Z_NULL, 0);
    
    size_t half = size / 2;
    if (half > 0) {
        adler1 = adler32(adler1, data, (uInt)half);
        adler2 = adler32(adler2, data + half, (uInt)(size - half));
    }
    
    // Combine the two Adler-32 checksums
    uLong combined_adler = adler32_combine(adler1, adler2, (off_t)(size - half));
    (void)combined_adler;
    
    // Process some data through decompression stream
    Bytef out_buffer[1024];
    decomp_stream.next_in = (Bytef*)data;
    decomp_stream.avail_in = (uInt)(size > 1024 ? 1024 : size);
    decomp_stream.next_out = out_buffer;
    decomp_stream.avail_out = sizeof(out_buffer);
    
    // Try to inflate a bit
    ret = inflate(&decomp_stream, Z_SYNC_FLUSH);
    (void)ret;
    
    // Cleanup in reverse order
    gzclose(gz_in);      // This also closes the underlying file descriptor
    gzclose(gz_out);     // This also closes the underlying file descriptor
    
    // Note: gzclose already closed the FILE*, so we don't need to fclose them again
    // But we need to close the FILE* if gzdopen failed earlier - but it didn't
    
    inflateEnd(&decomp_stream);
    inflateEnd(&decomp_stream_copy);
    deflateEnd(&comp_stream);
    
    // Since gzclose already closed the FILE streams, we just need to 
    // remove the temporary output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}