// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFGetClientInfo at tif_extension.c:64:7 in tiffio.h
// TIFFPrintDirectory at tif_print.c:244:6 in tiffio.h
// TIFFCleanup at tif_close.c:44:6 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFSetClientdata at tif_open.c:833:11 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static tmsize_t
my_read(thandle_t clientdata, tdata_t buf, tmsize_t size) {
    const uint8_t *data = (const uint8_t *)clientdata;
    size_t len = strlen((const char *)data);
    if (size > len) size = len;
    memcpy(buf, data, size);
    return size;
}

static tmsize_t
my_write(thandle_t clientdata, tdata_t buf, tmsize_t size) {
    (void)clientdata;
    (void)buf;
    (void)size;
    return size;
}

static toff_t
my_seek(thandle_t clientdata, toff_t off, int whence) {
    (void)clientdata;
    (void)off;
    (void)whence;
    return 0;
}

static int
my_close(thandle_t clientdata) {
    (void)clientdata;
    return 0;
}

static toff_t
my_size(thandle_t clientdata) {
    const uint8_t *data = (const uint8_t *)clientdata;
    return strlen((const char *)data);
}

static int
my_map(thandle_t clientdata, tdata_t *buf, toff_t *size) {
    (void)clientdata;
    (void)buf;
    (void)size;
    return 0;
}

static void
my_unmap(thandle_t clientdata, tdata_t buf, toff_t size) {
    (void)clientdata;
    (void)buf;
    (void)size;
}

int
LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure data is null-terminated for string operations if needed
    char *buffer = (char *)malloc(Size + 1);
    if (buffer == NULL) {
        return 0;
    }
    memcpy(buffer, Data, Size);
    buffer[Size] = '\0';

    // Create a dummy file for TIFFOpen if needed, though we use TIFFClientOpenExt
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(buffer, 1, Size, fp);
        fclose(fp);
    }

    // TIFFOpenOptions is an opaque type in newer libtiff versions, so we pass NULL
    TIFFOpenOptions *opts = NULL;

    TIFF *tif = NULL;

    // Attempt to open using custom callbacks
    tif = TIFFClientOpenExt(
        "./dummy_file", "r",
        (thandle_t)buffer,
        my_read, my_write, my_seek, my_close, my_size,
        my_map, my_unmap,
        opts
    );

    if (tif != NULL) {
        // Test TIFFSetClientdata
        thandle_t old_clientdata = TIFFSetClientdata(tif, (thandle_t)0x12345678);
        (void)old_clientdata;

        // Test TIFFClientdata
        thandle_t clientdata = TIFFClientdata(tif);
        (void)clientdata;

        // Test TIFFGetClientInfo (with a dummy key)
        void *info = TIFFGetClientInfo(tif, "fuzz_key");
        (void)info;

        // Test TIFFPrintDirectory
        FILE *stream = fopen("./dummy_file", "w");
        if (stream != NULL) {
            TIFFPrintDirectory(tif, stream, 0L);
            fclose(stream);
        }

        // Cleanup the TIFF instance
        TIFFCleanup(tif);
    }

    free(buffer);
    return 0;
}