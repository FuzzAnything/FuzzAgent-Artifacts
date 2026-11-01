// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzputs at gzwrite.c:350:13 in zlib.h
// gzsetparams at gzwrite.c:630:13 in zlib.h
// gzflush at gzwrite.c:603:13 in zlib.h
// gzclose_w at gzwrite.c:667:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzdirect at gzread.c:627:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    FILE *f = fopen(filename, "wb");
    if (!f) {
        return 0;
    }

    fwrite(Data, 1, Size, f);
    fclose(f);

    gzFile gz_write = gzopen(filename, "wb");
    if (!gz_write) {
        remove(filename);
        return 0;
    }

    if (Size > 0) {
        int c = Data[0] & 0xFF;
        gzputc(gz_write, c);
    }

    if (Size > 1) {
        char *str = (char *)malloc(Size + 1);
        if (str) {
            memcpy(str, Data, Size);
            str[Size] = '\0';
            gzputs(gz_write, str);
            free(str);
        }
    }

    int level = (Size % 10) - 1;
    int strategy = (Size % 5);
    gzsetparams(gz_write, level, strategy);

    int flush_mode = (Size % 6);
    gzflush(gz_write, flush_mode);

    gzclose_w(gz_write);

    gzFile gz_read = gzopen(filename, "rb");
    if (gz_read) {
        gzdirect(gz_read);
        gzclose(gz_read);
    }

    remove(filename);

    return 0;
}