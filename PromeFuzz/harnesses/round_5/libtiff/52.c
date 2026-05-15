// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFRegisterCODEC at tif_compress.c:206:12 in tiffio.h
// TIFFIsCODECConfigured at tif_codec.c:146:5 in tiffio.h
// TIFFFindCODEC at tif_compress.c:192:18 in tiffio.h
// TIFFGetConfiguredCODECs at tif_compress.c:261:12 in tiffio.h
// TIFFUnRegisterCODEC at tif_compress.c:232:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int dummy_init(TIFF* tif, int scheme) {
    return 1;
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    // Write data to dummy file for TIFF operations
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Test TIFFRegisterCODEC
    if (Size >= 3) {
        uint16_t codec_id = *(const uint16_t*)Data;
        const char *codec_name = (const char*)(Data + 2);
        size_t name_len = Size - 2;
        char *name_copy = malloc(name_len + 1);
        if (name_copy) {
            memcpy(name_copy, codec_name, name_len);
            name_copy[name_len] = '\0';
            
            TIFFCodec *registered = TIFFRegisterCODEC(codec_id, name_copy, dummy_init);
            free(name_copy);
            
            // Test TIFFIsCODECConfigured
            int configured = TIFFIsCODECConfigured(codec_id);
            
            // Test TIFFFindCODEC
            const TIFFCodec *found = TIFFFindCODEC(codec_id);
            
            // Test TIFFGetConfiguredCODECs
            TIFFCodec *codecs = TIFFGetConfiguredCODECs();
            if (codecs) {
                free(codecs);
            }
            
            // Test TIFFUnRegisterCODEC
            if (registered) {
                TIFFUnRegisterCODEC(registered);
            }
        }
    }
    
    // Test TIFFReadRGBAStrip with a dummy TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        uint32_t strip = 0;
        uint32_t *raster = NULL;
        
        // Try to get image dimensions to allocate proper raster size
        uint32_t width = 0, height = 0;
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
        
        if (width > 0 && height > 0) {
            // Allocate raster for at least one row
            raster = (uint32_t *)malloc(width * sizeof(uint32_t));
            if (raster) {
                TIFFReadRGBAStrip(tif, strip, raster);
                free(raster);
            }
        }
        
        TIFFClose(tif);
    }
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}