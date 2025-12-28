#pragma once
#include <GL/glew.h>
#include <string>


class Model {
private:
    // Pointer to private implementation (PIMPL Idiom)
    // Â–« «·√”·Ê» ÌŒ›Ì ﬂ·  ›«’Ì· „ﬂ »… «· Õ„Ì· ⁄‰ Â–« «·„·›
    struct ModelImpl;
    ModelImpl* pimpl;

public:
    Model();
    ~Model(); // œ«·… Â«œ„… · ‰ŸÌ› «·–«ﬂ—…

    //  ⁄ÿÌ· «·‰”Œ ·„‰⁄ „‘«ﬂ· «·–«ﬂ—…
    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;

    bool load(const std::string& filename);
    void draw();
};
