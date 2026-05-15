// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
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

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF* tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    uint32_t width = 100;
    uint32_t height = 100;
    uint16_t samplesperpixel = 3;
    uint16_t bitspersample = 8;
    uint16_t photometric = PHOTOMETRIC_RGB;
    uint16_t planarconfig = PLANARCONFIG_CONTIG;
    uint16_t compression = COMPRESSION_NONE;
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samplesperpixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bitspersample);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planarconfig);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    
    unsigned char* buf = (unsigned char*)malloc(width * samplesperpixel);
    if (buf) {
        for (uint32_t i = 0; i < Size && i < width * samplesperpixel; i++) {
            buf[i] = Data[i];
        }
        TIFFWriteScanline(tif, buf, 0, 0);
        free(buf);
    }
    
    TIFFWriteDirectory(tif);
    
    TIFFCreateEXIFDirectory(tif);
    
    uint32_t exif_tag = 0x9000;
    const char* exif_version = "0220";
    TIFFSetField(tif, exif_tag, exif_version);
    
    uint64_t offset = 0;
    TIFFWriteCustomDirectory(tif, &offset);
    
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}