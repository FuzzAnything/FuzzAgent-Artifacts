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
//<ID> 24
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 2
//<Quality> {"density":7,"unique_branches":{"gzflush":[[612,9,612,32,0,0,4,0]],"inflate":[[700,9,700,18,0,0,4,0]]},"library_calls":["gzopen64","inflateInit_","gzclose","deflateInit_","inflateEnd","gzclose","deflateGetDictionary","deflateResetKeep","deflateBound_z","inflateGetHeader","gzeof","gzread","inflate","crc32_z","gzflush","inflateReset","inflateEnd","deflateEnd","gzclose"],"critical_calls":["gzopen64","inflateInit_","deflateInit_","deflateGetDictionary","deflateResetKeep","deflateBound_z","inflateGetHeader","gzeof","gzread","inflate","crc32_z","gzflush","inflateReset","inflateEnd","deflateEnd","gzclose"],"visited":0}
/*Here's a C++ fuzzing driver that uses the specified zlib APIs to achieve decompression of zlib output data with CRC verification, file operations, and stream management:

*/


extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
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

    // Write input data to a temporary file for gzopen64
    FILE *tmp_input = fopen("input_file", "wb");
    if (!tmp_input) {
	assert_file_closed(&tmp_input);
	return 0;
	}
	
    fwrite(data, 1, size, tmp_input);
    assert_file_closed(&tmp_input);;

    // 1. Open the compressed file using gzopen64
    gzFile gzfile = gzopen64("input_file", "rb");
    if (!gzfile) {
        remove("input_file");
        assert_file_closed(&tmp_input);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // 2. Set up z_stream for inflation
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;

    int ret = inflateInit(&strm);
    if (ret != Z_OK) {
        gzclose(gzfile);
        remove("input_file");
        assert_file_closed(&tmp_input);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // 3. Set up deflate stream for deflateGetDictionary and deflateResetKeep
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    def_strm.zalloc = Z_NULL;
    def_strm.zfree = Z_NULL;
    def_strm.opaque = Z_NULL;
    
    if (deflateInit(&def_strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        inflateEnd(&strm);
        gzclose(gzfile);
        remove("input_file");
        assert_file_closed(&tmp_input);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // 4. Use deflateGetDictionary (even though no dictionary is set)
    Bytef dict_buf[1024];
    uInt dict_len = 1024;
    deflateGetDictionary(&def_strm, dict_buf, &dict_len);

    // 5. Use deflateResetKeep
    deflateResetKeep(&def_strm);

    // 6. Use deflateBound_z to estimate compressed size
    // Using first 50 bytes as source for estimation
    size_t source_len = size > 50 ? 50 : size;
    z_size_t bound = deflateBound_z(&def_strm, source_len);

    // 7. Set up gzip header for inflateGetHeader
    gz_header header;
    memset(&header, 0, sizeof(header));
    header.extra_max = 65536;
    header.name_max = 256;
    header.comm_max = 256;
    
    inflateGetHeader(&strm, &header);

    // 8. Read data from gzfile and inflate it
    std::vector<uint8_t> in_buf(4096);
    std::vector<uint8_t> out_buf(4096);
    uLong crc = 0;
    
    while (!gzeof(gzfile)) {
        // Read compressed data
        int bytes_read = gzread(gzfile, in_buf.data(), in_buf.size());
        if (bytes_read <= 0) break;

        // Setup input for inflation
        strm.avail_in = bytes_read;
        strm.next_in = in_buf.data();

        do {
            strm.avail_out = out_buf.size();
            strm.next_out = out_buf.data();

            ret = inflate(&strm, Z_NO_FLUSH);
            if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR) {
                break;
            }

            size_t have = out_buf.size() - strm.avail_out;
            
            // 9. Use crc32_z to calculate CRC of decompressed data
            if (have > 0) {
                crc = crc32_z(crc, out_buf.data(), have);
            }

        } while (strm.avail_out == 0);

        // 10. Use gzflush periodically
        if (rand() % 10 == 0) {
            gzflush(gzfile, Z_SYNC_FLUSH);
        }
    }

    // 11. Use inflateReset
    inflateReset(&strm);

    // Clean up resources
    inflateEnd(&strm);
    deflateEnd(&def_strm);
    gzclose(gzfile);
    remove("input_file");
    
    // Write dummy output to satisfy file operations
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        fwrite(&crc, sizeof(crc), 1, out_file);
        assert_file_closed(&out_file);;
        remove("output_file");
    }

    assert_file_closed(&out_file);
	assert_file_closed(&tmp_input);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}