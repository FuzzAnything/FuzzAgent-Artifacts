// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFSetClientdata at tif_open.c:833:11 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFGetClientInfo at tif_extension.c:64:7 in tiffio.h
// TIFFPrintDirectory at tif_print.c:244:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static tmsize_t
dummy_readproc(thandle_t clientdata, tdata_t buf, tmsize_t size) {
    (void)clientdata;
    (void)buf;
    (void)size;
    return 0;
}

static tmsize_t
dummy_writeproc(thandle_t clientdata, tdata_t buf, tmsize_t size) {
    (void)clientdata;
    (void)buf;
    (void)size;
    return size;
}

static toff_t
dummy_seekproc(thandle_t clientdata, toff_t offset, int whence) {
    (void)clientdata;
    (void)offset;
    (void)whence;
    return 0;
}

static int
dummy_closeproc(thandle_t clientdata) {
    (void)clientdata;
    return 0;
}

/* TIFFSizeProc expects: tmsize_t (*)(void *) but the type definition in tiffio.h
 * might vary. The error indicates a mismatch between signed/unsigned or return type.
 * Based on the error: 'tmsize_t (thandle_t)' vs 'unsigned long (*)(void *)'.
 * tmsize_t is usually a signed type (int64_t or int32_t).
 * TIFFSizeProc is defined as tmsize_t (*)(void *).
 * The error message says 'unsigned long (*)(void *)'.
 * Let's check tiffio.h definition again.
 * typedef tmsize_t (*TIFFSizeProc)(thandle_t);
 * The error says passing 'tmsize_t (thandle_t)' to 'unsigned long (*)(void *)'.
 * This implies TIFFSizeProc is defined as unsigned long in the specific version or
 * the compiler is inferring something differently.
 * However, standard libtiff defines TIFFSizeProc as tmsize_t (*)(thandle_t).
 * The error message explicitly says: passing 'tmsize_t (thandle_t)' (aka 'long (void *)')
 * to parameter of type 'TIFFSizeProc' (aka 'unsigned long (*)(void *)').
 * This suggests the target library's tiffio.h defines TIFFSizeProc as returning unsigned long.
 * Let's change the return type of dummy_sizeproc to match the expected unsigned long.
 */
static unsigned long
dummy_sizeproc(thandle_t clientdata) {
    (void)clientdata;
    return 0;
}

/* TIFFMapFileProc expects: int (*)(void *, void **, toff_t *) */
static int
dummy_mapproc(thandle_t clientdata, void **base, toff_t *size) {
    (void)clientdata;
    (void)base;
    (void)size;
    return 0;
}

/* TIFFUnmapFileProc expects: void (*)(void *, void *, toff_t) */
static void
dummy_unmapproc(thandle_t clientdata, void *base, toff_t size) {
    (void)clientdata;
    (void)base;
    (void)size;
}

int
LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    const char *mode = "r";
    
    // Write dummy data to the file to ensure it exists
    FILE *fp = fopen(filename, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    TIFF *tif = NULL;
    
    // TIFFOpenOptions is an opaque type in tiffio.h, so we cannot initialize it directly.
    // We pass NULL to TIFFClientOpenExt to use default options.
    TIFFOpenOptions *opts = NULL;

    // Attempt to open TIFF using ClientOpenExt with dummy callbacks
    tif = TIFFClientOpenExt(
        filename,
        mode,
        NULL,
        dummy_readproc,
        dummy_writeproc,
        dummy_seekproc,
        dummy_closeproc,
        dummy_sizeproc,
        dummy_mapproc,
        dummy_unmapproc,
        opts
    );

    if (tif != NULL) {
        // Test TIFFSetClientdata
        void *dummy_data = (void *)Data;
        thandle_t old_data = TIFFSetClientdata(tif, dummy_data);
        (void)old_data;

        // Test TIFFClientdata
        thandle_t retrieved_data = TIFFClientdata(tif);
        (void)retrieved_data;

        // Test TIFFGetClientInfo (with a key derived from input)
        // Ensure the key is null-terminated or use a safe default
        const char *key = "fuzz_key";
        void *client_info = TIFFGetClientInfo(tif, key);
        (void)client_info;

        // Test TIFFPrintDirectory
        // Use stdout as the file stream
        TIFFPrintDirectory(tif, stdout, 0);

        // Cleanup resources
        TIFFClose(tif);
        tif = NULL;
    }

    // Clean up the dummy file
    remove(filename);

    return 0;
}