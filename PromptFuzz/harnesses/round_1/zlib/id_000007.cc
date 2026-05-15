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
//<ID> 15
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["get_crc_table","gzdopen","inflateInit2_","gzclose","deflateInit2_","inflateEnd","gzclose","inflateGetHeader","deflateSetHeader","deflateTune","inflateValidate","inflatePrime","gzfread","inflate","inflateEnd","deflateEnd","gzclose"],"critical_calls":["get_crc_table","gzdopen","inflateInit2_","deflateInit2_","inflateGetHeader","deflateSetHeader","deflateTune","inflateValidate","inflatePrime","gzfread","inflate","inflateEnd","deflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    (void)fd_out; // May be unused but obtained as required

    // 1. Get CRC table (API: get_crc_table)
    const z_crc_t *crc_table = get_crc_table();
    
    // 2. Create gzFile for reading compressed data
    gzFile gz_in = gzdopen(fd_in, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 3. Setup inflation stream
    z_stream strm_inflate;
    memset(&strm_inflate, 0, sizeof(strm_inflate));
    strm_inflate.zalloc = Z_NULL;
    strm_inflate.zfree = Z_NULL;
    strm_inflate.opaque = Z_NULL;
    
    // Initialize inflation with windowBits for gzip header detection
    int ret = inflateInit2(&strm_inflate, 15 + 16); // 16 enables gzip decoding
    if (ret != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 4. Setup deflation stream for comparison
    z_stream strm_deflate;
    memset(&strm_deflate, 0, sizeof(strm_deflate));
    strm_deflate.zalloc = Z_NULL;
    strm_deflate.zfree = Z_NULL;
    strm_deflate.opaque = Z_NULL;
    
    // Initialize deflation
    ret = deflateInit2(&strm_deflate, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                       15 + 16, 8, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK) {
        inflateEnd(&strm_inflate);
        gzclose(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 5. Setup gz_header structures
    gz_header header_inflate, header_deflate;
    memset(&header_inflate, 0, sizeof(header_inflate));
    memset(&header_deflate, 0, sizeof(header_deflate));
    
    // API: inflateGetHeader - request header collection during inflation
    inflateGetHeader(&strm_inflate, &header_inflate);
    
    // API: deflateSetHeader - set header for deflation
    header_deflate.os = 255; // Unknown OS
    deflateSetHeader(&strm_deflate, &header_deflate);

    // 6. API: deflateTune - tune deflation parameters
    // Use bits from input to parameterize
    unsigned char tune_params = data[0];
    deflateTune(&strm_deflate,
                (tune_params & 0x0F) + 8,    // good_length
                ((tune_params >> 4) & 0x0F) + 8, // max_lazy
                (tune_params & 0x1F) + 8,    // nice_length
                ((tune_params >> 2) & 0x3F) + 4); // max_chain

    // 7. API: inflateValidate - enable/disable validation during inflation
    // Use second byte if available to determine validation
    int validate = (size > 1 && (data[1] & 0x01)) ? 1 : 0;
    inflateValidate(&strm_inflate, validate);

    // 8. API: inflatePrime - insert bits into the inflation stream
    // Use third byte if available
    if (size > 2) {
        inflatePrime(&strm_inflate, 8, data[2]);
    }

    // 9. Read compressed data using gzfread
    // API: gzfread - read from gzFile
    Bytef read_buffer[4096];
    z_size_t items_read;
    
    do {
        items_read = gzfread(read_buffer, 1, sizeof(read_buffer), gz_in);
        
        if (items_read > 0) {
            // Set input for inflation
            strm_inflate.next_in = read_buffer;
            strm_inflate.avail_in = (uInt)items_read;
            
            // Inflate loop
            Bytef out_buffer[8192];
            do {
                strm_inflate.next_out = out_buffer;
                strm_inflate.avail_out = sizeof(out_buffer);
                
                ret = inflate(&strm_inflate, Z_NO_FLUSH);
                
                if (ret != Z_STREAM_ERROR && ret != Z_DATA_ERROR) {
                    size_t have = sizeof(out_buffer) - strm_inflate.avail_out;
                    if (have > 0) {
                        fwrite(out_buffer, 1, have, out_file);
                    }
                }
            } while (strm_inflate.avail_out == 0);
        }
    } while (items_read == sizeof(read_buffer));

    // 10. Clean up
    inflateEnd(&strm_inflate);
    deflateEnd(&strm_deflate);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}