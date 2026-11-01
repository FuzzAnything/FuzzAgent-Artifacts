// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetMode at tif_open.c:848:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
// TIFFReadScanline at tif_read.c:454:5 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFdOpen at tif_unix.c:209:7 in tiffio.h
// TIFFGetMode at tif_open.c:843:5 in tiffio.h
// TIFFSetMode at tif_open.c:848:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <tiffio.h>

static int create_dummy_tiff_file(const char *filename, const uint8_t *data, size_t size) {
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return -1;
    }

    // Write a minimal valid TIFF header (Little Endian)
    // Offset 0-1: II (Little Endian)
    // Offset 2-3: 42 (Magic number)
    // Offset 4-7: 8 (Offset to first IFD)
    uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
    
    if (write(fd, header, sizeof(header)) != sizeof(header)) {
        close(fd);
        return -1;
    }

    // Write a minimal IFD at offset 8
    // Number of entries: 1 (0x0001)
    // Tag: ImageWidth (0x0100), Type: SHORT (0x0003), Count: 1 (0x0001), Value: 1 (0x0001)
    // Next IFD: 0 (0x00000000)
    uint8_t ifd[12] = {
        0x01, 0x00, // Count
        0x00, 0x01, // Tag (ImageWidth)
        0x03, 0x00, // Type (SHORT)
        0x01, 0x00, 0x00, 0x00, // Count
        0x01, 0x00, 0x00, 0x00, // Value (Width = 1)
        0x00, 0x00, 0x00, 0x00  // Next IFD
    };

    if (write(fd, ifd, sizeof(ifd)) != sizeof(ifd)) {
        close(fd);
        return -1;
    }

    // Write some dummy image data
    if (size > 0) {
        if (write(fd, data, size) != (ssize_t)size) {
            close(fd);
            return -1;
        }
    }

    close(fd);
    return 0;
}

static void cleanup_tiff(TIFF *tif) {
    if (tif != NULL) {
        TIFFClose(tif);
    }
}

int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    const char *filename = "./dummy_file";
    
    // Remove existing file if present to ensure clean state
    unlink(filename);

    // Create a dummy TIFF file
    if (create_dummy_tiff_file(filename, Data, Size) != 0) {
        return 0;
    }

    // Open the file descriptor
    int fd = open(filename, O_RDWR);
    if (fd < 0) {
        return 0;
    }

    // Open TIFF using TIFFFdOpen
    TIFF *tif = TIFFFdOpen(fd, filename, "r+");
    if (tif == NULL) {
        close(fd);
        return 0;
    }

    // Fuzzing logic
    // 1. Test TIFFGetMode
    int mode = TIFFGetMode(tif);
    (void)mode; // Use the return value to avoid unused variable warnings

    // 2. Test TIFFSetMode with various modes
    // O_RDONLY, O_RDWR, O_WRONLY
    int prev_mode = TIFFSetMode(tif, O_RDONLY);
    (void)prev_mode;
    prev_mode = TIFFSetMode(tif, O_RDWR);
    (void)prev_mode;

    // 3. Test TIFFSetDirectory
    // Try setting directory 0 (usually the first one)
    TIFFSetDirectory(tif, 0);
    // Try setting a potentially non-existent directory (fuzzing edge case)
    if (Size > 0) {
        uint32_t dir_index = Data[0];
        TIFFSetDirectory(tif, dir_index);
    }

    // 4. Test TIFFReadScanline
    // Allocate a buffer for reading
    uint8_t *read_buf = NULL;
    tmsize_t scanline_size = TIFFScanlineSize(tif);
    if (scanline_size > 0 && scanline_size < (1024 * 1024)) { // Limit allocation size
        read_buf = (uint8_t *)malloc(scanline_size);
        if (read_buf != NULL) {
            // Try reading different rows
            uint32_t row = 0;
            if (Size > 1) {
                row = (uint32_t)Data[1];
            }
            // Try reading with different sample numbers
            uint16_t sample = 0;
            if (Size > 2) {
                sample = (uint16_t)Data[2];
            }
            TIFFReadScanline(tif, read_buf, row, sample);
            free(read_buf);
        }
    }

    // 5. Test TIFFWriteScanline
    // Allocate a buffer for writing
    uint8_t *write_buf = NULL;
    if (scanline_size > 0 && scanline_size < (1024 * 1024)) {
        write_buf = (uint8_t *)malloc(scanline_size);
        if (write_buf != NULL) {
            memset(write_buf, 0, scanline_size); // Initialize buffer
            // Try writing to different rows
            uint32_t write_row = 0;
            if (Size > 3) {
                write_row = (uint32_t)Data[3];
            }
            // Try writing with different sample numbers
            uint16_t write_sample = 0;
            if (Size > 4) {
                write_sample = (uint16_t)Data[4];
            }
            TIFFWriteScanline(tif, write_buf, write_row, write_sample);
            free(write_buf);
        }
    }

    // Cleanup
    cleanup_tiff(tif);
    close(fd);
    unlink(filename);

    return 0;
}