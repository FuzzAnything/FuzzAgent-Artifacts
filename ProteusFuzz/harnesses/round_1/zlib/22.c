// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateBackInit_ at infback.c:25:13 in zlib.h
// inflateBackEnd at infback.c:572:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// inflateBackEnd at infback.c:572:13 in zlib.h
// inflateBackEnd at infback.c:572:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    z_stream strm = {0};
    unsigned char window[32768];
    int ret;
    int windowBits;
    int bits;
    int value;

    // Parse input to determine which API to test and parameters
    int mode = Data[0] % 6;
    size_t index = 1;

    if (Size > index) {
        windowBits = Data[index++] % 16 + 8; 
    } else {
        windowBits = 15;
    }

    if (Size > index) {
        bits = Data[index++] % 17;
    } else {
        bits = 10;
    }

    // Fixed: Ensure we have at least 2 bytes for value parsing
    if (Size > index + 1) {
        value = (Data[index++] << 8) | (Data[index++] % 256);
    } else {
        value = 0;
        // Consume remaining bytes if any to keep index consistent, though not strictly needed for logic
        if (Size > index) index++; 
    }

    // Prepare input/output buffers from fuzz data
    uint8_t *input_buf = NULL;
    uint8_t *output_buf = NULL;
    size_t input_size = 0;
    size_t output_size = 0;

    if (Size > index) {
        input_size = Size - index;
        input_buf = (uint8_t *)malloc(input_size);
        if (input_buf) {
            memcpy(input_buf, &Data[index], input_size);
        }
    }

    // Allocate output buffer (heuristic size)
    if (input_size > 0) {
        output_size = input_size * 10 + 1024;
        output_buf = (uint8_t *)malloc(output_size);
    }

    switch (mode) {
        case 0: {
            // Test inflateInit2_
            strm.next_in = NULL;
            strm.avail_in = 0;
            strm.total_in = 0;
            strm.next_out = NULL;
            strm.avail_out = 0;
            strm.total_out = 0;
            strm.msg = NULL;
            strm.zalloc = Z_NULL;
            strm.zfree = Z_NULL;
            strm.opaque = Z_NULL;
            strm.data_type = 0;
            strm.adler = 0;
            strm.reserved = 0;

            // Clamp windowBits for inflateInit2_ (typically -15 to 15, or 16-45 for raw)
            int wbits = (windowBits % 31) - 15; 
            if (wbits < -15) wbits = -15;
            if (wbits > 15) wbits = 15;

            ret = inflateInit2_(&strm, wbits, ZLIB_VERSION, (int)sizeof(z_stream));
            if (ret == Z_OK) {
                // Try to do something with the stream to exercise state
                if (input_buf && input_size > 0) {
                    strm.next_in = input_buf;
                    strm.avail_in = input_size;
                    if (output_buf) {
                        strm.next_out = output_buf;
                        strm.avail_out = output_size;
                        inflate(&strm, Z_NO_FLUSH);
                    }
                }
                inflateEnd(&strm);
            }
            break;
        }
        case 1: {
            // Test inflateInit_
            strm.next_in = NULL;
            strm.avail_in = 0;
            strm.total_in = 0;
            strm.next_out = NULL;
            strm.avail_out = 0;
            strm.total_out = 0;
            strm.msg = NULL;
            strm.zalloc = Z_NULL;
            strm.zfree = Z_NULL;
            strm.opaque = Z_NULL;
            strm.data_type = 0;
            strm.adler = 0;
            strm.reserved = 0;

            ret = inflateInit_(&strm, ZLIB_VERSION, (int)sizeof(z_stream));
            if (ret == Z_OK) {
                if (input_buf && input_size > 0) {
                    strm.next_in = input_buf;
                    strm.avail_in = input_size;
                    if (output_buf) {
                        strm.next_out = output_buf;
                        strm.avail_out = output_size;
                        inflate(&strm, Z_NO_FLUSH);
                    }
                }
                inflateEnd(&strm);
            }
            break;
        }
        case 2: {
            // Test inflateBackInit_ and inflateBackEnd
            strm.next_in = NULL;
            strm.avail_in = 0;
            strm.total_in = 0;
            strm.next_out = NULL;
            strm.avail_out = 0;
            strm.total_out = 0;
            strm.msg = NULL;
            strm.zalloc = Z_NULL;
            strm.zfree = Z_NULL;
            strm.opaque = Z_NULL;
            strm.data_type = 0;
            strm.adler = 0;
            strm.reserved = 0;

            // windowBits for inflateBack must be 8-15
            int wbits = (windowBits % 8) + 8; 

            ret = inflateBackInit_(&strm, wbits, window, ZLIB_VERSION, (int)sizeof(z_stream));
            if (ret == Z_OK) {
                // Minimal exercise of inflateBack state
                // Note: inflateBack requires specific I/O callbacks, skipping full execution
                // to avoid complex callback setup, just testing init/end cycle.
                inflateBackEnd(&strm);
            }
            break;
        }
        case 3: {
            // Test inflatePrime
            strm.next_in = NULL;
            strm.avail_in = 0;
            strm.total_in = 0;
            strm.next_out = NULL;
            strm.avail_out = 0;
            strm.total_out = 0;
            strm.msg = NULL;
            strm.zalloc = Z_NULL;
            strm.zfree = Z_NULL;
            strm.opaque = Z_NULL;
            strm.data_type = 0;
            strm.adler = 0;
            strm.reserved = 0;

            // Initialize first
            ret = inflateInit_(&strm, ZLIB_VERSION, (int)sizeof(z_stream));
            if (ret == Z_OK) {
                // bits can be negative to clear
                int b = bits;
                if (b > 16) b = 16;
                if (b < -16) b = -16;
                
                ret = inflatePrime(&strm, b, value);
                inflateEnd(&strm);
            }
            break;
        }
        case 4: {
            // Test zError with various codes derived from input
            int err_code = Data[0] % 100 - 50; // Range -50 to 49
            const char *msg = zError(err_code);
            (void)msg; // Suppress unused warning
            break;
        }
        case 5: {
            // Test inflateBackEnd on uninitialized or null stream
            z_stream *null_strm = NULL;
            ret = inflateBackEnd(null_strm);
            (void)ret;

            // Test on partially initialized stream (no state)
            z_stream dummy_strm = {0};
            ret = inflateBackEnd(&dummy_strm);
            (void)ret;
            break;
        }
        default:
            break;
    }

    if (input_buf) free(input_buf);
    if (output_buf) free(output_buf);

    return 0;
}