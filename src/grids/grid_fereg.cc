#include <sstream>
#include <fstream>
#include <memory>
#include <array>
#include <string>
#include <vector>
#include <cassert>

#include <tinyxml2.h>

#include <grid.h>
#include <cgs_units_file.h>
#include <namespace_toolkit.h>

Grid_fereg::Grid_fereg (const std::string &file_name){
    std::unique_ptr<tinyxml2::XMLDocument> doc = toolkit::loadxml (file_name);
    tinyxml2::XMLElement *ptr {toolkit::tracexml (doc.get(),{"FreeElectron"})};
    build_permission = toolkit::fetchbool (ptr,"cue","Regular");
    ptr = toolkit::tracexml (doc.get(),{"Fieldout"});
    read_permission = toolkit::fetchbool (ptr,"read","fereg_grid");
    write_permission = toolkit::fetchbool (ptr,"write","fereg_grid");
    if (read_permission or write_permission){
        filename = toolkit::fetchstring (ptr,"filename","fereg_grid");
        build_grid (doc.get());
    }
}

void Grid_fereg::build_grid (tinyxml2::XMLDocument *doc){
    tinyxml2::XMLElement *ptr {toolkit::tracexml (doc,{"Grid","Box_FE"})};
    // Cartesian grid
    nx = toolkit::fetchunsigned (ptr,"value","nx");
    ny = toolkit::fetchunsigned (ptr,"value","ny");
    nz = toolkit::fetchunsigned (ptr,"value","nz");
    full_size = nx*ny*nz;
    // box limit for filling field
    x_max = CGS_U_kpc*toolkit::fetchdouble (ptr,"value","x_max");
    x_min = CGS_U_kpc*toolkit::fetchdouble (ptr,"value","x_min");
    y_max = CGS_U_kpc*toolkit::fetchdouble (ptr,"value","y_max");
    y_min = CGS_U_kpc*toolkit::fetchdouble (ptr,"value","y_min");
    z_max = CGS_U_kpc*toolkit::fetchdouble (ptr,"value","z_max");
    z_min = CGS_U_kpc*toolkit::fetchdouble (ptr,"value","z_min");
    fe = std::make_unique<double[]> (full_size);
}

void Grid_fereg::export_grid (){
    assert (!filename.empty());
    std::ofstream output (filename.c_str(),
                          std::ios::out|std::ios::binary);
    assert (output.is_open());
    double tmp;
    for (decltype(full_size) i=0;i!=full_size;++i){
        assert (!output.eof());
        tmp = fe[i];
        assert (tmp>=0);
        output.write (reinterpret_cast<char*>(&tmp),
                      sizeof(double));
    }
    output.close();
    exit (0);
}

void Grid_fereg::import_grid (){
    assert (!filename.empty());
    std::ifstream input (filename.c_str(),
                         std::ios::in|std::ios::binary);
    assert (input.is_open());
    double tmp;
    for (decltype(full_size) i=0;i!=full_size;++i){
        assert (!input.eof());
        input.read (reinterpret_cast<char *>(&tmp),
                    sizeof(double));
        fe[i] = tmp;
    }
#ifndef NDEBUG
    auto eof = input.tellg();
    input.seekg (0,input.end);
#endif
    assert (eof==input.tellg());
    input.close();
}
