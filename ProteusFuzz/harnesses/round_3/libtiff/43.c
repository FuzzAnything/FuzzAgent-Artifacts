// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFReadFromUserBuffer at tif_read.c:1545:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFIsCODECConfigured at tif_codec.c:146:5 in tiffio.h
// TIFFIsCODECConfigured at tif_codec.c:146:5 in tiffio.h
// TIFFFindCODEC at tif_compress.c:192:18 in tiffio.h
// TIFFFindCODEC at tif_compress.c:192:18 in tiffio.h
// TIFFRegisterCODEC at tif_compress.c:206:12 in tiffio.h
// TIFFIsCODECConfigured at tif_codec.c:146:5 in tiffio.h
// TIFFFindCODEC at tif_compress.c:192:18 in tiffio.h
// TIFFUnRegisterCODEC at tif_compress.c:232:6 in tiffio.h
// TIFFIsCODECConfigured at tif_codec.c:146:5 in tiffio.h
// TIFFGetConfiguredCODECs at tif_compress.c:261:12 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static void dummy_readproc(thandle_t fd, void *buf, tmsize_t size) {
    (void)fd;
    (void)buf;
    (void)size;
}

static void dummy_writeproc(thandle_t fd, const void *buf, tmsize_t size) {
    (void)fd;
    (void)buf;
    (void)size;
}

static int dummy_seekproc(thandle_t fd, toff_t off) {
    (void)fd;
    (void)off;
    return 0;
}

static int dummy_closeproc(thandle_t fd) {
    (void)fd;
    return 0;
}

static toff_t dummy_sizeproc(thandle_t fd) {
    (void)fd;
    return 0;
}

/* TIFFInitMethod signature: int (*TIFFInitMethod)(TIFF*, int) */
static int dummy_init_func(TIFF *tif, int scheme) {
    (void)tif;
    (void)scheme;
    return 0;
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    uint16_t scheme_id = (uint16_t)(Data[0] | (Data[1] << 8));
    uint16_t custom_scheme_id = (uint16_t)(Data[2] | (Data[3] << 8));

    /* Test TIFFIsCODECConfigured */
    (void)TIFFIsCODECConfigured(scheme_id);
    (void)TIFFIsCODECConfigured(custom_scheme_id);

    /* Test TIFFFindCODEC */
    const TIFFCodec *codec = TIFFFindCODEC(scheme_id);
    (void)codec;
    (void)TIFFFindCODEC(custom_scheme_id);

    /* Test TIFFRegisterCODEC and TIFFUnRegisterCODEC */
    TIFFCodec *reg_codec = TIFFRegisterCODEC(custom_scheme_id, "FuzzCodec", dummy_init_func);
    if (reg_codec) {
        /* Verify registration */
        (void)TIFFIsCODECConfigured(custom_scheme_id);
        (void)TIFFFindCODEC(custom_scheme_id);
        /* Unregister */
        TIFFUnRegisterCODEC(reg_codec);
        /* Verify unregistration */
        (void)TIFFIsCODECConfigured(custom_scheme_id);
    }

    /* Test TIFFGetConfiguredCODECs */
    TIFFCodec *codecs = TIFFGetConfiguredCODECs();
    if (codecs) {
        /* Use _TIFFfree as the correct deallocation function for the array returned by TIFFGetConfiguredCODECs */
        _TIFFfree(codecs);
    }

    /* Test TIFFReadFromUserBuffer */
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (!fp) {
        return 0;
    }

    /* Write a minimal TIFF header (Little Endian, 42 magic, offset to first IFD) */
    uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
    fwrite(header, 1, 8, fp);
    
    /* Write a minimal IFD (0 entries, next IFD offset 0) */
    uint32_t ifd_data[2] = {0, 0};
    fwrite(ifd_data, sizeof(uint32_t), 2, fp);
    
    fclose(fp);

    TIFF *tif = TIFFOpen(dummy_file, "r");
    if (tif) {
        /* Prepare input buffer for TIFFReadFromUserBuffer */
        uint8_t *inbuf = (uint8_t *)malloc(Size);
        if (!inbuf) {
            TIFFClose(tif);
            remove(dummy_file);
            return 0;
        }
        memcpy(inbuf, Data, Size);

        /* Prepare output buffer */
        uint8_t *outbuf = (uint8_t *)malloc(Size + 1024);
        if (!outbuf) {
            free(inbuf);
            TIFFClose(tif);
            remove(dummy_file);
            return 0;
        }

        /* Try to read from user buffer. This will likely fail due to invalid TIFF data,
           but that is expected in fuzzing to trigger error handling paths. */
        (void)TIFFReadFromUserBuffer(tif, 0, inbuf, Size, outbuf, Size + 1024);

        free(inbuf);
        free(outbuf);
        TIFFClose(tif);
    }

    /* Cleanup dummy file */
    remove(dummy_file);

    return 0;
}