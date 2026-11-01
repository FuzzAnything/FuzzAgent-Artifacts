// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFGetConfiguredCODECs at tif_compress.c:261:12 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFIsCODECConfigured at tif_codec.c:146:5 in tiffio.h
// TIFFFindCODEC at tif_compress.c:192:18 in tiffio.h
// TIFFRegisterCODEC at tif_compress.c:206:12 in tiffio.h
// TIFFUnRegisterCODEC at tif_compress.c:232:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadFromUserBuffer at tif_read.c:1545:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>
#include <tiff.h>

/* Dummy codec initialization function for registration testing */
static int
DummyCodecInit(TIFF *tif, int scheme)
{
    (void)tif;
    (void)scheme;
    return 1;
}

/* Dummy codec cleanup function */
static void
DummyCodecCleanup(TIFF *tif)
{
    (void)tif;
}

/* Dummy codec setup/teardown functions to satisfy struct requirements if needed */
static int
DummySetupDecode(TIFF *tif, uint32_t *bpp)
{
    (void)tif;
    (void)bpp;
    return 1;
}

static int
DummySetupEncode(TIFF *tif, uint32_t *bpp)
{
    (void)tif;
    (void)bpp;
    return 1;
}

static int
DummyPreDecode(TIFF *tif)
{
    (void)tif;
    return 1;
}

static int
DummyPreEncode(TIFF *tif)
{
    (void)tif;
    return 1;
}

static int
DummyPostEncode(TIFF *tif)
{
    (void)tif;
    return 1;
}

static int
DummyDecodeRow(TIFF *tif, uint8_t *buf, tmsize_t cc, int row)
{
    (void)tif;
    (void)buf;
    (void)cc;
    (void)row;
    return 1;
}

static int
DummyEncodeRow(TIFF *tif, uint8_t *buf, tmsize_t cc, int row)
{
    (void)tif;
    (void)buf;
    (void)cc;
    (void)row;
    return 1;
}

static int
DummyDecodeStrip(TIFF *tif, uint8_t *buf, tmsize_t cc)
{
    (void)tif;
    (void)buf;
    (void)cc;
    return 1;
}

static int
DummyEncodeStrip(TIFF *tif, uint8_t *buf, tmsize_t cc)
{
    (void)tif;
    (void)buf;
    (void)cc;
    return 1;
}

static int
DummyDecodeTile(TIFF *tif, uint8_t *buf, tmsize_t cc)
{
    (void)tif;
    (void)buf;
    (void)cc;
    return 1;
}

static int
DummyEncodeTile(TIFF *tif, uint8_t *buf, tmsize_t cc)
{
    (void)tif;
    (void)buf;
    (void)cc;
    return 1;
}

static void
DummyCleanup(TIFF *tif)
{
    (void)tif;
}

static int
DummyFixupTags(TIFF *tif)
{
    (void)tif;
    return 1;
}

static int
DummySeek(TIFF *tif, toff_t off, int whence)
{
    (void)tif;
    (void)off;
    (void)whence;
    return 0;
}

static int
DummyClose(TIFF *tif)
{
    (void)tif;
    return 1;
}

static toff_t
DummySize(TIFF *tif)
{
    (void)tif;
    return 0;
}

static toff_t
DummySeekProc(TIFF *tif, toff_t off, int whence)
{
    (void)tif;
    (void)off;
    (void)whence;
    return 0;
}

static int
DummyReadProc(thandle_t clientdata, void *buf, tmsize_t size)
{
    (void)clientdata;
    (void)buf;
    (void)size;
    return 0;
}

static int
DummyWriteProc(thandle_t clientdata, void *buf, tmsize_t size)
{
    (void)clientdata;
    (void)buf;
    (void)size;
    return 0;
}

/*
 * Fuzz driver entry point
 */
int
LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size)
{
    if (Size == 0) {
        return 0;
    }

    /*
     * Test 1: TIFFGetConfiguredCODECs
     * Enumerate codecs and free them.
     * Note: _TIFFfreeExt is internal and not always exported. 
     * We use _TIFFfree as suggested by the compiler error if _TIFFfreeExt is unavailable,
     * or standard free if the library allocates via standard malloc.
     * The prompt mentioned _TIFFfreeExt, but the compiler suggests _TIFFfree.
     * We will use _TIFFfree which is declared in tiffio.h.
     */
    TIFFCodec *codecs = TIFFGetConfiguredCODECs();
    if (codecs != NULL) {
        _TIFFfree(codecs);
    }

    /*
     * Test 2: TIFFIsCODECConfigured
     * Check various compression scheme IDs.
     */
    uint16_t scheme_id;
    if (Size >= 2) {
        scheme_id = Data[0] | (Data[1] << 8);
    } else {
        scheme_id = Data[0];
    }
    (void)TIFFIsCODECConfigured(scheme_id);

    /*
     * Test 3: TIFFFindCODEC
     * Locate a codec by ID.
     */
    const TIFFCodec *found_codec = TIFFFindCODEC(scheme_id);
    (void)found_codec;

    /*
     * Test 4: TIFFRegisterCODEC and TIFFUnRegisterCODEC
     * Register a dummy codec, then unregister it.
     * We use a unique-ish ID based on input to explore different states.
     */
    uint16_t reg_id = (Size > 0) ? (Data[0] | (Size << 4)) : 0x1234;
    /* Ensure ID is not 0 or a standard one to avoid conflicts if possible,
     * though the library handles duplicates by returning existing or failing.
     */
    if (reg_id == 0) reg_id = 0xFFFF;

    TIFFCodec *reg_codec = TIFFRegisterCODEC(reg_id, "DummyFuzzCodec", DummyCodecInit);
    if (reg_codec != NULL) {
        /* Try to unregister the registered codec */
        TIFFUnRegisterCODEC(reg_codec);
    }

    /*
     * Test 5: TIFFReadFromUserBuffer
     * This requires a valid TIFF handle opened for reading.
     * We create a dummy file to open.
     */
    const char *dummy_file = "./dummy_file";
    
    /* Write the fuzz data to the dummy file */
    FILE *f = fopen(dummy_file, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    } else {
        /* If we can't write, we can't test file-based functions properly.
         * We skip the file-based part but continue with other API calls.
         */
        return 0;
    }

    TIFF *tif = TIFFOpen(dummy_file, "r");
    if (tif != NULL) {
        /*
         * Attempt to read from user buffer.
         * We need to allocate buffers for inbuf and outbuf.
         * We use the input data as the source buffer.
         */
        uint8_t *outbuf = NULL;
        tmsize_t outsize = 0;
        
        /* Estimate output size or use a fixed buffer for fuzzing */
        if (Size > 0) {
            outsize = Size * 2; 
            if (outsize > 1024 * 1024) outsize = 1024 * 1024; /* Cap memory */
            outbuf = (uint8_t *)malloc(outsize);
        }

        if (outbuf != NULL) {
            /* 
             * strile: strip/tile number. Use modulo to keep it in range if Size is large.
             * We don't know the actual strip count, so we try a few values.
             */
            uint32_t strile = 0;
            if (Size >= 4) {
                strile = Data[2] | (Data[3] << 8);
            }
            
            /* 
             * inbuf: the raw data from the file (or our fuzz data).
             * insize: size of inbuf.
             * outbuf: destination buffer.
             * outsize: size of outbuf.
             */
            (void)TIFFReadFromUserBuffer(tif, strile, (void *)Data, Size, outbuf, outsize);
            
            free(outbuf);
        }

        TIFFClose(tif);
    }

    /* Cleanup dummy file */
    (void)remove(dummy_file);

    return 0;
}