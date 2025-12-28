#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "Model.h"
#include <iostream>
#include <vector>

// تعريف بنية البيانات المخفية
struct Model::ModelImpl {
    std::vector<GLuint> displayLists;
    std::vector<tinyobj::material_t> materials;
};

// الدالة البانية
Model::Model() {
    pimpl = new ModelImpl();
}
// الدالة الهادمة
Model::~Model() {
    for (GLuint dlist : pimpl->displayLists) {
        glDeleteLists(dlist, 1);
    }
    delete pimpl;
}

// دالة تحميل الموديل
bool Model::load(const std::string& filename) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::string warn, err;

    std::string base_dir = "";
    if (filename.find_last_of("/\\") != std::string::npos) {
        base_dir = filename.substr(0, filename.find_last_of("/\\") + 1);
    }

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &(pimpl->materials), &warn, &err, filename.c_str(), base_dir.c_str(), true);

    if (!warn.empty()) std::cout << "TINYOBJ WARN: " << warn << std::endl;
    if (!err.empty()) std::cerr << "TINYOBJ ERR: " << err << std::endl;
    if (!ret) return false;

    if (pimpl->materials.empty()) {
        pimpl->materials.resize(1); // مادة افتراضية إذا لم يوجد ملف mtl
    }

    for (size_t i = 0; i < pimpl->materials.size(); i++) {
        GLuint dlist = glGenLists(1);
        glNewList(dlist, GL_COMPILE);

        const auto& mat = pimpl->materials[i];
        GLfloat ambient[] = { mat.ambient[0], mat.ambient[1], mat.ambient[2], 1.0f };
        GLfloat diffuse[] = { mat.diffuse[0], mat.diffuse[1], mat.diffuse[2], 1.0f };
        GLfloat specular[] = { mat.specular[0], mat.specular[1], mat.specular[2], 1.0f };

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat.shininess);

        glBegin(GL_TRIANGLES);
        for (const auto& shape : shapes) {
            for (size_t f_idx = 0; f_idx < shape.mesh.material_ids.size(); f_idx++) {
                if (shape.mesh.material_ids[f_idx] == i) {
                    for (int v = 0; v < 3; v++) {
                        tinyobj::index_t idx = shape.mesh.indices[3 * f_idx + v];
                        if (idx.normal_index >= 0) glNormal3fv(&attrib.normals[3 * idx.normal_index]);
                        glVertex3fv(&attrib.vertices[3 * idx.vertex_index]);
                    }
                }
            }
        }
        glEnd();
        glEndList();
        pimpl->displayLists.push_back(dlist);
    }
    return true;
}
// دالة الرسم
void Model::draw() {
    glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    for (GLuint dlist : pimpl->displayLists) {
        glCallList(dlist);
    }
    glPopAttrib();
}