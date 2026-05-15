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
//<ID> 21
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 1
//<Quality> {"density":16,"unique_branches":{"deflateGetDictionary":[[634,9,634,24,0,0,4,0]]},"library_calls":["gzopen64","gzdopen","gzclose","deflateInit2_","gzclose","gzclose","inflateInit2_","deflateEnd","gzclose","gzclose","deflateBound_z","deflateEnd","inflateEnd","gzclose","gzclose","deflateSetDictionary","deflate","deflateResetKeep","deflateGetDictionary","inflateGetHeader","inflate","inflateReset","gzwrite","gzflush","crc32_z","crc32_z","gzclose","gzopen64","gzread","gzclose","deflateEnd","inflateEnd","gzclose"],"critical_calls":["gzopen64","gzdopen","deflateInit2_","inflateInit2_","deflateBound_z","deflateSetDictionary","deflate","deflateResetKeep","deflateGetDictionary","inflateGetHeader","inflate","inflateReset","gzwrite","gzflush","crc32_z","crc32_z","gzclose","gzopen64","gzread","gzclose","deflateEnd","inflateEnd","gzclose"],"visited":1}
/*Here's a C++ fuzz driver that uses all the specified zlib APIs following your instructions:

*/


extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need enough data for meaningful testing
    
    // 1. Create a FILE* from input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Create output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Get file descriptors
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    
    // 4. Open gzip file for reading using gzopen64
    gzFile gz_in = gzopen64("output_file", "rb"); // Will read what we write later
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        remove("output_file");
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 5. Open another gzip file for writing
    gzFile gz_out = gzdopen(fd_out, "wb");
    if (!gz_out) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        remove("output_file");
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 6. Set up z_stream structures
    z_stream strm_compress, strm_decompress;
    memset(&strm_compress, 0, sizeof(z_stream));
    memset(&strm_decompress, 0, sizeof(z_stream));
    
    // 7. Initialize compression stream
    if (deflateInit2_(&strm_compress, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                     15 + 16, 8, Z_DEFAULT_STRATEGY,
                     ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        gzclose(gz_in);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        remove("output_file");
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 8. Initialize decompression stream
    if (inflateInit2_(&strm_decompress, 15 + 16, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        deflateEnd(&strm_compress);
        gzclose(gz_in);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        remove("output_file");
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 9. Use deflateBound_z to estimate compressed size
    z_size_t source_len = size / 2;
    z_size_t bound = deflateBound_z(&strm_compress, source_len);
    
    // Allocate buffers
    Bytef *compressed = (Bytef*)malloc(bound);
    Bytef *decompressed = (Bytef*)malloc(size);
    Bytef *dict_buf = (Bytef*)malloc(32768);
    uInt dict_len = 0;
    
    if (!compressed || !decompressed || !dict_buf) {
        free(compressed);
        free(decompressed);
        free(dict_buf);
        deflateEnd(&strm_compress);
        inflateEnd(&strm_decompress);
        gzclose(gz_in);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        remove("output_file");
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 10. Set up input for compression (use first half of data)
    strm_compress.next_in = (Bytef*)data;
    strm_compress.avail_in = source_len;
    strm_compress.next_out = compressed;
    strm_compress.avail_out = bound;
    
    // 11. Set dictionary for compression
    uInt dict_size = (size > 32768) ? 32768 : (uInt)size;
    if (deflateSetDictionary(&strm_compress, (const Bytef*)data, dict_size) != Z_OK) {
        // Continue anyway
    }
    
    // 12. Perform compression
    int flush = Z_FINISH;
    int ret_compress = deflate(&strm_compress, flush);
    
    // 13. Use deflateResetKeep to reset compression state
    if (deflateResetKeep(&strm_compress) != Z_OK) {
        // Continue anyway
    }
    
    // 14. Get dictionary from compression stream
    if (deflateGetDictionary(&strm_compress, dict_buf, &dict_len) != Z_OK) {
        // Continue anyway
    }
    
    // 15. Set up decompression stream with compressed data
    strm_decompress.next_in = compressed;
    strm_decompress.avail_in = strm_compress.total_out;
    strm_decompress.next_out = decompressed;
    strm_decompress.avail_out = size;
    
    // 16. Set up gzip header for decompression
    gz_header header;
    memset(&header, 0, sizeof(gz_header));
    if (inflateGetHeader(&strm_decompress, &header) != Z_OK) {
        // Continue anyway
    }
    
    // 17. Perform decompression
    int ret_decompress = inflate(&strm_decompress, Z_NO_FLUSH);
    
    // 18. Use inflateReset to reset decompression stream
    if (inflateReset(&strm_decompress) != Z_OK) {
        // Continue anyway
    }
    
    // 19. Write compressed data to gzip file
    size_t compressed_size = strm_compress.total_out;
    if (compressed_size > 0) {
        // Write to FILE* first
        fwrite(compressed, 1, compressed_size, out_file);
        fflush(out_file);
        
        // Also write to gzFile
        gzwrite(gz_out, compressed, compressed_size);
        
        // 20. Use gzflush
        gzflush(gz_out, Z_SYNC_FLUSH);
    }
    
    // 21. Calculate CRC32 using crc32_z
    uLong crc = crc32_z(0L, Z_NULL, 0);
    if (decompressed && strm_decompress.total_out > 0) {
        crc = crc32_z(crc, decompressed, strm_decompress.total_out);
    }
    
    // 22. Try to read from the gzip file we wrote to
    if (gz_out) {
        // Close and reopen for reading
        gzclose(gz_out);
        gz_out = gzopen64("output_file", "rb");
        if (gz_out) {
            char read_buf[1024];
            int bytes_read;
            while ((bytes_read = gzread(gz_out, read_buf, sizeof(read_buf))) > 0) {
                // Do nothing, just consume the data
            }
            gzclose(gz_out);
        }
    }
    
    // 23. Clean up
    free(compressed);
    free(decompressed);
    free(dict_buf);
    
    deflateEnd(&strm_compress);
    inflateEnd(&strm_decompress);
    
    if (gz_in) gzclose(gz_in);
    if (in_file) assert_file_closed(&in_file);;
    if (out_file) assert_file_closed(&out_file);;
    
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}