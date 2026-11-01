// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsReadUInt32Number at cmsplugin.c:156:20 in lcms2_plugin.h
// _cmsReadUInt32Number at cmsplugin.c:156:20 in lcms2_plugin.h
// _cmsReadUInt8Number at cmsplugin.c:111:20 in lcms2_plugin.h
// _cmsReadUInt8Number at cmsplugin.c:111:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsWriteUInt8Number at cmsplugin.c:258:20 in lcms2_plugin.h
// _cmsReadFloat32Number at cmsplugin.c:169:20 in lcms2_plugin.h
// _cmsReadFloat32Number at cmsplugin.c:169:20 in lcms2_plugin.h
// _cmsWriteUInt32Number at cmsplugin.c:295:20 in lcms2_plugin.h
// _cmsReadUInt32Number at cmsplugin.c:156:20 in lcms2_plugin.h
// _cmsReadUInt8Number at cmsplugin.c:111:20 in lcms2_plugin.h
// _cmsReadFloat32Number at cmsplugin.c:169:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "lcms2_plugin.h"

/* 
 * Fuzzing harness for lcms2 internal IO functions.
 * We use a union to ensure proper memory layout for the cmsIOHANDLER structure
 * while embedding our custom state.
 */

typedef struct {
    const uint8_t *data;
    size_t size;
    size_t offset;
    cmsContext ctx;
} MockIOState;

/* 
 * We define a union to overlay MockIOState on top of cmsIOHANDLER.
 * This ensures we have enough space and correct alignment.
 * The actual cmsIOHANDLER is at the start of the union.
 */
typedef union {
    cmsIOHANDLER handler;
    struct {
        MockIOState state;
        /* Padding to ensure the union is at least as large as the handler if needed,
           though MockIOState is usually larger or comparable. */
        char padding[1024]; 
    } layout;
} MockIOHandlerUnion;

/* Helper to convert MockIOHandlerUnion to cmsIOHANDLER* for API calls */
static cmsIOHANDLER* GetMockIOHandler(MockIOHandlerUnion* mock) {
    return &mock->handler;
}

/* Read implementation for the mock handler */
static cmsUInt32Number MockRead(cmsIOHANDLER* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    if (!iohandler || !Buffer) return 0;
    
    MockIOHandlerUnion* mock = (MockIOHandlerUnion*)iohandler;
    MockIOState* state = &mock->layout.state;
    
    if (!state || !state->data) return 0;
    
    cmsUInt32Number total = size * count;
    if (state->offset + total > state->size) {
        total = (cmsUInt32Number)(state->size - state->offset);
    }
    
    if (total == 0) return 0;
    
    memcpy(Buffer, state->data + state->offset, total);
    state->offset += total;
    return total / size;
}

/* Seek implementation for the mock handler */
static cmsBool MockSeek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    if (!iohandler) return FALSE;
    
    MockIOHandlerUnion* mock = (MockIOHandlerUnion*)iohandler;
    MockIOState* state = &mock->layout.state;
    
    if (!state) return FALSE;
    
    if (offset > state->size) return FALSE;
    state->offset = offset;
    return TRUE;
}

/* Close implementation for the mock handler */
static cmsBool MockClose(cmsIOHANDLER* iohandler) {
    /* No-op for mock */
    return TRUE;
}

/* Tell implementation for the mock handler */
static cmsUInt32Number MockTell(cmsIOHANDLER* iohandler) {
    if (!iohandler) return 0;
    
    MockIOHandlerUnion* mock = (MockIOHandlerUnion*)iohandler;
    MockIOState* state = &mock->layout.state;
    
    if (!state) return 0;
    return (cmsUInt32Number)state->offset;
}

/* Write implementation for the mock handler (dummy, just returns success) */
static cmsBool MockWrite(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* Buffer) {
    /* We don't actually store data, just simulate success */
    return TRUE;
}

/* Initialize a mock IO handler */
static MockIOHandlerUnion* CreateMockHandler(const uint8_t *data, size_t size) {
    MockIOHandlerUnion* mock = (MockIOHandlerUnion*)malloc(sizeof(MockIOHandlerUnion));
    if (!mock) return NULL;
    
    memset(mock, 0, sizeof(MockIOHandlerUnion));
    
    MockIOState* state = &mock->layout.state;
    state->data = data;
    state->size = size;
    state->offset = 0;
    state->ctx = cmsCreateContext(NULL, NULL);
    
    cmsIOHANDLER* handler = &mock->handler;
    handler->Read = MockRead;
    handler->Seek = MockSeek;
    handler->Close = MockClose;
    handler->Tell = MockTell;
    handler->Write = MockWrite;
    handler->ContextID = state->ctx;
    handler->UsedSpace = 0;
    handler->ReportedSize = (cmsUInt32Number)size;
    handler->stream = NULL;
    memset(handler->PhysicalFile, 0, cmsMAX_PATH);
    
    return mock;
}

/* Cleanup the mock handler */
static void DestroyMockHandler(MockIOHandlerUnion* mock) {
    if (!mock) return;
    MockIOState* state = &mock->layout.state;
    if (state->ctx) {
        cmsDeleteContext(state->ctx);
    }
    free(mock);
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    MockIOHandlerUnion* mock = CreateMockHandler(Data, Size);
    if (!mock) return 0;
    
    cmsIOHANDLER* handler = GetMockIOHandler(mock);
    
    /* Variables to hold read values */
    cmsUInt32Number u32_val = 0;
    cmsUInt8Number u8_val = 0;
    cmsFloat32Number f32_val = 0.0f;
    
    /* 
     * Test 1: _cmsReadUInt32Number
     * Ensure we have enough data to read 4 bytes
     */
    if (Size >= 4) {
        MockSeek(handler, 0);
        _cmsReadUInt32Number(handler, &u32_val);
        /* Also test with NULL pointer (should read but discard) */
        MockSeek(handler, 0);
        _cmsReadUInt32Number(handler, NULL);
    }
    
    /* 
     * Test 2: _cmsReadUInt8Number
     * Ensure we have at least 1 byte
     */
    if (Size >= 1) {
        MockSeek(handler, 0);
        _cmsReadUInt8Number(handler, &u8_val);
        /* Test with NULL pointer */
        MockSeek(handler, 0);
        _cmsReadUInt8Number(handler, NULL);
    }
    
    /* 
     * Test 3: _cmsReadAlignment
     * Try at various offsets to trigger alignment logic
     */
    if (Size >= 1) {
        for (int i = 0; i < 10 && i < (int)Size; i++) {
            MockSeek(handler, i);
            _cmsReadAlignment(handler);
        }
    }
    
    /* 
     * Test 4: _cmsWriteUInt8Number
     * Write a byte (mock implementation always succeeds)
     */
    _cmsWriteUInt8Number(handler, (cmsUInt8Number)(Data[0] & 0xFF));
    
    /* 
     * Test 5: _cmsReadFloat32Number
     * Ensure we have 4 bytes for a float
     */
    if (Size >= 4) {
        MockSeek(handler, 0);
        _cmsReadFloat32Number(handler, &f32_val);
        /* Test with NULL pointer */
        MockSeek(handler, 0);
        _cmsReadFloat32Number(handler, NULL);
    }
    
    /* 
     * Test 6: _cmsWriteUInt32Number
     * Write a 32-bit integer
     */
    cmsUInt32Number write_val = 0;
    if (Size >= 4) {
        write_val = (cmsUInt32Number)(Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24));
    } else {
        write_val = 0;
    }
    _cmsWriteUInt32Number(handler, write_val);
    
    /* 
     * Edge cases: Seek to end or beyond to test error handling
     */
    if (Size > 0) {
        MockSeek(handler, Size);
        _cmsReadUInt32Number(handler, &u32_val);
        _cmsReadUInt8Number(handler, &u8_val);
        _cmsReadFloat32Number(handler, &f32_val);
        _cmsReadAlignment(handler);
    }
    
    /* Cleanup */
    DestroyMockHandler(mock);
    
    return 0;
}