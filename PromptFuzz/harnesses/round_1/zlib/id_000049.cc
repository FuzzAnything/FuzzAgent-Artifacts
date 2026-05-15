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
//<ID> 97
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 4
//<Quality> {"density":14,"unique_branches":{"gz_comp":[[94,9,94,21,0,0,4,0],[97,13,97,32,0,0,4,0],[97,13,97,32,0,0,4,1],[97,36,97,55,0,0,4,1]]},"library_calls":["inflateInit2_","inflateEnd","inflateGetHeader","inflateValidate","inflatePrime","inflate","inflateEnd","compressBound","compress2","uncompress_z","deflateInit_","deflate","deflateUsed","deflateEnd","gzdopen","gzbuffer","gzwrite","gzflush","gzwrite","gzflush","gzclose"],"critical_calls":["inflateInit2_","inflateGetHeader","inflateValidate","inflatePrime","inflate","inflateEnd","compressBound","compress2","uncompress_z","deflateInit_","deflate","deflateUsed","deflateEnd","gzdopen","gzbuffer","gzwrite","gzflush","gzwrite","gzflush","gzclose"],"visited":1}
/*To create a fuzz driver that achieves the specified event using the given zlib APIs, I'll design a program that performs the following steps:

1. Validate and decompress input data using inflate operations with header reading
2. Recompress the data with validation
3. Use file operations with gzip utilities
4. Measure and manipulate compression state

Here's the complete implementation:

*/


extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if input is too small for meaningful operations
    if (size < 100) {
        return 0;
    }

    // Create files for operations
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

    // Step 1: Decompress the input data using inflate operations
    
    // Prepare z_stream for inflation
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Allocate memory for decompressed data
    Bytef *decompressed = nullptr;
    Bytef *recompressed = nullptr;
    int ret = Z_OK;
    
    // Initialize inflation with gzip/zlib format detection
    ret = inflateInit2(&strm, 15 + 32);  // 32 enables zlib/gzip detection
    if (ret != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up input buffer
    strm.avail_in = static_cast<uInt>(size);
    strm.next_in = (Bytef *)data;
    
    // Prepare for decompression
    uLongf decompressed_size = size * 4;  // Conservative estimate
    decompressed = (Bytef *)malloc(decompressed_size);
    if (!decompressed) {
        inflateEnd(&strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    strm.avail_out = decompressed_size;
    strm.next_out = decompressed;
    
    // Set up gzip header structure
    gz_header header;
    memset(&header, 0, sizeof(header));
    
    // Use inflateGetHeader to read gzip header if present
    inflateGetHeader(&strm, &header);
    
    // Enable validation during inflation
    inflateValidate(&strm, 1);  // Enable validation
    
    // Prime the inflate stream with some bits from input
    // Use first 16 bits for priming
    if (size >= 2) {
        int bits = (data[0] << 8) | data[1];
        inflatePrime(&strm, 16, bits);
        
        // Adjust input buffer after priming
        strm.avail_in = static_cast<uInt>(size - 2);
        strm.next_in = (Bytef *)data + 2;
    }
    
    // Perform inflation
    ret = inflate(&strm, Z_FINISH);
    
    // Calculate actual decompressed size
    uLongf actual_decompressed_size = decompressed_size - strm.avail_out;
    
    // End inflation
    inflateEnd(&strm);
    
    // Step 2: Recompress the decompressed data
    
    if (ret == Z_STREAM_END || ret == Z_OK) {
        // Prepare for compression
        uLongf compressed_size = compressBound(actual_decompressed_size);
        recompressed = (Bytef *)malloc(compressed_size);
        
        if (recompressed) {
            // Use compress2 to recompress with medium compression level
            int level = (data[0] % 10) - 1;  // Random level between -1 and 9
            compress2(recompressed, &compressed_size, 
                     decompressed, actual_decompressed_size, level);
            
            // Step 3: Use uncompress_z to verify compression
            Bytef *verified = (Bytef *)malloc(actual_decompressed_size);
            if (verified) {
                z_size_t verified_size = actual_decompressed_size;
                uncompress_z(verified, &verified_size, 
                           recompressed, compressed_size);
                free(verified);
            }
            
            // Step 4: Set up deflate stream to use deflateUsed
            z_stream def_strm;
            memset(&def_strm, 0, sizeof(def_strm));
            deflateInit(&def_strm, level);
            
            def_strm.avail_in = static_cast<uInt>(actual_decompressed_size);
            def_strm.next_in = decompressed;
            
            def_strm.avail_out = compressed_size;
            def_strm.next_out = recompressed;
            
            deflate(&def_strm, Z_FINISH);
            
            // Use deflateUsed to check bits used
            int bits_used = 0;
            deflateUsed(&def_strm, &bits_used);
            
            deflateEnd(&def_strm);
        }
    }
    
    // Step 5: Use gzFile operations with gzflush
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Open gzFile for writing
    gzFile gz_out = gzdopen(out_fd, "wb");
    if (gz_out) {
        // Set buffer size
        gzbuffer(gz_out, 8192);
        
        // Write some data to gzFile
        if (decompressed && actual_decompressed_size > 0) {
            // Use a portion of decompressed data
            size_t write_size = actual_decompressed_size > 1000 ? 1000 : actual_decompressed_size;
            gzwrite(gz_out, decompressed, static_cast<unsigned int>(write_size));
            
            // Use gzflush with different flush modes
            int flush_mode = Z_NO_FLUSH;
            if (size > 10) {
                flush_mode = data[10] % 4;  // Random flush mode
                switch (flush_mode) {
                    case 0: flush_mode = Z_NO_FLUSH; break;
                    case 1: flush_mode = Z_SYNC_FLUSH; break;
                    case 2: flush_mode = Z_FULL_FLUSH; break;
                    case 3: flush_mode = Z_FINISH; break;
                }
            }
            
            gzflush(gz_out, flush_mode);
            
            // Write more data
            if (actual_decompressed_size > write_size) {
                gzwrite(gz_out, decompressed + write_size, 
                       static_cast<unsigned int>(actual_decompressed_size - write_size));
            }
            
            // Final flush
            gzflush(gz_out, Z_FINISH);
        }
        
        // Close gzFile
        gzclose(gz_out);
    } else {
        // If gzdopen failed, we need to manually close the file descriptor
        close(out_fd);
    }
    
    // Clean up all allocated resources
    if (decompressed) {
        free(decompressed);
    }
    
    if (recompressed) {
        free(recompressed);
    }
    
    assert_file_closed(&in_file);;
    
    // Re-open output file to check it was written properly
    assert_file_closed(&out_file);;
    
    // Remove temporary output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}