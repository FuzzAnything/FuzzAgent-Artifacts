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
//<ID> 96
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 7
//<Quality> {"density":7,"unique_branches":{"inflate":[[700,9,700,18,0,0,4,0],[1111,9,1111,18,0,0,4,0]],"deflate":[[1214,10,1214,29,0,0,4,1],[1239,17,1239,41,0,0,4,0]],"_tr_align":[[276,7,276,40,1,0,4,1],[276,7,276,40,7,0,4,0],[276,7,276,40,7,0,4,1]]},"library_calls":["uncompress_z","compressBound","compress2","inflateInit2_","inflateGetHeader","inflateValidate","inflate","inflatePrime","inflate","deflateInit_","deflateUsed","deflateEnd","inflateEnd","gzdopen","gzwrite","gzflush","gzclose"],"critical_calls":["uncompress_z","compressBound","compress2","inflateInit2_","inflateGetHeader","inflateValidate","inflate","inflatePrime","inflate","deflateInit_","deflateUsed","deflateEnd","inflateEnd","gzdopen","gzwrite","gzflush","gzclose"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if input is too small to be meaningful
    if (size < 10) {
	return 0;
	}
	

    // Create a FILE* for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Create a FILE* for writing output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Convert FILE* to file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Allocate buffers for compressed and uncompressed data
    Bytef *compressed = nullptr;
    Bytef *uncompressed = nullptr;
    Bytef *recompressed = nullptr;
    
    // Try to uncompress the input data using uncompress_z
    z_size_t uncompressed_len = size * 4; // Initial guess
    z_size_t compressed_len = size;
    
    do {
        if (uncompressed) free(uncompressed);
        uncompressed_len *= 2;
        uncompressed = (Bytef *)malloc(uncompressed_len);
        if (!uncompressed) break;
        
        z_size_t dest_len = uncompressed_len;
        z_size_t src_len = size;
        int result = uncompress_z(uncompressed, &dest_len, data, src_len);
        
        if (result == Z_OK) {
            uncompressed_len = dest_len;
            // Now compress the uncompressed data using compress2
            uLongf recompressed_len = compressBound(uncompressed_len);
            recompressed = (Bytef *)malloc(recompressed_len);
            if (recompressed) {
                // Try multiple compression levels
                for (int level = 0; level <= 9; level += 3) {
                    uLongf current_len = recompressed_len;
                    compress2(recompressed, &current_len, uncompressed, uncompressed_len, level);
                }
            }
            break;
        } else if (result != Z_BUF_ERROR) {
            break;
        }
    } while (uncompressed_len < size * 100); // Limit growth

    // Set up inflate stream for processing compressed data
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    
    if (inflateInit2(&inflate_strm, 15 + 32) == Z_OK) { // 15 window bits + 32 for gzip decoding
        // Allocate buffer for inflation
        Bytef *inflate_buf = (Bytef *)malloc(4096);
        if (inflate_buf) {
            // Set up gzip header structure
            gz_header head;
            memset(&head, 0, sizeof(head));
            
            // Try to get header (works for gzip format)
            inflateGetHeader(&inflate_strm, &head);
            
            // Enable validation
            inflateValidate(&inflate_strm, 1);
            
            // Process input data with inflate
            inflate_strm.next_in = (Bytef *)data;
            inflate_strm.avail_in = size;
            inflate_strm.next_out = inflate_buf;
            inflate_strm.avail_out = 4096;
            
            // Try to inflate with partial data
            inflate(&inflate_strm, Z_NO_FLUSH);
            
            // Add some bits using inflatePrime (simulating extra input bits)
            inflatePrime(&inflate_strm, 3, 0x05);
            
            // Continue inflation
            inflate(&inflate_strm, Z_FINISH);
            
            free(inflate_buf);
        }
        
        // Try to get number of bits used (from deflate side)
        int bits_used = 0;
        // Note: We need a deflate stream for deflateUsed, so create one
        z_stream deflate_strm;
        memset(&deflate_strm, 0, sizeof(deflate_strm));
        if (deflateInit(&deflate_strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
            deflateUsed(&deflate_strm, &bits_used);
            deflateEnd(&deflate_strm);
        }
        
        inflateEnd(&inflate_strm);
    }

    // Test gzFile operations
    gzFile gz_out = gzdopen(out_fd, "wb");
    if (gz_out) {
        // Write some data if we have uncompressed data
        if (uncompressed && uncompressed_len > 0) {
            gzwrite(gz_out, uncompressed, std::min((unsigned int)uncompressed_len, 1000U));
        }
        
        // Try to flush the gzFile with different flush modes
        for (int flush = Z_NO_FLUSH; flush <= Z_FULL_FLUSH; ++flush) {
            gzflush(gz_out, flush);
        }
        
        gzclose(gz_out);
    } else {
        // If gzdopen failed, we still need to close the file descriptor
        close(out_fd);
    }

    // Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    if (uncompressed) free(uncompressed);
    if (recompressed) free(recompressed);
    
    // Remove the output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}