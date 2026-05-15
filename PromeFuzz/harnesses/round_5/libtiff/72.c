// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFFlushData at tif_flush.c:146:5 in tiffio.h
// TIFFGetCloseProc at tif_open.c:927:15 in tiffio.h
// TIFFCleanup at tif_close.c:44:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFFlush at tif_flush.c:30:5 in tiffio.h
// TIFFFlushData at tif_flush.c:146:5 in tiffio.h
// TIFFGetCloseProc at tif_open.c:927:15 in tiffio.h
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
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFFlushData at tif_flush.c:146:5 in tiffio.h
// TIFFFlush at tif_flush.c:30:5 in tiffio.h
// TIFFGetCloseProc at tif_open.c:927:15 in tiffio.h
// TIFFCleanup at tif_close.c:44:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFFlush at tif_flush.c:30:5 in tiffio.h
// TIFFGetCloseProc at tif_open.c:927:15 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
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
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        remove_dummy_file();
        return 0;
    }

    uint8_t mode = Data[0] % 4;
    
    switch (mode) {
        case 0: {
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 100);
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 100);
            TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
            TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
            TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 100);
            TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
            TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
            TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
            
            uint8_t buf[100];
            size_t buf_size = Size > 100 ? 100 : Size;
            memcpy(buf, Data, buf_size);
            
            uint32_t row = (Size > 4) ? *(uint32_t*)Data % 100 : 0;
            uint16_t sample = (Size > 6) ? *(uint16_t*)(Data + 4) % 1 : 0;
            
            TIFFWriteScanline(tif, buf, row, sample);
            TIFFFlushData(tif);
            TIFFFlush(tif);
            TIFFGetCloseProc(tif);
            TIFFCleanup(tif);
            break;
        }
        case 1: {
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 50);
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 50);
            TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
            TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 3);
            TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 50);
            TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
            TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
            TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_LZW);
            
            uint8_t buf[150];
            size_t buf_size = Size > 150 ? 150 : Size;
            memcpy(buf, Data, buf_size);
            
            for (uint32_t row = 0; row < 10; row++) {
                uint16_t sample = row % 3;
                TIFFWriteScanline(tif, buf, row, sample);
            }
            
            TIFFFlush(tif);
            TIFFGetCloseProc(tif);
            TIFFClose(tif);
            break;
        }
        case 2: {
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 200);
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 200);
            TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 16);
            TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
            TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 1);
            TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_SEPARATE);
            
            uint16_t buf[200];
            size_t buf_size = Size > 400 ? 400 : Size;
            memcpy(buf, Data, buf_size);
            
            for (uint32_t row = 0; row < 5; row++) {
                for (uint16_t sample = 0; sample < 1; sample++) {
                    TIFFWriteScanline(tif, buf, row, sample);
                }
            }
            
            TIFFFlushData(tif);
            TIFFGetCloseProc(tif);
            TIFFCleanup(tif);
            break;
        }
        case 3: {
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 10);
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 10);
            TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 32);
            TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 4);
            TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 10);
            TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
            TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_SEPARATED);
            
            float buf[40];
            size_t buf_size = Size > 160 ? 160 : Size;
            memcpy(buf, Data, buf_size);
            
            uint32_t row = (Size > 4) ? *(uint32_t*)Data % 10 : 0;
            uint16_t sample = (Size > 6) ? *(uint16_t*)(Data + 4) % 4 : 0;
            
            TIFFWriteScanline(tif, buf, row, sample);
            TIFFFlush(tif);
            TIFFFlushData(tif);
            TIFFGetCloseProc(tif);
            TIFFClose(tif);
            break;
        }
    }
    
    remove_dummy_file();
    return 0;
}