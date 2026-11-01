// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFIsCODECConfigured at tif_codec.c:146:5 in tiffio.h
// TIFFIsCODECConfigured at tif_codec.c:146:5 in tiffio.h
// TIFFFindCODEC at tif_compress.c:192:18 in tiffio.h
// TIFFRegisterCODEC at tif_compress.c:206:12 in tiffio.h
// TIFFUnRegisterCODEC at tif_compress.c:232:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadFromUserBuffer at tif_read.c:1545:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFGetConfiguredCODECs at tif_compress.c:261:12 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

/* Dummy init function for custom codec registration */
/* TIFFInitMethod signature is int (*)(TIFF *, int) */
static int
DummyInit(TIFF *tif, int scheme)
{
    (void)tif;
    (void)scheme;
    return 1;
}

/* Dummy read callback for file operations */
static tmsize_t
DummyReadProc(thandle_t fd, void *buf, tmsize_t size)
{
    (void)fd;
    (void)buf;
    (void)size;
    return 0;
}

/* Dummy write callback for file operations */
static tmsize_t
DummyWriteProc(thandle_t fd, const void *buf, tmsize_t size)
{
    (void)fd;
    (void)buf;
    (void)size;
    return 0;
}

/* Dummy seek callback for file operations */
static int
DummySeekProc(thandle_t fd, toff_t off, int whence)
{
    (void)fd;
    (void)off;
    (void)whence;
    return -1;
}

/* Dummy close callback for file operations */
static int
DummyCloseProc(thandle_t fd)
{
    (void)fd;
    return 0;
}

/* Dummy size callback for file operations */
static toff_t
DummySizeProc(thandle_t fd)
{
    (void)fd;
    return 0;
}

int
LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size)
{
    if (Size == 0)
        return 0;

    /* 1. Prepare environment: Create a dummy file for file-based operations */
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }

    /* 2. Fuzz TIFFGetConfiguredCODECs */
    TIFFCodec *codecs = TIFFGetConfiguredCODECs();
    if (codecs != NULL) {
        /* 
         * TIFFGetConfiguredCODECs returns a dynamically allocated array.
         * We must free it using _TIFFfree (or _TIFFfreeExt if available, but _TIFFfree is standard).
         * The count is not returned, but _TIFFfree handles the internal list structure.
         */
        _TIFFfree(codecs);
    }

    /* 3. Fuzz TIFFIsCODECConfigured with various IDs */
    uint16_t scheme_id = 0;
    if (Size >= 2) {
        scheme_id = (uint16_t)(Data[0] | (Data[1] << 8));
    } else if (Size == 1) {
        scheme_id = Data[0];
    }
    /* Also try some fixed values to ensure coverage */
    uint16_t test_ids[] = {0, 1, 2, 3, 4, 5, 32768, 65535};
    for (int i = 0; i < sizeof(test_ids)/sizeof(test_ids[0]); i++) {
        (void)TIFFIsCODECConfigured(test_ids[i]);
    }
    (void)TIFFIsCODECConfigured(scheme_id);

    /* 4. Fuzz TIFFFindCODEC */
    const TIFFCodec *codec = TIFFFindCODEC(scheme_id);
    if (codec != NULL) {
        /* Found a codec. We do not unregister it here as it might be a built-in. */
    }

    /* 5. Fuzz TIFFRegisterCODEC and TIFFUnRegisterCODEC */
    /* Register a custom codec with a unique ID derived from input */
    uint16_t custom_id = 0x1234; /* Use a fixed ID to avoid collisions with built-ins */
    if (Size >= 2) {
        custom_id = (uint16_t)(Data[0] | (Data[1] << 8));
        /* Ensure it's not a standard ID to avoid conflicts, though libtiff handles it */
        if (custom_id < 10) custom_id += 10000; 
    }
    
    TIFFCodec *registered_codec = TIFFRegisterCODEC(custom_id, "FuzzCodec", DummyInit);
    if (registered_codec != NULL) {
        /* Successfully registered, now unregister it */
        TIFFUnRegisterCODEC(registered_codec);
    }

    /* 6. Fuzz TIFFReadFromUserBuffer */
    /* Open the dummy file to get a TIFF handle */
    TIFF *tif = TIFFOpen(dummy_file, "r");
    if (tif != NULL) {
        /* Prepare buffers for TIFFReadFromUserBuffer */
        uint32_t strile = 0;
        if (Size >= 4) {
            strile = (uint32_t)(Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24));
        }
        
        /* Allocate input and output buffers */
        size_t insize = Size > 1000 ? 1000 : Size;
        size_t outsize = Size > 1000 ? 1000 : Size;
        uint8_t *inbuf = (uint8_t *)malloc(insize);
        uint8_t *outbuf = (uint8_t *)malloc(outsize);
        
        if (inbuf && outbuf) {
            if (insize > 0) {
                memcpy(inbuf, Data, insize);
            }
            
            /* Attempt to read from user buffer */
            int result = TIFFReadFromUserBuffer(tif, strile, inbuf, insize, outbuf, outsize);
            (void)result; /* Ignore result for fuzzing, just trigger the path */
            
            free(inbuf);
            free(outbuf);
        } else {
            if (inbuf) free(inbuf);
            if (outbuf) free(outbuf);
        }

        TIFFClose(tif);
    }

    /* 7. Cleanup dummy file */
    remove(dummy_file);

    return 0;
}