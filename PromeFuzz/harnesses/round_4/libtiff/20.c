// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCreateGPSDirectory at tif_dir.c:1742:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
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

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }
    
    uint32_t *uint32_vals = (uint32_t *)Data;
    uint16_t *uint16_vals = (uint16_t *)Data;
    float *float_vals = (float *)Data;
    const char *str_vals = (const char *)Data;
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, uint32_vals[0] % 4096 + 1);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, uint32_vals[1] % 4096 + 1);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, uint16_vals[4] % 32 + 1);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, uint16_vals[5] % 10);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, uint16_vals[6] % 6);
    TIFFSetField(tif, TIFFTAG_ORIENTATION, uint16_vals[7] % 8);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, uint16_vals[8] % 5 + 1);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, uint32_vals[4] % 100 + 1);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, uint16_vals[9] % 2);
    TIFFSetField(tif, TIFFTAG_RESOLUTIONUNIT, uint16_vals[10] % 3);
    TIFFSetField(tif, TIFFTAG_XRESOLUTION, float_vals[0] + 1.0f);
    TIFFSetField(tif, TIFFTAG_YRESOLUTION, float_vals[1] + 1.0f);
    TIFFSetField(tif, TIFFTAG_ARTIST, str_vals);
    TIFFSetField(tif, TIFFTAG_DATETIME, str_vals + 10);
    TIFFSetField(tif, TIFFTAG_DOCUMENTNAME, str_vals + 20);
    TIFFSetField(tif, TIFFTAG_MAKE, str_vals + 30);
    TIFFSetField(tif, TIFFTAG_MODEL, str_vals + 40);
    TIFFSetField(tif, TIFFTAG_SOFTWARE, str_vals + 50);
    TIFFSetField(tif, TIFFTAG_IMAGEDESCRIPTION, str_vals + 60);
    TIFFSetField(tif, TIFFTAG_COPYRIGHT, str_vals + 70);
    
    const TIFFField *field = TIFFFindField(tif, uint32_vals[2] % 1000, (TIFFDataType)(uint16_vals[3] % 12));
    
    TIFFSetField(tif, TIFFTAG_SUBFILETYPE, uint32_vals[3] % 4);
    TIFFSetField(tif, TIFFTAG_FILLORDER, uint16_vals[11] % 2);
    
    uint32_t width = uint32_vals[0] % 4096 + 1;
    uint32_t height = uint32_vals[1] % 4096 + 1;
    uint16_t samples = uint16_vals[8] % 5 + 1;
    uint16_t bits = uint16_vals[4] % 32 + 1;
    
    size_t scanline_size = (width * samples * bits + 7) / 8;
    uint8_t *scanline = (uint8_t *)malloc(scanline_size);
    if (scanline) {
        for (size_t i = 0; i < scanline_size && i < Size - 100; i++) {
            scanline[i] = Data[100 + i];
        }
        TIFFWriteScanline(tif, scanline, 0, 0);
        free(scanline);
    }
    
    TIFFWriteDirectory(tif);
    
    TIFFCreateGPSDirectory(tif);
    
    TIFFSetField(tif, 0x0000, (uint8_t)2, (uint8_t)2, (uint8_t)0, (uint8_t)0);
    TIFFSetField(tif, 0x0001, str_vals[80] == 'N' ? "N" : "S");
    
    float lat[3] = {float_vals[2], float_vals[3], float_vals[4]};
    TIFFSetField(tif, 0x0002, 3, lat);
    
    TIFFSetField(tif, 0x0003, str_vals[81] == 'E' ? "E" : "W");
    
    float lon[3] = {float_vals[5], float_vals[6], float_vals[7]};
    TIFFSetField(tif, 0x0004, 3, lon);
    
    TIFFSetField(tif, 0x0005, uint16_vals[12] % 2);
    TIFFSetField(tif, 0x0006, float_vals[8]);
    
    TIFFSetField(tif, 0x0007, 3, float_vals[9], float_vals[10], float_vals[11]);
    TIFFSetField(tif, 0x001d, str_vals + 90);
    TIFFSetField(tif, 0x001b, str_vals + 100);
    TIFFSetField(tif, 0x001c, str_vals + 120);
    TIFFSetField(tif, 0x000b, float_vals[12]);
    TIFFSetField(tif, 0x000c, str_vals[82] == 'K' ? "K" : "M");
    TIFFSetField(tif, 0x000d, float_vals[13]);
    TIFFSetField(tif, 0x000e, str_vals[83] == 'T' ? "T" : "M");
    TIFFSetField(tif, 0x000f, float_vals[14]);
    TIFFSetField(tif, 0x0010, str_vals[84] == 'T' ? "T" : "M");
    TIFFSetField(tif, 0x0011, float_vals[15]);
    TIFFSetField(tif, 0x0012, str_vals + 140);
    TIFFSetField(tif, 0x0013, str_vals[85] == 'N' ? "N" : "S");
    
    float dest_lat[3] = {float_vals[16], float_vals[17], float_vals[18]};
    TIFFSetField(tif, 0x0014, 3, dest_lat);
    
    TIFFSetField(tif, 0x0015, str_vals[86] == 'E' ? "E" : "W");
    
    float dest_lon[3] = {float_vals[19], float_vals[20], float_vals[21]};
    TIFFSetField(tif, 0x0016, 3, dest_lon);
    
    uint64_t offset = 0;
    TIFFWriteCustomDirectory(tif, &offset);
    
    TIFFClose(tif);
    cleanup_dummy_file();
    
    return 0;
}