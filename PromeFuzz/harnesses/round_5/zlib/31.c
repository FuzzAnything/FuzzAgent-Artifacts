// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzclose_r at gzread.c:645:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzdirect at gzread.c:627:13 in zlib.h
// gzgetc_ at gzread.c:500:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzread at gzread.c:396:13 in zlib.h
// gzrewind at gzlib.c:346:13 in zlib.h
// gzgetc_ at gzread.c:500:13 in zlib.h
// gzdirect at gzread.c:627:13 in zlib.h
// gzread at gzread.c:396:13 in zlib.h
// gzrewind at gzlib.c:346:13 in zlib.h
// gzread at gzread.c:396:13 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzgetc_ at gzread.c:500:13 in zlib.h
// gzgetc_ at gzread.c:500:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzgetc_ at gzread.c:500:13 in zlib.h
// gzread at gzread.c:396:13 in zlib.h
// gzdirect at gzread.c:627:13 in zlib.h
// gzrewind at gzlib.c:346:13 in zlib.h
// gzgetc_ at gzread.c:500:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static gzFile open_dummy_gzfile(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return NULL;
    fwrite(data, 1, size, fp);
    fclose(fp);
    
    return gzopen("./dummy_file", "rb");
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    gzFile file = open_dummy_gzfile(Data, Size);
    if (!file) return 0;
    
    int operation = Data[0] % 10;
    
    switch (operation) {
        case 0: {
            int result = gzdirect(file);
            (void)result;
            break;
        }
        case 1: {
            int byte = gzgetc_(file);
            if (byte != -1) {
                gzungetc(byte, file);
            }
            break;
        }
        case 2: {
            char buf[256];
            unsigned len = (Size > 256) ? 256 : (unsigned)Size;
            int read_bytes = gzread(file, buf, len);
            (void)read_bytes;
            break;
        }
        case 3: {
            gzrewind(file);
            break;
        }
        case 4: {
            int byte = gzgetc_(file);
            (void)byte;
            int direct = gzdirect(file);
            (void)direct;
            break;
        }
        case 5: {
            char buf[128];
            gzread(file, buf, 128);
            gzrewind(file);
            gzread(file, buf, 64);
            break;
        }
        case 6: {
            for (int i = 0; i < 10 && !gzeof(file); i++) {
                gzgetc_(file);
            }
            break;
        }
        case 7: {
            int byte1 = gzgetc_(file);
            if (byte1 != -1) {
                gzungetc(byte1, file);
                int byte2 = gzgetc_(file);
                (void)byte2;
            }
            break;
        }
        case 8: {
            char buf[512];
            gzread(file, buf, 512);
            int direct = gzdirect(file);
            (void)direct;
            gzrewind(file);
            break;
        }
        case 9: {
            int byte = gzgetc_(file);
            if (byte != -1) {
                gzungetc(byte, file);
                gzungetc(byte, file);
            }
            break;
        }
    }
    
    int close_result = gzclose_r(file);
    (void)close_result;
    
    remove("./dummy_file");
    return 0;
}