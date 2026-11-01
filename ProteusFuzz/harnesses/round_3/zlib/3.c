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
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    
    // Write raw data as is, or attempt to write valid gzip if possible
    // For fuzzing, we write the raw fuzz input to the file
    if (size > 0) {
        ssize_t written = write(fd, data, size);
        if (written != (ssize_t)size) {
            close(fd);
            return -1;
        }
    }
    
    close(fd);
    return 0;
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    // Ensure we have some data to work with
    if (Size == 0) {
        return 0;
    }

    // Write the input data to a dummy file
    if (write_dummy_file(Data, Size) != 0) {
        return 0;
    }

    // 1. gzclose (Initial cleanup call, expected to fail or be no-op on NULL)
    // The sequence requires calling gzclose first. Passing NULL is safe per docs.
    gzclose(NULL);

    // 2. gzopen
    gzFile file = gzopen("./dummy_file", "rb");
    if (file == NULL) {
        // If we can't open, we can't proceed with the sequence
        return 0;
    }

    // 3. gzread
    unsigned char read_buf[256];
    unsigned int read_len = (Size > 256) ? 256 : Size;
    gzread(file, read_buf, read_len);

    // 4. gzerror
    int errnum = 0;
    gzerror(file, &errnum);

    // 5. gzseek
    // Try seeking to the beginning or a small offset
    gzseek(file, 0, SEEK_SET);

    // 6. gztell
    gztell(file);

    // 7. gztell (Second call as per sequence)
    gztell(file);

    // 8. gzgetc
    int c = gzgetc(file);
    if (c == -1) {
        // If EOF or error, we can't ungetc a valid character easily, 
        // but we can try ungetc with a dummy value or skip.
        // The sequence requires gzungetc. Let's try with a dummy char 'A'.
        c = 'A'; 
    }

    // 9. gzungetc
    // Push the character back. If file is empty or error, this might fail, but we try.
    gzungetc(c, file);

    // 10. gzgets
    char line_buf[256];
    gzgets(file, line_buf, sizeof(line_buf));

    // 11. gzerror
    gzerror(file, &errnum);

    // 12. gzclose
    gzclose(file);

    return 0;
}