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
//<ID> 72
//<Prompt> []
/*<Combination>: [
*/
//<score> 23, nr_unique_branch: 0
//<Quality> {"density":23,"unique_branches":{},"library_calls":["gzdopen","gzdopen","gzclose","gzclose","gzbuffer","gzbuffer","uncompress_z","inflateInit2_","deflateBound","inflate","inflateReset","inflateUndermine","inflateGetDictionary","deflateInit2_","deflateSetHeader","deflateBound","deflate","deflatePending","deflateEnd","inflateEnd","gzrewind","adler32","crc32","gzread","adler32","crc32","gzwrite","gzdopen","gzgets","gzclose","gzeof","gztell","gztell64","gzoffset64","adler32","crc32","adler32_combine","crc32_combine","gzclose","gzclose"],"critical_calls":["gzdopen","gzdopen","gzbuffer","gzbuffer","uncompress_z","inflateInit2_","deflateBound","inflate","inflateReset","inflateUndermine","inflateGetDictionary","deflateInit2_","deflateSetHeader","deflateBound","deflate","deflatePending","deflateEnd","inflateEnd","gzrewind","adler32","crc32","gzread","adler32","crc32","gzwrite","gzdopen","gzgets","gzclose","gzeof","gztell","gztell64","gzoffset64","adler32","crc32","adler32_combine","crc32_combine","gzclose","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create input FILE* from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Create output FILE* for writing
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
    
    // 4. Open gzFile handles
    gzFile gz_in = gzdopen(fd_in, "rb");
    gzFile gz_out = gzdopen(fd_out, "wb");
    if (!gz_in || !gz_out) {
        if (gz_in) gzclose(gz_in);
        if (gz_out) gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 5. Set gzFile buffer size
    gzbuffer(gz_in, 8192);
    gzbuffer(gz_out, 8192);
    
    // 6. Initialize zlib streams for inflation
    z_stream inf_stream;
    memset(&inf_stream, 0, sizeof(z_stream));
    
    // Try to use the input data as zlib compressed data for inflation
    Bytef *inflate_buffer = nullptr;
    uLong inflate_bound = 0;
    
    // First attempt: try uncompress_z directly
    z_size_t destLen = 65536;
    Bytef *dest = (Bytef *)malloc(destLen);
    if (dest) {
        // Note: This may fail since input might not be zlib compressed data
        uncompress_z(dest, &destLen, data, (z_size_t)size);
        free(dest);
    }
    
    // Initialize inflation stream with windowBits=15+32 for automatic header detection
    int ret = inflateInit2_(&inf_stream, 15 + 32, ZLIB_VERSION, sizeof(z_stream));
    if (ret == Z_OK) {
        // 7. Set up input for inflation stream
        inf_stream.avail_in = (uInt)size;
        inf_stream.next_in = (Bytef *)data;
        
        // Calculate bound for inflated data
        inflate_bound = deflateBound(&inf_stream, size);
        if (inflate_bound > 0 && inflate_bound < 10 * 1024 * 1024) { // Reasonable limit
            inflate_buffer = (Bytef *)malloc(inflate_bound);
            if (inflate_buffer) {
                inf_stream.avail_out = (uInt)inflate_bound;
                inf_stream.next_out = inflate_buffer;
                
                // 8. Perform inflation
                ret = inflate(&inf_stream, Z_FINISH);
                
                // 9. Use various inflation functions
                inflateReset(&inf_stream);
                inflateUndermine(&inf_stream, 0);
                
                // Try to get dictionary
                uInt dict_len = 0;
                Bytef *dict_buf = nullptr;
                inflateGetDictionary(&inf_stream, dict_buf, &dict_len);
            }
        }
        
        // 10. Initialize deflation stream
        z_stream def_stream;
        memset(&def_stream, 0, sizeof(z_stream));
        
        ret = deflateInit2_(&def_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                           15 + 16, 8, Z_DEFAULT_STRATEGY,
                           ZLIB_VERSION, sizeof(z_stream));
        if (ret == Z_OK) {
            // 11. Set gz_header for deflation
            gz_header header;
            memset(&header, 0, sizeof(header));
            header.text = 1;
            header.time = 0;
            header.os = 255;
            deflateSetHeader(&def_stream, &header);
            
            // 12. Calculate deflate bound
            uLong deflate_bound = deflateBound(&def_stream, 
                inf_stream.total_out > 0 ? inf_stream.total_out : size);
            
            // 13. Perform deflation if we have inflated data
            if (inflate_buffer && inf_stream.total_out > 0) {
                def_stream.avail_in = (uInt)inf_stream.total_out;
                def_stream.next_in = inflate_buffer;
                
                Bytef *deflate_buffer = (Bytef *)malloc(deflate_bound);
                if (deflate_buffer) {
                    def_stream.avail_out = (uInt)deflate_bound;
                    def_stream.next_out = deflate_buffer;
                    
                    ret = deflate(&def_stream, Z_FINISH);
                    
                    // 14. Check pending bits
                    unsigned int pending;
                    int bits;
                    deflatePending(&def_stream, &pending, &bits);
                    
                    free(deflate_buffer);
                }
            }
            
            // 15. End deflation stream
            deflateEnd(&def_stream);
        }
        
        // 16. End inflation stream
        inflateEnd(&inf_stream);
    }
    
    // 17. Process data through gzFile API
    // Rewind input gzFile
    gzrewind(gz_in);
    
    // Read and write data in chunks
    char buffer[4096];
    int bytes_read;
    uLong adler1 = adler32(0L, Z_NULL, 0);
    uLong crc1 = crc32(0L, Z_NULL, 0);
    
    while ((bytes_read = gzread(gz_in, buffer, sizeof(buffer))) > 0) {
        // Update checksums
        adler1 = adler32(adler1, (const Bytef *)buffer, bytes_read);
        crc1 = crc32(crc1, (const Bytef *)buffer, bytes_read);
        
        // Write to output
        gzwrite(gz_out, buffer, bytes_read);
        
        // Try gzgets on first chunk
        static bool tried_gzgets = false;
        if (!tried_gzgets && bytes_read > 10) {
            char line_buf[256];
            // Temporarily create another gzFile for gzgets
            FILE *tmp_file = fmemopen((void *)buffer, bytes_read, "rb");
            if (tmp_file) {
	int fuzz_fd_0 = fuzz_fileno(tmp_file);

                gzFile tmp_gz = gzdopen(fuzz_fd_0, "rb");
                if (tmp_gz) {
                    gzgets(tmp_gz, line_buf, sizeof(line_buf));
                    gzclose(tmp_gz);
                }
                assert_file_closed(&tmp_file);;
            }
            tried_gzgets = true;
        }
    }
    
    // 18. Check if we're at EOF
    int at_eof = gzeof(gz_in);
    
    // 19. Get file positions
    off_t pos = gztell(gz_in);
    off64_t pos64 = gztell64(gz_in);
    off64_t offset64 = gzoffset64(gz_in);
    
    // 20. Combine checksums (using dummy values for demonstration)
    uLong adler2 = adler32(0L, (const Bytef *)"test", 4);
    uLong crc2 = crc32(0L, (const Bytef *)"test", 4);
    
    uLong combined_adler = adler32_combine(adler1, adler2, 4);
    uLong combined_crc = crc32_combine(crc1, crc2, 4);
    
    // 21. Clean up
    if (inflate_buffer) {
        free(inflate_buffer);
    }
    
    gzclose(gz_in);  // This also closes the underlying file descriptor
    gzclose(gz_out); // This also closes the underlying file descriptor
    
    // Note: gzclose already closed the FILE* objects, so we don't need to fclose them
    // But we opened them with fmemopen/fopen, so we should still close them
    // However, gzdopen duplicates the file descriptor, so we need to close our original FILE*
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove temporary output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}