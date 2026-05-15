// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// _cmsWriteTypeBase at cmsplugin.c:434:20 in lcms2_plugin.h
// _cmsReadUInt64Number at cmsplugin.c:206:21 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsReadTypeBase at cmsplugin.c:421:31 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    // Create a dummy profile to get an I/O handler
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (!hProfile) {
        return 0;
    }
    
    // Get I/O handler from profile
    cmsIOHANDLER* io = cmsGetProfileIOhandler(hProfile);
    if (!io) {
        cmsCloseProfile(hProfile);
        return 0;
    }
    
    // Test _cmsWriteTypeBase with various signatures
    if (Size >= 4) {
        cmsTagTypeSignature sig = *(cmsTagTypeSignature*)Data;
        _cmsWriteTypeBase(io, sig);
    }
    
    // Test _cmsReadUInt64Number
    cmsUInt64Number n;
    if (Size >= 8) {
        // Write some data to the I/O handler first so we can read it back
        cmsUInt64Number write_val = *(cmsUInt64Number*)Data;
        io->Write(io, sizeof(cmsUInt64Number), &write_val);
        
        // Reset position to read
        io->Seek(io, 0);
        _cmsReadUInt64Number(io, &n);
    }
    
    // Test _cmsReadAlignment
    io->Seek(io, 0);
    _cmsReadAlignment(io);
    
    // Test _cmsReadTypeBase
    io->Seek(io, 0);
    cmsTagTypeSignature read_sig = _cmsReadTypeBase(io);
    (void)read_sig; // Avoid unused variable warning
    
    // Test _cmsWriteAlignment
    _cmsWriteAlignment(io);
    
    // Cleanup
    cmsCloseProfile(hProfile);
    
    return 0;
}