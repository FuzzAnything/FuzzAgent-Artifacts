// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
// TIFFRGBAImageEnd at tif_getimage.c:253:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFIsCODECConfigured at tif_codec.c:146:5 in tiffio.h
// TIFFRGBAImageOK at tif_getimage.c:83:5 in tiffio.h
// TIFFRGBAImageBegin at tif_getimage.c:310:5 in tiffio.h
// TIFFRGBAImageGet at tif_getimage.c:587:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <tiffio.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write minimal valid TIFF header if input is empty to avoid immediate crash on open
            uint16_t header[8];
            header[0] = 0x4949; // II (Little Endian)
            header[1] = 42;     // Magic number
            header[2] = 8;      // Offset to first IFD
            header[3] = 0;
            header[4] = 0;
            header[5] = 0;
            header[6] = 0;
            header[7] = 0;
            fwrite(header, sizeof(uint16_t), 8, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment: Write fuzz data to a dummy file
    write_dummy_file(Data, Size);

    // Open the file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    char emsg[1024];
    int ret;

    // 1. Test TIFFIsCODECConfigured
    // Fuzz a compression scheme ID derived from input data
    uint16_t codec_id = 0;
    if (Size >= 2) {
        codec_id = Data[0] | (Data[1] << 8);
    }
    (void)TIFFIsCODECConfigured(codec_id);

    // 2. Test TIFFRGBAImageOK
    ret = TIFFRGBAImageOK(tif, emsg);
    // No specific action needed on return, just exercise the path

    // 3. Test TIFFRGBAImageBegin
    TIFFRGBAImage img;
    memset(&img, 0, sizeof(img));
    
    // Determine orientation from input if possible, else default
    int orientation = 1; // Default to bottom-up
    if (Size > 2) {
        orientation = Data[2] % 8 + 1; // Map to valid range 1-8
    }

    ret = TIFFRGBAImageBegin(&img, tif, orientation, emsg);
    if (ret == 0) {
        // If begin fails, we cannot safely call Get or ReadRGBAImageOriented with this struct
        // But we can still try ReadRGBAImageOriented directly on the TIFF handle
    }

    // 4. Test TIFFRGBAImageGet (only if Begin succeeded)
    if (ret != 0) {
        uint32_t *buf = NULL;
        uint32_t w = 0, h = 0;
        
        if (Size >= 4) {
            w = Data[3] | (Data[4] << 8);
            h = Data[5] | (Data[6] << 8);
        }
        
        // Prevent excessive allocation
        if (w > 10000 || h > 10000) {
            w = 100; h = 100;
        }
        if (w == 0) w = 1;
        if (h == 0) h = 1;

        buf = (uint32_t *)malloc(w * h * sizeof(uint32_t));
        if (buf) {
            TIFFRGBAImageGet(&img, buf, w, h);
            free(buf);
        }
    }

    // 5. Test TIFFReadRGBAImageOriented
    // Extract dimensions and orientation from input
    uint32_t rw = 0, rh = 0;
    int ro = 1;
    
    if (Size >= 6) {
        rw = Data[3] | (Data[4] << 8);
        rh = Data[5] | (Data[6] << 8);
        ro = (Data[7] % 8) + 1;
    }
    
    // Cap dimensions to prevent OOM
    if (rw > 10000) rw = 100;
    if (rh > 10000) rh = 100;
    if (rw == 0) rw = 1;
    if (rh == 0) rh = 1;

    uint32_t *rgba_buf = (uint32_t *)malloc(rw * rh * sizeof(uint32_t));
    if (rgba_buf) {
        TIFFReadRGBAImageOriented(tif, rw, rh, rgba_buf, ro, 0);
        free(rgba_buf);
    }

    // 6. Test TIFFReadRGBAStripExt
    // Requires strip alignment, which is hard to guarantee with fuzz data.
    // We attempt it with a row index derived from input.
    uint32_t strip_row = 0;
    if (Size > 8) {
        strip_row = Data[8];
    }
    
    // We need a buffer for the strip. Estimate size based on width.
    // Since we don't know the exact strip size, we allocate a reasonable buffer.
    if (rw > 0) {
        uint32_t *strip_buf = (uint32_t *)malloc(rw * 100 * sizeof(uint32_t)); // Arbitrary height for strip
        if (strip_buf) {
            // stop_on_error: 0 or 1
            int stop_on_error = (Size > 9) ? (Data[9] & 1) : 0;
            TIFFReadRGBAStripExt(tif, strip_row, strip_buf, stop_on_error);
            free(strip_buf);
        }
    }

    // Cleanup
    TIFFRGBAImageEnd(&img);
    TIFFClose(tif);

    // Cleanup dummy file
    remove("./dummy_file");

    return 0;
}