// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReadGPSDirectory at tif_dirread.c:5470:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFLastDirectory at tif_dir.c:2229:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFReadEXIFDirectory at tif_dirread.c:5462:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static tmsize_t dummy_read(thandle_t fd, void *buf, tmsize_t size) {
    (void)fd;
    (void)buf;
    (void)size;
    return 0;
}

static tmsize_t dummy_write(thandle_t fd, void *buf, tmsize_t size) {
    (void)fd;
    (void)buf;
    (void)size;
    return 0;
}

static toff_t dummy_seek(thandle_t fd, toff_t offset, int whence) {
    (void)fd;
    (void)offset;
    (void)whence;
    return 0;
}

static int dummy_close(thandle_t fd) {
    (void)fd;
    return 0;
}

static toff_t dummy_size(thandle_t fd) {
    (void)fd;
    return 0;
}

static int dummy_map(thandle_t fd, void **base, toff_t *size) {
    (void)fd;
    (void)base;
    (void)size;
    return 0;
}

static void dummy_unmap(thandle_t fd, void *base, toff_t size) {
    (void)fd;
    (void)base;
    (void)size;
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file to ensure file-based operations have a target
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Initialize TIFF handle with dummy callbacks
    // TIFFClientOpen requires 10 arguments: name, mode, clientdata, read, write, seek, close, size, map, unmap
    TIFF *tif = TIFFClientOpen("dummy", "r", NULL,
                               dummy_read, dummy_write, dummy_seek,
                               dummy_close, dummy_size,
                               dummy_map, dummy_unmap);

    if (!tif) {
        // If client open fails, try opening the file directly if it exists
        tif = TIFFOpen("./dummy_file", "r");
        if (!tif) {
            return 0;
        }
    }

    // Fuzz TIFFSetSubDirectory
    uint64_t offset1 = 0;
    if (Size >= 8) {
        memcpy(&offset1, Data, 8);
    }
    TIFFSetSubDirectory(tif, offset1);

    // Fuzz TIFFLastDirectory
    (void)TIFFLastDirectory(tif);

    // Fuzz TIFFReadDirectory
    TIFFReadDirectory(tif);
    TIFFReadDirectory(tif);

    // Fuzz TIFFReadEXIFDirectory
    toff_t exif_offset = 0;
    if (Size >= 12) {
        exif_offset = *(uint32_t *)(Data + 8);
    }
    TIFFReadEXIFDirectory(tif, exif_offset);

    // Fuzz TIFFReadGPSDirectory
    toff_t gps_offset = 0;
    if (Size >= 16) {
        gps_offset = *(uint32_t *)(Data + 12);
    }
    TIFFReadGPSDirectory(tif, gps_offset);

    // Cleanup
    if (tif) {
        TIFFClose(tif);
    }

    return 0;
}