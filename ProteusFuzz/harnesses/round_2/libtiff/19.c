// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReadFromUserBuffer at tif_read.c:1545:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFIsCODECConfigured at tif_codec.c:146:5 in tiffio.h
// TIFFFindCODEC at tif_compress.c:192:18 in tiffio.h
// TIFFGetConfiguredCODECs at tif_compress.c:261:12 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tiffio.h>

static void dummy_error_handler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warn_handler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    uint16_t scheme_id = 0;
    if (Size >= 2) {
        scheme_id = (uint16_t)((Data[0] << 8) | Data[1]);
    } else if (Size == 1) {
        scheme_id = Data[0];
    }

    // Test TIFFIsCODECConfigured
    int is_configured = TIFFIsCODECConfigured(scheme_id);
    (void)is_configured;

    // Test TIFFFindCODEC
    const TIFFCodec *codec = TIFFFindCODEC(scheme_id);
    (void)codec;

    // Test TIFFGetConfiguredCODECs
    TIFFCodec *configured_codecs = TIFFGetConfiguredCODECs();
    if (configured_codecs != NULL) {
        // Use _TIFFfree instead of _TIFFfreeExt as per the error message
        _TIFFfree(configured_codecs);
    }

    // Test TIFFReadFromUserBuffer
    TIFF *tif = NULL;
    
    // Create a dummy file for testing
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp != NULL) {
        // Write minimal TIFF header
        uint16_t magic = 0x4949; // Little endian
        uint16_t offset = 8;
        fwrite(&magic, sizeof(uint16_t), 1, fp);
        fwrite(&offset, sizeof(uint16_t), 1, fp);
        // Write dummy IFD
        uint16_t num_entries = 0;
        uint32_t next_ifd = 0;
        fwrite(&num_entries, sizeof(uint16_t), 1, fp);
        fwrite(&next_ifd, sizeof(uint32_t), 1, fp);
        fclose(fp);
    }

    // Try to open the file
    tif = TIFFOpen("./dummy_file", "r");
    if (tif != NULL) {
        // Set error handlers to avoid noise
        TIFFSetErrorHandler(dummy_error_handler);
        TIFFSetWarningHandler(dummy_warn_handler);

        // Prepare buffers for TIFFReadFromUserBuffer
        uint32_t strile = 0;
        tmsize_t insize = (Size > 1024) ? 1024 : Size;
        tmsize_t outsize = (Size > 1024) ? 1024 : Size;
        
        uint8_t *inbuf = NULL;
        uint8_t *outbuf = NULL;
        
        if (insize > 0) {
            inbuf = (uint8_t *)malloc(insize);
            if (inbuf != NULL) {
                memcpy(inbuf, Data, insize);
            }
        }
        
        if (outsize > 0) {
            outbuf = (uint8_t *)malloc(outsize);
            if (outbuf == NULL) {
                free(inbuf);
                TIFFClose(tif);
                remove("./dummy_file");
                return 0;
            }
        }

        if (inbuf != NULL && outbuf != NULL) {
            // Call the target function
            TIFFReadFromUserBuffer(tif, strile, inbuf, insize, outbuf, outsize);
        }

        free(inbuf);
        free(outbuf);
        TIFFClose(tif);
    }

    // Cleanup dummy file
    remove("./dummy_file");

    return 0;
}