// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7070:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7091:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
#include <pugixml.hpp>
#include <cstdint>
#include <cstddef>
#include <cstring>

static void fuzz_append_child_and_set_text(pugi::xml_node& parent, const uint8_t* data, size_t size, size_t& offset) {
    if (offset >= size) return;
    
    // Create child element
    // Ensure the name string is null-terminated within bounds before passing
    size_t str_len = 0;
    while (offset + str_len < size && data[offset + str_len] != 0) {
        str_len++;
    }
    
    // If the string is not null-terminated within the buffer, we cannot safely pass it
    // as a C-string to append_child. We skip this operation to prevent buffer overflow.
    if (offset + str_len >= size) return;

    // Create a temporary buffer with a guaranteed null terminator
    char* temp_name = new char[str_len + 1];
    std::memcpy(temp_name, data + offset, str_len);
    temp_name[str_len] = '\0';

    pugi::xml_node child = parent.append_child(temp_name);
    delete[] temp_name;

    if (!child) return;
    
    // Advance offset safely
    offset += str_len + 1;

    // Apply all 5 overloads of xml_text::set
    pugi::xml_text text_node = child.text();
    
    // Set with unsigned int
    if (offset + sizeof(unsigned int) <= size) {
        unsigned int uval = *reinterpret_cast<const unsigned int*>(data + offset);
        text_node.set(uval);
        offset += sizeof(unsigned int);
    }
    
    // Set with int
    if (offset + sizeof(int) <= size) {
        int ival = *reinterpret_cast<const int*>(data + offset);
        text_node.set(ival);
        offset += sizeof(int);
    }
    
    // Set with bool
    if (offset + sizeof(bool) <= size) {
        bool bval = *reinterpret_cast<const bool*>(data + offset);
        text_node.set(bval);
        offset += sizeof(bool);
    }
    
    // Set with float
    if (offset + sizeof(float) <= size) {
        float fval = *reinterpret_cast<const float*>(data + offset);
        text_node.set(fval);
        offset += sizeof(float);
    }
    
    // Set with double
    if (offset + sizeof(double) <= size) {
        double dval = *reinterpret_cast<const double*>(data + offset);
        text_node.set(dval);
        offset += sizeof(double);
    }
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    // Initialize the document with a root node to ensure we have a valid parent
    pugi::xml_node root = doc.append_child("root");
    if (!root) return 0;

    size_t offset = 0;

    // Helper lambda to safely append a child from fuzz data
    auto safe_append_child = [&](pugi::xml_node& parent) -> pugi::xml_node {
        if (offset >= Size) return pugi::xml_node();
        
        size_t str_len = 0;
        while (offset + str_len < Size && Data[offset + str_len] != 0) {
            str_len++;
        }
        
        // Must be null-terminated within buffer
        if (offset + str_len >= Size) return pugi::xml_node();
        
        char* temp_name = new char[str_len + 1];
        std::memcpy(temp_name, Data + offset, str_len);
        temp_name[str_len] = '\0';
        
        pugi::xml_node child = parent.append_child(temp_name);
        delete[] temp_name;
        
        if (!child) return pugi::xml_node();
        
        offset += str_len + 1;
        return child;
    };

    // 1. append_child
    pugi::xml_node node1 = safe_append_child(root);
    if (!node1) return 0;

    // 2. append_child
    pugi::xml_node node2 = safe_append_child(node1);
    if (!node2) return 0;

    // 3. text.set (all 5 overloads)
    fuzz_append_child_and_set_text(node2, Data, Size, offset);

    // 4. append_child
    pugi::xml_node node3 = safe_append_child(node2);
    if (!node3) return 0;

    // 5. append_child
    pugi::xml_node node4 = safe_append_child(node3);
    if (!node4) return 0;

    // 6. text.set (all 5 overloads)
    fuzz_append_child_and_set_text(node4, Data, Size, offset);

    // 7. append_child
    pugi::xml_node node5 = safe_append_child(node4);
    if (!node5) return 0;

    // 8. text.set (all 5 overloads)
    fuzz_append_child_and_set_text(node5, Data, Size, offset);

    // 9. append_child
    pugi::xml_node node6 = safe_append_child(node5);
    if (!node6) return 0;

    // 10. text.set (all 5 overloads)
    fuzz_append_child_and_set_text(node6, Data, Size, offset);

    // 11. append_child
    pugi::xml_node node7 = safe_append_child(node6);
    if (!node7) return 0;

    // 12. text.set (all 5 overloads)
    fuzz_append_child_and_set_text(node7, Data, Size, offset);

    return 0;
}