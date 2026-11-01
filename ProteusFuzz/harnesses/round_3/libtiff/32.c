// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFindCODEC at tif_compress.c:192:18 in tiffio.h
// TIFFIsCODECConfigured at tif_codec.c:146:5 in tiffio.h
// TIFFGetConfiguredCODECs at tif_compress.c:261:12 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFRegisterCODEC at tif_compress.c:206:12 in tiffio.h
// TIFFUnRegisterCODEC at tif_compress.c:232:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFReadFromUserBuffer at tif_read.c:1545:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <tiffio.h>

static void dummy_error_handler(void *client_data, const char *module, const char *fmt, va_list ap) {
    (void)client_data;
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warn_handler(void *client_data, const char *module, const char *fmt, va_list ap) {
    (void)client_data;
    (void)module;
    (void)fmt;
    (void)ap;
}

static TIFFCodec *g_registered_codec = NULL;

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    // 1. Prepare environment
    TIFF *tif = NULL;
    uint8_t *inbuf = NULL;
    uint8_t *outbuf = NULL;
    tmsize_t insize = 0;
    tmsize_t outsize = 0;
    uint32_t strile = 0;

    // Parse input for parameters
    if (Size >= 12) {
        insize = *(const uint32_t *)(Data + 4);
        outsize = *(const uint32_t *)(Data + 8);
        strile = *(const uint32_t *)(Data + 0);
    }

    // Clamp sizes to reasonable limits to avoid OOM
    if (insize > 1024 * 1024) insize = 1024 * 1024;
    if (outsize > 1024 * 1024) outsize = 1024 * 1024;

    // Allocate buffers
    if (insize > 0 && Size > 12) {
        inbuf = (uint8_t *)malloc(insize);
        if (inbuf) {
            size_t copy_len = (Size > 12) ? (Size - 12) : 0;
            if (copy_len > insize) copy_len = insize;
            memcpy(inbuf, Data + 12, copy_len);
        }
    }

    if (outsize > 0) {
        outbuf = (uint8_t *)malloc(outsize);
        if (!outbuf) {
            free(inbuf);
            return 0;
        }
    }

    // Create a dummy file for TIFFOpen
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        // Write a minimal TIFF header (4 bytes magic + 2 bytes offset + 2 bytes zero)
        // Little-endian: 0x49 0x49 (II), 0x2A 0x00 (42), 0x08 0x00 (offset 8)
        uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
        fwrite(header, 1, 8, fp);
        // Write some dummy data to fill the file
        if (Size > 12) {
            fwrite(Data + 12, 1, Size - 12, fp);
        }
        fclose(fp);
    }

    // Open TIFF
    tif = TIFFOpen(dummy_file, "rb");
    if (tif) {
        // Set error handlers to prevent aborts
        TIFFSetErrorHandlerExt(dummy_error_handler);
        TIFFSetWarningHandlerExt(dummy_warn_handler);

        // 2. Test TIFFReadFromUserBuffer
        // This function requires a valid strip/tile and setup.
        // We pass the user buffer to simulate reading from memory.
        if (inbuf && insize > 0 && outbuf && outsize > 0) {
            TIFFReadFromUserBuffer(tif, strile, inbuf, insize, outbuf, outsize);
        }

        // 3. Test Codec Functions
        // TIFFFindCODEC
        uint16_t scheme_id = 0;
        if (Size >= 16) {
            scheme_id = *(const uint16_t *)(Data + 14);
        }
        const TIFFCodec *codec = TIFFFindCODEC(scheme_id);
        (void)codec; // Suppress unused warning

        // TIFFIsCODECConfigured
        int is_configured = TIFFIsCODECConfigured(scheme_id);
        (void)is_configured;

        // TIFFGetConfiguredCODECs
        TIFFCodec *codecs = TIFFGetConfiguredCODECs();
        if (codecs) {
            // Use _TIFFfree as the public API to free the array
            _TIFFfree(codecs);
        }

        // TIFFRegisterCODEC and TIFFUnRegisterCODEC
        // We register a dummy codec if not already registered
        if (g_registered_codec == NULL) {
            // Use a unique scheme ID from input or a fixed one
            uint16_t reg_scheme = (Size >= 18) ? *(const uint16_t *)(Data + 16) : 0xFFFF;
            // Register a dummy codec (init function is NULL for testing purposes, might fail init later but registration works)
            g_registered_codec = TIFFRegisterCODEC(reg_scheme, "FuzzTestCodec", NULL);
        }

        if (g_registered_codec) {
            // Unregister it
            TIFFUnRegisterCODEC(g_registered_codec);
            g_registered_codec = NULL;
        }
        
        // Cleanup TIFF
        TIFFClose(tif);
    }

    // 4. Cleanup
    free(inbuf);
    free(outbuf);
    
    // Remove dummy file
    unlink(dummy_file);

    return 0;
}