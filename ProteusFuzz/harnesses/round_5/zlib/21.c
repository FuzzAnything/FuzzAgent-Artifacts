// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzbuffer at gzlib.c:322:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzread at gzread.c:396:13 in zlib.h
// gzgets at gzread.c:566:16 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    const char *filename = "./dummy_file";
    
    // Write input data to a temporary file
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;
    ssize_t written = write(fd, Data, Size);
    close(fd);
    if (written < 0) return 0;

    // Open the file for reading
    gzFile file = gzopen(filename, "rb");
    if (file == NULL) {
        unlink(filename);
        return 0;
    }

    // Fuzz gzbuffer: try setting buffer size
    unsigned buffer_size = (Data[0] % 1000) + 8;
    if (buffer_size > 65536) buffer_size = 65536;
    gzbuffer(file, buffer_size);

    // Allocate buffers for reading
    unsigned char *read_buf = (unsigned char *)malloc(Size + 10);
    char *line_buf = (char *)malloc(Size + 10);
    if (read_buf == NULL || line_buf == NULL) {
        gzclose(file);
        free(read_buf);
        free(line_buf);
        unlink(filename);
        return 0;
    }

    // Fuzz gzread
    unsigned len = (Data[1] % (Size > 0 ? Size : 1)) + 1;
    if (len > 65535) len = 65535; // Ensure len fits in int
    int bytes_read = gzread(file, read_buf, len);
    (void)bytes_read; // Use return value to avoid unused warning

    // Fuzz gzgets
    int line_len = (Data[0] % (Size > 0 ? Size : 1)) + 1;
    if (line_len > 1024) line_len = 1024;
    char *line_result = gzgets(file, line_buf, line_len);
    (void)line_result;

    // Fuzz gzgetc
    int c = gzgetc(file);
    if (c != -1) {
        // Fuzz gzungetc: push back the character
        gzungetc(c, file);
    }

    // Fuzz gzseek
    z_off_t offset = (z_off_t)(Data[0] * Data[1]);
    int whence = Data[0] % 3; // SEEK_SET, SEEK_CUR, SEEK_END
    z_off_t pos = gzseek(file, offset, whence);
    (void)pos;

    // Cleanup
    gzclose(file);
    free(read_buf);
    free(line_buf);
    unlink(filename);

    return 0;
}