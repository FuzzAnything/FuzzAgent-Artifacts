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
//<score> 29, nr_unique_branch: 0
//<Quality> {"density":29,"unique_branches":{},"library_calls":["gzdopen","gzdirect","gztell64","gzclose","gzdopen","gzclose","deflateInit_","gzclose","gzclose","adler32","adler32","gzread","adler32","gztell64","adler32","gzgetc_","adler32","adler32_combine","deflateReset","deflateEnd","gzclose","gzclose","deflateParams","deflateEnd","gzclose","gzclose","gzseek","deflateEnd","gzclose","gzclose","gzread","deflate","gzwrite","gzflush","deflateEnd","gzclose","gzclose"],"critical_calls":["gzdopen","gzdirect","gztell64","gzdopen","deflateInit_","adler32","adler32","gzread","adler32","gztell64","adler32","gzgetc_","adler32","adler32_combine","deflateReset","deflateParams","gzseek","gzread","deflate","gzwrite","gzflush","deflateEnd","gzclose","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimum data

    // Create input file from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
	int fuzz_fd_1 = fuzz_fileno(in_file);


    // Create gzFile from file descriptor
    gzFile gz_in = gzdopen(fuzz_fd_1, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_1);
	return 0;
    }

    // Check if file is in gzip format
    int direct = gzdirect(gz_in);

    // Get current position
    off64_t pos = gztell64(gz_in);

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_1);
	return 0;
    }
	int fuzz_fd_0 = fuzz_fileno(out_file);


    // Create gzFile for output
    gzFile gz_out = gzdopen(fuzz_fd_0, "wb");
    if (!gz_out) {
        assert_file_closed(&out_file);;
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	assert_fd_closed(fuzz_fd_1);
	return 0;
    }

    // Initialize deflate stream
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    int ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        gzclose(gz_out);
        assert_file_closed(&out_file);;
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	assert_fd_closed(fuzz_fd_1);
	return 0;
    }

    // Read and process data from gz_in
    std::vector<uint8_t> buffer(4096);
    uLong adler1 = adler32(0L, Z_NULL, 0);
    uLong adler2 = adler32(0L, Z_NULL, 0);
    size_t total_read = 0;

    // Read first part
    int bytes_read = 0;
    while (total_read < size / 2 && (bytes_read = gzread(gz_in, buffer.data(), 
             static_cast<unsigned int>(std::min(buffer.size(), size/2 - total_read)))) > 0) {
        adler1 = adler32(adler1, buffer.data(), bytes_read);
        total_read += bytes_read;
    }

    // Get current position again
    pos = gztell64(gz_in);
    
    // Read second part with gzgetc and gzgetc_
    int c;
    while (total_read < size && (c = gzgetc(gz_in)) != -1) {
        uint8_t byte = static_cast<uint8_t>(c);
        adler2 = adler32(adler2, &byte, 1);
        total_read++;
        
        // Also use gzgetc_ for every other byte
        if (total_read < size) {
            c = gzgetc_(gz_in);
            if (c != -1) {
                byte = static_cast<uint8_t>(c);
                adler2 = adler32(adler2, &byte, 1);
                total_read++;
            }
        }
    }

    // Combine Adler-32 checksums
    off_t combined_offset = static_cast<off_t>(total_read / 2);
    uLong combined_adler = adler32_combine(adler1, adler2, combined_offset);

    // Reset deflate stream
    ret = deflateReset(&strm);
    if (ret != Z_OK) {
        deflateEnd(&strm);
        gzclose(gz_out);
        assert_file_closed(&out_file);;
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	assert_fd_closed(fuzz_fd_1);
	return 0;
    }

    // Set deflate parameters
    ret = deflateParams(&strm, Z_BEST_COMPRESSION, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK) {
        deflateEnd(&strm);
        gzclose(gz_out);
        assert_file_closed(&out_file);;
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	assert_fd_closed(fuzz_fd_1);
	return 0;
    }

    // Seek back in input file
    ret = gzseek(gz_in, 0, SEEK_SET);
    if (ret == -1) {
        deflateEnd(&strm);
        gzclose(gz_out);
        assert_file_closed(&out_file);;
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	assert_fd_closed(fuzz_fd_1);
	return 0;
    }

    // Read data again and write to output
    total_read = 0;
    while (total_read < size && (bytes_read = gzread(gz_in, buffer.data(), 
             static_cast<unsigned int>(std::min(buffer.size(), size - total_read)))) > 0) {
        
        // Compress with deflate stream
        strm.avail_in = bytes_read;
        strm.next_in = buffer.data();
        
        do {
            strm.avail_out = static_cast<uInt>(buffer.size());
            strm.next_out = buffer.data();
            
            ret = deflate(&strm, Z_NO_FLUSH);
            if (ret == Z_STREAM_ERROR) break;
            
            uInt have = static_cast<uInt>(buffer.size()) - strm.avail_out;
            
            // Write compressed data to gz_out
            if (have > 0) {
                gzwrite(gz_out, buffer.data(), have);
            }
        } while (strm.avail_out == 0);
        
        total_read += bytes_read;
    }

    // Flush the output
    ret = gzflush(gz_out, Z_FINISH);
    if (ret != Z_OK) {
        // Handle error
    }

    // Clean up
    deflateEnd(&strm);
    gzclose(gz_out);
    assert_file_closed(&out_file);;
    gzclose(gz_in);
    assert_file_closed(&in_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	assert_fd_closed(fuzz_fd_1);
	return 0;
}