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
//<ID> 51
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["gzdopen","inflateInit2_","gzclose","deflateInit2_","inflateEnd","gzclose","deflateSetHeader","gzeof","gzread","inflate","inflateSync","inflateSyncPoint","deflate","gzdopen","gzsetparams","gzwrite","gzclose","deflate","deflateEnd","inflateEnd","gzclose"],"critical_calls":["gzdopen","inflateInit2_","deflateInit2_","deflateSetHeader","gzeof","gzread","inflate","inflateSync","inflateSyncPoint","deflate","gzdopen","gzsetparams","gzwrite","gzclose","deflate","deflateEnd","inflateEnd","gzclose"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified zlib APIs to process a compressed byte stream:

*/


extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
        return 0;
    }

    // 1. Create input file from fuzzed data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
	int fuzz_fd_1 = fuzz_fileno(in_file);


    // 2. Create gzFile for reading
    gzFile gz_in = gzdopen(fuzz_fd_1, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_1);
	return 0;
    }

    // 3. Create decompression stream
    z_stream strm_in;
    strm_in.zalloc = Z_NULL;
    strm_in.zfree = Z_NULL;
    strm_in.opaque = Z_NULL;
    strm_in.next_in = Z_NULL;
    strm_in.avail_in = 0;

    int ret = inflateInit2(&strm_in, 15 + 32); // 15 window bits + 32 for gzip auto detection
    if (ret != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_1);
	return 0;
    }

    // 4. Create compression stream with header
    z_stream strm_out;
    strm_out.zalloc = Z_NULL;
    strm_out.zfree = Z_NULL;
    strm_out.opaque = Z_NULL;

    // Setup gzip header
    gz_header header;
    memset(&header, 0, sizeof(header));
    header.text = 1;
    header.time = 1234567890;
    header.os = 255; // Unknown OS
    
    int ret2 = deflateInit2(&strm_out, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                           15 + 16, // 15 window bits + 16 for gzip
                           8, Z_DEFAULT_STRATEGY);
    if (ret2 != Z_OK) {
        inflateEnd(&strm_in);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_1);
	return 0;
    }

    // 5. Set header for output compression (deflateSetHeader)
    deflateSetHeader(&strm_out, &header);

    // 6. Process compressed data using gzFile APIs
    unsigned char buffer[4096];
    unsigned char compressed[8192];
    unsigned char decompressed[4096];
    
    // Read from gzFile to check gzeof
    while (!gzeof(gz_in)) {
        int bytes_read = gzread(gz_in, buffer, sizeof(buffer));
        if (bytes_read <= 0) {
            break;
        }

        // 7. Setup inflate stream with data
        strm_in.next_in = buffer;
        strm_in.avail_in = bytes_read;
        
        // 8. Decompress in chunks
        do {
            strm_in.next_out = decompressed;
            strm_in.avail_out = sizeof(decompressed);
            
            ret = inflate(&strm_in, Z_NO_FLUSH);
            
            // 9. Try inflateSync at certain points
            if (strm_in.avail_in > 0 && (size_t)strm_in.next_in[0] % 10 == 0) {
                inflateSync(&strm_in);
            }
            
            // 10. Check sync point
            if (inflateSyncPoint(&strm_in)) {
                // We're at a sync point
            }
            
            // 11. Compress the decompressed data
            if (strm_in.total_out > 0) {
                strm_out.next_in = decompressed;
                strm_out.avail_in = sizeof(decompressed) - strm_in.avail_out;
                strm_out.next_out = compressed;
                strm_out.avail_out = sizeof(compressed);
                
                deflate(&strm_out, Z_NO_FLUSH);
            }
        } while (strm_in.avail_out == 0 && ret == Z_OK);
    }

    // 12. Set compression parameters on gzFile (gzsetparams)
    // First create an output gzFile
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
	int fuzz_fd_0 = fuzz_fileno(out_file);

        gzFile gz_out = gzdopen(fuzz_fd_0, "wb");
        if (gz_out) {
            // Set compression parameters
            gzsetparams(gz_out, 6, Z_DEFAULT_STRATEGY);
            
            // Write some data
            gzwrite(gz_out, "Test data", 9);
            
            gzclose(gz_out);
        }
        assert_file_closed(&out_file);;
    }

    // 13. Finish compression
    strm_out.next_in = Z_NULL;
    strm_out.avail_in = 0;
    strm_out.next_out = compressed;
    strm_out.avail_out = sizeof(compressed);
    
    deflate(&strm_out, Z_FINISH);
    
    // 14. Cleanup
    deflateEnd(&strm_out);
    inflateEnd(&strm_in);
    gzclose(gz_in);
    assert_file_closed(&in_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_1);
	return 0;
}