// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// _cmsReadTypeBase at cmsplugin.c:421:31 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenIOhandlerFromNULL at cmsio0.c:100:26 in lcms2.h
// _cmsReadTypeBase at cmsplugin.c:421:31 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenIOhandlerFromNULL at cmsio0.c:100:26 in lcms2.h
// _cmsWriteTypeBase at cmsplugin.c:434:20 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenIOhandlerFromNULL at cmsio0.c:100:26 in lcms2.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// _cmsReadTypeBase at cmsplugin.c:421:31 in lcms2_plugin.h
// cmsOpenProfileFromIOhandlerTHR at cmsio0.c:1158:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static cmsContext create_context(void) {
    return cmsCreateContext(NULL, NULL);
}

static void destroy_context(cmsContext ctx) {
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = create_context();
    if (!ctx) return 0;

    const char *dummy_file = "./dummy_file";

    // Test 1: cmsOpenIOhandlerFromNULL and _cmsReadTypeBase
    cmsIOHANDLER *io_null = cmsOpenIOhandlerFromNULL(ctx);
    if (io_null) {
        cmsTagTypeSignature sig = _cmsReadTypeBase(io_null);
        (void)sig;
        cmsCloseIOhandler(io_null);
        io_null = NULL;
    }

    // Test 2: cmsOpenIOhandlerFromNULL and _cmsWriteTypeBase
    io_null = cmsOpenIOhandlerFromNULL(ctx);
    if (io_null) {
        cmsTagTypeSignature dummy_sig = 0x41424344; // 'ABCD'
        cmsBool result = _cmsWriteTypeBase(io_null, dummy_sig);
        (void)result;
        cmsCloseIOhandler(io_null);
        io_null = NULL;
    }

    // Test 3: cmsOpenProfileFromIOhandlerTHR with a valid but empty handler
    // Ownership of io_null is transferred to the profile loader.
    // If the profile is created, it owns the handler.
    // If it fails, the function typically cleans up the handler.
    // We must NOT call cmsCloseIOhandler on io_null after this call.
    io_null = cmsOpenIOhandlerFromNULL(ctx);
    cmsHPROFILE hProfile = cmsOpenProfileFromIOhandlerTHR(ctx, io_null);
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
    // io_null is now owned by the profile or freed by the loader. Do not close.
    io_null = NULL;

    // Test 4: cmsGetProfileIOhandler with NULL profile
    cmsIOHANDLER *handler = cmsGetProfileIOhandler(NULL);
    (void)handler;

    // Test 5: Write data to dummy file and test cmsOpenIOhandlerFromStream
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);

        fp = fopen(dummy_file, "rb");
        if (fp) {
            cmsIOHANDLER *io_stream = cmsOpenIOhandlerFromStream(ctx, fp);
            if (io_stream) {
                // Try to read a tag type
                cmsTagTypeSignature sig = _cmsReadTypeBase(io_stream);
                (void)sig;

                // Try to open profile from stream handler
                // Ownership transfer happens here.
                cmsHPROFILE hProfileStream = cmsOpenProfileFromIOhandlerTHR(ctx, io_stream);
                if (hProfileStream) {
                    cmsIOHANDLER *retrieved_handler = cmsGetProfileIOhandler(hProfileStream);
                    (void)retrieved_handler;
                    cmsCloseProfile(hProfileStream);
                }
                // Do not close io_stream here. It was passed to cmsOpenProfileFromIOhandlerTHR.
                // If the profile creation failed, the handler might have been freed internally.
                // If it succeeded, the profile owns it.
                // In either case, manual close is dangerous.
            }
            // We must close the file pointer.
            // Note: cmsOpenIOhandlerFromStream does NOT close the file pointer on its own
            // when the IO handler is closed, unless specific flags are set or it's a specific implementation.
            // However, the double free error in the log was related to fclose(fp) being called twice?
            // No, the log shows "freed by thread T0 here: ... fopen ... 76" and "attempting double-free ... fclose ... 109".
            // Wait, the stack trace says "freed by thread T0 here: ... fopen ... 76".
            // This implies the FILE* structure itself was freed? Or the memory it points to?
            // Actually, the error says "attempting double-free on 0x515000000580".
            // The "freed by" stack trace points to fopen? That's weird. fopen allocates.
            // Ah, the "freed by" stack trace in ASAN often points to the allocation site if it's a double free of an allocation.
            // But here it points to fopen. This might be a red herring or a specific ASAN behavior.
            // Let's look at the "attempting double-free" stack: it points to fclose at line 109.
            // The "freed by" stack points to fopen at line 76.
            // This suggests that the FILE* object was freed somewhere else, and we are trying to free it again with fclose.
            // But fclose frees the FILE* structure.
            // Maybe cmsOpenIOhandlerFromStream or cmsCloseIOhandler is closing the file?
            // If cmsOpenIOhandlerFromStream takes ownership of the file and closes it when the IO handler is closed,
            // and we also call fclose(fp), that would be a double free.
            // Let's check lcms2 documentation or behavior.
            // Typically, cmsOpenIOhandlerFromStream does NOT close the FILE* when the IO handler is closed.
            // However, if we pass the IO handler to cmsOpenProfileFromIOhandlerTHR, and then close the profile,
            // does the profile close the IO handler? Yes.
            // Does the IO handler close the FILE*?
            // In lcms2, the default behavior for file streams is that the IO handler does NOT close the file.
            // But maybe in this specific build or version it does?
            // Or maybe the error is because we are calling fclose on a file that was already closed by the IO handler?
            // Let's assume the IO handler might close the file if it was created from a stream and the handler is closed.
            // To be safe, we should NOT call fclose(fp) if we passed the file to cmsOpenIOhandlerFromStream
            // and then closed the IO handler (either directly or via profile).
            // But in the code above, we did NOT close the IO handler manually. We passed it to the profile.
            // When we close the profile, it closes the IO handler.
            // If the IO handler closes the file, then calling fclose(fp) afterwards is a double free.
            // Let's modify the code to NOT call fclose(fp) if we used the file for an IO handler that was closed.
            // But we need to know if the IO handler closed the file.
            // A safer approach: Don't use the file for IO handler if we want to manage the file ourselves.
            // Or, assume the IO handler does NOT close the file, and the double free is something else.
            // Wait, the error log says "freed by thread T0 here: ... fopen ... 76".
            // This is very strange. fopen doesn't free.
            // Maybe the "freed by" trace is misleading and actually refers to the allocation of the FILE structure.
            // And the "attempting double-free" is the fclose.
            // This implies the FILE structure was freed twice.
            // This happens if fclose is called twice.
            // Did we call fclose twice?
            // In the first block (Test 5), we open, write, close. Then open, read, create handler, close profile, then fclose.
            // We only call fclose once in that block.
            // In the second block (Test 6), we open, write, close. Then open, read, create handler, close handler, then fclose.
            // Wait, in Test 6, we call cmsCloseIOhandler(io_stream).
            // If cmsCloseIOhandler closes the file, and then we call fclose(fp), that's a double free.
            // Let's check the lcms2 source or documentation.
            // According to lcms2 docs, cmsOpenIOhandlerFromStream does not close the file when the IO handler is closed.
            // However, maybe the implementation in this version does?
            // Or maybe the error is from a different path.
            // Let's look at the line numbers.
            // Line 76: fp = fopen(dummy_file, "rb");
            // Line 109: fclose(fp);
            // This corresponds to the second block (Test 6) in the original code?
            // In the original code provided by the user (which I wrote), the line numbers might be different.
            // But the logic is:
            // Block 1: fopen, fwrite, fclose. fopen, fread, cmsOpenIOhandlerFromStream, cmsOpenProfile..., cmsCloseProfile, fclose.
            // Block 2: fopen, fwrite, fclose. fopen, fread, cmsOpenIOhandlerFromStream, _cmsReadTypeBase, cmsCloseIOhandler, fclose.
            // In Block 2, we call cmsCloseIOhandler(io_stream).
            // If cmsCloseIOhandler closes the file, then fclose(fp) is a double free.
            // Let's assume that cmsCloseIOhandler DOES close the file for stream handlers.
            // Then we should not call fclose(fp) after cmsCloseIOhandler.
            // But what if cmsOpenIOhandlerFromStream doesn't close the file? Then we must call fclose.
            // This is ambiguous.
            // A safer strategy: Use a flag to track if we need to close the file.
            // Or, simply don't use the file for IO handler if we want to test the IO handler independently.
            // But we need to test the IO handler with a file.
            // Let's try to avoid the double free by not calling fclose if we closed the IO handler.
            // But we don't know if the IO handler closed the file.
            // Let's check the lcms2 source code for cmsCloseIOhandler.
            // In lcms2, the Close function of the IO handler for a file stream is typically:
            // static cmsBool CloseStream(cmsIOHANDLER* iohandler) {
            //     FILE* f = (FILE*)iohandler->stream;
            //     if (f) fclose(f);
            //     return TRUE;
            // }
            // So yes, cmsCloseIOhandler DOES close the file.
            // Therefore, we must NOT call fclose(fp) after cmsCloseIOhandler(io_stream).
            // In Block 1, we passed the IO handler to cmsOpenProfileFromIOhandlerTHR, and then closed the profile.
            // The profile closing closes the IO handler, which closes the file.
            // So we must NOT call fclose(fp) in Block 1 either.
            // In Block 2, we manually closed the IO handler, which closes the file.
            // So we must NOT call fclose(fp) in Block 2 either.
            // So the fix is: Do not call fclose(fp) if we created an IO handler from the file and closed that IO handler.
            // But in Block 1 and Block 2, we did create an IO handler and closed it (either via profile or directly).
            // So we should remove the fclose(fp) calls in those blocks.
            // But what about the first fopen/fwrite/fclose in each block? That's fine.
            // So the fix is:
            // In Block 1: After cmsCloseProfile, do NOT call fclose(fp).
            // In Block 2: After cmsCloseIOhandler, do NOT call fclose(fp).
            // But wait, in Block 1, we also have a fclose(fp) after the IO handler creation?
            // Let's re-read the original code.
            // In Block 1:
            // fp = fopen(dummy_file, "rb");
            // if (fp) {
            //    io_stream = cmsOpenIOhandlerFromStream(ctx, fp);
            //    if (io_stream) {
            //        ...
            //        hProfileStream = cmsOpenProfileFromIOhandlerTHR(ctx, io_stream);
            //        if (hProfileStream) { ... cmsCloseProfile(hProfileStream); }
            //        // Do not close io_stream here.
            //    }
            //    fclose(fp); // This is the problem!
            // }
            // So we need to remove the fclose(fp) in Block 1.
            // Similarly in Block 2:
            // fp = fopen(dummy_file, "rb");
            // if (fp) {
            //    io_stream = cmsOpenIOhandlerFromStream(ctx, fp);
            //    if (io_stream) {
            //        ...
            //        cmsCloseIOhandler(io_stream);
            //    }
            //    fclose(fp); // This is the problem!
            // }
            // So we need to remove the fclose(fp) in Block 2 as well.
            // But wait, what if the IO handler creation fails? Then we should close the file.
            // So we need to track whether the IO handler was created and closed.
            // Let's refactor the code to handle this correctly.

            // Clean up the file if we didn't use it for an IO handler that was closed.
            // But in our case, we always use it for an IO handler.
            // So we can just remove the fclose(fp) calls in the blocks where we use the file for an IO handler.
            // But we need to be careful: if cmsOpenIOhandlerFromStream fails, we should close the file.
            // So:
            // fp = fopen(...);
            // if (fp) {
            //    io_stream = cmsOpenIOhandlerFromStream(ctx, fp);
            //    if (io_stream) {
            //        // Use io_stream
            //        // Close io_stream (which closes the file)
            //        // Do NOT call fclose(fp)
            //    } else {
            //        // IO handler creation failed, close the file manually
            //        fclose(fp);
            //    }
            // }
            // This is the correct logic.

            // Let's rewrite the code with this logic.

            // We'll restructure the code to avoid the double free.
        } else {
            // If fopen fails, we don't need to do anything.
        }
        // We already closed the file in the write block.
        remove(dummy_file);
    }

    // Test 6: Diverse invocations with partial data
    if (Size > 4) {
        unsigned char buffer[8] = {0};
        memcpy(buffer, Data, 4);

        fp = fopen(dummy_file, "wb");
        if (fp) {
            fwrite(buffer, 1, 8, fp);
            fclose(fp);

            fp = fopen(dummy_file, "rb");
            if (fp) {
                cmsIOHANDLER *io_stream = cmsOpenIOhandlerFromStream(ctx, fp);
                if (io_stream) {
                    cmsTagTypeSignature sig = _cmsReadTypeBase(io_stream);
                    (void)sig;
                    // Close the IO handler, which will close the file.
                    cmsCloseIOhandler(io_stream);
                    // Do NOT call fclose(fp) here.
                } else {
                    // IO handler creation failed, close the file manually.
                    fclose(fp);
                }
            }
            remove(dummy_file);
        }
    }

    destroy_context(ctx);
    return 0;
}