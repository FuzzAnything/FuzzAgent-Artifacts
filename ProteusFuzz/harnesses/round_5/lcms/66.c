// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsSaveProfileToStream at cmsio0.c:1520:19 in lcms2.h
// _cmsWriteUInt32Number at cmsplugin.c:295:20 in lcms2_plugin.h
// _cmsWriteUInt16Array at cmsplugin.c:281:20 in lcms2_plugin.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

// Helper to create a context
static cmsContext CreateContextFromData(const uint8_t *Data, size_t Size) {
    (void)Data;
    (void)Size;
    return cmsCreateContext(NULL, NULL);
}

// Helper to create a profile from data using a file stream approach
static cmsHPROFILE CreateProfileFromData(const uint8_t *Data, size_t Size, cmsContext ctx) {
    if (Size < 128) return NULL; // ICC profiles have a minimum size
    
    // Write data to a temporary file to open via standard API
    FILE *fp = fopen("./dummy_profile", "wb");
    if (!fp) return NULL;
    
    if (fwrite(Data, 1, Size, fp) != Size) {
        fclose(fp);
        return NULL;
    }
    fclose(fp);
    
    cmsHPROFILE hProfile = cmsOpenProfileFromFile("./dummy_profile", "r");
    return hProfile;
}

// Helper to create an IO handler using a file path since cmsOpenIOhandlerFromFile is the public API
static cmsIOHANDLER* CreateIOHandlerFromData(const uint8_t *Data, size_t Size, cmsContext ctx) {
    if (Size == 0) return NULL;
    
    // Write data to a temporary file
    FILE *fp = fopen("./dummy_io", "wb");
    if (!fp) return NULL;
    
    if (fwrite(Data, 1, Size, fp) != Size) {
        fclose(fp);
        return NULL;
    }
    fclose(fp);
    
    // Use the public API to open the IO handler from the file path
    return cmsOpenIOhandlerFromFile(ctx, "./dummy_io", "r+");
}

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = CreateContextFromData(Data, Size);
    if (!ctx) return 0;

    cmsHPROFILE hProfile = CreateProfileFromData(Data, Size, ctx);
    cmsIOHANDLER *io = CreateIOHandlerFromData(Data, Size, ctx);
    
    // Test 1: cmsSaveProfileToStream
    if (hProfile) {
        FILE *fp = fopen("./dummy_file", "wb");
        if (fp) {
            cmsSaveProfileToStream(hProfile, fp);
            fclose(fp);
        }
    }

    // Test 2: _cmsWriteUInt32Number
    if (io) {
        cmsUInt32Number testVal = 0;
        if (Size >= 4) {
            memcpy(&testVal, Data, 4);
        }
        _cmsWriteUInt32Number(io, testVal);
    }

    // Test 3: _cmsWriteUInt16Array
    if (io && Size >= 2) {
        cmsUInt16Number arr[2];
        arr[0] = (Size >= 2) ? (Data[0] << 8 | Data[1]) : 0;
        arr[1] = (Size >= 3) ? (Data[2] << 8 | (Size > 3 ? Data[3] : 0)) : 0;
        _cmsWriteUInt16Array(io, 2, arr);
    }

    // Test 4: cmsSaveProfileToMem
    if (hProfile) {
        cmsUInt32Number bytesNeeded = 0;
        // First call to get size
        cmsSaveProfileToMem(hProfile, NULL, &bytesNeeded);
        
        if (bytesNeeded > 0) {
            void *memPtr = malloc(bytesNeeded);
            if (memPtr) {
                cmsSaveProfileToMem(hProfile, memPtr, &bytesNeeded);
                free(memPtr);
            }
        }
    }

    // Test 5: cmsOpenProfileFromIOhandler2THR (write mode)
    // Create a fresh file for write mode test
    FILE *fpWrite = fopen("./dummy_io_write", "wb+");
    if (fpWrite) {
        fclose(fpWrite); // Close immediately so we can reopen via path
        cmsIOHANDLER *ioWrite = cmsOpenIOhandlerFromFile(ctx, "./dummy_io_write", "w");
        if (ioWrite) {
            cmsHPROFILE hProfileWrite = cmsOpenProfileFromIOhandler2THR(ctx, ioWrite, TRUE);
            if (hProfileWrite) {
                cmsCloseProfile(hProfileWrite);
            }
            // If hProfileWrite is NULL, library cleans up ioWrite.
            // If valid, profile owns it.
        }
    }

    // Cleanup
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
    
    // The 'io' variable created here was used for direct writes (Test 2 & 3).
    // We must close it manually.
    if (io) {
        cmsCloseIOhandler(io);
    }

    if (ctx) {
        cmsDeleteContext(ctx);
    }

    // Cleanup dummy files
    remove("./dummy_file");
    remove("./dummy_profile");
    remove("./dummy_io");
    remove("./dummy_io_write");

    return 0;
}