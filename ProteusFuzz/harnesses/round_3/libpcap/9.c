// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_datalink at pcap.c:2938:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

static void cleanup_pcap(pcap_t *handle, struct bpf_program *prog, pcap_dumper_t *dumper) {
    if (prog != NULL && prog->bf_insns != NULL) {
        pcap_freecode(prog);
    }
    if (dumper != NULL) {
        pcap_dump_close(dumper);
    }
    if (handle != NULL) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    // Prepare environment
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    struct bpf_program prog;
    pcap_dumper_t *dumper = NULL;

    // Initialize bpf_program
    memset(&prog, 0, sizeof(struct bpf_program));

    // Create a dummy file for pcap_dump_open
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp != NULL) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Try to open a handle using the dummy file (savefile mode)
    // pcap_open_offline activates the handle automatically for reading
    handle = pcap_open_offline("./dummy_file", errbuf);
    
    // If opening fails, try to create a live handle as fallback
    if (handle == NULL) {
        handle = pcap_create("any", errbuf);
        if (handle != NULL) {
            // Set some options to try to activate it
            pcap_set_snaplen(handle, 65535);
            pcap_set_promisc(handle, 1);
            pcap_set_timeout(handle, 1000);
            pcap_set_buffer_size(handle, 262144);
            // Do not call pcap_activate here if we are going to use it for compile
            // pcap_compile requires an active handle, but pcap_open_offline is already active.
            // For pcap_create, we must activate.
            int activate_res = pcap_activate(handle);
            if (activate_res != 0) {
                // If activation fails, handle might be invalid for further ops
                pcap_close(handle);
                handle = NULL;
            }
        }
    }

    if (handle == NULL) {
        return 0;
    }

    // 1. pcap_compile
    // Construct a filter string from input data, ensuring it's null-terminated
    char filter[256];
    size_t filter_len = (Size < 255) ? Size : 255;
    memcpy(filter, Data, filter_len);
    filter[filter_len] = '\0';

    // Use part of input as netmask safely
    bpf_u_int32 netmask = 0;
    if (Size >= filter_len + 4) {
        // Read 4 bytes manually to avoid alignment issues and ensure bounds
        netmask = ((bpf_u_int32)Data[filter_len] | 
                   ((bpf_u_int32)Data[filter_len + 1] << 8) | 
                   ((bpf_u_int32)Data[filter_len + 2] << 16) | 
                   ((bpf_u_int32)Data[filter_len + 3] << 24));
    }

    int optimize = 0;
    if (Size > filter_len + 4) {
        optimize = (Data[filter_len + 4] % 2);
    }

    int compile_res = pcap_compile(handle, &prog, filter, optimize, netmask);

    // 2. pcap_geterr (after pcap_compile)
    const char *err1 = pcap_geterr(handle);
    (void)err1; 

    // 3. pcap_setfilter
    int setfilter_res = -1;
    if (compile_res == 0 && prog.bf_insns != NULL) {
        setfilter_res = pcap_setfilter(handle, &prog);
    }

    // 4. pcap_geterr (after pcap_setfilter)
    const char *err2 = pcap_geterr(handle);
    (void)err2;

    // 5. pcap_dump_open
    dumper = pcap_dump_open(handle, "./dummy_file");

    // 6. pcap_geterr (after pcap_dump_open)
    const char *err3 = pcap_geterr(handle);
    (void)err3;

    // 7. pcap_datalink
    int linktype = -1;
    
    // pcap_datalink requires an activated handle.
    // pcap_open_offline returns an activated handle.
    // pcap_create + pcap_activate returns an activated handle.
    // We do NOT call pcap_activate again here because:
    // 1. If it's already active, calling it again might be a no-op or error depending on backend.
    // 2. The crash in the previous version happened because pcap_activate was called on a handle
    //    that was created via pcap_create but failed activation or was in a bad state, 
    //    or the device name was null/invalid in the internal structure.
    // Since we handled activation during creation above, we assume handle is ready if not NULL.
    // However, to be safe and strictly follow "call only after activate", we check if we need to activate.
    // But pcap_open_offline is already active.
    // Let's just call pcap_datalink directly. If the handle is valid and active, it works.
    // If not, it returns an error code (PCAP_ERROR_NOT_ACTIVATED) without crashing if the handle struct is valid.
    // The crash was due to internal NULL pointer in pcap_activate logic when device name was missing.
    // Since we passed "any" to pcap_create, device name should be set.
    // The safest path is to just call pcap_datalink.
    
    linktype = pcap_datalink(handle);

    // Cleanup
    cleanup_pcap(handle, &prog, dumper);

    return 0;
}