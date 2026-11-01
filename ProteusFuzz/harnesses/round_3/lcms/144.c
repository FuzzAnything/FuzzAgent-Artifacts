// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenIOhandlerFromNULL at cmsio0.c:100:26 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsWriteTypeBase at cmsplugin.c:434:20 in lcms2_plugin.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsOpenIOhandlerFromNULL at cmsio0.c:100:26 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsOpenIOhandlerFromNULL at cmsio0.c:100:26 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsOpenIOhandlerFromNULL at cmsio0.c:100:26 in lcms2.h
// _cmsReadTypeBase at cmsplugin.c:421:31 in lcms2_plugin.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// cmsOpenIOhandlerFromNULL at cmsio0.c:100:26 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_144(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) {
        return 0;
    }

    // Test 1: cmsOpenIOhandlerFromNULL
    cmsIOHANDLER* io = cmsOpenIOhandlerFromNULL(ContextID);
    if (io == NULL) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    // Test 2: _cmsWriteTypeBase
    cmsTagTypeSignature sig = 0;
    if (Size >= 4) {
        memcpy(&sig, Data, 4);
    }
    _cmsWriteTypeBase(io, sig);

    // Test 3: cmsOpenProfileFromIOhandler2THR (write mode)
    cmsHPROFILE hProfileWrite = cmsOpenProfileFromIOhandler2THR(ContextID, io, 1);
    cleanup_profile(hProfileWrite);

    // IMPORTANT: The previous call to cmsOpenProfileFromIOhandler2THR with write=1
    // might have consumed the IO handler or left it in a state where subsequent
    // reads fail or cause use-after-free if the profile closes the handler.
    // Since cmsOpenIOhandlerFromNULL creates a handler that is closed when the profile is closed,
    // and we just closed hProfileWrite, the 'io' pointer might now be invalid or the
    // internal state is corrupted for further use.
    // To safely test read operations, we should create a NEW IO handler.
    // However, cmsOpenIOhandlerFromNULL is designed for NULL streams.
    // The crash indicates that 'io' was freed when hProfileWrite was closed.
    // We must not use 'io' after closing a profile that owns it.
    // Let's create a fresh handler for the read test.
    
    cmsIOHANDLER* ioRead = cmsOpenIOhandlerFromNULL(ContextID);
    if (ioRead == NULL) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    // Test 4: cmsOpenProfileFromIOhandler2THR (read mode)
    cmsHPROFILE hProfileRead = cmsOpenProfileFromIOhandler2THR(ContextID, ioRead, 0);
    cleanup_profile(hProfileRead);

    // Test 5: cmsOpenProfileFromIOhandlerTHR
    cmsIOHANDLER* ioThr = cmsOpenIOhandlerFromNULL(ContextID);
    if (ioThr != NULL) {
        cmsHPROFILE hProfileThr = cmsOpenProfileFromIOhandlerTHR(ContextID, ioThr);
        cleanup_profile(hProfileThr);
    }

    // Test 6: _cmsReadTypeBase
    // Create another handler to ensure we have a valid one for reading
    cmsIOHANDLER* ioReadBase = cmsOpenIOhandlerFromNULL(ContextID);
    if (ioReadBase != NULL) {
        cmsTagTypeSignature readSig = _cmsReadTypeBase(ioReadBase);
        (void)readSig;
    }

    // Test 7: cmsGetProfileIOhandler
    cmsIOHANDLER* ioFromProfile = cmsGetProfileIOhandler(NULL);
    (void)ioFromProfile;

    // Re-open a profile in write mode to test cmsGetProfileIOhandler on a valid profile.
    cmsIOHANDLER* ioValid = cmsOpenIOhandlerFromNULL(ContextID);
    if (ioValid != NULL) {
        cmsHPROFILE hProfileValid = cmsOpenProfileFromIOhandler2THR(ContextID, ioValid, 1);
        if (hProfileValid != NULL) {
            cmsIOHANDLER* ioRetrieved = cmsGetProfileIOhandler(hProfileValid);
            (void)ioRetrieved;
            cleanup_profile(hProfileValid);
        }
    }

    cmsDeleteContext(ContextID);
    return 0;
}