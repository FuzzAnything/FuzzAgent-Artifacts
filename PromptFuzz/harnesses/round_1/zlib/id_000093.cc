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
//<ID> 279
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 3
//<Quality> {"density":9,"unique_branches":{"inflateSetDictionary":[[1196,9,1196,25,0,0,4,1],[1200,9,1200,28,0,0,4,1],[1210,9,1210,12,0,0,4,1]]},"library_calls":["gzdopen","inflateInit2_","gzclose","inflateEnd","gzclose","inflateSetDictionary","inflateGetDictionary","gzread","inflate","inflateCodesUsed","adler32","adler32","gzdopen","gzprintf","gzclose","inflateEnd","gzclose"],"critical_calls":["gzdopen","inflateInit2_","inflateSetDictionary","inflateGetDictionary","gzread","inflate","inflateCodesUsed","adler32","adler32","gzdopen","gzprintf","gzclose","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_93(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
        // Too small for meaningful zlib operations
        return 0;
    }

    // Create a memory buffer from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Open the input file with gzip compression
    int fd = fuzz_fileno(in_file);
    gzFile gz_input = gzdopen(fd, "rb");
    if (!gz_input) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Initialize zlib stream for inflation
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Use negative windowBits for raw deflate data (no zlib header)
    if (inflateInit2_(&strm, -MAX_WBITS, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        gzclose(gz_input);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Create output buffer
    const size_t output_size = 4096;
    Bytef *output = (Bytef *)malloc(output_size);
    if (!output) {
        inflateEnd(&strm);
        gzclose(gz_input);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Create dictionary buffer (using part of input data as dictionary)
    uInt dict_length = size > 1024 ? 1024 : (uInt)size;
    Bytef *dictionary = (Bytef *)malloc(dict_length);
    if (dictionary) {
        memcpy(dictionary, data, dict_length);
        
        // Set dictionary for inflation
        int dict_result = inflateSetDictionary(&strm, dictionary, dict_length);
        // Note: This might fail if the compressed data doesn't expect a dictionary
        
        // Try to get the dictionary back
        uInt get_dict_length = dict_length;
        Bytef *get_dict = (Bytef *)malloc(dict_length);
        if (get_dict) {
            int get_dict_result = inflateGetDictionary(&strm, get_dict, &get_dict_length);
            free(get_dict);
        }
    }

    // Read from gz file and inflate
    int total_read = 0;
    while (total_read < size) {
        // Read from gz file (this decompresses)
        char read_buffer[1024];
        int gz_bytes = gzread(gz_input, read_buffer, sizeof(read_buffer));
        if (gz_bytes <= 0) {
            break;
        }
        total_read += gz_bytes;
        
        // Also try to inflate directly using zlib stream
        strm.avail_in = gz_bytes < (int)sizeof(read_buffer) ? (uInt)gz_bytes : (uInt)sizeof(read_buffer);
        strm.next_in = (Bytef *)read_buffer;
        
        do {
            strm.avail_out = output_size;
            strm.next_out = output;
            
            int inflate_result = inflate(&strm, Z_NO_FLUSH);
            
            if (inflate_result == Z_STREAM_END || inflate_result == Z_NEED_DICT) {
                break;
            }
            
            // Get information about codes used
            unsigned long codes_used = inflateCodesUsed(&strm);
            
        } while (strm.avail_out == 0);
    }

    // Calculate Adler-32 checksum of the decompressed data
    if (strm.total_out > 0) {
        uLong adler = adler32(0L, Z_NULL, 0);
        adler = adler32(adler, output, (uInt)strm.total_out);
    }

    // Write some output using gzprintf
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        int out_fd = fuzz_fileno(out_file);
        gzFile gz_output = gzdopen(out_fd, "wb");
        if (gz_output) {
            gzprintf(gz_output, "Decompressed %lu bytes\n", (unsigned long)strm.total_out);
            gzclose(gz_output);
        }
        assert_file_closed(&out_file);;
    }

    // Clean up
    inflateEnd(&strm);
    if (dictionary) {
        free(dictionary);
    }
    free(output);
    gzclose(gz_input);
    assert_file_closed(&in_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}