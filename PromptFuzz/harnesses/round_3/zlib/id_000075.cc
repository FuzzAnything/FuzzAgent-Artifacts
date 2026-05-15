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
//<ID> 185
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 1
//<Quality> {"density":8,"unique_branches":{"deflate":[[1086,17,1086,32,0,0,4,0]]},"library_calls":["gzdopen","gzdopen","gzclose","gzclose","gzbuffer","inflateInit2_","gzclose","gzclose","inflateGetHeader","gzungetc","gzread","inflate","gzwrite","gzseek","gzread","deflateInit_","deflateReset","deflateEnd","inflateEnd","gzclose","gzclose"],"critical_calls":["gzdopen","gzdopen","gzbuffer","inflateInit2_","inflateGetHeader","gzungetc","gzread","inflate","gzwrite","gzseek","gzread","deflateInit_","deflateReset","deflateEnd","inflateEnd","gzclose","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_75(const uint8_t* f_data, size_t f_size) {
	if(f_size<22) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // 1. Create input and output files
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

    // 2. Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // 3. Open gzFile for reading and writing
    gzFile gz_in = gzdopen(in_fd, fuzz_str_1);
    gzFile gz_out = gzdopen(out_fd, fuzz_str_2);
    
    if (!gz_in || !gz_out) {
        if (gz_in) gzclose(gz_in);
        if (gz_out) gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // 4. Set buffer size for output gzFile
    unsigned int buffer_size = 8192; // Default buffer size
    if (size > 100) {
        buffer_size = (unsigned int)(data[0] % 65536) + 1;
    }
    gzbuffer(gz_out, fuzz_uint32_t_3);

    // 5. Prepare for decompression with header reading
    z_stream strm;
    gz_header header;
    memset(&strm, 0, sizeof(strm));
    memset(&header, 0, sizeof(header));
    
    // Initialize decompression
    if (inflateInit2(&strm, 15 + 16) != Z_OK) { // 15+16 for gzip decoding
        gzclose(gz_in);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // 6. Set up to read gzip header during decompression
    inflateGetHeader(&strm, &header);

    // 7. Read compressed data through gzFile and decompress
    Bytef read_buffer[4096];
    Bytef decompress_buffer[4096];
    int bytes_read;
    int decompress_result;
    
    strm.avail_out = sizeof(decompress_buffer);
    strm.next_out = decompress_buffer;

    // Read first byte using gzgetc
    int first_byte = gzgetc(gz_in);
    if (first_byte != -1) {
        // Put the byte back for decompression
        gzungetc(first_byte, gz_in);
    }

    // Read and decompress data
    while ((bytes_read = gzread(gz_in, read_buffer, sizeof(read_buffer))) > 0) {
        strm.avail_in = bytes_read;
        strm.next_in = read_buffer;
        
        do {
            decompress_result = inflate(&strm, fuzz_int32_t_4);
            
            // Write decompressed data to output gzFile
            if (strm.avail_out < sizeof(decompress_buffer)) {
                unsigned int bytes_to_write = sizeof(decompress_buffer) - strm.avail_out;
                gzwrite(gz_out, decompress_buffer, bytes_to_write);
                
                strm.avail_out = sizeof(decompress_buffer);
                strm.next_out = decompress_buffer;
            }
        } while (strm.avail_in > 0 && decompress_result == Z_OK);
        
        if (decompress_result != Z_OK && decompress_result != Z_STREAM_END) {
            break;
        }
    }

    // 8. Seek in the input gzFile (if we have enough data)
    if (size > 100) {
        off_t seek_offset = (off_t)(data[1] % (size/2));
        gzseek(gz_in, seek_offset, fuzz_int32_t_5);
        
        // Try to read a bit more after seeking
        Bytef seek_buffer[100];
        gzread(gz_in, seek_buffer, sizeof(seek_buffer));
    }

    // 9. Create a deflate stream and use deflateReset
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    
    if (deflateInit(&deflate_strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
        // Add some dummy data to the deflate stream
        Bytef dummy_data[] = "test";
        deflate_strm.avail_in = sizeof(dummy_data) - 1;
        deflate_strm.next_in = dummy_data;
        
        // Reset the deflate stream
        deflateReset(&deflate_strm);
        
        // End the deflate stream
        deflateEnd(&deflate_strm);
    }

    // 10. Clean up decompression stream
    inflateEnd(&strm);

    // 11. Close all files and release resources
    gzclose(gz_in);
    gzclose(gz_out);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    // Clean up dynamically allocated header fields if they were allocated
    // (zlib should handle this internally when we call inflateEnd)

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}