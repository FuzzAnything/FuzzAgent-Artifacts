// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFSetClientInfo at tif_extension.c:78:6 in tiffio.h
// TIFFSetClientdata at tif_open.c:833:11 in tiffio.h
// TIFFGetClientInfo at tif_extension.c:64:7 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFCleanup at tif_close.c:44:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static thandle_t dummy_client_data = (thandle_t)0x12345678;

/* Dummy I/O callbacks for TIFFClientOpenExt */
static tmsize_t dummy_read(thandle_t handle, void *buf, tmsize_t size) {
    (void)handle;
    (void)buf;
    (void)size;
    return 0;
}

static tmsize_t dummy_write(thandle_t handle, void *buf, tmsize_t size) {
    (void)handle;
    (void)buf;
    (void)size;
    return size;
}

static toff_t dummy_seek(thandle_t handle, toff_t off, int whence) {
    (void)handle;
    (void)off;
    (void)whence;
    return 0;
}

static int dummy_close(thandle_t handle) {
    (void)handle;
    return 0;
}

/* TIFFSizeProc expects: tmsize_t (*)(thandle_t) or unsigned long (*)(void *) depending on build, 
   but tiffio.h defines it as tmsize_t. The error suggests a mismatch in signedness or return type.
   Let's match the exact signature expected by the library's typedef.
   According to tiffio.h: typedef tmsize_t (*TIFFSizeProc)(thandle_t);
   The error says: passing 'tmsize_t (thandle_t)' to 'TIFFSizeProc' (aka 'unsigned long (*)(void *)')
   This implies tmsize_t might be signed long, but TIFFSizeProc expects unsigned long.
   We will cast or adjust the return type to match the unsigned expectation if needed, 
   but typically tmsize_t is the correct type. Let's try casting the function pointer or adjusting the function.
   Actually, the error message says TIFFSizeProc is 'unsigned long (*)(void *)'.
   So we define dummy_size to return unsigned long.
*/
static unsigned long dummy_size(thandle_t handle) {
    (void)handle;
    return 0;
}

/* TIFFMapFileProc expects: void* (*)(thandle_t, toff_t *) 
   Error says: passing 'void *(thandle_t, toff_t *)' to 'TIFFMapFileProc' (aka 'int (*)(void *, void **, unsigned long *)')
   This is a significant mismatch. The library expects an int return and different pointer types.
   Let's check the standard libtiff definition.
   Usually: typedef void* (*TIFFMapFileProc)(thandle_t, toff_t*);
   But the error suggests the specific build of libtiff has a different definition.
   Let's look at the error again: 'int (*)(void *, void **, unsigned long *)'
   This looks like a custom or older definition, or a specific platform quirk.
   However, standard libtiff uses:
   typedef void* (*TIFFMapFileProc)(thandle_t, toff_t*);
   typedef void (*TIFFUnmapFileProc)(thandle_t, void*, toff_t);
   
   If the compiler says TIFFMapFileProc is 'int (*)(void *, void **, unsigned long *)', 
   then we must match that signature exactly to avoid the error.
   Let's redefine dummy_map and dummy_unmap to match the error message's expected types.
   
   Expected: int (*)(void *, void **, unsigned long *)
   Our current: void *(thandle_t, toff_t *)
   
   Let's try to match the error's expectation:
   int dummy_map(thandle_t handle, void **buf, toff_t *size)
   But wait, toff_t is uint32_t or uint64_t. The error says unsigned long *.
   Let's assume toff_t is compatible with unsigned long on this platform or cast it.
   
   Actually, looking at the error: 'TIFFMapFileProc' (aka 'int (*)(void *, void **, unsigned long *)')
   This is very specific. Let's define the functions to match this signature exactly.
*/

static int dummy_map(thandle_t handle, void **buf, unsigned long *size) {
    (void)handle;
    (void)buf;
    (void)size;
    return 0;
}

/* TIFFUnmapFileProc error: 'void (*)(thandle_t, void *, toff_t)' vs expected?
   The error didn't show unmap, but map failed. Let's assume unmap is standard or adjust if needed.
   Standard: void (*TIFFUnmapFileProc)(thandle_t, void*, toff_t);
   Let's keep it standard unless we get an error.
*/
static void dummy_unmap(thandle_t handle, void *buf, toff_t size) {
    (void)handle;
    (void)buf;
    (void)size;
    return;
}

/* Helper to create a valid TIFF handle from a file */
static TIFF *create_tiff_from_file(const char *filename, const char *mode) {
    TIFF *tif = TIFFOpen(filename, mode);
    return tif;
}

/* Helper to create a TIFF handle using custom callbacks */
static TIFF *create_tiff_client_open_ext(const char *filename, const char *mode) {
    return TIFFClientOpenExt(
        filename,
        mode,
        dummy_client_data,
        dummy_read,
        dummy_write,
        dummy_seek,
        dummy_close,
        dummy_size,
        dummy_map,
        dummy_unmap,
        NULL
    );
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    /* Write input data to a dummy file */
    const char *filename = "./dummy_file";
    FILE *f = fopen(filename, "wb");
    if (!f) {
        return 0;
    }
    fwrite(Data, 1, Size, f);
    fclose(f);

    TIFF *tif = NULL;
    uint8_t mode = Data[0];
    uint8_t open_method = Data[1];
    uint8_t set_client_info = (Data[2] % 2);
    uint8_t set_client_data = (Data[3] % 2);

    /* Determine open mode string */
    const char *mode_str = (mode & 1) ? "w" : "r";

    /* Open TIFF based on method */
    if (open_method == 0) {
        tif = create_tiff_from_file(filename, mode_str);
    } else {
        tif = create_tiff_client_open_ext(filename, mode_str);
    }

    if (tif) {
        /* Fuzz TIFFSetClientInfo */
        if (set_client_info) {
            char key[64];
            size_t key_len = Size > 64 ? 64 : Size;
            if (key_len > 0) {
                memcpy(key, Data + 4, key_len);
            }
            key[key_len] = '\0';
            
            void *client_data_ptr = (void *)Data;
            TIFFSetClientInfo(tif, client_data_ptr, key);
        }

        /* Fuzz TIFFSetClientdata */
        if (set_client_data) {
            thandle_t new_client_data = (thandle_t)(uintptr_t)(Data[0] ^ Data[1] ^ Data[2] ^ Data[3]);
            thandle_t old_client_data = TIFFSetClientdata(tif, new_client_data);
            (void)old_client_data; /* Suppress unused variable warning */
        }

        /* Fuzz TIFFGetClientInfo */
        if (Size > 4) {
            char key[64];
            size_t key_len = Size > 64 ? 64 : Size;
            if (key_len > 0) {
                memcpy(key, Data + 4, key_len);
            }
            key[key_len] = '\0';
            void *retrieved = TIFFGetClientInfo(tif, key);
            (void)retrieved; /* Suppress unused variable warning */
        }

        /* Fuzz TIFFClientdata */
        thandle_t client_data = TIFFClientdata(tif);
        (void)client_data; /* Suppress unused variable warning */

        /* Cleanup */
        TIFFCleanup(tif);
    }

    return 0;
}