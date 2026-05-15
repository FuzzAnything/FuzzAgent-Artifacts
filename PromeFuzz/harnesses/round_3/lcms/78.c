// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsWriteUInt32Number at cmsplugin.c:295:20 in lcms2_plugin.h
// _cmsWriteTypeBase at cmsplugin.c:434:20 in lcms2_plugin.h
// _cmsWrite15Fixed16Number at cmsplugin.c:337:20 in lcms2_plugin.h
// cmsWriteRawTag at cmsio0.c:2034:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsAdjustEndianess32 at cmsplugin.c:58:28 in lcms2_plugin.h
// _cmsWriteFloat32Number at cmsplugin.c:309:20 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

// Memory-based I/O handler implementation for fuzzing
static cmsUInt32Number ReadMemory(cmsIOHANDLER* iohandler, void* Buffer,
                                  cmsUInt32Number size, cmsUInt32Number count) {
    return 0; // Not used for writing operations
}

static cmsBool SeekMemory(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    return TRUE;
}

static cmsBool CloseMemory(cmsIOHANDLER* iohandler) {
    if (iohandler && iohandler->stream) {
        free(iohandler->stream);
    }
    if (iohandler) {
        free(iohandler);
    }
    return TRUE;
}

static cmsUInt32Number TellMemory(cmsIOHANDLER* iohandler) {
    return 0;
}

static cmsBool WriteMemory(cmsIOHANDLER* iohandler, cmsUInt32Number size,
                           const void* Buffer) {
    return TRUE;
}

static cmsIOHANDLER* CreateMemoryIOHandler(void) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)calloc(1, sizeof(cmsIOHANDLER));
    if (!io) return NULL;
    
    io->stream = malloc(1024);
    if (!io->stream) {
        free(io);
        return NULL;
    }
    
    io->Read = ReadMemory;
    io->Seek = SeekMemory;
    io->Close = CloseMemory;
    io->Tell = TellMemory;
    io->Write = WriteMemory;
    
    return io;
}

// Dummy profile creation for cmsWriteRawTag
static cmsHPROFILE CreateDummyProfile(void) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return NULL;
    
    // Create a simple RGB profile with sRGB primaries and gamma curves
    cmsToneCurve* gamma = cmsBuildGamma(ctx, 2.2);
    if (!gamma) {
        cmsDeleteContext(ctx);
        return NULL;
    }
    
    cmsToneCurve* transferFunction[3] = { gamma, gamma, gamma };
    cmsHPROFILE profile = cmsCreateRGBProfileTHR(ctx,
        cmsD50_xyY(),
        NULL,  // Use NULL for primaries (will use sRGB primaries)
        transferFunction);
    
    cmsFreeToneCurve(gamma);
    cmsDeleteContext(ctx);
    
    return profile;
}

int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Test _cmsAdjustEndianess32
    cmsUInt32Number value;
    memcpy(&value, Data, sizeof(cmsUInt32Number));
    cmsUInt32Number adjusted = _cmsAdjustEndianess32(value);
    (void)adjusted;
    
    // Test _cmsWriteFloat32Number
    if (Size >= 8) {
        cmsIOHANDLER* io1 = CreateMemoryIOHandler();
        if (io1) {
            cmsFloat32Number float_val;
            memcpy(&float_val, Data + 4, sizeof(cmsFloat32Number));
            _cmsWriteFloat32Number(io1, float_val);
            io1->Close(io1);
        }
    }
    
    // Test _cmsWriteUInt32Number
    if (Size >= 12) {
        cmsIOHANDLER* io2 = CreateMemoryIOHandler();
        if (io2) {
            cmsUInt32Number uint_val;
            memcpy(&uint_val, Data + 8, sizeof(cmsUInt32Number));
            _cmsWriteUInt32Number(io2, uint_val);
            io2->Close(io2);
        }
    }
    
    // Test _cmsWriteTypeBase
    if (Size >= 16) {
        cmsIOHANDLER* io3 = CreateMemoryIOHandler();
        if (io3) {
            cmsTagTypeSignature sig;
            memcpy(&sig, Data + 12, sizeof(cmsTagTypeSignature));
            _cmsWriteTypeBase(io3, sig);
            io3->Close(io3);
        }
    }
    
    // Test _cmsWrite15Fixed16Number
    if (Size >= 24) {
        cmsIOHANDLER* io4 = CreateMemoryIOHandler();
        if (io4) {
            cmsFloat64Number fixed_val;
            memcpy(&fixed_val, Data + 16, sizeof(cmsFloat64Number));
            _cmsWrite15Fixed16Number(io4, fixed_val);
            io4->Close(io4);
        }
    }
    
    // Test cmsWriteRawTag
    if (Size >= 32) {
        cmsHPROFILE profile = CreateDummyProfile();
        if (profile) {
            cmsTagSignature tag_sig;
            memcpy(&tag_sig, Data + 24, sizeof(cmsTagSignature));
            cmsUInt32Number data_size = Size - 28;
            if (data_size > 0) {
                const void* tag_data = Data + 28;
                cmsWriteRawTag(profile, tag_sig, tag_data, data_size);
            }
            cmsCloseProfile(profile);
        }
    }
    
    // Write to dummy file if needed (for file-based operations)
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        if (Size > 0) {
            fwrite(Data, 1, Size, dummy_file);
        }
        fclose(dummy_file);
    }
    
    return 0;
}