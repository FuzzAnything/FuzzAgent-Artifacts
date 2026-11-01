// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsSetHeaderFlags at cmsio0.c:1002:16 in lcms2.h
// cmsSetEncodedICCversion at cmsio0.c:1111:16 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsSetHeaderManufacturer at cmsio0.c:1014:16 in lcms2.h
// cmsGetHeaderCreator at cmsio0.c:1020:27 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// _cmsAdjustEndianess32 at cmsplugin.c:58:28 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static cmsUInt32Number extract_u32(const uint8_t *data, size_t size, size_t offset) {
    if (offset + 4 > size) return 0;
    cmsUInt32Number val = 0;
    memcpy(&val, data + offset, 4);
    return val;
}

static cmsUInt32Number extract_u8_as_u32(const uint8_t *data, size_t size, size_t offset) {
    if (offset >= size) return 0;
    return (cmsUInt32Number)data[offset];
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    cmsContext *ctx = cmsCreateContext(NULL, 0);
    if (!ctx) return 0;

    cmsHPROFILE hProfile = NULL;
    cmsUInt32Number flags = extract_u32(Data, Size, 0);
    cmsUInt32Number version = extract_u32(Data, Size, 4);
    cmsUInt32Number intent = extract_u8_as_u32(Data, Size, 8) % 4;
    cmsUInt32Number manufacturer = extract_u32(Data, Size, 12);
    cmsUInt32Number endian_val = extract_u32(Data, Size, 16);

    // Create a dummy profile using cmsCreateProfilePlaceholder
    hProfile = cmsCreateProfilePlaceholder(ctx);

    if (hProfile) {
        // Test cmsSetHeaderFlags
        cmsSetHeaderFlags(hProfile, flags);
        
        // Test cmsSetEncodedICCversion
        cmsSetEncodedICCversion(hProfile, version);
        
        // Test cmsSetHeaderRenderingIntent
        cmsSetHeaderRenderingIntent(hProfile, intent);
        
        // Test cmsSetHeaderManufacturer
        cmsSetHeaderManufacturer(hProfile, manufacturer);
        
        // Test cmsGetHeaderCreator (should not crash if profile is valid)
        cmsUInt32Number creator = cmsGetHeaderCreator(hProfile);
        (void)creator;
        
        // Clean up profile
        cmsCloseProfile(hProfile);
    }

    // Test _cmsAdjustEndianess32 (stateless function)
    cmsUInt32Number adjusted = _cmsAdjustEndianess32(endian_val);
    (void)adjusted;

    // Clean up context
    cmsDeleteContext(ctx);

    return 0;
}