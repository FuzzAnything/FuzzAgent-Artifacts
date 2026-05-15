// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFMergeFieldInfo at tif_dirinfo.c:1147:5 in tiffio.h
// TIFFRegisterCODEC at tif_compress.c:206:12 in tiffio.h
// TIFFGetConfiguredCODECs at tif_compress.c:261:12 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFFindCODEC at tif_compress.c:192:18 in tiffio.h
// TIFFIsCODECConfigured at tif_codec.c:146:5 in tiffio.h
// TIFFUnRegisterCODEC at tif_compress.c:232:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFIsCODECConfigured at tif_codec.c:146:5 in tiffio.h
// TIFFFindCODEC at tif_compress.c:192:18 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "libtiff/tiffio.h"

static int dummy_init(TIFF* tif, int scheme)
{
    (void)tif;
    (void)scheme;
    return 1;
}

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size)
{
    if (Size < 2)
        return 0;

    // Create a dummy TIFF file for functions that need it
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp)
        return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open the dummy TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif)
    {
        remove("./dummy_file");
        return 0;
    }

    // Test TIFFMergeFieldInfo
    // Create some dummy field info structures
    TIFFFieldInfo field_info[2];
    field_info[0].field_tag = 65000;
    field_info[0].field_readcount = 1;
    field_info[0].field_writecount = 1;
    field_info[0].field_type = TIFF_SHORT;
    field_info[0].field_name = "CustomField1";
    
    field_info[1].field_tag = 65001;
    field_info[1].field_readcount = 1;
    field_info[1].field_writecount = 1;
    field_info[1].field_type = TIFF_ASCII;
    field_info[1].field_name = "CustomField2";
    
    TIFFMergeFieldInfo(tif, field_info, 2);

    // Test TIFFRegisterCODEC
    uint16_t codec_id = (Data[0] << 8) | Data[1];
    char codec_name[32];
    snprintf(codec_name, sizeof(codec_name), "CustomCodec_%u", codec_id);
    
    TIFFCodec *custom_codec = TIFFRegisterCODEC(codec_id, codec_name, dummy_init);
    
    // Test TIFFGetConfiguredCODECs
    TIFFCodec *codec_list = TIFFGetConfiguredCODECs();
    if (codec_list)
    {
        // Free the codec list as required
        _TIFFfree(codec_list);
    }

    // Test TIFFFindCODEC
    const TIFFCodec *found_codec = TIFFFindCODEC(codec_id);
    (void)found_codec; // Mark as used
    
    // Test TIFFIsCODECConfigured
    int is_configured = TIFFIsCODECConfigured(codec_id);
    (void)is_configured; // Mark as used
    
    // Test TIFFUnRegisterCODEC if we registered one
    if (custom_codec)
    {
        TIFFUnRegisterCODEC(custom_codec);
    }

    // Clean up
    TIFFClose(tif);
    remove("./dummy_file");

    // Test with another compression scheme ID
    if (Size >= 4)
    {
        uint16_t another_id = (Data[2] << 8) | Data[3];
        TIFFIsCODECConfigured(another_id);
        TIFFFindCODEC(another_id);
    }

    return 0;
}