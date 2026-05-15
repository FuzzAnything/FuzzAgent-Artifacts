// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFRegisterCODEC at tif_compress.c:206:12 in tiffio.h
// TIFFFindCODEC at tif_compress.c:192:18 in tiffio.h
// TIFFIsCODECConfigured at tif_codec.c:146:5 in tiffio.h
// TIFFGetConfiguredCODECs at tif_compress.c:261:12 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFUnRegisterCODEC at tif_compress.c:232:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "libtiff/tiffio.h"

static int dummy_init(TIFF* tif, int scheme) {
    return 0;
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Create dummy TIFF file
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }
    write(fd, Data, Size);
    close(fd);

    // Open TIFF file
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    
    // Test TIFFRegisterCODEC
    uint16_t codec_id = (Data[0] << 8) | Data[1];
    char codec_name[32];
    snprintf(codec_name, sizeof(codec_name), "CustomCodec_%u", codec_id);
    
    TIFFCodec* registered_codec = TIFFRegisterCODEC(codec_id, codec_name, dummy_init);
    
    // Test TIFFFindCODEC
    const TIFFCodec* found_codec = TIFFFindCODEC(codec_id);
    
    // Test TIFFIsCODECConfigured
    int is_configured = TIFFIsCODECConfigured(codec_id);
    
    // Test TIFFGetConfiguredCODECs
    TIFFCodec* codec_list = TIFFGetConfiguredCODECs();
    if (codec_list) {
        _TIFFfree(codec_list);
    }
    
    // Test TIFFReadRGBAStrip if we have a valid TIFF
    if (tif) {
        uint32_t strip = 0;
        if (Size > 4) {
            strip = (Data[2] << 24) | (Data[3] << 16) | (Data[4] << 8) | Data[5];
        }
        
        uint32_t raster_size = TIFFStripSize(tif);
        if (raster_size > 0 && raster_size < 10 * 1024 * 1024) {
            uint32_t* raster = (uint32_t*)_TIFFmalloc(raster_size);
            if (raster) {
                TIFFReadRGBAStrip(tif, strip, raster);
                _TIFFfree(raster);
            }
        }
        
        TIFFClose(tif);
    }
    
    // Test TIFFUnRegisterCODEC
    if (registered_codec) {
        TIFFUnRegisterCODEC(registered_codec);
    }
    
    // Clean up
    unlink("./dummy_file");
    
    return 0;
}