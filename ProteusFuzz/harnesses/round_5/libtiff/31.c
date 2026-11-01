// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFdOpen at tif_unix.c:209:7 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFSetClientdata at tif_open.c:833:11 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFSetClientInfo at tif_extension.c:78:6 in tiffio.h
// TIFFPrintDirectory at tif_print.c:244:6 in tiffio.h
// TIFFCleanup at tif_close.c:44:6 in tiffio.h
// TIFFFdOpen at tif_unix.c:209:7 in tiffio.h
// TIFFSetClientdata at tif_open.c:833:11 in tiffio.h
// TIFFSetClientInfo at tif_extension.c:78:6 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFPrintDirectory at tif_print.c:244:6 in tiffio.h
// TIFFCleanup at tif_close.c:44:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <tiffio.h>

/* Helper to write fuzz data to a dummy file */
static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }
}

/* Custom I/O callbacks for TIFFClientOpenExt with correct signatures */
static tmsize_t dummy_readproc(thandle_t clientdata, tdata_t buf, tmsize_t size) {
    return 0;
}

static tmsize_t dummy_writeproc(thandle_t clientdata, const tdata_t buf, tmsize_t size) {
    return 0;
}

static toff_t dummy_seekproc(thandle_t clientdata, toff_t off, int whence) {
    return -1;
}

static int dummy_closeproc(thandle_t clientdata) {
    return 0;
}

/* TIFFSizeProc expects unsigned long return type on some platforms, matching tmsize_t might differ.
   We cast or define to match exactly. tmsize_t is usually signed long/long long.
   TIFFSizeProc is typedef'd as tmsize_t (*)(thandle_t).
   However, the error says it expects 'unsigned long'. This implies tmsize_t might be defined differently
   or the typedef in tiffio.h is specific.
   Let's check the typedef: typedef TIFF_SSIZE_T tmsize_t;
   TIFFSizeProc is defined as: typedef tmsize_t (*TIFFSizeProc)(thandle_t);
   The error message says: passing 'tmsize_t (thandle_t)' to 'unsigned long (*)(void *)'.
   This suggests tmsize_t is signed long, but TIFFSizeProc expects unsigned long.
   This is a mismatch in the specific build environment.
   To fix, we cast the function pointer or change the return type to match the expected typedef if possible.
   Since we cannot change tiffio.h, we must match the expected signature.
   If TIFFSizeProc is 'unsigned long (*)(void *)', we define our function to return unsigned long.
   But tmsize_t is used for size. Let's try to match the typedef strictly.
   Actually, the error says TIFFSizeProc is 'unsigned long (*)(void *)'.
   So we define: static unsigned long dummy_sizeproc(thandle_t clientdata) { return 0; }
   Wait, tdata_t is void*. toff_t is unsigned long.
   Let's redefine the helpers to match the exact types expected by the library in this environment.
*/

static unsigned long dummy_sizeproc(thandle_t clientdata) {
    return 0;
}

/* TIFFMapFileProc: typedef void* (*TIFFMapFileProc)(thandle_t, toff_t*);
   Error says: expects 'int (*)(void *, void **, unsigned long *)'.
   This is very strange. The standard libtiff typedef is void* (*)(thandle_t, toff_t*).
   The error suggests the environment has a different definition or the user provided struct is misleading.
   Let's look at the error again: 'void *(thandle_t, toff_t *)' to 'int (*)(void *, void **, unsigned long *)'.
   This implies TIFFMapFileProc is defined as returning int? Or maybe the arguments are different.
   Actually, looking at older libtiff or specific patches, sometimes signatures vary.
   However, the standard is:
   typedef void* (*TIFFMapFileProc)(thandle_t, toff_t*);
   typedef void (*TIFFUnmapFileProc)(thandle_t, void*, toff_t);
   
   If the compiler says it expects 'int (*)(void *, void **, unsigned long *)',
   then the typedef in the included tiffio.h is likely:
   typedef int (*TIFFMapFileProc)(thandle_t, void **, toff_t*);
   Wait, the error says 'int (*)(void *, void **, unsigned long *)'.
   The arguments are (thandle_t, void **, toff_t*).
   Let's assume the typedef is:
   typedef int (*TIFFMapFileProc)(thandle_t, void **, toff_t*);
   And TIFFUnmapFileProc:
   typedef void (*TIFFUnmapFileProc)(thandle_t, void*, toff_t);
   
   Let's redefine the functions to match these specific signatures inferred from the error.
*/

static int dummy_mapproc(thandle_t clientdata, void **base, toff_t *size) {
    *base = NULL;
    *size = 0;
    return 0;
}

static void dummy_unmapproc(thandle_t clientdata, void *base, toff_t size) {
    /* No-op */
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Write input to dummy file for standard open attempts */
    write_dummy_file(Data, Size);

    TIFF *tif = NULL;
    FILE *fp = NULL;
    thandle_t old_clientdata = NULL;
    void *client_info_data = NULL;
    const char *client_info_name = "fuzz_test_info";
    long print_flags = 0;

    /* 1. Try opening via standard TIFFOpen */
    fp = fopen("./dummy_file", "rb");
    if (fp) {
        tif = TIFFFdOpen(fileno(fp), "./dummy_file", "r");
        fclose(fp);
    }

    /* If standard open failed, try TIFFClientOpenExt with custom callbacks */
    if (!tif) {
        tif = TIFFClientOpenExt("./dummy_file", "r", NULL,
                                dummy_readproc, dummy_writeproc,
                                dummy_seekproc, dummy_closeproc,
                                dummy_sizeproc, dummy_mapproc, dummy_unmapproc,
                                NULL);
    }

    if (tif) {
        /* 2. Test TIFFSetClientdata */
        client_info_data = malloc(1024);
        if (client_info_data) {
            memset(client_info_data, 0, 1024);
            old_clientdata = TIFFSetClientdata(tif, client_info_data);
        }

        /* 3. Test TIFFClientdata */
        thandle_t retrieved_data = TIFFClientdata(tif);
        (void)retrieved_data; /* Suppress unused warning */

        /* 4. Test TIFFSetClientInfo */
        if (client_info_data) {
            TIFFSetClientInfo(tif, client_info_data, client_info_name);
        }

        /* 5. Test TIFFPrintDirectory */
        fp = fopen("./dummy_file", "wb");
        if (fp) {
            TIFFPrintDirectory(tif, fp, print_flags);
            fclose(fp);
        }

        /* 6. Test TIFFCleanup */
        TIFFCleanup(tif);
        tif = NULL; /* Ensure we don't double cleanup */
    }

    /* Cleanup allocated memory */
    if (client_info_data) {
        free(client_info_data);
    }

    /* Additional exploration: Try opening with write mode if file exists */
    if (Size > 0) {
        fp = fopen("./dummy_file", "r+b");
        if (fp) {
            tif = TIFFFdOpen(fileno(fp), "./dummy_file", "r+");
            fclose(fp);

            if (tif) {
                /* Re-test client data functions in write mode */
                client_info_data = malloc(64);
                if (client_info_data) {
                    memset(client_info_data, 0, 64);
                    TIFFSetClientdata(tif, client_info_data);
                    TIFFSetClientInfo(tif, client_info_data, "fuzz_write_info");
                    TIFFClientdata(tif);
                }
                
                /* Try printing directory again */
                fp = fopen("./dummy_file", "wb");
                if (fp) {
                    TIFFPrintDirectory(tif, fp, 0);
                    fclose(fp);
                }

                TIFFCleanup(tif);
                tif = NULL;
            }
            if (client_info_data) free(client_info_data);
        }
    }

    return 0;
}