// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFMergeFieldInfo at tif_dirinfo.c:1147:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFCreateGPSDirectory at tif_dir.c:1742:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <tiffio.h>

static void dummy_error_handler(void *client_data, const char *module, const char *fmt, va_list ap) {
    (void)client_data;
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warn_handler(void *client_data, const char *module, const char *fmt, va_list ap) {
    (void)client_data;
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    TIFF *tif = TIFFOpen(filename, "w8");
    if (!tif) {
        unlink(filename);
        return 0;
    }

    TIFFSetErrorHandlerExt(dummy_error_handler);
    TIFFSetWarningHandlerExt(dummy_warn_handler);

    static const TIFFFieldInfo custom_fields[] = {
        { 0xFFFF, 1, 1, TIFF_SHORT, 0, 1, 1, "CustomTag", 0 },
        { 0xFFFE, 1, 1, TIFF_LONG, 0, 1, 1, "CustomTag2", 0 },
    };
    uint32_t custom_count = sizeof(custom_fields) / sizeof(custom_fields[0]);

    if (TIFFMergeFieldInfo(tif, custom_fields, custom_count) != -1) {
        // Custom fields registered
    }

    if (TIFFCreateEXIFDirectory(tif)) {
        TIFFWriteDirectory(tif);
    }

    TIFFFreeDirectory(tif);
    if (TIFFCreateGPSDirectory(tif)) {
        TIFFWriteDirectory(tif);
    }

    TIFFFreeDirectory(tif);
    TIFFWriteDirectory(tif);

    TIFFFreeDirectory(tif);

    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 64);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 64);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    
    TIFFWriteDirectory(tif);

    TIFFClose(tif);
    unlink(filename);

    return 0;
}