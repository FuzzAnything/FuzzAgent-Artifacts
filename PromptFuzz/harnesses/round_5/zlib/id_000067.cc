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
//<ID> 173
//<Prompt> []
/*<Combination>: [
*/
//<score> 40, nr_unique_branch: 7
//<Quality> {"density":20,"unique_branches":{"deflate":[[1106,17,1106,43,0,0,4,0],[1118,13,1118,39,0,0,4,0],[1121,20,1121,59,0,0,4,1],[293,64,293,65,45,0,4,0],[293,64,293,65,45,0,4,0],[293,64,293,65,45,0,4,0],[975,13,975,28,57,0,4,1]]},"library_calls":["gzopen","gzopen","gzopen","gzclose","gzsetparams","zError","deflateInit2_","zError","deflateSetHeader","zError","deflateSetDictionary","zError","gzeof","gzread","gztell64","gzwrite","deflate","zError","deflate","zError","deflateEnd","gzclose","gzclose"],"critical_calls":["gzopen","gzopen","gzopen","gzsetparams","zError","deflateInit2_","zError","deflateSetHeader","zError","deflateSetDictionary","zError","gzeof","gzread","gztell64","gzwrite","deflate","zError","deflate","zError","deflateEnd","gzclose","gzclose"],"visited":0}
/**/


// Helper function to read bytes from input
static size_t  id_000067_read_bytes(FILE* file, uint8_t* buffer, size_t size) {
    return fread(buffer, 1, size, file);
}

// Helper function to write bytes to output
static size_t  id_000067_write_bytes(FILE* file, const uint8_t* buffer, size_t size) {
    return fwrite(buffer, 1, size, file);
}

extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	  // Need enough data for meaningful testing

    // Create input file from fuzzer data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Create output files
    FILE* out_file = fopen("output_file", "wb");
    FILE* out_file2 = fopen("output_file2", "wb");
    if (!out_file || !out_file2) {
        assert_file_closed(&in_file);;
        if (out_file) assert_file_closed(&out_file);;
        if (out_file2) assert_file_closed(&out_file2);;
        assert_file_closed(&out_file2);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 1: Open compressed data file for reading
    gzFile gz_in = gzopen("input_file", "rb");
    if (!gz_in) {
        // Create input file first by writing fuzzer data
        assert_file_closed(&in_file);;
        in_file = fopen("input_file", "wb");
        if (in_file) {
             id_000067_write_bytes(in_file, data, size);
            assert_file_closed(&in_file);;
        }
        gz_in = gzopen("input_file", "rb");
    }
    if (!gz_in) {
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file2);;
        assert_file_closed(&out_file2);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // Step 2: Open output file for writing compressed data
    gzFile gz_out = gzopen("output_file", "wb");
    if (!gz_out) {
        gzclose(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file2);;
        assert_file_closed(&out_file2);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // Step 3: Set compression parameters on output stream
    int level = data[0] % 10;  // Random level from fuzzer input
    int strategy = data[1] % 4;  // Random strategy
    int ret = gzsetparams(gz_out, level, strategy);
    if (ret != Z_OK) {
        const char* err_msg = zError(ret);
        // Error message available for debugging
    }

    // Step 4: Create z_stream for deflation with header
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize deflation with header support (windowBits = 15 + 16 for gzip)
    ret = deflateInit2(&strm, level, Z_DEFLATED, 15 + 16, 8, strategy);
    if (ret != Z_OK) {
        const char* err_msg = zError(ret);
        // Error message available for debugging
    }

    // Step 5: Set gzip header
    gz_header header;
    memset(&header, 0, sizeof(header));
    header.text = data[2] & 1;  // Random text flag
    header.time = *(uint32_t*)(data + 3);  // Timestamp from fuzzer data
    header.os = data[7] % 256;  // OS identifier
    
    // Set extra field if we have enough data
    if (size > 50) {
        header.extra = (Bytef*)(data + 20);
        header.extra_len = (size > 100) ? 10 : (uInt)(size - 50);
        header.extra_max = header.extra_len;
    }
    
    ret = deflateSetHeader(&strm, &header);
    if (ret != Z_OK) {
        const char* err_msg = zError(ret);
        // Error message available for debugging
    }

    // Step 6: Set dictionary for deflation
    if (size > 30) {
        // Use part of fuzzer data as dictionary
        uInt dict_len = (size > 100) ? 32 : (uInt)(size / 3);
        if (dict_len > 32768) dict_len = 32768;  // Reasonable limit
        
        ret = deflateSetDictionary(&strm, data + 10, dict_len);
        if (ret != Z_OK) {
            const char* err_msg = zError(ret);
            // Error message available for debugging
        }
    }

    // Step 7: Read from input gzFile and check EOF
    uint8_t buffer[4096];
    while (!gzeof(gz_in)) {
        int bytes_read = gzread(gz_in, buffer, sizeof(buffer));
        if (bytes_read <= 0) break;
        
        // Get current position in input file
        off64_t pos = gztell64(gz_in);
        (void)pos;  // Use position value (avoid unused variable warning)
        
        // Write to output gzFile
        gzwrite(gz_out, buffer, bytes_read);
        
        // Also compress using the z_stream we set up
        strm.avail_in = bytes_read;
        strm.next_in = buffer;
        
        do {
            uint8_t out_buffer[4096];
            strm.avail_out = sizeof(out_buffer);
            strm.next_out = out_buffer;
            
            ret = deflate(&strm, Z_NO_FLUSH);
            if (ret == Z_STREAM_ERROR) {
                const char* err_msg = zError(ret);
                break;
            }
            
            size_t have = sizeof(out_buffer) - strm.avail_out;
            if (have > 0) {
                 id_000067_write_bytes(out_file2, out_buffer, have);
            }
        } while (strm.avail_out == 0);
    }

    // Step 8: Finish compression
    strm.avail_in = 0;
    do {
        uint8_t out_buffer[4096];
        strm.avail_out = sizeof(out_buffer);
        strm.next_out = out_buffer;
        
        ret = deflate(&strm, Z_FINISH);
        if (ret == Z_STREAM_ERROR) {
            const char* err_msg = zError(ret);
        }
        
        size_t have = sizeof(out_buffer) - strm.avail_out;
        if (have > 0) {
             id_000067_write_bytes(out_file2, out_buffer, have);
        }
    } while (ret != Z_STREAM_END);

    // Step 9: Cleanup
    deflateEnd(&strm);
    gzclose(gz_in);
    gzclose(gz_out);
    
    // Close file handles
    assert_file_closed(&out_file);;
    assert_file_closed(&out_file2);;
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    remove("output_file2");

    assert_file_closed(&out_file2);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}