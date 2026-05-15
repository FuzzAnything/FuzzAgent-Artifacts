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
//<ID> 82
//<Prompt> []
/*<Combination>: [
*/
//<score> 16.666666, nr_unique_branch: 24
//<Quality> {"density":10,"unique_branches":{"gz_skip":[[287,13,287,26,0,0,4,0],[289,17,289,55,0,0,4,0],[298,18,298,28,0,0,4,1],[304,17,304,38,0,0,4,1],[307,14,307,25,0,0,4,0],[307,14,307,25,0,0,4,1]],"gz_read":[[327,9,327,20,0,0,4,0],[327,24,327,44,0,0,4,1]],"inflateBack":[[404,17,404,20,0,0,4,1],[413,17,413,20,0,0,4,1],[437,28,437,49,0,0,4,0],[442,25,442,66,0,0,4,0],[442,25,442,66,0,0,4,1],[491,17,491,38,0,0,4,0],[496,25,496,66,0,0,4,0],[496,25,496,66,0,0,4,1],[101,13,101,22,82,0,4,1],[101,13,101,22,86,0,4,1]],"gzseek64":[[380,9,380,27,0,0,4,0],[380,31,380,56,0,0,4,1],[413,13,413,35,0,0,4,1],[416,13,416,23,0,0,4,1],[418,13,418,33,0,0,4,1],[424,38,424,71,0,0,4,0]]},"library_calls":["compressBound_z","compress_z","gzdopen","gzfwrite","gzclose","gzdopen","gzrewind","gzseek","gzseek","gzgetc_","gzgets","gzseek64","gzeof","deflateInit_","deflatePrime","deflateEnd","inflateBackInit_","inflateBack","inflateBackEnd","crc32","crc32","crc32_combine_gen","gzclose"],"critical_calls":["compressBound_z","compress_z","gzdopen","gzfwrite","gzclose","gzdopen","gzrewind","gzseek","gzseek","gzgetc_","gzgets","gzseek64","gzeof","deflateInit_","deflatePrime","deflateEnd","inflateBackInit_","inflateBack","inflateBackEnd","crc32","crc32","crc32_combine_gen","gzclose"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Step 1: Recompress the input data using compress_z
    z_size_t bound = compressBound_z(size);
    Bytef *dest = (Bytef *)malloc(bound);
    if (!dest) {
        return 0;
    }
    z_size_t destLen = bound;
    int ret = compress_z(dest, &destLen, (const Bytef *)data, size);
    if (ret != Z_OK) {
        free(dest);
        return 0;
    }

    // Step 2: Write recompressed data to a temporary gzip file using gzdopen and gzfwrite
    FILE *tmp_file = fopen("temp.gz", "wb");
    if (!tmp_file) {
        free(dest);
        assert_file_closed(&tmp_file);
	return 0;
    }
    int fd_out = fuzz_fileno(tmp_file);
    int fd_out_dup = dup(fd_out);
    gzFile gz_out = gzdopen(fd_out_dup, "wb");
    assert_file_closed(&tmp_file);;  // Close original FILE*, fd_out_dup remains open
    if (!gz_out) {
        close(fd_out_dup);
        free(dest);
        assert_file_closed(&tmp_file);
	assert_fd_closed(fd_out);
	return 0;
    }
    z_size_t items_written = gzfwrite(dest, 1, destLen, gz_out);
    (void)items_written;  // Avoid unused variable warning
    gzclose(gz_out);  // Closes fd_out_dup

    // Step 3: Open the same file for reading with gzdopen and perform gzFile operations
    FILE *tmp_in = fopen("temp.gz", "rb");
    if (!tmp_in) {
        free(dest);
        assert_file_closed(&tmp_in);
	assert_file_closed(&tmp_file);
	assert_fd_closed(fd_out);
	return 0;
    }
    int fd_in = fuzz_fileno(tmp_in);
    int fd_in_dup = dup(fd_in);
    gzFile gz_in = gzdopen(fd_in_dup, "rb");
    assert_file_closed(&tmp_in);;  // Close original FILE*, fd_in_dup remains open
    if (!gz_in) {
        close(fd_in_dup);
        free(dest);
        assert_file_closed(&tmp_in);
	assert_file_closed(&tmp_file);
	assert_fd_closed(fd_in);
	assert_fd_closed(fd_out);
	return 0;
    }

    // Perform various gzFile operations
    gzrewind(gz_in);
    off_t offset = gzseek(gz_in, 10, SEEK_CUR);  // Move forward 10 bytes
    (void)offset;
    offset = gzseek(gz_in, -5, SEEK_CUR);  // Move back 5 bytes
    (void)offset;
    int c = gzgetc_(gz_in);  // Read one character
    (void)c;
    char buf[100];
    char *str = gzgets(gz_in, buf, sizeof(buf));  // Read a string
    (void)str;
    off64_t offset64 = gzseek64(gz_in, 20, SEEK_SET);  // Seek to absolute position 20
    (void)offset64;
    int is_eof = gzeof(gz_in);  // Check for EOF
    (void)is_eof;

    // Step 4: Use deflate functions (deflateInit_, deflatePrime, deflateEnd)
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    ret = deflateInit_(&deflate_strm, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, sizeof(z_stream));
    if (ret == Z_OK) {
        // Prime the deflate stream with the first 8 bits of input data (if available)
        if (size >= 1) {
            deflatePrime(&deflate_strm, 8, data[0]);
        }
        deflateEnd(&deflate_strm);
    }

    // Step 5: Use inflateBack to decompress the recompressed data (skipping zlib header)
    if (destLen >= 2) {  // Skip 2-byte zlib header
        // Define callback structures
        struct in_desc {
            const Bytef *data;
            z_size_t size;
            z_size_t pos;
        };
        in_desc idesc = {dest + 2, destLen - 2, 0};

        // Input callback for inflateBack
        auto in_callback = [](void *desc, unsigned char **buf) -> unsigned int {
            in_desc *d = (in_desc *)desc;
            if (d->pos >= d->size) {
                return 0;  // No more data
            }
            *buf = (unsigned char *)(d->data + d->pos);
            unsigned int avail = d->size - d->pos;
            d->pos = d->size;  // Consume all data
            return avail;
        };

        // Output callback for inflateBack (discard data)
        auto out_callback = [](void * /* desc */, unsigned char * /* buf */, unsigned int /* len */) -> int {
            return 0;  // Success, discard output
        };

        z_stream inflate_strm;
        memset(&inflate_strm, 0, sizeof(inflate_strm));
        unsigned char window[1 << 15];  // 32K window
        int windowBits = 15;
        ret = inflateBackInit_(&inflate_strm, windowBits, window, ZLIB_VERSION, sizeof(z_stream));
        if (ret == Z_OK) {
            ret = inflateBack(&inflate_strm, in_callback, &idesc, out_callback, nullptr);
            inflateBackEnd(&inflate_strm);
        }
    }

    // Step 6: Use crc32_combine_gen
    if (size >= 2) {
        // Compute two CRCs from first and second half of input data
        uLong crc1 = crc32(0L, data, size / 2);
        uLong crc2 = crc32(0L, data + size / 2, size - size / 2);
        off_t delta = size - size / 2;  // Distance between the two data segments
        uLong comb_gen = crc32_combine_gen(delta);
        (void)comb_gen;
    }

    // Cleanup
    gzclose(gz_in);  // Closes fd_in_dup
    free(dest);
    remove("temp.gz");

    assert_file_closed(&tmp_in);
	assert_file_closed(&tmp_file);
	assert_fd_closed(fd_in);
	assert_fd_closed(fd_out);
	return 0;
}