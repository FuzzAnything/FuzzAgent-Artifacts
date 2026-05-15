// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetProfileVersion at cmsio0.c:1147:28 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// _cmsRead15Fixed16Number at cmsplugin.c:224:20 in lcms2_plugin.h
// _cmsWrite15Fixed16Number at cmsplugin.c:337:20 in lcms2_plugin.h
// _cms15Fixed16toDouble at cmsplugin.c:377:28 in lcms2_plugin.h
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
// cmsGetEncodedCMMversion at cmserr.c:30:15 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

// Custom I/O handler for memory operations
static cmsUInt32Number mem_read(cmsIOHANDLER* iohandler, void* Buffer, 
                                cmsUInt32Number size, cmsUInt32Number count) {
    if (!iohandler || !Buffer || size == 0 || count == 0) return 0;
    
    cmsUInt32Number total = size * count;
    cmsUInt32Number available = iohandler->ReportedSize - iohandler->UsedSpace;
    cmsUInt32Number to_read = total < available ? total : available;
    
    if (to_read > 0) {
        memcpy(Buffer, (uint8_t*)iohandler->stream + iohandler->UsedSpace, to_read);
        iohandler->UsedSpace += to_read;
    }
    return to_read / size;
}

static cmsBool mem_seek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    if (!iohandler) return FALSE;
    if (offset > iohandler->ReportedSize) return FALSE;
    iohandler->UsedSpace = offset;
    return TRUE;
}

static cmsBool mem_close(cmsIOHANDLER* iohandler) {
    if (!iohandler) return FALSE;
    // Memory is managed externally, just mark as closed
    iohandler->stream = NULL;
    return TRUE;
}

static cmsUInt32Number mem_tell(cmsIOHANDLER* iohandler) {
    return iohandler ? iohandler->UsedSpace : 0;
}

static cmsBool mem_write(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* Buffer) {
    if (!iohandler || !Buffer || size == 0) return FALSE;
    
    cmsUInt32Number available = iohandler->ReportedSize - iohandler->UsedSpace;
    if (size > available) return FALSE;
    
    memcpy((uint8_t*)iohandler->stream + iohandler->UsedSpace, Buffer, size);
    iohandler->UsedSpace += size;
    return TRUE;
}

static cmsIOHANDLER* create_mem_handler(const uint8_t* data, size_t size, const char* mode) {
    cmsIOHANDLER* handler = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (!handler) return NULL;
    
    memset(handler, 0, sizeof(cmsIOHANDLER));
    handler->stream = (void*)data;
    handler->ContextID = 0;
    handler->UsedSpace = 0;
    handler->ReportedSize = (cmsUInt32Number)size;
    handler->Read = mem_read;
    handler->Seek = mem_seek;
    handler->Close = mem_close;
    handler->Tell = mem_tell;
    handler->Write = mem_write;
    
    return handler;
}

static void destroy_mem_handler(cmsIOHANDLER* handler) {
    if (handler) {
        free(handler);
    }
}

int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Test 1: _cms15Fixed16toDouble
    cmsS15Fixed16Number fix32;
    memcpy(&fix32, Data, sizeof(fix32));
    cmsFloat64Number result1 = _cms15Fixed16toDouble(fix32);
    (void)result1;
    
    // Test 2: _cmsDoubleTo15Fixed16
    cmsFloat64Number double_val;
    if (Size >= 12) {
        memcpy(&double_val, Data + 4, sizeof(double_val));
        cmsS15Fixed16Number result2 = _cmsDoubleTo15Fixed16(double_val);
        (void)result2;
    }
    
    // Test 3: cmsGetEncodedCMMversion
    int version = cmsGetEncodedCMMversion();
    (void)version;
    
    // Test 4: cmsGetProfileVersion
    // Create a simple profile from input data
    if (Size >= 128) {  // Minimum ICC profile size
        cmsHPROFILE profile = cmsOpenProfileFromMem(Data, Size);
        if (profile) {
            cmsFloat64Number profile_version = cmsGetProfileVersion(profile);
            (void)profile_version;
            cmsCloseProfile(profile);
        }
    }
    
    // Test 5 & 6: _cmsRead15Fixed16Number and _cmsWrite15Fixed16Number
    if (Size >= 20) {
        // Create memory handler for reading
        cmsIOHANDLER* read_handler = create_mem_handler(Data, Size, "r");
        if (read_handler) {
            cmsFloat64Number read_number;
            cmsBool read_success = _cmsRead15Fixed16Number(read_handler, &read_number);
            (void)read_success;
            destroy_mem_handler(read_handler);
        }
        
        // Create memory handler for writing
        uint8_t write_buffer[256];
        cmsIOHANDLER* write_handler = create_mem_handler(write_buffer, sizeof(write_buffer), "w");
        if (write_handler) {
            cmsFloat64Number write_number;
            if (Size >= 28) {
                memcpy(&write_number, Data + 20, sizeof(write_number));
            } else {
                write_number = 1.0;
            }
            cmsBool write_success = _cmsWrite15Fixed16Number(write_handler, write_number);
            (void)write_success;
            destroy_mem_handler(write_handler);
        }
    }
    
    return 0;
}