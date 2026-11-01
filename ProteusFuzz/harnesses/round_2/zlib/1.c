// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzread at gzread.c:396:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzseek at gzlib.c:438:17 in zlib.h
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

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    const char *filename = "./dummy_file";
    gzFile file = NULL;
    int errnum;
    char buf[4096];
    int ret;
    z_off_t offset;
    int seek_mode;
    int c;

    // Step 1: Prepare the environment
    // Write fuzz data to the dummy file to ensure it exists and has content
    FILE *fp = fopen(filename, "wb");
    if (fp != NULL) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Initial gzclose on NULL or invalid handle (safe per spec)
    gzclose(NULL);

    // Open the file for reading
    file = gzopen(filename, "rb");
    if (file == NULL) {
        unlink(filename);
        return 0;
    }

    // gzread: Read some data into buffer
    if (Size > 0) {
        unsigned len = (Size > sizeof(buf)) ? sizeof(buf) : Size;
        gzread(file, buf, len);
    }

    // gzerror: Check for errors after read
    gzerror(file, &errnum);

    // gzseek: Seek to a position derived from input
    // Ensure we have enough data before accessing Data[0] and Data[1]
    if (Size >= 2) {
        offset = (z_off_t)(Data[0] % 1024);
        seek_mode = (Data[1] % 3); // 0: SEEK_SET, 1: SEEK_CUR, 2: SEEK_END
        gzseek(file, offset, seek_mode);
    } else if (Size == 1) {
        // Fallback if only 1 byte is available
        offset = (z_off_t)(Data[0] % 1024);
        seek_mode = 0; // Default to SEEK_SET
        gzseek(file, offset, seek_mode);
    }

    // gztell: Get current position
    gztell(file);

    // gztell: Call again to explore state
    gztell(file);

    // gzgetc: Read a single byte
    c = gzgetc(file);

    // gzungetc: Push the byte back if valid
    if (c != -1) {
        gzungetc(c, file);
    }

    // gzgets: Read a line
    gzgets(file, buf, sizeof(buf));

    // gzerror: Check for errors after operations
    gzerror(file, &errnum);

    // Final gzclose: Close the file
    ret = gzclose(file);

    // Cleanup: Remove the dummy file
    unlink(filename);

    return 0;
}