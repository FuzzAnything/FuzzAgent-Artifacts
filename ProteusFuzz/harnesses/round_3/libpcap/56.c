// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// bpf_image at bpf_image.c:130:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
// bpf_image at bpf_image.c:130:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// bpf_dump at bpf_dump.c:30:1 in bpf.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_offline_filter at pcap.c:4278:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// bpf_validate at bpf_filter.c:541:1 in bpf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pcap.h>
#include <bpf.h>
#include <sys/time.h>

static void cleanup_pcap(pcap_t *handle) {
    if (handle) {
        pcap_close(handle);
    }
}

static void cleanup_bpf_program(struct bpf_program *prog) {
    if (prog && prog->bf_insns) {
        pcap_freecode(prog);
        prog->bf_insns = NULL;
        prog->bf_len = 0;
    }
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Create a dummy file for offline capture if needed
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        // Write minimal pcap header (magic, version, snaplen, linktype, zone, sigfigs, network)
        uint32_t magic = 0xa1b2c3d4;
        uint16_t version_major = 2;
        uint16_t version_minor = 4;
        int32_t zone = 0;
        int32_t sigfigs = 0;
        int32_t snaplen = 65535;
        int32_t network = 1; // Ethernet
        fwrite(&magic, sizeof(magic), 1, fp);
        fwrite(&version_major, sizeof(version_major), 1, fp);
        fwrite(&version_minor, sizeof(version_minor), 1, fp);
        fwrite(&zone, sizeof(zone), 1, fp);
        fwrite(&sigfigs, sizeof(sigfigs), 1, fp);
        fwrite(&snaplen, sizeof(snaplen), 1, fp);
        fwrite(&network, sizeof(network), 1, fp);
        // Write a dummy packet header and data if size allows
        if (Size > 10) {
            struct pcap_pkthdr hdr;
            hdr.ts.tv_sec = 0;
            hdr.ts.tv_usec = 0;
            hdr.caplen = Size - 10;
            hdr.len = Size - 10;
            fwrite(&hdr, sizeof(hdr), 1, fp);
            fwrite(Data + 10, 1, Size - 10, fp);
        }
        fclose(fp);
    }

    // Allocate pcap handle
    pcap_t *handle = pcap_open_offline(dummy_file, NULL);
    if (!handle) {
        // Try to create a live handle on loopback as fallback if offline fails
        handle = pcap_create("lo", NULL);
        if (handle) {
            pcap_activate(handle);
        }
    }

    struct bpf_program prog = {0};
    struct bpf_insn *insns = NULL;
    int num_insns = 0;
    char *filter_str = NULL;

    // Parse input to determine fuzzing strategy
    uint8_t strategy = Data[0];
    size_t remaining = Size - 1;

    if (remaining > 0) {
        // Allocate filter string from input (ensure null termination)
        filter_str = (char *)malloc(remaining + 1);
        if (filter_str) {
            memcpy(filter_str, Data + 1, remaining);
            filter_str[remaining] = '\0';
        }
    }

    // Strategy 0: Test pcap_compile and related functions
    if (strategy == 0 && handle && filter_str) {
        if (pcap_compile(handle, &prog, filter_str, 1, 0) == 0) {
            // Test bpf_validate
            if (prog.bf_insns && prog.bf_len > 0) {
                bpf_validate(prog.bf_insns, prog.bf_len);
            }
            // Test bpf_image
            char *img = bpf_image(prog.bf_insns, prog.bf_len);
            (void)img; // Use result to avoid warning
            // Test bpf_dump with different options
            bpf_dump(&prog, 0);
            bpf_dump(&prog, 1);
            bpf_dump(&prog, 2);
            // Test pcap_offline_filter
            if (fp) {
                // Re-open file to read a packet for filtering
                FILE *rfile = fopen(dummy_file, "rb");
                if (rfile) {
                    uint32_t magic, vma, vmi, snap, net;
                    int32_t zone, sig;
                    if (fread(&magic, sizeof(magic), 1, rfile) &&
                        fread(&vma, sizeof(vma), 1, rfile) &&
                        fread(&vmi, sizeof(vmi), 1, rfile) &&
                        fread(&zone, sizeof(zone), 1, rfile) &&
                        fread(&sig, sizeof(sig), 1, rfile) &&
                        fread(&snap, sizeof(snap), 1, rfile) &&
                        fread(&net, sizeof(net), 1, rfile)) {
                        struct pcap_pkthdr hdr;
                        if (fread(&hdr, sizeof(hdr), 1, rfile)) {
                            u_char *pkt_data = (u_char *)malloc(hdr.caplen);
                            if (pkt_data && fread(pkt_data, 1, hdr.caplen, rfile)) {
                                pcap_offline_filter(&prog, &hdr, pkt_data);
                                free(pkt_data);
                            }
                        }
                    }
                    fclose(rfile);
                }
            }
            cleanup_bpf_program(&prog);
        }
    }

    // Strategy 1: Test bpf_validate with manually constructed instructions
    if (strategy == 1) {
        // Allocate instructions based on input size
        num_insns = (remaining > 100) ? 100 : remaining;
        insns = (struct bpf_insn *)malloc(num_insns * sizeof(struct bpf_insn));
        if (insns) {
            for (int i = 0; i < num_insns; i++) {
                // Construct instruction from input data
                if (i * sizeof(struct bpf_insn) + sizeof(struct bpf_insn) <= remaining) {
                    memcpy(&insns[i], Data + 1 + (i * sizeof(struct bpf_insn)), sizeof(struct bpf_insn));
                } else {
                    insns[i].code = 0;
                    insns[i].jt = 0;
                    insns[i].jf = 0;
                    insns[i].k = 0;
                }
            }
            // Validate the instruction sequence
            bpf_validate(insns, num_insns);
            // Generate image
            bpf_image(insns, num_insns);
            // Dump instructions
            bpf_dump(&(struct bpf_program){.bf_len = num_insns, .bf_insns = insns}, 0);
            bpf_dump(&(struct bpf_program){.bf_len = num_insns, .bf_insns = insns}, 1);
            bpf_dump(&(struct bpf_program){.bf_len = num_insns, .bf_insns = insns}, 2);
            free(insns);
        }
    }

    // Strategy 2: Test pcap_offline_filter with NULL filter
    if (strategy == 2 && handle) {
        struct pcap_pkthdr hdr;
        hdr.ts.tv_sec = 0;
        hdr.ts.tv_usec = 0;
        hdr.caplen = 64;
        hdr.len = 64;
        u_char *pkt_data = (u_char *)malloc(64);
        if (pkt_data) {
            memset(pkt_data, 0, 64);
            // Test with NULL filter
            pcap_offline_filter(NULL, &hdr, pkt_data);
            free(pkt_data);
        }
    }

    // Cleanup
    if (filter_str) {
        free(filter_str);
    }
    cleanup_pcap(handle);

    return 0;
}