// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// gzgets at gzread.c:566:16 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/zlib.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void create_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    create_dummy_file(Data, Size);
    
    gzFile file = gzopen("./dummy_file", "rb");
    if (!file) return 0;
    
    int op = Data[0] % 6;
    
    switch (op) {
        case 0: {
            if (Size >= 9) {
                z_off_t offset = (Data[1] << 24) | (Data[2] << 16) | (Data[3] << 8) | Data[4];
                int whence = Data[5] % 3;
                gzseek(file, offset, whence);
            }
            break;
        }
        case 1: {
            int errnum = 0;
            const char *errmsg = gzerror(file, &errnum);
            (void)errmsg;
            break;
        }
        case 2: {
            if (Size >= 2) {
                int c = Data[1];
                if (c != -1) {
                    gzungetc(c, file);
                }
            }
            break;
        }
        case 3: {
            if (Size >= 2) {
                int errcode = Data[1];
                const char *errstr = zError(errcode);
                (void)errstr;
            }
            break;
        }
        case 4: {
            if (Size >= 3) {
                int len = Data[1] % 256 + 1;
                char *buf = (char *)malloc(len);
                if (buf) {
                    gzgets(file, buf, len);
                    free(buf);
                }
            }
            break;
        }
        case 5: {
            gzFile write_file = gzopen("./dummy_file", "ab");
            if (write_file) {
                if (Size >= 10) {
                    char format[10];
                    memcpy(format, Data + 1, 9);
                    format[9] = '\0';
                    int value = Data[10];
                    gzprintf(write_file, format, value);
                }
                gzclose(write_file);
            }
            break;
        }
    }
    
    gzclose(file);
    return 0;
}