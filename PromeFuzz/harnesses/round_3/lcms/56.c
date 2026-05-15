// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateDeviceLinkFromCubeFileTHR at cmscgats.c:3211:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsIT8LoadFromFile at cmscgats.c:2603:22 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char* DUMMY_FILE = "./dummy_file";

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE* f = fopen(DUMMY_FILE, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_file(void) {
    remove(DUMMY_FILE);
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;
    
    write_dummy_file(Data, Size);
    
    // Test cmsCreateDeviceLinkFromCubeFileTHR
    cmsHPROFILE profile1 = cmsCreateDeviceLinkFromCubeFileTHR(context, DUMMY_FILE);
    if (profile1) cmsCloseProfile(profile1);
    
    // Test cmsOpenProfileFromFileTHR with different access modes
    cmsHPROFILE profile2 = cmsOpenProfileFromFileTHR(context, DUMMY_FILE, "r");
    if (profile2) cmsCloseProfile(profile2);
    
    profile2 = cmsOpenProfileFromFileTHR(context, DUMMY_FILE, "w");
    if (profile2) cmsCloseProfile(profile2);
    
    // Test cmsIT8LoadFromFile
    cmsHANDLE it8 = cmsIT8LoadFromFile(context, DUMMY_FILE);
    if (it8) cmsIT8Free(it8);
    
    // Test cmsOpenIOhandlerFromMem with different access modes
    cmsIOHANDLER* io1 = cmsOpenIOhandlerFromMem(context, (void*)Data, (cmsUInt32Number)Size, "r");
    if (io1) cmsCloseIOhandler(io1);
    
    io1 = cmsOpenIOhandlerFromMem(context, (void*)Data, (cmsUInt32Number)Size, "w");
    if (io1) cmsCloseIOhandler(io1);
    
    // Test cmsOpenProfileFromStreamTHR - stream is managed by lcms
    FILE* stream = fopen(DUMMY_FILE, "rb");
    if (stream) {
        cmsHPROFILE profile3 = cmsOpenProfileFromStreamTHR(context, stream, "r");
        if (profile3) {
            cmsCloseProfile(profile3);  // This will close the stream
        } else {
            // Only close stream if profile creation failed
            fclose(stream);
        }
    }
    
    // Test cmsOpenIOhandlerFromFile with different access modes
    cmsIOHANDLER* io2 = cmsOpenIOhandlerFromFile(context, DUMMY_FILE, "r");
    if (io2) cmsCloseIOhandler(io2);
    
    io2 = cmsOpenIOhandlerFromFile(context, DUMMY_FILE, "w");
    if (io2) cmsCloseIOhandler(io2);
    
    // Additional test: Use memory buffer with cmsOpenProfileFromMemTHR if available
    cmsHPROFILE profile4 = cmsOpenProfileFromMemTHR(context, Data, (cmsUInt32Number)Size);
    if (profile4) cmsCloseProfile(profile4);
    
    // Cleanup
    cleanup_file();
    cmsDeleteContext(context);
    
    return 0;
}