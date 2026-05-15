// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFCreateCustomDirectory at tif_dir.c:1704:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadEXIFDirectory at tif_dirread.c:5462:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadCustomDirectory at tif_dirread.c:5278:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
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

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) return 0;
    
    TIFFFreeDirectory(tif);
    
    TIFFCreateCustomDirectory(tif, NULL);
    
    uint32_t width = 100, height = 100;
    uint16_t samples = 3, bits = 8;
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    
    uint64_t offset = 0;
    TIFFWriteCustomDirectory(tif, &offset);
    
    TIFFSetDirectory(tif, 0);
    
    uint32_t rowsperstrip = 10;
    uint16_t compression = COMPRESSION_NONE;
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rowsperstrip);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    
    TIFFClose(tif);
    
    tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    uint32_t get_width = 0, get_height = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &get_width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &get_height);
    
    toff_t exif_offset = 100;
    TIFFReadEXIFDirectory(tif, exif_offset);
    
    uint16_t get_compression = 0;
    TIFFGetField(tif, TIFFTAG_COMPRESSION, &get_compression);
    
    TIFFReadCustomDirectory(tif, exif_offset, NULL);
    
    uint32_t get_rowsperstrip = 0;
    uint16_t get_samples = 0;
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &get_rowsperstrip);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &get_samples);
    
    TIFFClose(tif);
    TIFFClose(tif);
    
    unlink("./dummy_file");
    return 0;
}