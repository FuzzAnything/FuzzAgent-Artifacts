// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFFlushData at tif_flush.c:146:5 in tiffio.h
// TIFFFlush at tif_flush.c:30:5 in tiffio.h
// TIFFGetCloseProc at tif_open.c:927:15 in tiffio.h
// TIFFCleanup at tif_close.c:44:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

static TIFF* open_test_tiff(const char* mode) {
    return TIFFOpen("./dummy_file", mode);
}

static void setup_test_tags(TIFF* tif) {
    if (tif) {
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 64);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 64);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 16);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    }
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF* tif = open_test_tiff("w");
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }
    
    setup_test_tags(tif);
    
    uint8_t scanline[64];
    size_t scanline_size = sizeof(scanline);
    if (scanline_size > Size) scanline_size = Size;
    memcpy(scanline, Data, scanline_size);
    
    uint32_t row = Data[0] % 64;
    uint16_t sample = Data[0] % 2;
    
    TIFFWriteScanline(tif, scanline, row, sample);
    
    TIFFFlushData(tif);
    TIFFFlush(tif);
    
    TIFFCloseProc close_proc = TIFFGetCloseProc(tif);
    if (close_proc) {
        TIFFCleanup(tif);
    }
    
    TIFFClose(tif);
    cleanup_dummy_file();
    
    return 0;
}