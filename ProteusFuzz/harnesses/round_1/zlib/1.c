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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

static void create_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f != NULL) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    create_dummy_file(Data, Size);

    gzFile file = NULL;
    char buffer[256];
    int errnum;

    // 1. gzclose (on NULL or invalid to test error handling)
    gzclose(NULL);

    // 2. gzopen
    file = gzopen("./dummy_file", "rb");
    if (file == NULL) {
        return 0;
    }

    // 3. gzread
    int read_bytes = gzread(file, buffer, sizeof(buffer));
    (void)read_bytes;

    // 4. gzerror
    const char *err_msg = gzerror(file, &errnum);
    (void)err_msg;

    // 5. gzseek
    z_off_t seek_res = gzseek(file, 0, SEEK_SET);
    (void)seek_res;

    // 6. gztell
    z_off_t tell_res1 = gztell(file);
    (void)tell_res1;

    // 7. gztell (second call)
    z_off_t tell_res2 = gztell(file);
    (void)tell_res2;

    // 8. gzgetc
    int c = gzgetc(file);
    if (c != -1) {
        // 9. gzungetc
        gzungetc(c, file);
    }

    // 10. gzgets
    char line_buf[128];
    char *line_res = gzgets(file, line_buf, sizeof(line_buf));
    (void)line_res;

    // 11. gzerror
    const char *err_msg2 = gzerror(file, &errnum);
    (void)err_msg2;

    // 12. gzclose
    int close_res = gzclose(file);
    (void)close_res;

    return 0;
}