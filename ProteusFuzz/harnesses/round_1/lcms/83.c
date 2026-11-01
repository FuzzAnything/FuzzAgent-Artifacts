// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsGetHeaderCreationDateTime at cmsio0.c:1062:20 in lcms2.h
// _cmsEncodeDateTimeNumber at cmsplugin.c:407:16 in lcms2_plugin.h
// _cmsDecodeDateTimeNumber at cmsplugin.c:390:16 in lcms2_plugin.h
// _cmsWriteUInt16Number at cmsplugin.c:268:20 in lcms2_plugin.h
// cmsSaveProfileToIOhandler at cmsio0.c:1446:27 in lcms2.h
// cmsSaveProfileToIOhandler at cmsio0.c:1446:27 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

// Forward declarations for internal file IO helpers if not exported, 
// or implement custom wrappers if necessary. 
// Since these are internal to lcms2, we will implement simple wrappers 
// that use standard FILE* operations to satisfy the IO handler interface.

static cmsUInt32Number dummy_read(struct _cms_io_handler* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    if (iohandler->stream == NULL) return 0;
    FILE *fp = (FILE*)iohandler->stream;
    return fread(Buffer, size, count, fp);
}

static cmsBool dummy_seek(struct _cms_io_handler* iohandler, cmsUInt32Number offset) {
    if (iohandler->stream == NULL) return FALSE;
    FILE *fp = (FILE*)iohandler->stream;
    return (fseek(fp, offset, SEEK_SET) == 0) ? TRUE : FALSE;
}

static cmsBool dummy_close(struct _cms_io_handler* iohandler) {
    if (iohandler->stream == NULL) return TRUE;
    FILE *fp = (FILE*)iohandler->stream;
    fclose(fp);
    iohandler->stream = NULL;
    return TRUE;
}

static cmsUInt32Number dummy_tell(struct _cms_io_handler* iohandler) {
    if (iohandler->stream == NULL) return 0;
    FILE *fp = (FILE*)iohandler->stream;
    return (cmsUInt32Number)ftell(fp);
}

static cmsBool dummy_write(struct _cms_io_handler* iohandler, cmsUInt32Number size, const void* Buffer) {
    if (iohandler->stream == NULL) return FALSE;
    FILE *fp = (FILE*)iohandler->stream;
    return (fwrite(Buffer, 1, size, fp) == size) ? TRUE : FALSE;
}

static cmsContext g_ctx = NULL;

static cmsContext get_context(const uint8_t *Data, size_t Size) {
    (void)Data;
    (void)Size;
    if (g_ctx == NULL) {
        g_ctx = cmsCreateContext(NULL, NULL);
        if (g_ctx == NULL) return NULL;
    }
    return g_ctx;
}

static cmsHPROFILE create_dummy_profile(cmsContext ctx) {
    // Create a standard sRGB profile which is a valid starting point
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (hProfile == NULL) return NULL;
    return hProfile;
}

static cmsIOHANDLER* create_dummy_iohandler(const uint8_t *Data, size_t Size) {
    cmsIOHANDLER *io = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (io == NULL) return NULL;
    
    memset(io, 0, sizeof(cmsIOHANDLER));
    
    // Create a dummy file to back the IO handler
    FILE *fp = fopen("./dummy_file", "wb+");
    if (fp == NULL) {
        free(io);
        return NULL;
    }
    
    if (Size > 0) {
        fwrite(Data, 1, Size, fp);
    }
    // Reset file pointer to beginning for reading
    rewind(fp);
    
    io->stream = fp;
    io->ContextID = get_context(Data, Size);
    io->UsedSpace = 0;
    io->ReportedSize = Size;
    strncpy(io->PhysicalFile, "./dummy_file", cmsMAX_PATH - 1);
    io->PhysicalFile[cmsMAX_PATH - 1] = '\0';
    
    // Assign custom file IO functions
    io->Read = dummy_read;
    io->Seek = dummy_seek;
    io->Close = dummy_close;
    io->Tell = dummy_tell;
    io->Write = dummy_write;
    
    return io;
}

static void cleanup_iohandler(cmsIOHANDLER *io) {
    if (io == NULL) return;
    if (io->Close != NULL) {
        io->Close(io);
    }
    free(io);
}

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = get_context(Data, Size);
    if (ctx == NULL) return 0;

    cmsHPROFILE hProfile = create_dummy_profile(ctx);
    if (hProfile == NULL) return 0;

    // Fuzz cmsWriteTag
    // Construct a tag signature from input data
    cmsTagSignature sig;
    if (Size >= 4) {
        sig = (cmsTagSignature)(Data[0] << 24 | Data[1] << 16 | Data[2] << 8 | Data[3]);
    } else {
        sig = 0;
    }
    
    // Write tag with data
    cmsBool res1 = cmsWriteTag(hProfile, sig, (Size > 4 ? &Data[4] : NULL));
    (void)res1;

    // Fuzz cmsWriteTag with NULL data (delete)
    cmsBool res2 = cmsWriteTag(hProfile, sig, NULL);
    (void)res2;

    // Fuzz cmsGetHeaderCreationDateTime
    struct tm tm_dest;
    memset(&tm_dest, 0, sizeof(struct tm));
    cmsBool res3 = cmsGetHeaderCreationDateTime(hProfile, &tm_dest);
    (void)res3;

    // Fuzz _cmsEncodeDateTimeNumber
    cmsDateTimeNumber dt_num;
    memset(&dt_num, 0, sizeof(cmsDateTimeNumber));
    _cmsEncodeDateTimeNumber(&dt_num, &tm_dest);

    // Fuzz _cmsDecodeDateTimeNumber
    struct tm tm_decoded;
    memset(&tm_decoded, 0, sizeof(struct tm));
    _cmsDecodeDateTimeNumber(&dt_num, &tm_decoded);

    // Fuzz _cmsWriteUInt16Number and cmsSaveProfileToIOhandler
    cmsIOHANDLER *io = create_dummy_iohandler(Data, Size);
    if (io != NULL) {
        cmsUInt16Number val = (cmsUInt16Number)(Size >= 2 ? (Data[0] << 8 | Data[1]) : 0);
        cmsBool res4 = _cmsWriteUInt16Number(io, val);
        (void)res4;
        
        // Fuzz cmsSaveProfileToIOhandler
        cmsUInt32Number res5 = cmsSaveProfileToIOhandler(hProfile, io);
        (void)res5;
        
        cleanup_iohandler(io);
    }

    // Fuzz cmsSaveProfileToIOhandler with NULL io (internal buffer)
    cmsUInt32Number res6 = cmsSaveProfileToIOhandler(hProfile, NULL);
    (void)res6;

    cmsCloseProfile(hProfile);

    return 0;
}