// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzvprintf at gzwrite.c:403:15 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzbuffer at gzlib.c:322:13 in zlib.h
// gzclose_w at gzwrite.c:667:13 in zlib.h
// gzsetparams at gzwrite.c:630:13 in zlib.h
// gzwrite at gzwrite.c:255:13 in zlib.h
// gzflush at gzwrite.c:603:13 in zlib.h
// gzclose_w at gzwrite.c:667:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

static int gzvprintf_wrapper(gzFile file, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int ret = gzvprintf(file, format, args);
    va_end(args);
    return ret;
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    gzFile file = gzopen("./dummy_file", "wb");
    if (!file) return 0;
    
    unsigned char *buffer = NULL;
    unsigned buffer_size = 0;
    int flush_mode = 0;
    int level = 0;
    int strategy = 0;
    
    size_t offset = 0;
    
    if (Size >= 1) {
        buffer_size = (Data[offset] % 256) + 1;
        offset++;
        if (gzbuffer(file, buffer_size) != Z_OK) {
            gzclose_w(file);
            return 0;
        }
    }
    
    if (Size - offset >= 1) {
        level = Data[offset] % 10;
        offset++;
        if (Size - offset >= 1) {
            strategy = Data[offset] % 4;
            offset++;
            gzsetparams(file, level, strategy);
        }
    }
    
    if (Size - offset >= 1) {
        unsigned write_len = Data[offset] % 256;
        offset++;
        if (write_len > 0 && Size - offset >= write_len) {
            buffer = (unsigned char *)Data + offset;
            gzwrite(file, buffer, write_len);
            offset += write_len;
        }
    }
    
    if (Size - offset >= 1) {
        flush_mode = Data[offset] % 4;
        offset++;
        gzflush(file, flush_mode);
    }
    
    if (Size - offset >= 1) {
        int value = Data[offset];
        offset++;
        gzvprintf_wrapper(file, "Test value: %d\n", value);
    }
    
    gzclose_w(file);
    
    return 0;
}