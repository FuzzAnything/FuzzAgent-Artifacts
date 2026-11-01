// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFIsCODECConfigured at tif_codec.c:146:5 in tiffio.h
// TIFFFindCODEC at tif_compress.c:192:18 in tiffio.h
// TIFFGetConfiguredCODECs at tif_compress.c:261:12 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFRegisterCODEC at tif_compress.c:206:12 in tiffio.h
// TIFFFindCODEC at tif_compress.c:192:18 in tiffio.h
// TIFFUnRegisterCODEC at tif_compress.c:232:6 in tiffio.h
// TIFFFindCODEC at tif_compress.c:192:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadFromUserBuffer at tif_read.c:1545:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"

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

static void dummy_predecode(TIFF *tif) {
    (void)tif;
}

static void dummy_preencode(TIFF *tif) {
    (void)tif;
}

static void dummy_postencode(TIFF *tif) {
    (void)tif;
}

static int dummy_decoderow(TIFF *tif, uint8_t *buf, tmsize_t occ, int16_t s) {
    (void)tif;
    (void)buf;
    (void)occ;
    (void)s;
    return 0;
}

static int dummy_encoderow(TIFF *tif, uint8_t *buf, tmsize_t occ, int16_t s) {
    (void)tif;
    (void)buf;
    (void)occ;
    (void)s;
    return 0;
}

static int dummy_decodestrip(TIFF *tif, uint8_t *buf, tmsize_t occ, int16_t s) {
    (void)tif;
    (void)buf;
    (void)occ;
    (void)s;
    return 0;
}

static int dummy_encodestrip(TIFF *tif, uint8_t *buf, tmsize_t occ, int16_t s) {
    (void)tif;
    (void)buf;
    (void)occ;
    (void)s;
    return 0;
}

static int dummy_decodetile(TIFF *tif, uint8_t *buf, tmsize_t occ, int16_t s) {
    (void)tif;
    (void)buf;
    (void)occ;
    (void)s;
    return 0;
}

static int dummy_encodetile(TIFF *tif, uint8_t *buf, tmsize_t occ, int16_t s) {
    (void)tif;
    (void)buf;
    (void)occ;
    (void)s;
    return 0;
}

static int dummy_defstripsize(TIFF *tif, uint32_t *rows) {
    (void)tif;
    (void)rows;
    return 0;
}

static int dummy_deftilesize(TIFF *tif, uint32_t *row, uint32_t *col) {
    (void)tif;
    (void)row;
    (void)col;
    return 0;
}

static int dummy_fixuptags(TIFF *tif) {
    (void)tif;
    return 1;
}

static void dummy_seek(TIFF *tif, toff_t off) {
    (void)tif;
    (void)off;
}

static void dummy_close(TIFF *tif) {
    (void)tif;
}

static TIFFCodec *registered_codec = NULL;

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    uint16_t scheme_id = (uint16_t)((Data[0] << 8) | Data[1]);
    uint16_t scheme_id_2 = (uint16_t)((Data[Size - 2] << 8) | Data[Size - 1]);

    // 1. Test TIFFIsCODECConfigured
    int is_configured = TIFFIsCODECConfigured(scheme_id);
    (void)is_configured;

    // 2. Test TIFFFindCODEC
    const TIFFCodec *found_codec = TIFFFindCODEC(scheme_id);
    (void)found_codec;

    // 3. Test TIFFGetConfiguredCODECs
    TIFFCodec *configured_codecs = TIFFGetConfiguredCODECs();
    if (configured_codecs != NULL) {
        // Use _TIFFfree as per the header definition found in the error log
        _TIFFfree(configured_codecs);
    }

    // 4. Test TIFFRegisterCODEC and TIFFUnRegisterCODEC
    // Only register if we haven't already to avoid double registration issues in fuzzing
    if (registered_codec == NULL) {
        registered_codec = TIFFRegisterCODEC(scheme_id, "FuzzCodec", dummy_init);
    }
    
    if (registered_codec != NULL) {
        // Verify it was registered
        const TIFFCodec *check = TIFFFindCODEC(scheme_id);
        (void)check;
        
        // Test unregistration
        TIFFUnRegisterCODEC(registered_codec);
        registered_codec = NULL;
        
        // Verify it is gone
        check = TIFFFindCODEC(scheme_id);
        (void)check;
    }

    // 5. Test TIFFReadFromUserBuffer
    // We need a valid TIFF handle. Create a dummy file first.
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        // Write a minimal valid TIFF header (Classic)
        // Magic: 0x002A (Little endian)
        uint16_t magic = 0x002A; 
        uint32_t offset = 8;
        fwrite(&magic, sizeof(magic), 1, fp);
        fwrite(&offset, sizeof(offset), 1, fp);
        // Pad to offset 8
        uint8_t padding[4] = {0};
        fwrite(padding, 1, 4, fp);
        // Write a minimal IFD (0 entries)
        uint16_t entries = 0;
        uint32_t next_ifd = 0;
        fwrite(&entries, sizeof(entries), 1, fp);
        fwrite(&next_ifd, sizeof(next_ifd), 1, fp);
        fclose(fp);
    }

    TIFF *tif = TIFFOpen(dummy_file, "r");
    if (tif != NULL) {
        // Prepare buffer for TIFFReadFromUserBuffer
        size_t in_size = Size > 100 ? 100 : Size;
        uint8_t *inbuf = (uint8_t *)malloc(in_size);
        if (inbuf) {
            memcpy(inbuf, Data, in_size);
            
            // Output buffer
            uint8_t *outbuf = (uint8_t *)malloc(1024);
            if (outbuf) {
                // Try to read from user buffer
                int result = TIFFReadFromUserBuffer(tif, 0, inbuf, in_size, outbuf, 1024);
                (void)result;
                free(outbuf);
            }
            free(inbuf);
        }
        TIFFClose(tif);
    }

    // Cleanup dummy file
    remove(dummy_file);

    return 0;
}