// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzread at gzread.c:396:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gztell at gzlib.c:461:17 in zlib.h
// gztell at gzlib.c:461:17 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzgets at gzread.c:566:16 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    gzFile file = NULL;
    int errnum = 0;
    const char *error_str = NULL;
    char buffer[1024];
    int len = 1024;

    gzclose(NULL);

    file = gzopen("./dummy_file", "rb");
    if (file == NULL) {
        return 0;
    }

    int bytes_read = gzread(file, buffer, len);

    error_str = gzerror(file, &errnum);

    z_off_t seek_result = gzseek(file, 0, SEEK_SET);

    z_off_t tell_result1 = gztell(file);
    z_off_t tell_result2 = gztell(file);

    int getc_result = gzgetc(file);

    int ungetc_result = gzungetc(getc_result, file);

    char *gets_result = gzgets(file, buffer, len);

    error_str = gzerror(file, &errnum);

    gzclose(file);

    return 0;
}