// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsAdjustEndianess32 at cmsplugin.c:58:28 in lcms2_plugin.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsWriteRawTag at cmsio0.c:2034:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsWriteUInt32Number at cmsplugin.c:295:20 in lcms2_plugin.h
// _cmsWriteFloat32Number at cmsplugin.c:309:20 in lcms2_plugin.h
// _cmsWriteUInt16Number at cmsplugin.c:268:20 in lcms2_plugin.h
// _cmsWrite15Fixed16Number at cmsplugin.c:337:20 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

// Static helper functions for dummy IOHANDLER
static cmsUInt32Number dummy_read(struct _cms_io_handler* iohandler, void* Buffer, 
                                  cmsUInt32Number size, cmsUInt32Number count) {
    return 0;
}

static cmsBool dummy_seek(struct _cms_io_handler* iohandler, cmsUInt32Number offset) {
    if (offset > iohandler->ReportedSize)
        return FALSE;
    iohandler->UsedSpace = offset;
    return TRUE;
}

static cmsBool dummy_close(struct _cms_io_handler* iohandler) {
    return TRUE;
}

static cmsUInt32Number dummy_tell(struct _cms_io_handler* iohandler) {
    return iohandler->UsedSpace;
}

static cmsBool dummy_write(struct _cms_io_handler* iohandler, cmsUInt32Number size, 
                           const void* Buffer) {
    if (iohandler->UsedSpace + size > iohandler->ReportedSize)
        return FALSE;
    iohandler->UsedSpace += size;
    return TRUE;
}

// Helper function to create a minimal cmsIOHANDLER for testing
static cmsIOHANDLER* create_dummy_iohandler(void) {
    static uint8_t buffer[1024];
    static cmsIOHANDLER dummy_handler;
    
    dummy_handler.stream = buffer;
    dummy_handler.ContextID = NULL;
    dummy_handler.UsedSpace = 0;
    dummy_handler.ReportedSize = sizeof(buffer);
    dummy_handler.PhysicalFile[0] = '\0';
    
    dummy_handler.Read = dummy_read;
    dummy_handler.Seek = dummy_seek;
    dummy_handler.Close = dummy_close;
    dummy_handler.Tell = dummy_tell;
    dummy_handler.Write = dummy_write;
    
    return &dummy_handler;
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create dummy IOHANDLER for write functions
    cmsIOHANDLER* io = create_dummy_iohandler();
    
    // Test _cmsWriteUInt32Number
    cmsUInt32Number uint32_val = 0;
    if (Size >= sizeof(cmsUInt32Number)) {
        memcpy(&uint32_val, Data, sizeof(cmsUInt32Number));
    }
    _cmsWriteUInt32Number(io, uint32_val);
    
    // Reset handler for next test
    io->UsedSpace = 0;
    
    // Test _cmsWriteFloat32Number
    cmsFloat32Number float32_val = 0.0f;
    if (Size >= sizeof(cmsFloat32Number)) {
        memcpy(&float32_val, Data, sizeof(cmsFloat32Number));
    }
    _cmsWriteFloat32Number(io, float32_val);
    
    // Reset handler for next test
    io->UsedSpace = 0;
    
    // Test _cmsWriteUInt16Number
    cmsUInt16Number uint16_val = 0;
    if (Size >= sizeof(cmsUInt16Number)) {
        memcpy(&uint16_val, Data, sizeof(cmsUInt16Number));
    }
    _cmsWriteUInt16Number(io, uint16_val);
    
    // Reset handler for next test
    io->UsedSpace = 0;
    
    // Test _cmsWrite15Fixed16Number
    cmsFloat64Number fixed_val = 0.0;
    if (Size >= sizeof(cmsFloat64Number)) {
        memcpy(&fixed_val, Data, sizeof(cmsFloat64Number));
    }
    _cmsWrite15Fixed16Number(io, fixed_val);
    
    // Test _cmsAdjustEndianess32
    cmsUInt32Number adjusted = _cmsAdjustEndianess32(uint32_val);
    
    // Test cmsWriteRawTag - create a dummy profile first
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context) {
        // Create a simple sRGB profile instead of RGB profile with NULL primaries
        cmsHPROFILE hProfile = cmsCreate_sRGBProfileTHR(context);
        
        if (hProfile) {
            // Use remaining data for raw tag
            cmsTagSignature sig = 0;
            size_t data_offset = 0;
            
            if (Size >= sizeof(cmsTagSignature)) {
                memcpy(&sig, Data, sizeof(cmsTagSignature));
                data_offset = sizeof(cmsTagSignature);
            }
            
            const void* tag_data = NULL;
            cmsUInt32Number tag_size = 0;
            
            if (Size > data_offset) {
                tag_data = Data + data_offset;
                tag_size = (Size - data_offset) > 1024 ? 1024 : 
                          (cmsUInt32Number)(Size - data_offset);
                
                cmsWriteRawTag(hProfile, sig, tag_data, tag_size);
            }
            
            cmsCloseProfile(hProfile);
        }
        cmsDeleteContext(context);
    }
    
    // Clean up dummy handler
    io->Close(io);
    
    return 0;
}