// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateSetHeader at deflate.c:714:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateReset at inflate.c:125:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    z_stream strm = {0};
    z_stream strm_copy = {0};
    gz_header header = {0};
    Bytef *out_buf = NULL;
    uInt out_size = 0;

    // Allocate output buffer based on input size to prevent overflow
    if (Size > 0) {
        out_size = Size * 10;
        if (out_size < 1024) out_size = 1024;
        out_buf = (Bytef *)malloc(out_size);
        if (!out_buf) return 0;
    }

    // Determine operation based on first byte
    uint8_t op = Data[0];
    const uint8_t *input_data = Data + 1;
    size_t input_len = Size - 1;

    int ret;

    switch (op % 6) {
        case 0: {
            // Test inflate
            if (inflateInit2(&strm, -15) == Z_OK) { // Raw deflate
                strm.next_in = (z_const Bytef *)input_data;
                strm.avail_in = (uInt)input_len;
                strm.next_out = out_buf;
                strm.avail_out = out_size;

                ret = inflate(&strm, Z_NO_FLUSH);
                while (ret == Z_OK && strm.avail_in > 0) {
                    if (strm.avail_out == 0) {
                        // Resize buffer if needed
                        out_size *= 2;
                        Bytef *new_buf = (Bytef *)realloc(out_buf, out_size);
                        if (!new_buf) {
                            inflateEnd(&strm);
                            free(out_buf);
                            return 0;
                        }
                        out_buf = new_buf;
                        strm.next_out = out_buf + (out_size / 2);
                        strm.avail_out = out_size / 2;
                    }
                    ret = inflate(&strm, Z_NO_FLUSH);
                }
                inflateEnd(&strm);
            }
            break;
        }
        case 1: {
            // Test inflateCopy
            // First initialize a source stream
            if (inflateInit2(&strm, -15) == Z_OK) {
                strm.next_in = (z_const Bytef *)input_data;
                strm.avail_in = (uInt)input_len;
                strm.next_out = out_buf;
                strm.avail_out = out_size;
                
                // Run one step to ensure state is active
                inflate(&strm, Z_NO_FLUSH);

                // Copy the stream
                ret = inflateCopy(&strm_copy, &strm);
                if (ret == Z_OK) {
                    // Try to use the copy
                    strm_copy.next_out = out_buf;
                    strm_copy.avail_out = out_size;
                    inflate(&strm_copy, Z_NO_FLUSH);
                    inflateEnd(&strm_copy);
                }
                inflateEnd(&strm);
            }
            break;
        }
        case 2: {
            // Test deflateSetHeader
            // Initialize deflate stream with GZIP wrap (2)
            if (deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -15 + 16, 8, Z_DEFAULT_STRATEGY) == Z_OK) {
                // Setup header
                header.text = 1;
                header.time = (uLong)input_len;
                header.xflags = 0;
                header.os = 0;
                header.extra = NULL;
                header.extra_len = 0;
                header.extra_max = 0;
                header.name = NULL;
                header.name_max = 0;
                header.comment = NULL;
                header.comm_max = 0;
                header.hcrc = 1;
                header.done = 0;

                ret = deflateSetHeader(&strm, &header);
                
                // Attempt to compress some data to trigger header usage
                strm.next_in = (z_const Bytef *)input_data;
                strm.avail_in = (uInt)input_len;
                strm.next_out = out_buf;
                strm.avail_out = out_size;
                
                deflate(&strm, Z_FINISH);
                deflateEnd(&strm);
            }
            break;
        }
        case 3: {
            // Test deflateReset
            if (deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY) == Z_OK) {
                strm.next_in = (z_const Bytef *)input_data;
                strm.avail_in = (uInt)input_len;
                strm.next_out = out_buf;
                strm.avail_out = out_size;
                
                // Compress a bit
                deflate(&strm, Z_NO_FLUSH);
                
                // Reset
                ret = deflateReset(&strm);
                
                // Try to compress again after reset
                if (ret == Z_OK) {
                    strm.next_in = (z_const Bytef *)input_data;
                    strm.avail_in = (uInt)input_len;
                    deflate(&strm, Z_FINISH);
                }
                deflateEnd(&strm);
            }
            break;
        }
        case 4: {
            // Test inflateGetHeader
            // Initialize with GZIP wrap (15)
            if (inflateInit2(&strm, 15) == Z_OK) {
                ret = inflateGetHeader(&strm, &header);
                
                if (ret == Z_OK) {
                    strm.next_in = (z_const Bytef *)input_data;
                    strm.avail_in = (uInt)input_len;
                    strm.next_out = out_buf;
                    strm.avail_out = out_size;
                    
                    ret = inflate(&strm, Z_NO_FLUSH);
                    // Check if header was populated if data was valid gzip
                }
                inflateEnd(&strm);
            }
            break;
        }
        case 5: {
            // Test inflateReset
            if (inflateInit2(&strm, -15) == Z_OK) {
                strm.next_in = (z_const Bytef *)input_data;
                strm.avail_in = (uInt)input_len;
                strm.next_out = out_buf;
                strm.avail_out = out_size;
                
                // Run some decompression
                inflate(&strm, Z_NO_FLUSH);
                
                // Reset
                ret = inflateReset(&strm);
                
                // Try to decompress again
                if (ret == Z_OK) {
                    strm.next_in = (z_const Bytef *)input_data;
                    strm.avail_in = (uInt)input_len;
                    inflate(&strm, Z_NO_FLUSH);
                }
                inflateEnd(&strm);
            }
            break;
        }
        default:
            break;
    }

    free(out_buf);
    return 0;
}