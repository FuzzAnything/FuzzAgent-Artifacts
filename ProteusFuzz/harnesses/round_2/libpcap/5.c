// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <bpf.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

static void cleanup_pcap(pcap_t *handle) {
    if (handle != NULL) {
        pcap_close(handle);
    }
}

static void cleanup_bpf_program(struct bpf_program *prog) {
    if (prog != NULL && prog->bf_insns != NULL) {
        free(prog->bf_insns);
        prog->bf_insns = NULL;
        prog->bf_len = 0;
    }
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    pcap_t *handle = NULL;
    struct bpf_program prog = {0};
    char errbuf[PCAP_ERRBUF_SIZE];
    struct pcap_pkthdr *header = NULL;
    const u_char *packet = NULL;
    int ret;

    // Ensure we have enough data to create a dummy file and potentially a filter
    if (Size < 4) {
        return 0;
    }

    // Step 0: Create a dummy file for offline capture
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        // Write a minimal pcap header (24 bytes) + some dummy data
        // Magic number, version, timezone, sigfigs, snaplen, network
        uint32_t magic = 0xa1b2c3d4;
        uint16_t version_major = 2;
        uint16_t version_minor = 4;
        int32_t tz = 0;
        int32_t sigfigs = 0;
        uint32_t snaplen = 65535;
        uint32_t network = 1; // Ethernet

        fwrite(&magic, sizeof(magic), 1, f);
        fwrite(&version_major, sizeof(version_major), 1, f);
        fwrite(&version_minor, sizeof(version_minor), 1, f);
        fwrite(&tz, sizeof(tz), 1, f);
        fwrite(&sigfigs, sizeof(sigfigs), 1, f);
        fwrite(&snaplen, sizeof(snaplen), 1, f);
        fwrite(&network, sizeof(network), 1, f);

        // Write a dummy packet header and data
        struct pcap_pkthdr dummy_header;
        dummy_header.ts.tv_sec = 0;
        dummy_header.ts.tv_usec = 0;
        dummy_header.caplen = 4;
        dummy_header.len = 4;
        fwrite(&dummy_header, sizeof(dummy_header), 1, f);
        uint8_t dummy_data[4] = {0x00, 0x01, 0x02, 0x03};
        fwrite(dummy_data, sizeof(dummy_data), 1, f);

        fclose(f);
    }

    // Open the offline capture
    handle = pcap_open_offline("./dummy_file", errbuf);
    if (handle == NULL) {
        // If we can't open the file, we can still test error handling
        // but the sequence requires a handle for most functions.
        // Try to proceed with NULL checks or return early if critical.
        // For fuzzing, let's try to handle the error path.
        char *err = pcap_geterr(NULL);
        (void)err; // Suppress unused warning
        return 0;
    }

    // 1. pcap_geterr
    // Call immediately to get initial state (should be empty or previous error)
    char *err1 = pcap_geterr(handle);
    (void)err1;

    // Prepare a BPF program for validation
    // We need to construct a bpf_insn array from the fuzz input if possible
    // or use a fixed valid one. Let's try to parse a simple filter or use a default.
    // Since bpf_validate takes raw instructions, we can construct a dummy one.
    // We need at least one instruction.
    
    struct bpf_insn dummy_insn[] = {
        {BPF_LD | BPF_W | BPF_ABS, 0, 0, 0}, // Load word at offset 0
        {BPF_JMP | BPF_JEQ | BPF_K, 0, 1, 0}, // Jump if equal to 0
        {BPF_RET | BPF_K, 0, 0, 0}, // Return 0
        {BPF_RET | BPF_K, 0, 0, 0}  // Return 0
    };

    // If input is large enough, we might try to interpret it as instructions,
    // but for safety and stability, we use the dummy_insn and maybe modify it slightly
    // based on input bytes to trigger different paths in bpf_validate.
    
    // Copy dummy instructions to a heap allocated buffer to simulate dynamic generation
    size_t num_insns = sizeof(dummy_insn) / sizeof(dummy_insn[0]);
    prog.bf_insns = malloc(num_insns * sizeof(struct bpf_insn));
    if (prog.bf_insns == NULL) {
        cleanup_pcap(handle);
        return 0;
    }
    memcpy(prog.bf_insns, dummy_insn, num_insns * sizeof(struct bpf_insn));
    prog.bf_len = num_insns;

    // Modify instructions based on input to explore different validation states
    if (Size > 0) {
        // Modify the jump target or opcode slightly based on input
        prog.bf_insns[1].jt = (Size % 2) ? 1 : 0;
        prog.bf_insns[1].jf = (Size % 3) ? 1 : 0;
    }

    // 2. bpf_validate
    // Validate the BPF program
    ret = bpf_validate(prog.bf_insns, prog.bf_len);
    // ret is 0 on success, -1 on failure. We don't care about the result for the flow,
    // just that we called it.

    // 3. bpf_dump
    // Dump the BPF program. Options: >2 raw, >1 C syntax, <=1 human readable.
    // Use input to select dump mode
    int dump_mode = (Size > 0) ? (Data[0] % 3) + 1 : 1;
    bpf_dump(&prog, dump_mode);

    // 4. pcap_next_ex
    // Try to read a packet
    ret = pcap_next_ex(handle, &header, &packet);
    // ret: 1 (success), -1 (error), -2 (EOF/timeout)
    // We don't need to do anything specific with the packet data for this sequence,
    // but we ensure the function is called.

    // 5. pcap_geterr
    // Check for errors after pcap_next_ex
    char *err2 = pcap_geterr(handle);
    (void)err2;

    // 6. pcap_offline_filter
    // Apply the filter to the packet if we have one
    int match = 0;
    if (ret == 1 && header != NULL && packet != NULL) {
        match = pcap_offline_filter(&prog, header, packet);
    } else {
        // If no packet, we can still call it with a dummy header if we want to test the filter logic
        // But pcap_offline_filter expects valid pointers. If ret != 1, header/packet might be NULL.
        // Let's construct a minimal dummy header for testing if needed, or just skip.
        // To ensure we call the function, let's create a local dummy header if packet is missing.
        if (ret != 1) {
            struct pcap_pkthdr dummy_h = {0};
            dummy_h.caplen = 0;
            dummy_h.len = 0;
            match = pcap_offline_filter(&prog, &dummy_h, NULL);
        }
    }
    (void)match;

    // Cleanup
    cleanup_bpf_program(&prog);
    cleanup_pcap(handle);

    return 0;
}