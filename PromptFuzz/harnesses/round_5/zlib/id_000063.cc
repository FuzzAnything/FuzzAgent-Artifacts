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
//<ID> 152
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":7,"unique_branches":{"adler32_combine_":[[150,9,150,21,0,0,4,1]]},"library_calls":["gzdopen","gzdirect","gztell64","gzseek","gzgetc_","deflateInit_","gzclose","deflateReset","deflateParams","adler32_combine","gzdopen","gzwrite","gzflush","gzclose","deflateEnd","gzclose"],"critical_calls":["gzdopen","gzdirect","gztell64","gzseek","gzgetc_","deflateInit_","deflateReset","deflateParams","adler32_combine","gzdopen","gzwrite","gzflush","gzclose","deflateEnd","gzclose"],"visited":0}
/**/


// Helper function to read a 32-bit integer from the input data
static uint32_t  id_000063_ReadUInt32(const uint8_t* data, size_t size, size_t* pos) {
    if (*pos + 4 > size) return 0;
    uint32_t value = 0;
    for (int i = 0; i < 4; ++i) {
        value = (value << 8) | data[*pos];
        (*pos)++;
    }
    return value;
}

// Helper function to read a 16-bit integer from the input data
static uint16_t  id_000063_ReadUInt16(const uint8_t* data, size_t size, size_t* pos) {
    if (*pos + 2 > size) return 0;
    uint16_t value = 0;
    for (int i = 0; i < 2; ++i) {
        value = (value << 8) | data[*pos];
        (*pos)++;
    }
    return value;
}

extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // We need at least some data to work with
    if (size < 100) {
	return 0;
	}
	
    
    size_t pos = 0;
    
    // Create a FILE* for the input data (compressed zlib data)
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Get file descriptor for the input file
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Open the compressed data as a gzFile for reading
    gzFile gz_in = gzdopen(fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 1. Check if the file is in direct mode (not compressed)
    int is_direct = gzdirect(gz_in);
    
    // 2. Get current position in the file
    off64_t current_pos = gztell64(gz_in);
    
    // 3. Seek to a new position (if possible)
    if (size > 50) {
        // Try to seek to position 10
        off_t seek_result = gzseek(gz_in, 10, SEEK_SET);
        (void)seek_result; // Ignore result for fuzzing
    }
    
    // 4. Read a byte using gzgetc
    int byte1 = gzgetc(gz_in);
    
    // 5. Read a byte using gzgetc_ (internal version)
    int byte2 = gzgetc_(gz_in);
    
    // 6. Create a z_stream for deflate operations
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize deflate with default parameters
    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 7. Reset the deflate state
    int reset_result = deflateReset(&strm);
    (void)reset_result; // Ignore result for fuzzing
    
    // 8. Change deflate parameters
    // Read parameters from input data
    int level = ( id_000063_ReadUInt16(data, size, &pos) % 10) - 1; // -1 to 8
    int strategy =  id_000063_ReadUInt16(data, size, &pos) % 4; // 0 to 3
    
    // Clamp values to valid ranges
    if (level < -1) level = -1;
    if (level > 9) level = 9;
    if (strategy < 0) strategy = 0;
    if (strategy > 4) strategy = 4;
    
    int params_result = deflateParams(&strm, level, strategy);
    (void)params_result; // Ignore result for fuzzing
    
    // 9. Combine two Adler-32 checksums
    // Read two checksums and a length from input data
    uLong adler1 =  id_000063_ReadUInt32(data, size, &pos);
    uLong adler2 =  id_000063_ReadUInt32(data, size, &pos);
    off_t len =  id_000063_ReadUInt32(data, size, &pos);
    
    uLong combined_adler = adler32_combine(adler1, adler2, len);
    (void)combined_adler; // Ignore result for fuzzing
    
    // 10. Create an output file for gzflush
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        int out_fd = fuzz_fileno(out_file);
        if (out_fd >= 0) {
            // Open output file as gzFile for writing
            gzFile gz_out = gzdopen(out_fd, "wb");
            if (gz_out) {
                // Write some data to the output file
                const char* test_data = "test data";
                gzwrite(gz_out, test_data, strlen(test_data));
                
                // 11. Flush the output file
                int flush_result = gzflush(gz_out, Z_SYNC_FLUSH);
                (void)flush_result; // Ignore result for fuzzing
                
                gzclose(gz_out);
            }
        }
        assert_file_closed(&out_file);;
    }
    
    // Clean up
    deflateEnd(&strm);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    
    // Remove the output file if it exists
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}