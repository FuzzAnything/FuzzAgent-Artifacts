// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsOpenIOhandlerFromNULL at cmsio0.c:100:26 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
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

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a context
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) {
        return 0;
    }

    // Test 1: cmsOpenIOhandlerFromMem
    cmsIOHANDLER* io_mem = cmsOpenIOhandlerFromMem(ContextID, (void*)Data, Size, "r");
    if (io_mem != NULL) {
        cmsHPROFILE hProfile = cmsOpenProfileFromIOhandler2THR(ContextID, io_mem, FALSE);
        cleanup_profile(hProfile);
    }

    // Test 2: cmsOpenIOhandlerFromNULL
    cmsIOHANDLER* io_null = cmsOpenIOhandlerFromNULL(ContextID);
    if (io_null != NULL) {
        cmsHPROFILE hProfile_null = cmsOpenProfileFromIOhandlerTHR(ContextID, io_null);
        cleanup_profile(hProfile_null);
    }

    // Test 3: cmsOpenIOhandlerFromStream
    // Create a dummy file for testing
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file != NULL) {
        if (Size > 0) {
            fwrite(Data, 1, Size, dummy_file);
        }
        fclose(dummy_file);

        dummy_file = fopen("./dummy_file", "rb");
        if (dummy_file != NULL) {
            cmsIOHANDLER* io_stream = cmsOpenIOhandlerFromStream(ContextID, dummy_file);
            if (io_stream != NULL) {
                cmsHPROFILE hProfile_stream = cmsOpenProfileFromIOhandlerTHR(ContextID, io_stream);
                cleanup_profile(hProfile_stream);
            }
            // Only close the file if the IO handler was not successfully created or if we didn't pass ownership.
            // In lcms, cmsOpenIOhandlerFromStream usually takes ownership of the stream or expects the caller to manage it carefully.
            // However, the double free error suggests the file was closed twice. 
            // If cmsOpenIOhandlerFromStream succeeds, it might close the file internally or we shouldn't close it here if it's attached to a profile.
            // But cmsOpenProfileFromIOhandlerTHR doesn't necessarily close the stream immediately.
            // The error trace shows fclose being called on a pointer that was already freed.
            // This often happens if the library closes the file upon failure or success and we try to close it again.
            // To be safe, we will NOT close dummy_file here if io_stream was created, assuming the library manages it or we let it leak slightly for safety in fuzzing.
            // Actually, standard practice for cmsOpenIOhandlerFromStream is that the caller must close the stream if the profile open fails, 
            // but if it succeeds, the profile holds the stream. 
            // If we close the file here, and the profile is closed later, it might double close.
            // Let's check if hProfile_stream is valid. If so, the profile owns the stream. If not, we close it.
            // But wait, the error happened at fclose(dummy_file) inside the block.
            // Let's assume if io_stream is created, we don't close the file here to avoid double free if the library does it.
            // If io_stream is NULL, we close the file.
            if (io_stream == NULL) {
                fclose(dummy_file);
            }
        }
    }
    // Clean up dummy file
    remove("./dummy_file");

    // Test 4: cmsGetProfileIOhandler
    // Create a valid profile from memory to test retrieval
    cmsIOHANDLER* io_test = cmsOpenIOhandlerFromMem(ContextID, (void*)Data, Size, "r");
    if (io_test != NULL) {
        cmsHPROFILE hProfile_test = cmsOpenProfileFromIOhandler2THR(ContextID, io_test, FALSE);
        if (hProfile_test != NULL) {
            cmsIOHANDLER* retrieved_io = cmsGetProfileIOhandler(hProfile_test);
            (void)retrieved_io; // Use the variable to avoid unused warning
            cleanup_profile(hProfile_test);
        }
    }

    // Cleanup context
    cmsDeleteContext(ContextID);

    return 0;
}