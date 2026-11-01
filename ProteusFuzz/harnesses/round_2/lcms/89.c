// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenIOhandlerFromNULL at cmsio0.c:100:26 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    cmsHPROFILE profile = NULL;
    cmsIOHANDLER *io = NULL;

    // Test 1: cmsOpenIOhandlerFromNULL
    io = cmsOpenIOhandlerFromNULL(ctx);
    if (io != NULL) {
        profile = cmsOpenProfileFromIOhandlerTHR(ctx, io);
        if (profile != NULL) {
            cmsCloseProfile(profile);
        }
        // If cmsOpenProfileFromIOhandlerTHR returns NULL, it closes the handler internally.
        // If it returns a profile, the profile owns the handler.
        // So we do not manually close io here.
    }

    // Test 2: cmsOpenIOhandlerFromMem
    const char *access_mode = "r";
    io = cmsOpenIOhandlerFromMem(ctx, (void*)Data, Size, access_mode);
    if (io != NULL) {
        profile = cmsOpenProfileFromIOhandlerTHR(ctx, io);
        if (profile != NULL) {
            cmsCloseProfile(profile);
        }
        // Handler ownership logic remains the same as above.
    }

    // Test 3: cmsOpenProfileFromIOhandler2THR
    io = cmsOpenIOhandlerFromMem(ctx, (void*)Data, Size, "r");
    if (io != NULL) {
        cmsBool write_flag = (Size > 1) ? (Data[0] % 2) : 0;
        profile = cmsOpenProfileFromIOhandler2THR(ctx, io, write_flag);
        if (profile != NULL) {
            cmsCloseProfile(profile);
        } else {
            // If it returns NULL, the library automatically frees the handler.
            io = NULL; 
        }
    }

    // Test 4: cmsGetProfileIOhandler
    io = cmsOpenIOhandlerFromMem(ctx, (void*)Data, Size, "r");
    if (io != NULL) {
        profile = cmsOpenProfileFromIOhandlerTHR(ctx, io);
        if (profile != NULL) {
            cmsIOHANDLER *handler = cmsGetProfileIOhandler(profile);
            (void)handler; // Suppress unused variable warning
            cmsCloseProfile(profile);
        }
    }

    // Test 5: cmsOpenIOhandlerFromStream
    const char *dummy_file = "./dummy_file";
    FILE *stream = fopen(dummy_file, "wb");
    if (stream != NULL) {
        fwrite(Data, 1, Size, stream);
        fclose(stream);
        
        stream = fopen(dummy_file, "rb");
        if (stream != NULL) {
            io = cmsOpenIOhandlerFromStream(ctx, stream);
            if (io != NULL) {
                profile = cmsOpenProfileFromIOhandlerTHR(ctx, io);
                if (profile != NULL) {
                    cmsCloseProfile(profile);
                }
            }
            fclose(stream);
        }
        remove(dummy_file);
    }

    cmsDeleteContext(ctx);

    return 0;
}