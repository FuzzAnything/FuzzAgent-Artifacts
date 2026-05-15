// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFRegisterCODEC at tif_compress.c:206:12 in tiffio.h
// TIFFFindCODEC at tif_compress.c:192:18 in tiffio.h
// TIFFUnRegisterCODEC at tif_compress.c:232:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFMergeFieldInfo at tif_dirinfo.c:1147:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFGetConfiguredCODECs at tif_compress.c:261:12 in tiffio.h
// TIFFIsCODECConfigured at tif_codec.c:146:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "libtiff/tiffio.h"

static int dummy_init(TIFF* tif, int scheme) {
    return 0;
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Create dummy TIFF file for operations that need a TIFF*
    FILE* dummy = fopen("./dummy_file", "wb");
    if (!dummy) {
        return 0;
    }
    fwrite(Data, 1, Size, dummy);
    fclose(dummy);

    // Use first 2 bytes for compression scheme
    uint16_t scheme = (Data[0] << 8) | Data[1];
    
    // Use next bytes for codec name (if available)
    const char* codec_name = "FUZZCODEC";
    if (Size > 2) {
        // Use up to 32 bytes for name, ensure null termination
        size_t name_len = Size - 2;
        if (name_len > 32) name_len = 32;
        char* name_buf = malloc(name_len + 1);
        if (name_buf) {
            memcpy(name_buf, Data + 2, name_len);
            name_buf[name_len] = '\0';
            codec_name = name_buf;
        }
    }

    // 1. Test TIFFRegisterCODEC
    TIFFCodec* registered_codec = TIFFRegisterCODEC(scheme, codec_name, dummy_init);
    
    // 2. Test TIFFFindCODEC
    const TIFFCodec* found_codec = TIFFFindCODEC(scheme);
    
    // 3. Test TIFFUnRegisterCODEC (if we registered successfully)
    if (registered_codec) {
        TIFFUnRegisterCODEC(registered_codec);
    }
    
    // 4. Test TIFFMergeFieldInfo - need a TIFF* first
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        // Create dummy field info array
        TIFFFieldInfo field_info[1];
        field_info[0].field_tag = 65000; // Custom tag
        field_info[0].field_readcount = 1;
        field_info[0].field_writecount = 1;
        field_info[0].field_type = TIFF_SHORT;
        field_info[0].field_bit = 0;
        field_info[0].field_oktochange = 1;
        field_info[0].field_passcount = 0;
        field_info[0].field_name = "FuzzField";
        
        TIFFMergeFieldInfo(tif, field_info, 1);
        TIFFClose(tif);
    }
    
    // 5. Test TIFFGetConfiguredCODECs
    TIFFCodec* codecs = TIFFGetConfiguredCODECs();
    if (codecs) {
        free(codecs);
    }
    
    // 6. Test TIFFIsCODECConfigured
    int is_configured = TIFFIsCODECConfigured(scheme);
    
    // Clean up
    if (Size > 2 && codec_name != "FUZZCODEC") {
        free((void*)codec_name);
    }
    
    remove("./dummy_file");
    
    return 0;
}