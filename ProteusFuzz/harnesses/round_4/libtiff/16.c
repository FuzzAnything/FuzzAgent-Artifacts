// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFSetWriteOffset at tif_write.c:961:6 in tiffio.h
// TIFFSetWriteOffset at tif_write.c:961:6 in tiffio.h
// TIFFCurrentStrip at tif_open.c:874:10 in tiffio.h
// TIFFScanlineSize64 at tif_strip.c:257:10 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFGetStrileOffset at tif_dirread.c:8349:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <tiffio.h>

static void dummy_error_handler(void *clientdata, const char *module, const char *fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warn_handler(void *clientdata, const char *module, const char *fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    
    /* Write input data to a temporary file */
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    /* Open the file for reading and writing */
    TIFF *tif = TIFFOpen(filename, "rw+");
    if (!tif) {
        /* Try opening as read-only if rw+ fails */
        tif = TIFFOpen(filename, "r");
    }
    
    if (!tif) {
        return 0;
    }

    /* Set custom error handlers to suppress output */
    TIFFSetErrorHandlerExt(dummy_error_handler);
    TIFFSetWarningHandlerExt(dummy_warn_handler);

    /* Fuzz TIFFSetWriteOffset */
    if (Size >= sizeof(toff_t)) {
        toff_t off = *(const toff_t *)(Data);
        TIFFSetWriteOffset(tif, off);
    } else {
        /* Use a small offset if data is too short */
        TIFFSetWriteOffset(tif, 0);
    }

    /* Fuzz TIFFCurrentStrip */
    uint32_t curstrip = TIFFCurrentStrip(tif);
    (void)curstrip;

    /* Fuzz TIFFScanlineSize64 */
    uint64_t scanline_size = TIFFScanlineSize64(tif);
    (void)scanline_size;

    /* Fuzz TIFFCurrentDirOffset */
    uint64_t dir_offset = TIFFCurrentDirOffset(tif);
    (void)dir_offset;

    /* Fuzz TIFFWriteScanline */
    if (Size > 0) {
        uint32_t row = 0;
        uint16_t sample = 0;
        if (Size >= sizeof(uint32_t)) {
            row = *(const uint32_t *)(Data);
        }
        if (Size >= sizeof(uint32_t) + sizeof(uint16_t)) {
            sample = *(const uint16_t *)(Data + sizeof(uint32_t));
        }
        /* Write a small portion of data or just a dummy buffer */
        size_t write_len = (Size > 1024) ? 1024 : Size;
        int result = TIFFWriteScanline(tif, (void *)(Data), row, sample);
        (void)result;
    }

    /* Fuzz TIFFGetStrileOffset */
    uint32_t strile = 0;
    if (Size >= sizeof(uint32_t) + sizeof(uint16_t)) {
        strile = *(const uint32_t *)(Data + sizeof(uint32_t) + sizeof(uint16_t));
    }
    uint64_t strile_offset = TIFFGetStrileOffset(tif, strile);
    (void)strile_offset;

    /* Cleanup */
    TIFFClose(tif);

    /* Remove the dummy file */
    unlink(filename);

    return 0;
}