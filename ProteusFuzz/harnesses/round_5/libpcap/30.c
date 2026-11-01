// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_nametoportrange at nametoaddr.c:437:1 in namedb.h
// pcap_nametoeproto at nametoaddr.c:585:1 in namedb.h
// pcap_tstamp_type_name_to_val at pcap.c:3419:1 in pcap.h
// pcap_nametoport at nametoaddr.c:304:1 in namedb.h
// pcap_nametollc at nametoaddr.c:609:1 in namedb.h
// pcap_nametoproto at nametoaddr.c:475:1 in namedb.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pcap.h>
#include <namedb.h>

static void fuzz_pcap_nametoportrange(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    int start_port, end_port, proto;
    char *input = (char *)data;
    
    // Ensure null termination for safety
    char *safe_input = malloc(size + 1);
    if (!safe_input) return;
    memcpy(safe_input, data, size);
    safe_input[size] = '\0';
    
    pcap_nametoportrange(safe_input, &start_port, &end_port, &proto);
    
    free(safe_input);
}

static void fuzz_pcap_nametoeproto(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *input = (char *)data;
    char *safe_input = malloc(size + 1);
    if (!safe_input) return;
    memcpy(safe_input, data, size);
    safe_input[size] = '\0';
    
    pcap_nametoeproto(safe_input);
    
    free(safe_input);
}

static void fuzz_pcap_tstamp_type_name_to_val(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *input = (char *)data;
    char *safe_input = malloc(size + 1);
    if (!safe_input) return;
    memcpy(safe_input, data, size);
    safe_input[size] = '\0';
    
    pcap_tstamp_type_name_to_val(safe_input);
    
    free(safe_input);
}

static void fuzz_pcap_nametoport(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    int port, proto;
    char *input = (char *)data;
    char *safe_input = malloc(size + 1);
    if (!safe_input) return;
    memcpy(safe_input, data, size);
    safe_input[size] = '\0';
    
    pcap_nametoport(safe_input, &port, &proto);
    
    free(safe_input);
}

static void fuzz_pcap_nametollc(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *input = (char *)data;
    char *safe_input = malloc(size + 1);
    if (!safe_input) return;
    memcpy(safe_input, data, size);
    safe_input[size] = '\0';
    
    pcap_nametollc(safe_input);
    
    free(safe_input);
}

static void fuzz_pcap_nametoproto(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *input = (char *)data;
    char *safe_input = malloc(size + 1);
    if (!safe_input) return;
    memcpy(safe_input, data, size);
    safe_input[size] = '\0';
    
    pcap_nametoproto(safe_input);
    
    free(safe_input);
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Fuzz all target functions with the same input
    fuzz_pcap_nametoportrange(Data, Size);
    fuzz_pcap_nametoeproto(Data, Size);
    fuzz_pcap_tstamp_type_name_to_val(Data, Size);
    fuzz_pcap_nametoport(Data, Size);
    fuzz_pcap_nametollc(Data, Size);
    fuzz_pcap_nametoproto(Data, Size);
    
    return 0;
}