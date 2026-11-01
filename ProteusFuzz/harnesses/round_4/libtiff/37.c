// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFGetStrileOffsetWithErr at tif_dirread.c:8356:10 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
// TIFFWriteRawStrip at tif_write.c:328:10 in tiffio.h
// TIFFWriteRawTile at tif_write.c:533:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tiffio.h>

static void dummy_error_handler(void *clientdata, const char* module, const char* fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warning_handler(void *clientdata, const char* module, const char* fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    const char *filename = "./dummy_file";
    
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    TIFF *tif = TIFFOpen(filename, "r+");
    if (!tif) {
        tif = TIFFOpen(filename, "w");
        if (!tif) {
            return 0;
        }
        
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 16);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 16);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
    }

    if (!tif) {
        return 0;
    }

    // Set error handlers with the correct signature including clientdata
    TIFFSetErrorHandlerExt(dummy_error_handler);
    TIFFSetWarningHandlerExt(dummy_warning_handler);

    uint32_t strile = 0;
    if (Size >= 4) {
        strile = (uint32_t)Data[0] | ((uint32_t)Data[1] << 8) | 
                 ((uint32_t)Data[2] << 16) | ((uint32_t)Data[3] << 24);
    }

    size_t buf_size = (Size > 0) ? Size : 1;
    uint8_t *buf = (uint8_t *)malloc(buf_size);
    if (!buf) {
        TIFFClose(tif);
        return 0;
    }
    memcpy(buf, Data, buf_size);

    uint64_t byte_count = TIFFGetStrileByteCount(tif, strile);
    (void)byte_count;

    int err = 0;
    uint64_t offset = TIFFGetStrileOffsetWithErr(tif, strile, &err);
    (void)offset;

    tmsize_t read_result = TIFFReadRawStrip(tif, strile, buf, buf_size);
    (void)read_result;

    tmsize_t write_enc_result = TIFFWriteEncodedStrip(tif, strile, buf, buf_size);
    (void)write_enc_result;

    tmsize_t write_raw_result = TIFFWriteRawStrip(tif, strile, buf, buf_size);
    (void)write_raw_result;

    tmsize_t write_tile_result = TIFFWriteRawTile(tif, strile, buf, buf_size);
    (void)write_tile_result;

    free(buf);
    TIFFClose(tif);

    remove(filename);

    return 0;
}