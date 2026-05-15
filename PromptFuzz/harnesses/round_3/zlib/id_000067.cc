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
//<ID> 151
//<Prompt> []
/*<Combination>: [
*/
//<score> 52, nr_unique_branch: 8
//<Quality> {"density":13,"unique_branches":{"gz_write":[[230,13,230,33,0,0,4,0],[230,37,230,69,0,0,4,1]],"crc32_combine_op":[[970,9,970,16,0,0,4,0]],"gzgetc":[[485,9,485,27,0,0,4,0],[485,31,485,56,0,0,4,0],[485,31,485,56,0,0,4,1],[485,60,485,73,0,0,4,0],[490,9,490,22,0,0,4,0]]},"library_calls":["gzdopen","compressBound","gzclose","compress2","gzclose","compressBound_z","gzclose","compress2_z","gzclose","gzfwrite","gzfwrite","gzflush","gzclose","gzdopen","gzoffset","gzgetc_","crc32","crc32_combine_op","gzclose"],"critical_calls":["gzdopen","compressBound","compress2","compressBound_z","compress2_z","gzfwrite","gzfwrite","gzflush","gzclose","gzdopen","gzoffset","gzgetc_","crc32","crc32_combine_op","gzclose"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified zlib APIs to process zlib-compressed data:

*/


extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t* f_data, size_t f_size) {
	if(f_size<30) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
	//fuzzer shim end}




    // Check minimum size for processing
    if (size < 10) {
        return 0;
    }

    // Create input and output files
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

    int fd_out = fuzz_fileno(out_file);
    if (fd_out < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	return 0;
    }

    // Create gzFile for writing
    gzFile gz_out = gzdopen(fd_out, fuzz_str_1);
    if (!gz_out) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	return 0;
    }

    // Variables for compression
    uLongf destLen = compressBound(size);
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) {
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	return 0;
    }

    // 1. First compression using compress2
    uLongf destLen1 = destLen;
    int level1 = data[0] % 10; // Use first byte for level (0-9)
    int ret1 = compress2(dest, &destLen1, data, size, level1);
    
    if (ret1 != Z_OK) {
        free(dest);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	return 0;
    }

    // 2. Second compression using compress2_z
    z_size_t destLen_z = compressBound_z(size);
    Bytef *dest_z = (Bytef *)malloc(destLen_z);
    if (!dest_z) {
        free(dest);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	return 0;
    }

    z_size_t destLen2 = destLen_z;
    int level2 = data[1] % 10; // Use second byte for level
    int ret2 = compress2_z(dest_z, &destLen2, data, size, level2);
    
    if (ret2 != Z_OK) {
        free(dest);
        free(dest_z);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	return 0;
    }

    // 3. Write compressed data to gzFile using gzfwrite
    // Write first compressed data
    z_size_t items_written1 = gzfwrite(dest, 1, destLen1, gz_out);
    
    // Write second compressed data
    z_size_t items_written2 = gzfwrite(dest_z, 1, destLen2, gz_out);

    // Flush and close gz file for writing
    gzflush(gz_out, fuzz_int32_t_2);
    gzclose(gz_out);
    assert_file_closed(&out_file);;

    // 4. Reopen output file for reading as gzFile
    FILE *out_read_file = fopen("output_file", "rb");
    if (!out_read_file) {
        free(dest);
        free(dest_z);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_read_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	return 0;
    }

    int fd_in = fuzz_fileno(out_read_file);
    if (fd_in < 0) {
        free(dest);
        free(dest_z);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_read_file);;
        assert_file_closed(&out_read_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	assert_fd_closed(fd_out);
	return 0;
    }

    gzFile gz_in = gzdopen(fd_in, fuzz_str_3);
    if (!gz_in) {
        free(dest);
        free(dest_z);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_read_file);;
        assert_file_closed(&out_read_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	assert_fd_closed(fd_out);
	return 0;
    }

    // 5. Read from gzFile using gzgetc_ and track offset
    int bytes_read = 0;
    uLong crc1 = 0;
    uLong crc2 = 0;
    int op_counter = 0;

    while (true) {
        // Get current offset
        off_t offset = gzoffset(gz_in);
        
        // Read a character
        int c = gzgetc_(gz_in);
        if (c < 0) {
            break;
        }
        
        bytes_read++;
        
        // Update CRCs based on position
        Bytef byte_val = (Bytef)c;
        if (offset % 2 == 0) {
            // Use crc32_combine_op to combine CRCs
            uLong new_crc = crc32(fuzz_uint64_t_4, &byte_val, 1);
            if (op_counter == 0) {
                crc1 = new_crc;
            } else {
                // Create an operation value from input data
                uLong op = (data[op_counter % size] << 24) | 
                          (data[(op_counter + 1) % size] << 16) |
                          (data[(op_counter + 2) % size] << 8) |
                          (data[(op_counter + 3) % size]);
                
                crc2 = crc32_combine_op(fuzz_uint64_t_5, new_crc, op);
                crc1 = crc2;
            }
            op_counter++;
        }
        
        // Stop if we've read enough or reached end
        if (bytes_read > 1000) {
            break;
        }
    }

    // 6. Clean up
    free(dest);
    free(dest_z);
    gzclose(gz_in);  // This will also close the underlying file descriptor
    assert_file_closed(&in_file);;
    
    // Note: out_read_file was closed by gzclose(gz_in)
    // Remove the temporary output file
    remove("output_file");

    assert_file_closed(&out_read_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_in);
	assert_fd_closed(fd_out);
	return 0;
}