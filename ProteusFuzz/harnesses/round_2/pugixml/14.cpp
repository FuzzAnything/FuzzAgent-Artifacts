// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7105:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7091:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7070:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7063:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
#include <pugixml.hpp>
#include <cstdint>
#include <cstddef>

static pugi::xml_document* create_document_from_input(const uint8_t* data, size_t size) {
    pugi::xml_document* doc = new pugi::xml_document();
    if (!doc) return nullptr;

    if (size > 0) {
        doc->load_buffer(data, size);
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document* doc = create_document_from_input(Data, Size);
    if (!doc) return 0;

    try {
        pugi::xml_node root = doc->child("root");
        if (!root) {
            root = doc->append_child("root");
            if (!root) {
                delete doc;
                return 0;
            }
        }

        // 1. append_child
        pugi::xml_node node1 = root.append_child("child1");
        if (!node1) {
            delete doc;
            return 0;
        }

        // 2. append_child
        pugi::xml_node node2 = root.append_child("child2");
        if (!node2) {
            delete doc;
            return 0;
        }

        // 3. xml_text::set (float)
        pugi::xml_node textNode1 = node2.append_child(pugi::node_pcdata);
        if (textNode1) {
            pugi::xml_text text1 = textNode1.text();
            text1.set(3.14f);
        }

        // 4. append_child
        pugi::xml_node node3 = root.append_child("child3");
        if (!node3) {
            delete doc;
            return 0;
        }

        // 5. append_child
        pugi::xml_node node4 = root.append_child("child4");
        if (!node4) {
            delete doc;
            return 0;
        }

        // 6. xml_text::set (unsigned int)
        pugi::xml_node textNode2 = node4.append_child(pugi::node_pcdata);
        if (textNode2) {
            pugi::xml_text text2 = textNode2.text();
            unsigned int val2 = (Size > 0) ? static_cast<unsigned int>(Data[0]) + 100 : 100;
            text2.set(val2);
        }

        // 7. append_child
        pugi::xml_node node5 = root.append_child("child5");
        if (!node5) {
            delete doc;
            return 0;
        }

        // 8. xml_text::set (int)
        pugi::xml_node textNode3 = node5.append_child(pugi::node_pcdata);
        if (textNode3) {
            pugi::xml_text text3 = textNode3.text();
            int val3 = (Size > 1) ? static_cast<int>(Data[1]) - 50 : -50;
            text3.set(val3);
        }

        // 9. append_child
        pugi::xml_node node6 = root.append_child("child6");
        if (!node6) {
            delete doc;
            return 0;
        }

        // 10. xml_text::set (bool)
        pugi::xml_node textNode4 = node6.append_child(pugi::node_pcdata);
        if (textNode4) {
            pugi::xml_text text4 = textNode4.text();
            bool val4 = (Size > 2) ? (Data[2] % 2 == 0) : true;
            text4.set(val4);
        }

        // 11. append_child
        pugi::xml_node node7 = root.append_child("child7");
        if (!node7) {
            delete doc;
            return 0;
        }

        // 12. xml_text::set (double)
        pugi::xml_node textNode5 = node7.append_child(pugi::node_pcdata);
        if (textNode5) {
            pugi::xml_text text5 = textNode5.text();
            double val5 = (Size > 3) ? static_cast<double>(Data[3]) / 10.0 : 0.0;
            text5.set(val5);
        }

    } catch (...) {
        delete doc;
        return 0;
    }

    delete doc;
    return 0;
}