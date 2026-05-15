// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFRegisterCODEC at tif_compress.c:206:12 in tiffio.h
// TIFFFindCODEC at tif_compress.c:192:18 in tiffio.h
// TIFFIsCODECConfigured at tif_codec.c:146:5 in tiffio.h
// TIFFGetConfiguredCODECs at tif_compress.c:261:12 in tiffio.h
// TIFFMergeFieldInfo at tif_dirinfo.c:1147:5 in tiffio.h
// TIFFUnRegisterCODEC at tif_compress.c:232:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static int dummy_init(TIFF* tif, int scheme) {
    return 0;
}

static void dummy_cleanup() {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Create dummy TIFF file
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;
    write(fd, Data, Size);
    close(fd);
    
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        dummy_cleanup();
        return 0;
    }
    
    // Test TIFFRegisterCODEC with fuzzed scheme and name
    uint16_t scheme = *(uint16_t*)Data;
    const char* name = (const char*)(Data + 2);
    size_t name_len = Size - 2;
    if (name_len > 100) name_len = 100;
    char codec_name[101];
    memcpy(codec_name, name, name_len);
    codec_name[name_len] = '\0';
    
    TIFFCodec* registered_codec = TIFFRegisterCODEC(scheme, codec_name, dummy_init);
    
    // Test TIFFFindCODEC
    const TIFFCodec* found_codec = TIFFFindCODEC(scheme);
    
    // Test TIFFIsCODECConfigured
    int is_configured = TIFFIsCODECConfigured(scheme);
    
    // Test TIFFGetConfiguredCODECs
    TIFFCodec* codecs = TIFFGetConfiguredCODECs();
    if (codecs) free(codecs);
    
    // Test TIFFMergeFieldInfo with dummy field info
    TIFFFieldInfo field_info[1];
    if (Size >= sizeof(TIFFFieldInfo)) {
        memcpy(field_info, Data, sizeof(TIFFFieldInfo));
        int merge_result = TIFFMergeFieldInfo(tif, field_info, 1);
        (void)merge_result; // Use result to avoid unused variable warning
    }
    
    // Test TIFFUnRegisterCODEC if we successfully registered one
    if (registered_codec) {
        TIFFUnRegisterCODEC(registered_codec);
    }
    
    TIFFClose(tif);
    dummy_cleanup();
    
    return 0;
}