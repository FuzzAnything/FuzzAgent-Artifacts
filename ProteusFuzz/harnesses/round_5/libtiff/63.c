// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadFromUserBuffer at tif_read.c:1545:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFGetConfiguredCODECs at tif_compress.c:261:12 in tiffio.h
// TIFFIsCODECConfigured at tif_codec.c:146:5 in tiffio.h
// TIFFIsCODECConfigured at tif_codec.c:146:5 in tiffio.h
// TIFFIsCODECConfigured at tif_codec.c:146:5 in tiffio.h
// TIFFIsCODECConfigured at tif_codec.c:146:5 in tiffio.h
// TIFFFindCODEC at tif_compress.c:192:18 in tiffio.h
// TIFFRegisterCODEC at tif_compress.c:206:12 in tiffio.h
// TIFFFindCODEC at tif_compress.c:192:18 in tiffio.h
// TIFFUnRegisterCODEC at tif_compress.c:232:6 in tiffio.h
// TIFFFindCODEC at tif_compress.c:192:18 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <tiffio.h>

/* Dummy initialization function for custom codec registration */
static int DummyCodecInit(TIFF *tif, int scheme) {
    (void)tif;
    (void)scheme;
    return 0;
}

/* Helper to safely free the codec array returned by TIFFGetConfiguredCODECs */
static void FreeConfiguredCodecs(TIFFCodec *codecs) {
    if (codecs != NULL) {
        /* _TIFFfreeExt is internal and may not be exported in all builds.
         * The public API _TIFFfree is generally used for internal allocations.
         * If _TIFFfreeExt is strictly required by the specific libtiff version
         * and not available, this might need adjustment, but _TIFFfree is the
         * standard public fallback for internal memory management in libtiff.
         * Based on the error, _TIFFfree is the correct public function to use here. */
        _TIFFfree(codecs);
    }
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Data == NULL || Size == 0) {
        return 0;
    }

    /*
     * Test 1: TIFFGetConfiguredCODECs
     * Enumerate active codecs and free them.
     */
    TIFFCodec *configured_codecs = TIFFGetConfiguredCODECs();
    if (configured_codecs != NULL) {
        FreeConfiguredCodecs(configured_codecs);
    }

    /*
     * Test 2: TIFFIsCODECConfigured
     * Check various scheme IDs derived from input data.
     */
    uint16_t scheme_id = 0;
    if (Size >= 2) {
        scheme_id = Data[0] | (Data[1] << 8);
    } else if (Size == 1) {
        scheme_id = Data[0];
    }
    
    /* Test with the derived ID */
    int is_configured = TIFFIsCODECConfigured(scheme_id);
    (void)is_configured; /* Use result to avoid unused warning */

    /* Test with a few fixed common IDs to increase coverage */
    (void)TIFFIsCODECConfigured(1); /* None */
    (void)TIFFIsCODECConfigured(5); /* LZW */
    (void)TIFFIsCODECConfigured(6); /* JPEG */

    /*
     * Test 3: TIFFFindCODEC
     * Locate codec by scheme ID.
     */
    const TIFFCodec *found_codec = TIFFFindCODEC(scheme_id);
    (void)found_codec;

    /*
     * Test 4: TIFFRegisterCODEC and TIFFUnRegisterCODEC
     * Register a custom codec with a unique ID derived from input, then unregister it.
     * We use a high ID range to avoid conflicts with built-ins.
     */
    uint16_t custom_id = 0xFFFF;
    if (Size >= 2) {
        custom_id = (Data[0] << 8) | Data[1];
        /* Ensure it's in a safe range, avoiding 0-1000 usually reserved */
        if (custom_id < 1000) custom_id += 10000;
    }

    TIFFCodec *registered_codec = TIFFRegisterCODEC(custom_id, "FuzzCustomCodec", DummyCodecInit);
    if (registered_codec != NULL) {
        /* Verify it was found */
        const TIFFCodec *verify = TIFFFindCODEC(custom_id);
        (void)verify;
        
        /* Unregister it */
        TIFFUnRegisterCODEC(registered_codec);
        
        /* Verify it's gone */
        const TIFFCodec *verify_gone = TIFFFindCODEC(custom_id);
        (void)verify_gone;
    }

    /*
     * Test 5: TIFFReadFromUserBuffer
     * This requires a valid TIFF object opened for reading.
     * Since we cannot easily create a valid TIFF in memory without complex setup,
     * we will attempt to create a dummy file and open it, then try to read.
     * If the file is invalid, the function should fail gracefully.
     */
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp != NULL) {
        /* Write some garbage data to simulate a TIFF file */
        size_t write_size = (Size > 1000) ? 1000 : Size;
        fwrite(Data, 1, write_size, fp);
        fclose(fp);

        TIFF *tif = TIFFOpen(dummy_file, "r");
        if (tif != NULL) {
            /* Prepare buffers for TIFFReadFromUserBuffer */
            /* We use the input data as the raw buffer to decode */
            uint32_t strile = 0;
            void *inbuf = (void *)Data;
            tmsize_t insize = Size;
            
            /* Allocate a small output buffer */
            uint8_t *outbuf = (uint8_t *)malloc(256);
            if (outbuf != NULL) {
                tmsize_t outsize = 256;
                int result = TIFFReadFromUserBuffer(tif, strile, inbuf, insize, outbuf, outsize);
                (void)result;
                free(outbuf);
            }
            
            TIFFClose(tif);
        }
        /* Clean up dummy file */
        unlink(dummy_file);
    }

    return 0;
}