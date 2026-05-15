// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsAdjustEndianess32 at cmsplugin.c:58:28 in lcms2_plugin.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsWriteRawTag at cmsio0.c:2034:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// _cmsWriteUInt32Number at cmsplugin.c:295:20 in lcms2_plugin.h
// _cmsWriteFloat32Number at cmsplugin.c:309:20 in lcms2_plugin.h
// _cmsWriteUInt16Number at cmsplugin.c:268:20 in lcms2_plugin.h
// _cmsWriteTypeBase at cmsplugin.c:434:20 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "lcms2.h"
#include "lcms2_plugin.h"

// Mock I/O handler implementation for testing
static cmsUInt32Number mock_read(cmsIOHANDLER* iohandler, void* Buffer, 
                                 cmsUInt32Number size, cmsUInt32Number count) {
    return 0; // Always return 0 bytes read for fuzzing
}

static cmsBool mock_seek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    return TRUE; // Always succeed for fuzzing
}

static cmsBool mock_close(cmsIOHANDLER* iohandler) {
    free(iohandler->stream);
    return TRUE;
}

static cmsUInt32Number mock_tell(cmsIOHANDLER* iohandler) {
    return 0;
}

static cmsBool mock_write(cmsIOHANDLER* iohandler, cmsUInt32Number size, 
                          const void* Buffer) {
    // Track written bytes but don't actually write anywhere
    iohandler->UsedSpace += size;
    return TRUE;
}

static cmsIOHANDLER* create_mock_iohandler(void) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (!io) return NULL;
    
    io->stream = malloc(1024);
    if (!io->stream) {
        free(io);
        return NULL;
    }
    
    io->ContextID = NULL;
    io->UsedSpace = 0;
    io->ReportedSize = 1024;
    memset(io->PhysicalFile, 0, cmsMAX_PATH);
    
    io->Read = mock_read;
    io->Seek = mock_seek;
    io->Close = mock_close;
    io->Tell = mock_tell;
    io->Write = mock_write;
    
    return io;
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0; // Need minimum data for testing
    
    // Create mock I/O handler
    cmsIOHANDLER* io = create_mock_iohandler();
    if (!io) return 0;
    
    // Test _cmsWriteUInt32Number
    cmsUInt32Number uint32_val;
    if (Size >= sizeof(cmsUInt32Number)) {
        memcpy(&uint32_val, Data, sizeof(cmsUInt32Number));
        _cmsWriteUInt32Number(io, uint32_val);
    }
    
    // Test _cmsWriteFloat32Number
    cmsFloat32Number float_val;
    if (Size >= sizeof(cmsFloat32Number)) {
        memcpy(&float_val, Data, sizeof(cmsFloat32Number));
        _cmsWriteFloat32Number(io, float_val);
    }
    
    // Test _cmsWriteUInt16Number
    cmsUInt16Number uint16_val;
    if (Size >= sizeof(cmsUInt16Number)) {
        memcpy(&uint16_val, Data, sizeof(cmsUInt16Number));
        _cmsWriteUInt16Number(io, uint16_val);
    }
    
    // Test _cmsWriteTypeBase
    cmsTagTypeSignature type_sig;
    if (Size >= sizeof(cmsTagTypeSignature)) {
        memcpy(&type_sig, Data, sizeof(cmsTagTypeSignature));
        _cmsWriteTypeBase(io, type_sig);
    }
    
    // Test _cmsAdjustEndianess32
    if (Size >= sizeof(cmsUInt32Number)) {
        cmsUInt32Number adjusted = _cmsAdjustEndianess32(uint32_val);
        (void)adjusted; // Use result to avoid compiler warning
    }
    
    // Test cmsWriteRawTag
    // Create a dummy profile for testing
    cmsHPROFILE hProfile = cmsCreateRGBProfile(NULL, NULL, NULL);
    if (hProfile) {
        cmsTagSignature tag_sig;
        if (Size >= sizeof(cmsTagSignature)) {
            memcpy(&tag_sig, Data, sizeof(cmsTagSignature));
            
            // Use remaining data for raw tag
            size_t data_size = Size - sizeof(cmsTagSignature);
            if (data_size > 0) {
                const void* tag_data = Data + sizeof(cmsTagSignature);
                cmsWriteRawTag(hProfile, tag_sig, tag_data, 
                              (cmsUInt32Number)(data_size > UINT32_MAX ? UINT32_MAX : data_size));
            }
        }
        cmsCloseProfile(hProfile);
    }
    
    // Cleanup
    io->Close(io);
    
    return 0;
}