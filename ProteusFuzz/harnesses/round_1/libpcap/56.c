// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_set_immediate_mode at pcap.c:2616:1 in pcap.h
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_can_set_rfmon at pcap.c:466:1 in pcap.h
// pcap_set_protocol_linux at pcap-linux.c:6201:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

/* 
 * Helper function to create a dummy pcap handle for fuzzing.
 * We use pcap_create with a non-existent device to get an inactive handle
 * that allows us to test the pre-activation configuration functions.
 */
static pcap_t *create_dummy_handle(void) {
    pcap_t *handle;
    char errbuf[PCAP_ERRBUF_SIZE];

    /* 
     * Try to create a handle for a loopback device or a dummy interface.
     * If pcap_create fails, we return NULL, which the fuzzer will handle.
     * We intentionally do not call pcap_activate to keep the handle inactive,
     * as required by the target API functions.
     */
    handle = pcap_create("lo", errbuf);
    if (handle == NULL) {
        /* Try a generic dummy name if 'lo' fails on some systems */
        handle = pcap_create("dummy0", errbuf);
    }
    
    return handle;
}

/*
 * Helper to safely close a pcap handle if it exists.
 */
static void safe_pcap_close(pcap_t *handle) {
    if (handle != NULL) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    pcap_t *handle;
    int ret;
    int val;
    
    /* 
     * Ensure we have enough data to extract parameters.
     * We need at least 4 bytes for 4 different int parameters to diversify calls.
     */
    if (Size < 4) {
        return 0;
    }

    /* Create an inactive handle */
    handle = create_dummy_handle();
    if (handle == NULL) {
        /* If we cannot create a handle, we cannot test the APIs. */
        return 0;
    }

    /* Extract fuzzing parameters from the input data */
    int snaplen = (int)(Data[0] | (Data[1] << 8));
    int rfmon_val = (int)Data[2];
    int protocol_val = (int)Data[3];
    int immediate_val = (int)(Data[4 % Size]);
    int timeout_val = (int)(Data[5 % Size]);

    /* 
     * 1. Test pcap_set_snaplen
     * Sets the maximum capture length. Must be called before activation.
     */
    ret = pcap_set_snaplen(handle, snaplen);
    (void)ret; /* Ignore return value to avoid unused warnings, but logic is tested */

    /* 
     * 2. Test pcap_can_set_rfmon
     * Checks if monitor mode is supported.
     */
    ret = pcap_can_set_rfmon(handle);
    (void)ret;

    /* 
     * 3. Test pcap_set_protocol_linux
     * Configures the capture protocol (Linux specific).
     * We cast the input to int to fuzz the protocol type.
     */
    ret = pcap_set_protocol_linux(handle, protocol_val);
    (void)ret;

    /* 
     * 4. Test pcap_set_immediate_mode
     * Configures immediate mode (1 for enable, 0 for disable).
     */
    ret = pcap_set_immediate_mode(handle, immediate_val);
    (void)ret;

    /* 
     * 5. Test pcap_set_rfmon
     * Enables/disables monitor mode.
     */
    ret = pcap_set_rfmon(handle, rfmon_val);
    (void)ret;

    /* 
     * 6. Test pcap_set_timeout
     * Sets the capture timeout in milliseconds.
     */
    ret = pcap_set_timeout(handle, timeout_val);
    (void)ret;

    /* 
     * Cleanup: Close the handle.
     * Since we never called pcap_activate, pcap_close should just free resources.
     */
    safe_pcap_close(handle);

    return 0;
}