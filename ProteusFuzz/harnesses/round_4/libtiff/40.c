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

/* Dummy codec functions matching libtiff signatures */
static int dummy_init(TIFF *tif, int scheme) {
    (void)tif;
    (void)scheme;
    return 0;
}

static void dummy_cleanup(TIFF *tif) {
    (void)tif;
}

static int dummy_setupdecode(TIFF *tif, uint32_t s) {
    (void)tif;
    (void)s;
    return 0;
}

static int dummy_setupencode(TIFF *tif, uint32_t s) {
    (void)tif;
    (void)s;
    return 0;
}

static void dummy_predecode(TIFF *tif) {
    (void)tif;
}

static void dummy_preencode(TIFF *tif) {
    (void)tif;
}

static void dummy_postencode(TIFF *tif) {
    (void)tif;
}

static int dummy_decoderow(TIFF *tif, uint8_t *buf, tmsize_t len, int16_t s) {
    (void)tif;
    (void)buf;
    (void)len;
    (void)s;
    return 0;
}

static int dummy_encoderow(TIFF *tif, const uint8_t *buf, tmsize_t len, int16_t s) {
    (void)tif;
    (void)buf;
    (void)len;
    (void)s;
    return 0;
}

static int dummy_decodestrip(TIFF *tif, uint8_t *buf, tmsize_t len, int16_t s) {
    (void)tif;
    (void)buf;
    (void)len;
    (void)s;
    return 0;
}

static int dummy_encodestrip(TIFF *tif, const uint8_t *buf, tmsize_t len, int16_t s) {
    (void)tif;
    (void)buf;
    (void)len;
    (void)s;
    return 0;
}

static int dummy_decodetile(TIFF *tif, uint8_t *buf, tmsize_t len, int16_t s) {
    (void)tif;
    (void)buf;
    (void)len;
    (void)s;
    return 0;
}

static int dummy_encodetile(TIFF *tif, const uint8_t *buf, tmsize_t len, int16_t s) {
    (void)tif;
    (void)buf;
    (void)len;
    (void)s;
    return 0;
}

static int dummy_fixuptags(TIFF *tif) {
    (void)tif;
    return 0;
}

static TIFFCodec *g_registered_codec = NULL;

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    uint16_t scheme_id;
    uint32_t strile;
    uint32_t outsize;
    uint32_t insize;

    // Extract parameters from fuzz data safely
    if (Size >= 2) {
        scheme_id = Data[0] | (Data[1] << 8);
    } else {
        scheme_id = 0;
    }

    if (Size >= 6) {
        strile = Data[2] | (Data[3] << 8) | (Data[4] << 16) | (Data[5] << 24);
    } else {
        strile = 0;
    }

    if (Size >= 10) {
        outsize = Data[6] | (Data[7] << 8) | (Data[8] << 16) | (Data[9] << 24);
    } else {
        outsize = 0;
    }

    if (Size >= 14) {
        insize = Data[10] | (Data[11] << 8) | (Data[12] << 16) | (Data[13] << 24);
    } else {
        insize = 0;
    }

    // Ensure sizes are reasonable to avoid allocation failures or crashes
    if (insize > 1024 * 1024) insize = 1024 * 1024;
    if (outsize > 1024 * 1024) outsize = 1024 * 1024;

    // 1. Test TIFFGetConfiguredCODECs
    TIFFCodec *configured_codecs = TIFFGetConfiguredCODECs();
    if (configured_codecs != NULL) {
        _TIFFfree(configured_codecs);
    }

    // 2. Test TIFFIsCODECConfigured
    int is_configured = TIFFIsCODECConfigured(scheme_id);
    (void)is_configured;

    // 3. Test TIFFFindCODEC
    const TIFFCodec *found_codec = TIFFFindCODEC(scheme_id);
    (void)found_codec;

    // 4. Test TIFFRegisterCODEC and TIFFUnRegisterCODEC
    // Only register if not already registered to avoid double free or logic errors
    if (g_registered_codec == NULL) {
        uint16_t unique_id = 0xFFFF; 
        if (Size >= 2) {
            unique_id = (Data[0] << 8) | Data[1];
            if (unique_id < 100) unique_id += 0xFFFF; // Push to high range
        }

        g_registered_codec = TIFFRegisterCODEC(unique_id, "FuzzCodec", dummy_init);
    }

    if (g_registered_codec != NULL) {
        // Try to unregister
        TIFFUnRegisterCODEC(g_registered_codec);
        g_registered_codec = NULL;
    }

    // 5. Test TIFFReadFromUserBuffer
    // Create a dummy file to open
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        // Write minimal valid TIFF header (Classic)
        uint16_t magic = 0x4949; // Little endian
        uint16_t offset = 8;
        fwrite(&magic, sizeof(uint16_t), 1, fp);
        fwrite(&offset, sizeof(uint16_t), 1, fp);
        // Write a minimal IFD (Image File Directory)
        uint16_t entry_count = 0; // Empty IFD
        uint32_t next_ifd = 0;
        fwrite(&entry_count, sizeof(uint16_t), 1, fp);
        fwrite(&next_ifd, sizeof(uint32_t), 1, fp);
        fclose(fp);
    }

    TIFF *tif = TIFFOpen(dummy_file, "r");
    if (tif) {
        // Allocate buffers for reading
        uint8_t *inbuf = NULL;
        uint8_t *outbuf = NULL;

        if (insize > 0 && Size >= 14) {
            inbuf = (uint8_t *)malloc(insize);
            if (inbuf) {
                memcpy(inbuf, Data + 14, Size > 14 ? (Size - 14) : 0);
            }
        }

        if (outsize > 0) {
            outbuf = (uint8_t *)malloc(outsize);
        }

        if (inbuf && outbuf) {
            int ret = TIFFReadFromUserBuffer(tif, strile, inbuf, insize, outbuf, outsize);
            (void)ret;
        }

        if (inbuf) free(inbuf);
        if (outbuf) free(outbuf);

        TIFFClose(tif);
    }

    // Cleanup dummy file
    remove(dummy_file);

    return 0;
}