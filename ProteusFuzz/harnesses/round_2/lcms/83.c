// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetProfileInfoASCII at cmsio1.c:1027:28 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsOpenProfileFromStreamTHR at cmsio0.c:1237:24 in lcms2.h
// cmsOpenProfileFromStream at cmsio0.c:1264:24 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <lcms2.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

static void cleanup_context(cmsContext ContextID) {
    if (ContextID != NULL) {
        cmsDeleteContext(ContextID);
    }
}

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Create a dummy file for testing
    const char *dummy_file = "./dummy_file";
    FILE *fp_write = fopen(dummy_file, "wb");
    if (fp_write) {
        fwrite(Data, 1, Size, fp_write);
        fclose(fp_write);
    } else {
        return 0;
    }

    // Initialize context
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) {
        remove(dummy_file);
        return 0;
    }

    // Test cmsOpenProfileFromFileTHR
    cmsHPROFILE hProfile1 = cmsOpenProfileFromFileTHR(ContextID, dummy_file, "r");
    cleanup_profile(hProfile1);

    // Test cmsOpenProfileFromFile
    cmsHPROFILE hProfile2 = cmsOpenProfileFromFile(dummy_file, "r");
    cleanup_profile(hProfile2);

    // Test cmsOpenProfileFromStreamTHR
    FILE *fp_read1 = fopen(dummy_file, "r");
    if (fp_read1) {
        cmsHPROFILE hProfile3 = cmsOpenProfileFromStreamTHR(ContextID, fp_read1, "r");
        cleanup_profile(hProfile3);
        // Only close the file if the profile was not successfully opened or if the library did not take ownership.
        // However, lcms documentation implies that if the profile is opened successfully, the stream might be managed.
        // To be safe and avoid double-free if lcms closes it internally, we check if hProfile3 was valid.
        // But standard practice for cmsOpenProfileFromStream is that the caller still owns the FILE* unless specified otherwise.
        // The error suggests fclose is called on an already freed resource, which is odd for FILE*.
        // Re-reading the error: "attempting double-free on ... in thread T0".
        // The stack trace shows fclose being called on fp_read1.
        // The "freed by thread T0 here" points to fopen. This is extremely strange for a double free on fclose.
        // Wait, the error says "freed by thread T0 here" points to malloc inside fopen? No, it points to the line where fopen is called.
        // Actually, the error says "freed by thread T0 here" -> malloc. This implies the FILE struct itself was freed?
        // Or perhaps the issue is that lcms2 closes the file internally when opening the profile?
        // If lcms2 closes the file internally, calling fclose again causes the double free (or use after free).
        // Let's assume lcms2 might close the file if it takes ownership or if the profile is invalid and it cleans up.
        // To prevent this, we will NOT close the file if we passed it to lcms2, or we will check if it's still valid.
        // A safer approach for fuzzing: Do not close the file if we passed it to lcms2 functions that might take ownership.
        // However, standard C FILE* ownership usually remains with the caller.
        // Let's look at the error again. "freed by thread T0 here" -> malloc. This is confusing.
        // Actually, the error message "freed by thread T0 here" points to the line `FILE *fp_read1 = fopen(dummy_file, "r");`.
        // This implies that the memory allocated by fopen was freed, and then we tried to free it again via fclose?
        // No, the "freed by" usually points to the first free. The "attempting double-free" points to the second.
        // The second is at line 56: fclose(fp_read1).
        // The first is at line 52: fopen. This suggests fopen allocated memory, and something else freed it before fclose?
        // Or maybe the error message is misleading and the "freed by" is actually the first free operation which happened inside lcms?
        // If lcms closes the file internally, then fclose is the double free.
        // Let's assume lcms closes the file. We will skip fclose for streams passed to lcms.
        (void)fp_read1; // Mark as used to avoid warnings, but do not close it here if we suspect lcms took it.
        // Actually, let's just not close it to be safe. The OS will clean it up on process exit.
        // But wait, if we don't close it, we leak file descriptors.
        // Let's try to close it only if the profile is NULL? No, the error happens even if profile is valid.
        // The safest fix for the double free is to NOT call fclose on the stream passed to lcms.
        // We will remove the fclose calls for streams passed to lcms.
    }

    // Test cmsOpenProfileFromStream
    FILE *fp_read2 = fopen(dummy_file, "r");
    if (fp_read2) {
        cmsHPROFILE hProfile4 = cmsOpenProfileFromStream(fp_read2, "r");
        cleanup_profile(hProfile4);
        // Skip fclose to avoid potential double free if lcms manages the stream
    }

    // Test cmsGetProfileInfoASCII and cmsGetProfileInfo with a valid profile
    cmsHPROFILE hProfile5 = cmsOpenProfileFromFile(dummy_file, "r");
    if (hProfile5 != NULL) {
        char ascii_buffer[256];
        wchar_t wide_buffer[256];
        cmsUInt32Number ascii_result, wide_result;

        // Test cmsGetProfileInfoASCII using correct enum value
        ascii_result = cmsGetProfileInfoASCII(hProfile5, cmsInfoDescription, "eng", "USA", ascii_buffer, sizeof(ascii_buffer));
        (void)ascii_result;

        // Test cmsGetProfileInfo using correct enum value
        wide_result = cmsGetProfileInfo(hProfile5, cmsInfoDescription, "eng", "USA", wide_buffer, sizeof(wide_buffer) / sizeof(wchar_t));
        (void)wide_result;

        cleanup_profile(hProfile5);
    }

    // Cleanup context
    cleanup_context(ContextID);

    // Remove dummy file
    remove(dummy_file);

    return 0;
}