// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFIsCODECConfigured at tif_codec.c:146:5 in tiffio.h
// TIFFFindCODEC at tif_compress.c:192:18 in tiffio.h
// TIFFRegisterCODEC at tif_compress.c:206:12 in tiffio.h
// TIFFUnRegisterCODEC at tif_compress.c:232:6 in tiffio.h
// TIFFGetConfiguredCODECs at tif_compress.c:261:12 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadFromUserBuffer at tif_read.c:1545:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static int dummy_init(TIFF *tif, int scheme) {
    (void)tif;
    (void)scheme;
    return 1;
}

static void dummy_cleanup(TIFF *tif) {
    (void)tif;
}

static int dummy_setupdecode(TIFF *tif) {
    (void)tif;
    return 1;
}

static int dummy_setupencode(TIFF *tif) {
    (void)tif;
    return 1;
}

static void dummy_close(TIFF *tif) {
    (void)tif;
}

static int dummy_decoderow(TIFF *tif, uint8_t *buf, tmsize_t cc, int16_t s) {
    (void)tif;
    (void)buf;
    (void)cc;
    (void)s;
    return 1;
}

static int dummy_encoderow(TIFF *tif, uint8_t *buf, tmsize_t cc, int16_t s) {
    (void)tif;
    (void)buf;
    (void)cc;
    (void)s;
    return 1;
}

static int dummy_decodestrip(TIFF *tif, uint8_t *buf, tmsize_t cc, int16_t s) {
    (void)tif;
    (void)buf;
    (void)cc;
    (void)s;
    return 1;
}

static int dummy_encodestrip(TIFF *tif, uint8_t *buf, tmsize_t cc, int16_t s) {
    (void)tif;
    (void)buf;
    (void)cc;
    (void)s;
    return 1;
}

static int dummy_decodetile(TIFF *tif, uint8_t *buf, tmsize_t cc, int16_t s) {
    (void)tif;
    (void)buf;
    (void)cc;
    (void)s;
    return 1;
}

static int dummy_encodetile(TIFF *tif, uint8_t *buf, tmsize_t cc, int16_t s) {
    (void)tif;
    (void)buf;
    (void)cc;
    (void)s;
    return 1;
}

static void dummy_seek(TIFF *tif, toff_t off) {
    (void)tif;
    (void)off;
}

static void dummy_cleanup_codec(TIFF *tif) {
    (void)tif;
}

static int dummy_predecode(TIFF *tif, uint16_t s, int16_t b) {
    (void)tif;
    (void)s;
    (void)b;
    return 1;
}

static int dummy_preencode(TIFF *tif, uint16_t s, int16_t b) {
    (void)tif;
    (void)s;
    (void)b;
    return 1;
}

static int dummy_postencode(TIFF *tif, uint8_t *buf, tmsize_t cc) {
    (void)tif;
    (void)buf;
    (void)cc;
    return 1;
}

static int dummy_fixuptags(TIFF *tif) {
    (void)tif;
    return 1;
}

static tmsize_t dummy_defstripsize(TIFF *tif, tmsize_t s) {
    (void)tif;
    return s;
}

static tmsize_t dummy_deftilesize(TIFF *tif, tmsize_t s) {
    (void)tif;
    return s;
}

static int dummy_postdecode(TIFF *tif, uint16_t s, uint8_t *buf, tmsize_t cc) {
    (void)tif;
    (void)s;
    (void)buf;
    (void)cc;
    return 1;
}

static TIFFCodec *g_registered_codec = NULL;

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    uint16_t scheme_id = Data[0] | (Data[1] << 8);
    if (scheme_id == 0) {
        scheme_id = 1;
    }

    // Test TIFFIsCODECConfigured
    int is_configured = TIFFIsCODECConfigured(scheme_id);
    (void)is_configured;

    // Test TIFFFindCODEC
    const TIFFCodec *codec = TIFFFindCODEC(scheme_id);
    (void)codec;

    // Test TIFFRegisterCODEC
    TIFFCodec *registered = TIFFRegisterCODEC(scheme_id, "FuzzCodec", dummy_init);
    if (registered != NULL) {
        g_registered_codec = registered;
        
        // Test TIFFUnRegisterCODEC
        TIFFUnRegisterCODEC(registered);
        g_registered_codec = NULL;
    }

    // Test TIFFGetConfiguredCODECs
    TIFFCodec *configured_codecs = TIFFGetConfiguredCODECs();
    if (configured_codecs != NULL) {
        // Use _TIFFfree as per the header definition found in the error message
        _TIFFfree(configured_codecs);
    }

    // Create a dummy file for TIFFReadFromUserBuffer testing
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp != NULL) {
        // Write minimal valid TIFF header (ClassicTIFF)
        // II (little endian) = 0x4949, Magic = 0x002A, Offset to first IFD = 8
        uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
        fwrite(header, 1, 8, fp);
        fclose(fp);

        TIFF *tif = TIFFOpen(dummy_file, "r");
        if (tif != NULL) {
            // Prepare buffers for TIFFReadFromUserBuffer
            uint8_t *inbuf = (uint8_t *)malloc(Size);
            uint8_t *outbuf = (uint8_t *)malloc(Size);
            
            if (inbuf != NULL && outbuf != NULL) {
                memcpy(inbuf, Data, Size);
                
                // Attempt to read from user buffer
                // This function requires a valid strip/tile and setup
                // We pass 0 for strip index, which might fail but is safe for fuzzing
                int result = TIFFReadFromUserBuffer(tif, 0, inbuf, Size, outbuf, Size);
                (void)result;
            }
            
            if (inbuf != NULL) free(inbuf);
            if (outbuf != NULL) free(outbuf);
            
            TIFFClose(tif);
        }
    }

    // Cleanup dummy file
    remove(dummy_file);

    return 0;
}