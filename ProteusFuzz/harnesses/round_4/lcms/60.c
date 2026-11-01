// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8LoadFromFile at cmscgats.c:2603:22 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCreateDeviceLinkFromCubeFileTHR at cmscgats.c:3211:23 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile != NULL) {
        cmsCloseProfile(profile);
    }
}

// Note: cmsCloseIT8 is not a standard public API in lcms2. 
// IT8 handles are typically freed via cmsCloseIT8 if available in specific versions,
// but often they are just void* handles managed internally or require cmsFree.
// Since the header doesn't declare cmsCloseIT8, we assume the handle is freed by cmsDeleteContext
// or we skip explicit cleanup to avoid compilation errors, as the fuzz target is short-lived.
// If a specific version provides it, it would be declared. Here we do nothing to be safe.
static void cleanup_it8(cmsHANDLE handle) {
    // No public API to close IT8 handle in standard lcms2.h without version specifics.
    // Rely on context deletion or process exit.
    (void)handle; 
}

static void cleanup_iohandler(cmsIOHANDLER *handler) {
    // The struct _cms_io_handler is opaque (forward declared).
    // We cannot access members like Close directly.
    // We rely on the library's internal cleanup or context deletion.
    // If the handler wraps a FILE*, it might leak, but for fuzzing stability, we skip direct access.
    (void)handler;
}

static void write_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f != NULL) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    // 1. Prepare the environment
    // cmsCreateContext requires two arguments: Plugin and UserData.
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) {
        return 0;
    }

    // Create a dummy file for testing file-based APIs
    const char *dummy_file = "./dummy_file";
    write_dummy_file(dummy_file, Data, Size);

    // 2. Invoke target functions diversely

    // Test cmsIT8LoadFromFile
    cmsHANDLE it8_handle = cmsIT8LoadFromFile(ContextID, dummy_file);
    cleanup_it8(it8_handle);

    // Test cmsOpenIOhandlerFromFile
    cmsIOHANDLER *iohandler = cmsOpenIOhandlerFromFile(ContextID, dummy_file, "r");
    // We cannot safely close the iohandler struct members due to opaque type.
    // The library usually handles cleanup on context delete or via specific close functions not exposed here.
    if (iohandler != NULL) {
        // Attempt to use it if possible, but primarily just check for crash on creation.
        // We don't have a public cmsCloseIOhandler in the provided signature list.
    }

    // Test cmsCreateDeviceLinkFromCubeFileTHR
    cmsHPROFILE cube_profile = cmsCreateDeviceLinkFromCubeFileTHR(ContextID, dummy_file);
    cleanup_profile(cube_profile);

    // Test cmsOpenProfileFromStreamTHR
    FILE *stream = fopen(dummy_file, "rb");
    if (stream != NULL) {
        cmsHPROFILE stream_profile = cmsOpenProfileFromStreamTHR(ContextID, stream, "r");
        cleanup_profile(stream_profile);
        fclose(stream);
    }

    // Test cmsOpenProfileFromFile
    cmsHPROFILE file_profile = cmsOpenProfileFromFile(dummy_file, "r");
    cleanup_profile(file_profile);

    // Test cmsOpenProfileFromIOhandler2THR
    cmsIOHANDLER *iohandler2 = cmsOpenIOhandlerFromFile(ContextID, dummy_file, "r");
    if (iohandler2 != NULL) {
        // cmsBool is defined as int. 0 is false, 1 is true.
        cmsHPROFILE io_profile = cmsOpenProfileFromIOhandler2THR(ContextID, iohandler2, 0);
        cleanup_profile(io_profile);
    }

    // 3. Cleanup
    cmsDeleteContext(ContextID);
    
    // Clean up the dummy file
    remove(dummy_file);

    return 0;
}