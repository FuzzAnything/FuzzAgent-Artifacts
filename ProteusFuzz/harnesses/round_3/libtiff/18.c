// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFdOpen at tif_unix.c:209:7 in tiffio.h
// TIFFGetMode at tif_open.c:843:5 in tiffio.h
// TIFFSetMode at tif_open.c:848:5 in tiffio.h
// TIFFSetMode at tif_open.c:848:5 in tiffio.h
// TIFFSetMode at tif_open.c:848:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFReadScanline at tif_read.c:454:5 in tiffio.h
// TIFFReadScanline at tif_read.c:454:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

static void cleanup_tiff(TIFF *tif) {
    if (tif != NULL) {
        TIFFClose(tif);
    }
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a temporary file
    const char *filename = "./dummy_file";
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }

    // Write fuzzing data to the file
    if (write(fd, Data, Size) != (ssize_t)Size) {
        close(fd);
        return 0;
    }

    // Open the TIFF file using TIFFFdOpen
    TIFF *tif = TIFFFdOpen(fd, filename, "w+");
    if (tif == NULL) {
        close(fd);
        return 0;
    }

    // Test TIFFGetMode
    int mode = TIFFGetMode(tif);
    (void)mode; // Suppress unused variable warning

    // Test TIFFSetMode with various modes
    int prev_mode = TIFFSetMode(tif, O_RDONLY);
    (void)prev_mode;
    prev_mode = TIFFSetMode(tif, O_RDWR);
    (void)prev_mode;
    prev_mode = TIFFSetMode(tif, O_WRONLY);
    (void)prev_mode;

    // Test TIFFCreateEXIFDirectory
    int exif_result = TIFFCreateEXIFDirectory(tif);
    (void)exif_result;

    // Prepare a buffer for scanline operations
    size_t buf_size = (Size > 1024) ? 1024 : Size;
    uint8_t *buf = (uint8_t *)malloc(buf_size);
    if (buf == NULL) {
        cleanup_tiff(tif);
        return 0;
    }
    memcpy(buf, Data, buf_size);

    // Test TIFFWriteScanline with various row indices and sample numbers
    uint32_t row = (Size > 0) ? Data[0] : 0;
    uint16_t sample = (Size > 1) ? (uint16_t)Data[1] : 0;
    int write_result = TIFFWriteScanline(tif, buf, row, sample);
    (void)write_result;

    // Test with different row and sample values
    row = (Size > 2) ? (uint32_t)Data[2] : 0;
    sample = (Size > 3) ? (uint16_t)Data[3] : 0;
    write_result = TIFFWriteScanline(tif, buf, row, sample);
    (void)write_result;

    // Test TIFFReadScanline (may fail if file is write-only or data is invalid)
    int read_result = TIFFReadScanline(tif, buf, row, sample);
    (void)read_result;

    // Test with different row and sample values for reading
    row = (Size > 4) ? (uint32_t)Data[4] : 0;
    sample = (Size > 5) ? (uint16_t)Data[5] : 0;
    read_result = TIFFReadScanline(tif, buf, row, sample);
    (void)read_result;

    // Cleanup
    free(buf);
    cleanup_tiff(tif);
    close(fd);
    unlink(filename);

    return 0;
}