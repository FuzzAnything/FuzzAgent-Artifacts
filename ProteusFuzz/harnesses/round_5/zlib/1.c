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
#include <zlib.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    const char *filename = "./dummy_file";
    gzFile file = NULL;
    char buffer[1024];
    int errnum;

    // Write input data to the dummy file
    FILE *fp = fopen(filename, "wb");
    if (fp != NULL) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }

    // 1. gzclose (on NULL or uninitialized, expected to fail safely)
    gzclose(NULL);

    // 2. gzopen
    file = gzopen(filename, "rb");
    if (file == NULL) {
        return 0;
    }

    // 3. gzread
    unsigned len = (Size > 0) ? (unsigned)Size : 0;
    if (len > 1024) len = 1024;
    if (len > 0) {
        gzread(file, buffer, len);
    }

    // 4. gzerror
    gzerror(file, &errnum);

    // 5. gzseek
    z_off_t offset = (Size > 0) ? (z_off_t)Data[0] : 0;
    gzseek(file, offset, SEEK_SET);

    // 6. gztell
    gztell(file);

    // 7. gztell (again)
    gztell(file);

    // 8. gzgetc
    if (Size > 0) {
        gzgetc(file);
    }

    // 9. gzungetc
    if (Size > 0) {
        gzungetc(Data[0], file);
    }

    // 10. gzgets
    if (Size > 0) {
        gzgets(file, buffer, 1024);
    }

    // 11. gzerror
    gzerror(file, &errnum);

    // 12. gzclose
    gzclose(file);

    // Cleanup: remove dummy file
    remove(filename);

    return 0;
}