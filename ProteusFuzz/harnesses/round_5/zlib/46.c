// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzopen64 at gzlib.c:293:16 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzfwrite at gzwrite.c:280:18 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzfwrite at gzwrite.c:280:18 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen64 at gzlib.c:293:16 in zlib.h
// gzfwrite at gzwrite.c:280:18 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzfwrite at gzwrite.c:280:18 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Create a temporary file for testing
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }

    // Write input data to the temporary file
    size_t written = fwrite(Data, 1, Size, fp);
    if (written != Size) {
        fclose(fp);
        return 0;
    }
    fclose(fp);

    // Test gzopen and gzopen64
    gzFile file = gzopen(filename, "rb");
    gzFile file64 = gzopen64(filename, "rb");

    // Buffer for reading
    unsigned char read_buf[1024];
    int errnum;

    // Test gzfread on gzopen handle
    if (file) {
        z_size_t count = gzfread(read_buf, 1, Size, file);
        (void)count; // Suppress unused variable warning

        // Test gzerror
        const char *err_str = gzerror(file, &errnum);
        (void)err_str;

        // Test gzclearerr
        gzclearerr(file);

        // Try reading again after clear
        gzfread(read_buf, 1, Size, file);

        gzclose(file);
    }

    // Test gzfread on gzopen64 handle
    if (file64) {
        z_size_t count = gzfread(read_buf, 1, Size, file64);
        (void)count;

        const char *err_str = gzerror(file64, &errnum);
        (void)err_str;

        gzclearerr(file64);
        gzfread(read_buf, 1, Size, file64);

        gzclose(file64);
    }

    // Test writing with gzopen
    file = gzopen(filename, "wb");
    if (file) {
        // Write some data
        z_size_t written_items = gzfwrite(Data, 1, Size, file);
        (void)written_items;

        // Check error status
        const char *err_str = gzerror(file, &errnum);
        (void)err_str;

        // Clear error and try again
        gzclearerr(file);
        gzfwrite(Data, 1, Size, file);

        gzclose(file);
    }

    // Test writing with gzopen64
    file64 = gzopen64(filename, "wb");
    if (file64) {
        z_size_t written_items = gzfwrite(Data, 1, Size, file64);
        (void)written_items;

        const char *err_str = gzerror(file64, &errnum);
        (void)err_str;

        gzclearerr(file64);
        gzfwrite(Data, 1, Size, file64);

        gzclose(file64);
    }

    // Clean up temporary file
    remove(filename);

    return 0;
}