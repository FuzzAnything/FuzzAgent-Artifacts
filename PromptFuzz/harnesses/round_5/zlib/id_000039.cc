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
//<ID> 57
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 2
//<Quality> {"density":8,"unique_branches":{"inflateInit2_":[[183,9,183,38,0,0,4,1],[191,9,191,36,0,0,4,1]]},"library_calls":["gzopen64","gzopen64","gzdopen","gzclose","deflateInit2_","gzclose","gzclose","inflateInit2_","deflateEnd","gzclose","gzclose","gzwrite","gzclose_w","inflateEnd","inflateInit2_","deflateEnd","gzclose","deflateResetKeep","inflateEnd","deflateEnd","gzclose","deflateGetDictionary","inflateGetHeader","inflate","inflateReset","gzopen64","gzwrite","gzflush","crc32_z","gzclose","gzread","deflateEnd","inflateEnd","gzclose"],"critical_calls":["gzopen64","gzopen64","gzdopen","deflateInit2_","inflateInit2_","gzwrite","gzclose_w","inflateEnd","inflateInit2_","deflateResetKeep","deflateGetDictionary","inflateGetHeader","inflate","inflateReset","gzopen64","gzwrite","gzflush","crc32_z","gzclose","gzread","deflateEnd","inflateEnd","gzclose"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create an output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Variables for zlib operations
    z_stream deflate_stream = {};
    z_stream inflate_stream = {};
    gz_header header = {};
    Bytef dictionary[32768];
    uInt dictLength = 0;
    unsigned int pending;
    int bits;
    long mark;
    int ret;
    uLong crc = 0;

    // 1. Open gzFile for reading using gzopen64
    gzFile gz_in = gzopen64("input_file", "rb");
    if (!gz_in) {
        // Create a dummy input file with the fuzzer data
        FILE *tmp = fopen("input_file", "wb");
        if (tmp) {
            fwrite(data, 1, size, tmp);
            assert_file_closed(&tmp);;
            gz_in = gzopen64("input_file", "rb");
        }
    }

    // 2. Open another gzFile using gzdopen
    gzFile gz_out = gzdopen(out_fd, "wb");
    if (!gz_out) {
        if (gz_in) gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // 3. Initialize deflate with deflateInit2_
    ret = deflateInit2_(&deflate_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                        15 + 16, 8, Z_DEFAULT_STRATEGY,
                        ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        if (gz_in) gzclose(gz_in);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // 4. Initialize inflate with inflateInit2_
    ret = inflateInit2_(&inflate_stream, 15 + 16, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        deflateEnd(&deflate_stream);
        if (gz_in) gzclose(gz_in);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // 5. Write some data to gzFile using gzwrite
    if (size > 0) {
        gzwrite(gz_out, data, (unsigned int)(size > 100 ? 100 : size));
    }

    // 6. Close the writing gzFile with gzclose_w
    gzclose_w(gz_out);

    // 7. Reinitialize inflate with different windowBits
    inflateEnd(&inflate_stream);
    ret = inflateInit2_(&inflate_stream, 15, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        deflateEnd(&deflate_stream);
        if (gz_in) gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // 8. Use deflateResetKeep
    ret = deflateResetKeep(&deflate_stream);
    if (ret != Z_OK) {
        inflateEnd(&inflate_stream);
        deflateEnd(&deflate_stream);
        if (gz_in) gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // 9. Try to get dictionary from deflate stream
    deflateGetDictionary(&deflate_stream, dictionary, &dictLength);

    // 10. Try to get header from inflate stream
    inflateGetHeader(&inflate_stream, &header);

    // 11. Inflate some data
    if (size > 0) {
        Bytef in_buf[1024];
        Bytef out_buf[2048];
        size_t chunk = size > sizeof(in_buf) ? sizeof(in_buf) : size;
        memcpy(in_buf, data, chunk);
        
        inflate_stream.next_in = in_buf;
        inflate_stream.avail_in = (uInt)chunk;
        inflate_stream.next_out = out_buf;
        inflate_stream.avail_out = sizeof(out_buf);
        
        inflate(&inflate_stream, Z_NO_FLUSH);
    }

    // 12. Reset inflate stream
    inflateReset(&inflate_stream);

    // 13. Write more data to a new gzFile
    gzFile gz_out2 = gzopen64("output_file2", "wb");
    if (gz_out2 && size > 100) {
        gzwrite(gz_out2, data + 100, (unsigned int)(size - 100 > 100 ? 100 : size - 100));
    }

    // 14. Flush the gzFile
    if (gz_out2) {
        gzflush(gz_out2, Z_SYNC_FLUSH);
    }

    // 15. Calculate CRC32
    if (size > 0) {
        crc = crc32_z(crc, data, size);
    }

    // 16. Close gzFile
    if (gz_out2) {
        gzclose(gz_out2);
    }

    // 17. Read from gzFile if it was opened
    if (gz_in) {
        Bytef read_buf[1024];
        gzread(gz_in, read_buf, sizeof(read_buf));
    }

    // 18. End deflate stream
    deflateEnd(&deflate_stream);

    // 19. End inflate stream
    inflateEnd(&inflate_stream);

    // Cleanup
    if (gz_in) {
        gzclose(gz_in);
    }
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    remove("output_file2");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}