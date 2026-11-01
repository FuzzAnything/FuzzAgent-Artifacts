// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenIOhandlerFromNULL at cmsio0.c:100:26 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsOpenIOhandlerFromNULL at cmsio0.c:100:26 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_197(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a context for thread-safe operations
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) return 0;

    cmsIOHANDLER* io = NULL;
    cmsHPROFILE hProfile = NULL;
    FILE* stream = NULL;

    // Use the first byte to decide which function to fuzz
    int choice = Data[0] % 6;

    switch (choice) {
        case 0: {
            // Fuzz cmsOpenIOhandlerFromStream
            const char* dummy_file = "./dummy_file";
            stream = fopen(dummy_file, "wb");
            if (stream != NULL) {
                fwrite(Data, 1, Size, stream);
                fclose(stream);
                stream = fopen(dummy_file, "rb");
                if (stream != NULL) {
                    io = cmsOpenIOhandlerFromStream(ContextID, stream);
                    if (io != NULL) {
                        hProfile = cmsOpenProfileFromIOhandlerTHR(ContextID, io);
                    }
                }
            }
            break;
        }
        case 1: {
            // Fuzz cmsOpenIOhandlerFromNULL
            io = cmsOpenIOhandlerFromNULL(ContextID);
            if (io != NULL) {
                hProfile = cmsOpenProfileFromIOhandlerTHR(ContextID, io);
            }
            break;
        }
        case 2: {
            // Fuzz cmsOpenIOhandlerFromMem
            cmsUInt32Number size = (Size > 0) ? (Size - 1) : 0;
            const char* mode = "r";
            io = cmsOpenIOhandlerFromMem(ContextID, (void*)(Data + 1), size, mode);
            if (io != NULL) {
                hProfile = cmsOpenProfileFromIOhandlerTHR(ContextID, io);
            }
            break;
        }
        case 3: {
            // Fuzz cmsOpenIOhandlerFromFile
            const char* dummy_file = "./dummy_file";
            FILE* write_stream = fopen(dummy_file, "wb");
            if (write_stream != NULL) {
                fwrite(Data, 1, Size, write_stream);
                fclose(write_stream);
            }
            const char* mode = "r";
            io = cmsOpenIOhandlerFromFile(ContextID, dummy_file, mode);
            if (io != NULL) {
                hProfile = cmsOpenProfileFromIOhandlerTHR(ContextID, io);
            }
            break;
        }
        case 4: {
            // Fuzz cmsGetProfileIOhandler
            cmsUInt32Number size = (Size > 0) ? (Size - 1) : 0;
            cmsIOHANDLER* mem_io = cmsOpenIOhandlerFromMem(ContextID, (void*)(Data + 1), size, "r");
            if (mem_io != NULL) {
                hProfile = cmsOpenProfileFromIOhandlerTHR(ContextID, mem_io);
                if (hProfile != NULL) {
                    cmsIOHANDLER* retrieved_io = cmsGetProfileIOhandler(hProfile);
                    (void)retrieved_io;
                }
            }
            break;
        }
        case 5: {
            // Fuzz cmsOpenProfileFromIOhandlerTHR directly with various handlers
            
            // Try NULL handler (Removed: this causes NULL pointer dereference in lcms)
            // hProfile = cmsOpenProfileFromIOhandlerTHR(ContextID, NULL);
            // if (hProfile != NULL) {
            //     cleanup_profile(hProfile);
            //     hProfile = NULL;
            // }
            
            // Try NULL context with valid handler (should fail gracefully)
            cmsIOHANDLER* null_ctx_io = cmsOpenIOhandlerFromNULL(NULL);
            if (null_ctx_io != NULL) {
                hProfile = cmsOpenProfileFromIOhandlerTHR(NULL, null_ctx_io);
                if (hProfile != NULL) {
                    cleanup_profile(hProfile);
                    hProfile = NULL;
                }
            }
            break;
        }
        default:
            break;
    }

    // Cleanup
    if (hProfile != NULL) {
        cleanup_profile(hProfile);
    }
    
    cmsDeleteContext(ContextID);

    return 0;
}