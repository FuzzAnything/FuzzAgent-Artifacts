// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFdOpen at tif_unix.c:209:7 in tiffio.h
// TIFFFdOpen at tif_unix.c:209:7 in tiffio.h
// TIFFGetMode at tif_open.c:843:5 in tiffio.h
// TIFFSetMode at tif_open.c:848:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFReadScanline at tif_read.c:454:5 in tiffio.h
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
#include <tiffio.h>

static void cleanup_tiff(TIFF *tif) {
    if (tif != NULL) {
        TIFFClose(tif);
    }
}

int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a temporary file to hold the fuzzed data
    const char *filename = "./dummy_file";
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }

    // Write the fuzzed data to the file
    if (write(fd, Data, Size) != (ssize_t)Size) {
        close(fd);
        return 0;
    }

    // Seek back to the beginning
    if (lseek(fd, 0, SEEK_SET) == -1) {
        close(fd);
        return 0;
    }

    // Try opening the file in different modes
    TIFF *tif = TIFFFdOpen(fd, filename, "r");
    if (tif == NULL) {
        // Try opening in write mode if read failed
        close(fd);
        fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
            return 0;
        }
        // Write some minimal valid TIFF header if needed, but for fuzzing
        // we just pass the raw data which might be invalid.
        if (write(fd, Data, Size) != (ssize_t)Size) {
            close(fd);
            return 0;
        }
        lseek(fd, 0, SEEK_SET);
        tif = TIFFFdOpen(fd, filename, "w");
    }

    if (tif == NULL) {
        close(fd);
        unlink(filename);
        return 0;
    }

    // Fuzz TIFFGetMode
    int mode = TIFFGetMode(tif);
    (void)mode; // Use the value to avoid unused warning

    // Fuzz TIFFSetMode with various modes
    int modes[] = {O_RDONLY, O_WRONLY, O_RDWR, 0};
    for (int i = 0; i < sizeof(modes) / sizeof(modes[0]); i++) {
        TIFFSetMode(tif, modes[i]);
    }

    // Fuzz TIFFSetDirectory with various directory indices
    tdir_t dirs[] = {0, 1, 10, 100, 0xFFFFFFFF};
    for (int i = 0; i < sizeof(dirs) / sizeof(dirs[0]); i++) {
        TIFFSetDirectory(tif, dirs[i]);
    }

    // Allocate a buffer for scanline operations
    uint8_t *buf = malloc(Size > 0 ? Size : 1);
    if (buf == NULL) {
        cleanup_tiff(tif);
        close(fd);
        unlink(filename);
        return 0;
    }

    // Fuzz TIFFReadScanline
    uint32_t rows[] = {0, 1, 10, 100, 0xFFFFFFFF};
    uint16_t samples[] = {0, 1, 2, 3, 65535};
    for (int i = 0; i < sizeof(rows) / sizeof(rows[0]); i++) {
        for (int j = 0; j < sizeof(samples) / sizeof(samples[0]); j++) {
            TIFFReadScanline(tif, buf, rows[i], samples[j]);
        }
    }

    // Fuzz TIFFWriteScanline (only if opened in write mode)
    if (mode & O_WRONLY || mode & O_RDWR) {
        for (int i = 0; i < sizeof(rows) / sizeof(rows[0]); i++) {
            for (int j = 0; j < sizeof(samples) / sizeof(samples[0]); j++) {
                TIFFWriteScanline(tif, buf, rows[i], samples[j]);
            }
        }
    }

    // Cleanup
    free(buf);
    cleanup_tiff(tif);
    close(fd);
    unlink(filename);

    return 0;
}