// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetClientdata at tif_open.c:833:11 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFSetClientInfo at tif_extension.c:78:6 in tiffio.h
// TIFFGetClientInfo at tif_extension.c:64:7 in tiffio.h
// TIFFGetClientInfo at tif_extension.c:64:7 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static thandle_t dummy_client_data = (thandle_t)0x12345678;

static tmsize_t dummy_read(thandle_t clientdata, void *buf, tmsize_t size) {
    return 0;
}

static tmsize_t dummy_write(thandle_t clientdata, void *buf, tmsize_t size) {
    return size;
}

static toff_t dummy_seek(thandle_t clientdata, toff_t off, int whence) {
    return 0;
}

static int dummy_close(thandle_t clientdata) {
    return 0;
}

// TIFFSizeProc expects unsigned long (tmsize_t is signed, but often compatible in size)
// However, strict types require matching. TIFFSizeProc is defined as tmsize_t (*)(thandle_t)
// in some versions, but the error suggests it expects unsigned long.
// Let's match the signature exactly as implied by the error: TIFFSizeProc is unsigned long (*)(void *)
static unsigned long dummy_size(thandle_t clientdata) {
    return 0;
}

// TIFFMapFileProc expects int (*)(void *, void **, unsigned long *)
// The error message says: 'void *(void *, unsigned long *)' passed to 'int (*)(void *, void **, unsigned long *)'
static int dummy_mmap(thandle_t clientdata, void **buf, unsigned long *size) {
    *buf = NULL;
    *size = 0;
    return 0;
}

// TIFFUnmapFileProc expects void (*)(void *, void *, unsigned long)
static void dummy_unmap(thandle_t clientdata, void *buf, unsigned long size) {
    return;
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    TIFF *tif = NULL;
    void *client_info_data = NULL;
    thandle_t client_data_val = NULL;
    const char *key = "fuzz_key";
    const char *filename = "./dummy_file";
    const char *mode = "w";

    // Create a dummy file to ensure TIFFOpen or TIFFClientOpenExt can initialize
    FILE *fp = fopen(filename, "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }

    // Attempt to open via standard file path first
    tif = TIFFOpen(filename, mode);
    
    // If standard open failed, try ClientOpenExt with dummy callbacks
    if (tif == NULL) {
        tif = TIFFClientOpenExt(filename, mode, dummy_client_data,
                                dummy_read, dummy_write,
                                dummy_seek, dummy_close,
                                dummy_size, dummy_mmap, dummy_unmap,
                                NULL);
    }

    if (tif != NULL) {
        // Fuzz TIFFSetClientInfo and TIFFGetClientInfo
        // Allocate some data to attach
        size_t info_size = Size > 1024 ? 1024 : Size;
        client_info_data = malloc(info_size);
        if (client_info_data) {
            memcpy(client_info_data, Data, info_size);
            TIFFSetClientInfo(tif, client_info_data, key);
            
            // Retrieve to ensure the path is exercised
            void *retrieved = TIFFGetClientInfo(tif, key);
            (void)retrieved;
            
            // Try with a non-existent key
            retrieved = TIFFGetClientInfo(tif, "non_existent_key");
            (void)retrieved;
            
            free(client_info_data);
        }

        // Fuzz TIFFSetClientdata and TIFFClientdata
        client_data_val = (thandle_t)Data; // Use input data as handle
        thandle_t prev_val = TIFFSetClientdata(tif, client_data_val);
        (void)prev_val;
        
        thandle_t current_val = TIFFClientdata(tif);
        (void)current_val;

        // Close the TIFF handle
        TIFFClose(tif);
    }

    // Cleanup dummy file
    remove(filename);

    return 0;
}