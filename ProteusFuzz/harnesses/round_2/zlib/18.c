// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzdopen at gzlib.c:298:16 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzfwrite at gzwrite.c:280:18 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzfwrite at gzwrite.c:280:18 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen64 at gzlib.c:293:16 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzfwrite at gzwrite.c:280:18 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <zlib.h>

static void create_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f != NULL) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_file(const char *filename) {
    if (filename != NULL) {
        unlink(filename);
    }
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    const char *filename = "./dummy_file";
    gzFile file = NULL;
    int fd = -1;
    uint8_t *buffer = NULL;
    z_size_t items_written, items_read;
    const char *mode;

    // Create a dummy file with the fuzz input
    create_dummy_file(filename, Data, Size);

    // Test gzopen
    mode = (Data[0] % 2 == 0) ? "rb" : "wb";
    file = gzopen(filename, mode);
    if (file != NULL) {
        // Test gzclearerr
        gzclearerr(file);

        if (mode[0] == 'w') {
            // Test gzfwrite
            buffer = malloc(Size > 0 ? Size : 1);
            if (buffer != NULL) {
                memcpy(buffer, Data, Size);
                items_written = gzfwrite(buffer, 1, Size, file);
                (void)items_written; // Suppress unused variable warning
                free(buffer);
            }
            gzclose(file);
            file = NULL;
        } else {
            // Test gzfread
            buffer = malloc(Size > 0 ? Size : 1);
            if (buffer != NULL) {
                items_read = gzfread(buffer, 1, Size, file);
                (void)items_read; // Suppress unused variable warning
                free(buffer);
            }
            gzclose(file);
            file = NULL;
        }
    }

    // Test gzopen64
    mode = (Data[0] % 2 == 0) ? "rb" : "wb";
    file = gzopen64(filename, mode);
    if (file != NULL) {
        gzclearerr(file);
        if (mode[0] == 'w') {
            buffer = malloc(Size > 0 ? Size : 1);
            if (buffer != NULL) {
                memcpy(buffer, Data, Size);
                items_written = gzfwrite(buffer, 1, Size, file);
                (void)items_written;
                free(buffer);
            }
            gzclose(file);
            file = NULL;
        } else {
            buffer = malloc(Size > 0 ? Size : 1);
            if (buffer != NULL) {
                items_read = gzfread(buffer, 1, Size, file);
                (void)items_read;
                free(buffer);
            }
            gzclose(file);
            file = NULL;
        }
    }

    // Test gzdopen
    fd = open(filename, O_RDONLY);
    if (fd != -1) {
        file = gzdopen(fd, "rb");
        if (file != NULL) {
            gzclearerr(file);
            buffer = malloc(Size > 0 ? Size : 1);
            if (buffer != NULL) {
                items_read = gzfread(buffer, 1, Size, file);
                (void)items_read;
                free(buffer);
            }
            gzclose(file);
            file = NULL;
        }
        close(fd);
        fd = -1;
    }

    // Test with write mode via gzdopen
    fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (fd != -1) {
        file = gzdopen(fd, "wb");
        if (file != NULL) {
            gzclearerr(file);
            buffer = malloc(Size > 0 ? Size : 1);
            if (buffer != NULL) {
                memcpy(buffer, Data, Size);
                items_written = gzfwrite(buffer, 1, Size, file);
                (void)items_written;
                free(buffer);
            }
            gzclose(file);
            file = NULL;
        }
        close(fd);
        fd = -1;
    }

    // Cleanup
    cleanup_file(filename);

    return 0;
}