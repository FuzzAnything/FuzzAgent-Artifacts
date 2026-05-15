// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsPluginTHR at cmsplugin.c:546:19 in lcms2.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

// Helper function to create a dummy tone curve from fuzzing data
static cmsToneCurve* create_dummy_tone_curve(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < 256) return NULL;
    
    cmsUInt16Number table[256];
    for (int i = 0; i < 256; i++) {
        table[i] = (cmsUInt16Number)((data[i] << 8) | data[(i + 128) % size]);
    }
    
    cmsToneCurve* curve = cmsBuildTabulatedToneCurve16(ctx, 256, table);
    return curve;
}

// Simple file-based I/O handler for testing
static cmsIOHANDLER* create_file_iohandler(cmsContext ctx, const uint8_t* data, size_t size) {
    // Write data to dummy file
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return NULL;
    
    fwrite(data, 1, size, fp);
    fclose(fp);
    
    // Open file for reading via lcms
    cmsIOHANDLER* io = cmsOpenIOhandlerFromFile(ctx, "./dummy_file", "r");
    return io;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    // Create a context first
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Test cmsCreate_sRGBProfileTHR
    cmsHPROFILE srgb_profile = cmsCreate_sRGBProfileTHR(ctx);
    if (srgb_profile) {
        cmsCloseProfile(srgb_profile);
    }
    
    // Test cmsCreateRGBProfileTHR with dummy data
    cmsCIExyY white_point = {0.3127, 0.3290, 1.0};
    cmsCIExyYTRIPLE primaries = {
        {0.6400, 0.3300, 1.0},
        {0.3000, 0.6000, 1.0},
        {0.1500, 0.0600, 1.0}
    };
    
    cmsToneCurve* transfer[3] = {NULL, NULL, NULL};
    for (int i = 0; i < 3 && Size >= 768; i++) {
        transfer[i] = create_dummy_tone_curve(ctx, Data + i * 256, Size - i * 256);
    }
    
    cmsHPROFILE rgb_profile = cmsCreateRGBProfileTHR(ctx, &white_point, &primaries, transfer);
    if (rgb_profile) {
        cmsCloseProfile(rgb_profile);
    }
    
    // Clean up tone curves
    for (int i = 0; i < 3; i++) {
        if (transfer[i]) {
            cmsFreeToneCurve(transfer[i]);
        }
    }
    
    // Test cmsPluginTHR with dummy plugin data
    // Create a minimal plugin structure using only basic types
    struct {
        uint32_t Magic;
        uint32_t ExpectedVersion;
        uint32_t Type;
        uint8_t data[64];
    } dummy_plugin;
    
    memset(&dummy_plugin, 0, sizeof(dummy_plugin));
    // Use arbitrary values that might be valid
    dummy_plugin.Magic = *(uint32_t*)Data;
    dummy_plugin.ExpectedVersion = LCMS_VERSION;
    dummy_plugin.Type = *(uint32_t*)(Data + 4);
    
    // Copy some fuzzing data into plugin
    size_t copy_size = Size < sizeof(dummy_plugin.data) ? Size : sizeof(dummy_plugin.data);
    memcpy(dummy_plugin.data, Data, copy_size);
    
    cmsPluginTHR(ctx, (void*)&dummy_plugin);
    
    // Test cmsGetContextUserData
    void* user_data = cmsGetContextUserData(ctx);
    (void)user_data; // Suppress unused warning
    
    // Test cmsOpenProfileFromIOhandler2THR
    cmsIOHANDLER* io = create_file_iohandler(ctx, Data, Size);
    if (io) {
        cmsHPROFILE io_profile = cmsOpenProfileFromIOhandler2THR(ctx, io, FALSE);
        if (io_profile) {
            cmsCloseProfile(io_profile);
        }
        // Note: Do NOT call cmsCloseIOhandler() here as per instructions
        // The library handles closing on failure, and on success cmsCloseProfile handles it
    }
    
    // Clean up
    cmsDeleteContext(ctx);
    
    // Remove dummy file
    remove("./dummy_file");
    
    return 0;
}