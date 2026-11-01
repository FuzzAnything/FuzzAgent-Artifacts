// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReadFromUserBuffer at tif_read.c:1545:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFGetConfiguredCODECs at tif_compress.c:261:12 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFIsCODECConfigured at tif_codec.c:146:5 in tiffio.h
// TIFFIsCODECConfigured at tif_codec.c:146:5 in tiffio.h
// TIFFIsCODECConfigured at tif_codec.c:146:5 in tiffio.h
// TIFFFindCODEC at tif_compress.c:192:18 in tiffio.h
// TIFFRegisterCODEC at tif_compress.c:206:12 in tiffio.h
// TIFFIsCODECConfigured at tif_codec.c:146:5 in tiffio.h
// TIFFFindCODEC at tif_compress.c:192:18 in tiffio.h
// TIFFUnRegisterCODEC at tif_compress.c:232:6 in tiffio.h
// TIFFIsCODECConfigured at tif_codec.c:146:5 in tiffio.h
// TIFFFindCODEC at tif_compress.c:192:18 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

/* Dummy codec initialization function for testing */
static int DummyCodecInit(TIFF *tif, int scheme) {
    (void)tif;
    (void)scheme;
    return 1;
}

/* Callbacks for TIFFClientOpen */
static tmsize_t DummyReadProc(thandle_t clientdata, void *buf, tmsize_t size) {
    (void)clientdata;
    (void)buf;
    (void)size;
    return 0;
}

static tmsize_t DummyWriteProc(thandle_t clientdata, void *buf, tmsize_t size) {
    (void)clientdata;
    (void)buf;
    (void)size;
    return 0;
}

/* TIFFSeekProc signature: toff_t (*)(thandle_t, toff_t, int) */
static toff_t DummySeekProc(thandle_t clientdata, toff_t off, int whence) {
    (void)clientdata;
    (void)off;
    (void)whence;
    return 0;
}

static int DummyCloseProc(thandle_t clientdata) {
    (void)clientdata;
    return 0;
}

static toff_t DummySizeProc(thandle_t clientdata) {
    (void)clientdata;
    return 0;
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* 1. Test TIFFGetConfiguredCODECs */
    TIFFCodec *codecs = TIFFGetConfiguredCODECs();
    if (codecs) {
        /* Use _TIFFfree as _TIFFfreeExt is not available in standard headers */
        _TIFFfree(codecs);
    }

    /* 2. Test TIFFIsCODECConfigured with various IDs derived from input */
    uint16_t scheme_id = 0;
    if (Size >= 2) {
        scheme_id = Data[0] | (Data[1] << 8);
    } else if (Size == 1) {
        scheme_id = Data[0];
    }
    
    (void) TIFFIsCODECConfigured(scheme_id);
    (void) TIFFIsCODECConfigured(0);
    (void) TIFFIsCODECConfigured(1);

    /* 3. Test TIFFFindCODEC */
    const TIFFCodec *codec = TIFFFindCODEC(scheme_id);
    (void) codec;

    /* 4. Test TIFFRegisterCODEC and TIFFUnRegisterCODEC */
    uint16_t custom_id = 0x9999; 
    if (Size >= 2) {
        custom_id = (Data[0] | (Data[1] << 8)) + 1;
    }
    
    TIFFCodec *new_codec = TIFFRegisterCODEC(custom_id, "DummyCodec", (TIFFInitMethod)DummyCodecInit);
    
    if (new_codec) {
        (void) TIFFIsCODECConfigured(custom_id);
        (void) TIFFFindCODEC(custom_id);
        
        TIFFUnRegisterCODEC(new_codec);
        
        (void) TIFFIsCODECConfigured(custom_id);
        (void) TIFFFindCODEC(custom_id);
    }

    /* 5. Test TIFFReadFromUserBuffer */
    size_t out_size = Size > 0 ? Size : 1;
    uint8_t *out_buf = malloc(out_size);
    if (!out_buf) return 0;

    /* Open a dummy TIFF handle using callbacks */
    TIFF *tif = TIFFClientOpen("dummy_file", "rb", NULL,
                               DummyReadProc, DummyWriteProc,
                               DummySeekProc, DummyCloseProc,
                               DummySizeProc, NULL, NULL);
    
    if (tif) {
        /* Attempt to read from user buffer. 
           This will likely fail due to invalid directory, but tests the API path. */
        int ret = TIFFReadFromUserBuffer(tif, 0, (void*)Data, Size, out_buf, out_size);
        (void) ret;
        
        TIFFClose(tif);
    }

    free(out_buf);

    return 0;
}