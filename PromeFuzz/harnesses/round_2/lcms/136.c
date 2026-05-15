// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsGetEncodedCMMversion at cmserr.c:30:15 in lcms2.h
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
// _cmsRead15Fixed16Number at cmsplugin.c:224:20 in lcms2_plugin.h
// _cmsWrite15Fixed16Number at cmsplugin.c:337:20 in lcms2_plugin.h
// _cms15Fixed16toDouble at cmsplugin.c:377:28 in lcms2_plugin.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetProfileVersion at cmsio0.c:1147:28 in lcms2.h
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

static cmsUInt32Number dummy_read(cmsIOHANDLER* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    if (size == 0 || count == 0) return 0;
    
    cmsUInt32Number total = size * count;
    cmsUInt32Number to_read = total;
    
    if (iohandler->UsedSpace + total > iohandler->ReportedSize) {
        if (iohandler->UsedSpace >= iohandler->ReportedSize) return 0;
        to_read = iohandler->ReportedSize - iohandler->UsedSpace;
    }
    
    if (to_read > 0) {
        memcpy(Buffer, (char*)iohandler->stream + iohandler->UsedSpace, to_read);
        iohandler->UsedSpace += to_read;
    }
    
    return to_read / size;
}

static cmsBool dummy_seek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    if (offset > iohandler->ReportedSize) return FALSE;
    iohandler->UsedSpace = offset;
    return TRUE;
}

static cmsBool dummy_close(cmsIOHANDLER* iohandler) {
    free(iohandler->stream);
    free(iohandler);
    return TRUE;
}

static cmsUInt32Number dummy_tell(cmsIOHANDLER* iohandler) {
    return iohandler->UsedSpace;
}

static cmsBool dummy_write(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* Buffer) {
    if (size == 0) return TRUE;
    
    if (iohandler->UsedSpace + size > iohandler->ReportedSize) {
        cmsUInt32Number new_size = iohandler->UsedSpace + size;
        void* new_stream = realloc(iohandler->stream, new_size);
        if (!new_stream) return FALSE;
        
        iohandler->stream = new_stream;
        iohandler->ReportedSize = new_size;
    }
    
    memcpy((char*)iohandler->stream + iohandler->UsedSpace, Buffer, size);
    iohandler->UsedSpace += size;
    return TRUE;
}

static cmsIOHANDLER* create_dummy_iohandler(const uint8_t* data, size_t size) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)calloc(1, sizeof(cmsIOHANDLER));
    if (!io) return NULL;
    
    io->stream = malloc(size);
    if (!io->stream) {
        free(io);
        return NULL;
    }
    
    memcpy(io->stream, data, size);
    io->ReportedSize = (cmsUInt32Number)size;
    io->UsedSpace = 0;
    io->Read = dummy_read;
    io->Seek = dummy_seek;
    io->Close = dummy_close;
    io->Tell = dummy_tell;
    io->Write = dummy_write;
    
    return io;
}

int LLVMFuzzerTestOneInput_136(const uint8_t *Data, size_t Size) {
    if (Size < 12) return 0;
    
    // Test _cms15Fixed16toDouble
    cmsS15Fixed16Number fixed_input;
    memcpy(&fixed_input, Data, sizeof(fixed_input));
    cmsFloat64Number double_result = _cms15Fixed16toDouble(fixed_input);
    (void)double_result;
    
    // Test cmsGetProfileVersion
    cmsHPROFILE profile = cmsOpenProfileFromMem(Data, Size);
    if (profile) {
        cmsFloat64Number version = cmsGetProfileVersion(profile);
        (void)version;
        cmsCloseProfile(profile);
    }
    
    // Test cmsGetEncodedCMMversion
    int cmm_version = cmsGetEncodedCMMversion();
    (void)cmm_version;
    
    // Test _cmsDoubleTo15Fixed16
    cmsFloat64Number double_input;
    memcpy(&double_input, Data + 4, sizeof(double_input));
    cmsS15Fixed16Number fixed_result = _cmsDoubleTo15Fixed16(double_input);
    (void)fixed_result;
    
    // Test _cmsRead15Fixed16Number
    cmsIOHANDLER* read_io = create_dummy_iohandler(Data, Size);
    if (read_io) {
        cmsFloat64Number read_number;
        cmsBool read_success = _cmsRead15Fixed16Number(read_io, &read_number);
        (void)read_success;
        read_io->Close(read_io);
    }
    
    // Test _cmsWrite15Fixed16Number
    cmsIOHANDLER* write_io = create_dummy_iohandler(NULL, 0);
    if (write_io) {
        cmsFloat64Number write_number = 1.0;
        if (Size >= 20) {
            memcpy(&write_number, Data + 12, sizeof(write_number));
        }
        cmsBool write_success = _cmsWrite15Fixed16Number(write_io, write_number);
        (void)write_success;
        write_io->Close(write_io);
    }
    
    return 0;
}