// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static cmsContext create_test_context(cmsContext parent) {
    return cmsCreateContext(NULL, parent);
}

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

static cmsHPROFILE create_dummy_profile(cmsContext ctx) {
    // Create a minimal dummy profile for testing
    // We use cmsOpenProfileFromMem which is the correct API for creating from memory
    // Construct a minimal valid ICC header (128 bytes minimum)
    unsigned char header[128] = {0};
    
    // Size (4 bytes) - 128 bytes = 0x00000080
    header[0] = 0x00; header[1] = 0x00; header[2] = 0x00; header[3] = 0x80;
    
    // Signature: "acsp" (4 bytes)
    header[4] = 'a'; header[5] = 'c'; header[6] = 's'; header[7] = 'p';
    
    // Version: 2.0 (4 bytes) = 0x020000
    header[8] = 0x02; header[9] = 0x00; header[10] = 0x00; header[11] = 0x00;
    
    // CMM Type: "lcms" (4 bytes)
    header[12] = 'l'; header[13] = 'c'; header[14] = 'm'; header[15] = 's';
    
    // Device Class: "mntr" (4 bytes)
    header[16] = 'm'; header[17] = 'n'; header[18] = 't'; header[19] = 'r';
    
    // ColorSpace: "XYZ " (4 bytes)
    header[20] = 'X'; header[21] = 'Y'; header[22] = 'Z'; header[23] = ' ';
    
    // PCS Connection Space: XYZ
    header[24] = 0x00; header[25] = 0x00; header[26] = 0x00; header[27] = 0x00; // XYZ
    header[28] = 0x00; header[29] = 0x00; header[30] = 0x00; header[31] = 0x00;
    
    // Profile Date and Time
    header[32] = 0x00; header[33] = 0x00; header[34] = 0x00; header[35] = 0x00;
    header[36] = 0x00; header[37] = 0x00; header[38] = 0x00; header[39] = 0x00;
    header[40] = 0x00; header[41] = 0x00; header[42] = 0x00; header[43] = 0x00;
    
    // Profile Version: 2.0
    header[44] = 0x02; header[45] = 0x00; header[46] = 0x00; header[47] = 0x00;
    
    // Device Manufacturer: "lcms"
    header[48] = 'l'; header[49] = 'c'; header[50] = 'm'; header[51] = 's';
    
    // Device Model: 0
    header[52] = 0x00; header[53] = 0x00; header[54] = 0x00; header[55] = 0x00;
    
    // Device Attributes
    header[56] = 0x00; header[57] = 0x00; header[58] = 0x00; header[59] = 0x00;
    header[60] = 0x00; header[61] = 0x00; header[62] = 0x00; header[63] = 0x00;
    
    // Rendering Intent
    header[64] = 0x00; header[65] = 0x00; header[66] = 0x00; header[67] = 0x00;
    
    // Connection Space Illuminant
    header[68] = 0x00; header[69] = 0x00; header[70] = 0x00; header[71] = 0x00;
    header[72] = 0x00; header[73] = 0x00; header[74] = 0x00; header[75] = 0x00;
    header[76] = 0x00; header[77] = 0x00; header[78] = 0x00; header[79] = 0x00;
    
    // Profile Creator: "lcms"
    header[80] = 'l'; header[81] = 'c'; header[82] = 'm'; header[83] = 's';
    
    // Profile ID (UUID) - zeros
    for (int i = 84; i < 128; i++) {
        header[i] = 0;
    }

    // Note: cmsOpenProfileFromMem takes (MemPtr, dwSize) and uses the global context or current context implicitly
    // We don't pass ctx as the first argument to cmsOpenProfileFromMem
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(header, 128);
    return hProfile;
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    cmsContext ctx = NULL;
    cmsHPROFILE hProfile = NULL;
    void *allocated_ptr = NULL;
    wchar_t buffer[256];
    cmsUInt32Number info_type;
    cmsUInt32Number buffer_size;
    char lang_code[4] = "en";
    char country_code[4] = "US";

    // Initialize context
    ctx = create_test_context(NULL);
    if (ctx == NULL) {
        return 0;
    }

    // Create a dummy profile for testing
    hProfile = create_dummy_profile(ctx);
    if (hProfile == NULL) {
        cleanup_context(ctx);
        return 0;
    }

    // Extract fuzzing parameters from input data
    if (Size >= 4) {
        info_type = Data[0];
        buffer_size = ((Data[1] << 24) | (Data[2] << 16) | (Data[3] << 8) | Data[4]);
        if (buffer_size > 256) {
            buffer_size = 256;
        }
    } else {
        info_type = cmsInfoDescription;
        buffer_size = 256;
    }

    // Initialize buffer
    memset(buffer, 0, sizeof(buffer));

    // First call: cmsGetProfileInfo
    cmsGetProfileInfo(hProfile, (cmsInfoType)info_type, lang_code, country_code, buffer, buffer_size);

    // Second call: _cmsMalloc
    cmsUInt32Number alloc_size = 0;
    if (Size >= 2) {
        alloc_size = (Data[0] << 8) | Data[1];
        if (alloc_size == 0) {
            alloc_size = 1;
        }
        if (alloc_size > 1024 * 1024) {
            alloc_size = 1024 * 1024;
        }
    } else {
        alloc_size = 64;
    }

    allocated_ptr = _cmsMalloc(ctx, alloc_size);
    if (allocated_ptr != NULL) {
        // Write some data to the allocated memory to ensure it's valid
        memset(allocated_ptr, 0xAB, alloc_size);
    }

    // Third call: cmsGetProfileInfo again with potentially different parameters
    cmsUInt32Number info_type2 = (Size >= 5) ? Data[4] : cmsInfoDescription;
    cmsGetProfileInfo(hProfile, (cmsInfoType)info_type2, lang_code, country_code, buffer, buffer_size);

    // Fourth call: _cmsFree
    _cmsFree(ctx, allocated_ptr);

    // Cleanup
    cmsCloseProfile(hProfile);
    cleanup_context(ctx);

    return 0;
}