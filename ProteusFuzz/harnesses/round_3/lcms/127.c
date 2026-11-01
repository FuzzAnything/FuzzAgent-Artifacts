// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsDecodeDateTimeNumber at cmsplugin.c:390:16 in lcms2_plugin.h
// _cmsDecodeDateTimeNumber at cmsplugin.c:390:16 in lcms2_plugin.h
// _cmsEncodeDateTimeNumber at cmsplugin.c:407:16 in lcms2_plugin.h
// cmsGetHeaderCreationDateTime at cmsio0.c:1062:20 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsSaveProfileToIOhandler at cmsio0.c:1446:27 in lcms2.h
// cmsSaveProfileToIOhandler at cmsio0.c:1446:27 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsWriteUInt16Number at cmsplugin.c:268:20 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static cmsContext g_context = NULL;
static cmsHPROFILE g_profile = NULL;

static void cleanup_resources(void) {
    if (g_profile != NULL) {
        cmsCloseProfile(g_profile);
        g_profile = NULL;
    }
    if (g_context != NULL) {
        cmsDeleteContext(g_context);
        g_context = NULL;
    }
}

static cmsBool my_write_handler(cmsIOHANDLER* io, cmsUInt32Number size, const void* Buffer) {
    if (!io || !Buffer) return FALSE;
    return TRUE;
}

static cmsUInt32Number my_read_handler(cmsIOHANDLER* io, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    if (!io || !Buffer) return 0;
    return 0;
}

static cmsBool my_seek_handler(cmsIOHANDLER* io, cmsUInt32Number offset) {
    return TRUE;
}

static cmsBool my_close_handler(cmsIOHANDLER* io) {
    return TRUE;
}

static cmsUInt32Number my_tell_handler(cmsIOHANDLER* io) {
    return 0;
}

static cmsIOHANDLER* create_dummy_io_handler(void) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (!io) return NULL;
    memset(io, 0, sizeof(cmsIOHANDLER));
    io->stream = NULL;
    io->ContextID = NULL;
    io->UsedSpace = 0;
    io->ReportedSize = 0;
    memset(io->PhysicalFile, 0, cmsMAX_PATH);
    io->Read = my_read_handler;
    io->Seek = my_seek_handler;
    io->Close = my_close_handler;
    io->Tell = my_tell_handler;
    io->Write = my_write_handler;
    return io;
}

int LLVMFuzzerTestOneInput_127(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cleanup_resources();

    // Initialize context
    g_context = cmsCreateContext(NULL, NULL);
    if (!g_context) return 0;

    // Create a dummy profile for testing using cmsCreateProfilePlaceholder
    g_profile = cmsCreateProfilePlaceholder(g_context);
    if (!g_profile) {
        cmsDeleteContext(g_context);
        g_context = NULL;
        return 0;
    }

    // Fuzz _cmsWriteUInt16Number
    cmsIOHANDLER* io = create_dummy_io_handler();
    if (io) {
        cmsUInt16Number val16 = 0;
        if (Size >= 2) {
            val16 = (Data[0] << 8) | Data[1];
        } else if (Size == 1) {
            val16 = Data[0];
        }
        _cmsWriteUInt16Number(io, val16);
        free(io);
        io = NULL;
    }

    // Fuzz _cmsDecodeDateTimeNumber and _cmsEncodeDateTimeNumber
    cmsDateTimeNumber dt;
    struct tm tm_time;
    memset(&dt, 0, sizeof(dt));
    memset(&tm_time, 0, sizeof(tm_time));

    if (Size >= 10) {
        dt.year = (Data[0] << 8) | Data[1];
        dt.month = (Data[2] << 8) | Data[3];
        dt.day = (Data[4] << 8) | Data[5];
        dt.hours = (Data[6] << 8) | Data[7];
        dt.minutes = (Data[8] << 8) | Data[9];
        dt.seconds = 0;
        _cmsDecodeDateTimeNumber(&dt, &tm_time);
    } else {
        // Use default values if input is too small
        dt.year = 2023;
        dt.month = 1;
        dt.day = 1;
        dt.hours = 0;
        dt.minutes = 0;
        dt.seconds = 0;
        _cmsDecodeDateTimeNumber(&dt, &tm_time);
    }

    // Encode back
    _cmsEncodeDateTimeNumber(&dt, &tm_time);

    // Fuzz cmsGetHeaderCreationDateTime
    struct tm tm_dest;
    memset(&tm_dest, 0, sizeof(tm_dest));
    cmsGetHeaderCreationDateTime(g_profile, &tm_dest);

    // Fuzz cmsWriteTag
    if (Size >= 4) {
        cmsTagSignature tag_sig = (cmsTagSignature)(Data[0] << 24 | Data[1] << 16 | Data[2] << 8 | Data[3]);
        cmsWriteTag(g_profile, tag_sig, &Data[4]);
        cmsWriteTag(g_profile, tag_sig, NULL); // Try to delete
    }

    // Fuzz cmsSaveProfileToIOhandler
    cmsIOHANDLER* io_save = create_dummy_io_handler();
    if (io_save) {
        cmsUInt32Number bytes = cmsSaveProfileToIOhandler(g_profile, io_save);
        (void)bytes; // Use return value
        free(io_save);
    }

    // Try saving to internal buffer (NULL io)
    cmsUInt32Number bytes_internal = cmsSaveProfileToIOhandler(g_profile, NULL);
    (void)bytes_internal;

    cleanup_resources();
    return 0;
}