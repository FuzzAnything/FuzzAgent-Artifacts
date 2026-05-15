#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include "magic.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

namespace harness_012 {
/*
 * Fuzzing harness for libmagic ELF core file processing functions
 * Target: dophn_core() and donote() functions in readelf.c with 643+ undiscovered branches
 * 
 * Critical coverage gap functions (high complexity, low coverage):
 * - dophn_core(): Processes program headers in ELF core dumps
 * - donote(): Processes note sections in ELF core dumps
 * 
 * Strategy: Generate ELF core dump files with varied program headers and note sections
 * to exercise dophn_core() and donote() parsing logic through libmagic's ELF detection.
 * 
 * ELF core file structure to target:
 * 1. ELF header with e_type = ET_CORE (4)
 * 2. Program headers with p_type = PT_NOTE (4)
 * 3. Note sections with various note types (NT_PRPSINFO, NT_PRSTATUS, etc.)
 * 4. Core-specific note names ("CORE", "FreeBSD", "NetBSD-CORE")
 */


// Include libmagic headers

// Include necessary system headers for POSIX functions

// Simple ELF constant definitions
#define EI_CLASS 4
#define EI_DATA 5
#define EI_VERSION 6
#define ELFCLASS32 1
#define ELFCLASS64 2
#define ELFDATA2LSB 1
#define ELFDATA2MSB 2
#define EV_CURRENT 1
#define ET_CORE 4
#define PT_NOTE 4
#define PF_R 4

// Simple ELF structure definitions
typedef struct {
    unsigned char e_ident[16];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint64_t e_entry;
    uint64_t e_phoff;
    uint64_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
} Elf64_Ehdr;

typedef struct {
    uint32_t p_type;
    uint32_t p_flags;
    uint64_t p_offset;
    uint64_t p_vaddr;
    uint64_t p_paddr;
    uint64_t p_filesz;
    uint64_t p_memsz;
    uint64_t p_align;
} Elf64_Phdr;

typedef struct {
    uint32_t n_namesz;
    uint32_t n_descsz;
    uint32_t n_type;
} Elf64_Nhdr;

// Helper to create ELF core header
std::vector<uint8_t> create_elf_core_header(FuzzedDataProvider& fdp, bool elf64) {
    std::vector<uint8_t> header;
    
    if (elf64) {
        // ELF64 header
        header.resize(sizeof(Elf64_Ehdr));
        Elf64_Ehdr* ehdr = reinterpret_cast<Elf64_Ehdr*>(header.data());
        
        // ELF identification
        memcpy(ehdr->e_ident, "\x7F""ELF", 4);  // ELF magic
        ehdr->e_ident[EI_CLASS] = ELFCLASS64;
        ehdr->e_ident[EI_DATA] = fdp.ConsumeBool() ? ELFDATA2LSB : ELFDATA2MSB;
        ehdr->e_ident[EI_VERSION] = EV_CURRENT;
        
        // Core file type
        ehdr->e_type = ET_CORE;  // Core file
        ehdr->e_machine = fdp.ConsumeIntegral<uint16_t>() % 0x100;  // Various architectures
        ehdr->e_version = EV_CURRENT;
        ehdr->e_entry = 0;
        ehdr->e_phoff = sizeof(Elf64_Ehdr);  // Program headers right after ELF header
        ehdr->e_shoff = 0;  // No section headers for core files
        ehdr->e_flags = 0;
        ehdr->e_ehsize = sizeof(Elf64_Ehdr);
        ehdr->e_phentsize = sizeof(Elf64_Phdr);
        ehdr->e_phnum = fdp.ConsumeIntegralInRange<uint16_t>(0, 20);  // 0-20 program headers
        ehdr->e_shentsize = 0;
        ehdr->e_shnum = 0;
        ehdr->e_shstrndx = 0;
    } else {
        // ELF32 header (simplified)
        header.resize(52);  // Size of ELF32 header
        // ELF magic
        memcpy(header.data(), "\x7F""ELF", 4);
        header[4] = ELFCLASS32;
        header[5] = fdp.ConsumeBool() ? ELFDATA2LSB : ELFDATA2MSB;
        header[6] = EV_CURRENT;
        
        // e_type = ET_CORE
        uint16_t e_type = ET_CORE;
        memcpy(header.data() + 16, &e_type, 2);
        
        // e_phnum - number of program headers
        uint16_t e_phnum = fdp.ConsumeIntegralInRange<uint16_t>(0, 20);
        memcpy(header.data() + 44, &e_phnum, 2);
        
        // e_phentsize - size of program header
        uint16_t e_phentsize = 32;  // sizeof(Elf32_Phdr)
        memcpy(header.data() + 42, &e_phentsize, 2);
        
        // e_phoff - offset to program headers
        uint32_t e_phoff = 52;  // After ELF header
        memcpy(header.data() + 28, &e_phoff, 4);
    }
    
    return header;
}

// Helper to create program header for PT_NOTE section
std::vector<uint8_t> create_note_program_header(FuzzedDataProvider& fdp, bool elf64, uint64_t offset, uint64_t size) {
    std::vector<uint8_t> phdr;
    
    if (elf64) {
        phdr.resize(sizeof(Elf64_Phdr));
        Elf64_Phdr* ph = reinterpret_cast<Elf64_Phdr*>(phdr.data());
        
        ph->p_type = PT_NOTE;  // Note segment
        ph->p_flags = PF_R;  // Readable
        ph->p_offset = offset;
        ph->p_vaddr = 0;
        ph->p_paddr = 0;
        ph->p_filesz = size;
        ph->p_memsz = size;
        ph->p_align = 4;
    } else {
        // ELF32 program header
        phdr.resize(32);
        uint32_t p_type = PT_NOTE;
        memcpy(phdr.data(), &p_type, 4);
        memcpy(phdr.data() + 4, &offset, 4);  // p_offset
        memcpy(phdr.data() + 8, &size, 4);    // p_filesz
    }
    
    return phdr;
}

// Helper to create note section
std::vector<uint8_t> create_note_section(FuzzedDataProvider& fdp) {
    std::vector<uint8_t> note;
    
    // Choose note type
    uint32_t note_type = fdp.ConsumeIntegral<uint32_t>() % 10;
    
    // Note names for core files
    const char* core_names[] = {"CORE", "FreeBSD", "NetBSD-CORE"};
    const char* name = core_names[fdp.ConsumeIntegralInRange<size_t>(0, 2)];
    size_t namesz = strlen(name) + 1;  // Include null terminator
    
    // Note description size
    size_t descsz = fdp.ConsumeIntegralInRange<size_t>(0, 512);
    
    // Create note header
    Elf64_Nhdr nhdr;
    nhdr.n_namesz = namesz;
    nhdr.n_descsz = descsz;
    nhdr.n_type = note_type;
    
    // Add header
    note.resize(sizeof(nhdr) + namesz + descsz);
    memcpy(note.data(), &nhdr, sizeof(nhdr));
    
    // Add name (with null terminator)
    memcpy(note.data() + sizeof(nhdr), name, namesz);
    
    // Add description data
    if (descsz > 0) {
        std::vector<uint8_t> desc = fdp.ConsumeBytes<uint8_t>(descsz);
        memcpy(note.data() + sizeof(nhdr) + namesz, desc.data(), desc.size());
    }
    
    // Align to 4 bytes
    size_t aligned_size = (note.size() + 3) & ~3;
    note.resize(aligned_size, 0);
    
    return note;
}

// Main fuzzing function
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for ELF core file testing
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ============================================================
    // PHASE 1: Create magic handle with ELF-related flags
    // ============================================================
    
    int flags = MAGIC_NONE;
    // Enable compression detection in case core files are compressed
    if (fdp.ConsumeBool()) flags |= MAGIC_COMPRESS;
    // Enable MIME mode for different parsing path
    if (fdp.ConsumeBool()) flags |= MAGIC_MIME;
    // Enable raw mode
    if (fdp.ConsumeBool()) flags |= MAGIC_RAW;
    // Disable ELF checking to see if it affects core detection
    if (fdp.ConsumeBool()) flags |= MAGIC_NO_CHECK_ELF;
    
    magic_t magic = magic_open(flags);
    if (magic == nullptr) {
        magic = magic_open(MAGIC_NONE);
        if (magic == nullptr) {
            return 0;
        }
    }
    
    // Set ELF-specific parameters to control parsing
    if (fdp.ConsumeBool()) {
        size_t elf_phnum_max = fdp.ConsumeIntegralInRange<size_t>(1, 1000);
        magic_setparam(magic, MAGIC_PARAM_ELF_PHNUM_MAX, &elf_phnum_max);
    }
    
    if (fdp.ConsumeBool()) {
        size_t elf_notes_max = fdp.ConsumeIntegralInRange<size_t>(1, 1000);
        magic_setparam(magic, MAGIC_PARAM_ELF_NOTES_MAX, &elf_notes_max);
    }
    
    // ============================================================
    // PHASE 2: Generate ELF core file data
    // ============================================================
    
    // Choose ELF class (32-bit or 64-bit)
    bool elf64 = fdp.ConsumeBool();
    
    // Create ELF header
    std::vector<uint8_t> elf_header = create_elf_core_header(fdp, elf64);
    
    // Parse number of program headers from header
    uint16_t phnum = 0;
    if (elf64) {
        const Elf64_Ehdr* ehdr = reinterpret_cast<const Elf64_Ehdr*>(elf_header.data());
        phnum = ehdr->e_phnum;
    } else {
        memcpy(&phnum, elf_header.data() + 44, 2);
    }
    
    // Create program headers and note sections
    std::vector<uint8_t> program_headers;
    std::vector<uint8_t> note_data;
    
    uint64_t note_offset = elf_header.size() + (elf64 ? sizeof(Elf64_Phdr) : 32) * phnum;
    
    for (uint16_t i = 0; i < phnum && fdp.remaining_bytes() > 100; i++) {
        // Decide if this program header is PT_NOTE
        bool is_note = fdp.ConsumeBool();
        
        if (is_note) {
            // Create note section
            std::vector<uint8_t> note = create_note_section(fdp);
            
            // Create PT_NOTE program header
            std::vector<uint8_t> phdr = create_note_program_header(fdp, elf64, note_offset, note.size());
            
            // Add to buffers
            program_headers.insert(program_headers.end(), phdr.begin(), phdr.end());
            note_data.insert(note_data.end(), note.begin(), note.end());
            
            note_offset += note.size();
        } else {
            // Create non-NOTE program header (loadable segment, etc.)
            if (elf64) {
                std::vector<uint8_t> phdr(sizeof(Elf64_Phdr), 0);
                program_headers.insert(program_headers.end(), phdr.begin(), phdr.end());
            } else {
                std::vector<uint8_t> phdr(32, 0);
                program_headers.insert(program_headers.end(), phdr.begin(), phdr.end());
            }
        }
    }
    
    // Build complete ELF core file
    std::vector<uint8_t> elf_core;
    elf_core.insert(elf_core.end(), elf_header.begin(), elf_header.end());
    elf_core.insert(elf_core.end(), program_headers.begin(), program_headers.end());
    elf_core.insert(elf_core.end(), note_data.begin(), note_data.end());
    
    // Add random trailing data to test boundary conditions
    if (fdp.remaining_bytes() > 0) {
        size_t extra_size = fdp.ConsumeIntegralInRange<size_t>(0, fdp.remaining_bytes());
        std::vector<uint8_t> extra = fdp.ConsumeBytes<uint8_t>(extra_size);
        elf_core.insert(elf_core.end(), extra.begin(), extra.end());
    }
    
    // ============================================================
    // PHASE 3: Test ELF core file detection through libmagic
    // ============================================================
    
    // Write ELF core to temporary file
    char core_template[] = "/tmp/elf_core_fuzz_XXXXXX";
    int core_fd = mkstemp(core_template);
    if (core_fd == -1) {
        magic_close(magic);
        return 0;
    }
    
    // Write the ELF core data
    ssize_t written = write(core_fd, elf_core.data(), elf_core.size());
    if (written < 0) {
        close(core_fd);
        unlink(core_template);
        magic_close(magic);
        return 0;
    }
    fsync(core_fd);
    close(core_fd);
    
    // Test 1: magic_file - should trigger dophn_core/donote via elf_check
    const char* file_result = magic_file(magic, core_template);
    (void)file_result;
    
    // Test 2: magic_descriptor - test file descriptor path
    int read_fd = open(core_template, O_RDONLY);
    if (read_fd != -1) {
        const char* desc_result = magic_descriptor(magic, read_fd);
        (void)desc_result;
        close(read_fd);
    }
    
    // Test 3: magic_buffer - test buffer path
    const char* buffer_result = magic_buffer(magic, elf_core.data(), elf_core.size());
    (void)buffer_result;
    
    // ============================================================
    // PHASE 4: Test with different flag combinations
    // ============================================================
    
    // Toggle flags to see if different parsing paths are taken
    if (fdp.ConsumeBool()) {
        int alt_flags = fdp.ConsumeIntegral<int>();
        magic_setflags(magic, alt_flags);
        
        // Retest with new flags
        const char* alt_file_result = magic_file(magic, core_template);
        (void)alt_file_result;
    }
    
    // ============================================================
    // PHASE 5: Test error handling with corrupted ELF cores
    // ============================================================
    
    if (fdp.remaining_bytes() > 50) {
        // Create corrupted ELF core by modifying parts of the data
        std::vector<uint8_t> corrupted_core = elf_core;
        
        // Corrupt random bytes
        size_t num_corruptions = fdp.ConsumeIntegralInRange<size_t>(1, 10);
        for (size_t i = 0; i < num_corruptions && corrupted_core.size() > 0; i++) {
            size_t corrupt_pos = fdp.ConsumeIntegralInRange<size_t>(0, corrupted_core.size() - 1);
            corrupted_core[corrupt_pos] = fdp.ConsumeIntegral<uint8_t>();
        }
        
        // Test corrupted core
        const char* corrupt_result = magic_buffer(magic, corrupted_core.data(), corrupted_core.size());
        (void)corrupt_result;
    }
    
    // ============================================================
    // PHASE 6: Test with partial/incomplete ELF cores
    // ============================================================
    
    if (fdp.remaining_bytes() > 0) {
        // Test with truncated ELF data
        for (int i = 0; i < 3 && elf_core.size() > 10; i++) {
            size_t truncate_size = fdp.ConsumeIntegralInRange<size_t>(10, elf_core.size());
            std::vector<uint8_t> truncated(elf_core.begin(), elf_core.begin() + truncate_size);
            
            const char* trunc_result = magic_buffer(magic, truncated.data(), truncated.size());
            (void)trunc_result;
        }
    }
    
    // Clean up
    unlink(core_template);
    magic_close(magic);
    
    return 0;
}

} // namespace harness_012
