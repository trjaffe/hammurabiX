#include <iostream>
#include <vec3.h>
#include <vector>
#include <array>
#include <cmath>
#include <fftw3.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_integration.h>
#include <param.h>
#include <grid.h>
#include <fernd.h>
#include <fereg.h>
#include <cgs_units_file.h>
#include <namespace_toolkit.h>
#include <cassert>
using namespace std;

double FErnd::get_fernd(const vec3_t<double> &pos,Grid_fernd *grid){
    if(grid->read_permission){
        return read_grid(pos,grid);
    }
    // if no specific turbulent field model is called
    // base class will return zero vector
    else{
        return 0.;
    }
}

double FErnd::read_grid(const vec3_t<double> &pos, Grid_fernd *grid){
    double tmp {(grid->nx-1)*(pos.x-grid->x_min)/(grid->x_max-grid->x_min)};
    if (tmp<0 or tmp>grid->nx-1) { return 0.;}
    decltype(grid->nx) xl {(std::size_t)floor(tmp)};
    const double xd {tmp - xl};
    
    tmp = (grid->ny-1)*(pos.y-grid->y_min)/(grid->y_max-grid->y_min);
    if (tmp<0 or tmp>grid->ny-1) { return 0.;}
    decltype(grid->nx) yl {(std::size_t)floor(tmp)};
    const double yd {tmp - yl};
    
    tmp = (grid->nz-1)*(pos.z-grid->z_min)/(grid->z_max-grid->z_min);
    if (tmp<0 or tmp>grid->nz-1) { return 0.;}
    decltype(grid->nx) zl {(std::size_t)floor(tmp)};
    const double zd {tmp - zl};
    assert(xd>=0 and yd>=0 and zd>=0 and xd<=1 and yd<=1 and zd<=1);
    double density;
    //trilinear interpolation
    if (xl+1<grid->nx and yl+1<grid->ny and zl+1<grid->nz) {
        //interpolate along z direction, there are four interpolated vectors
        std::size_t idx1 {toolkit::Index3d(grid->nx,grid->ny,grid->nz,xl,yl,zl)};
        std::size_t idx2 {toolkit::Index3d(grid->nx,grid->ny,grid->nz,xl,yl,zl+1)};
        double i1 {grid->fe[idx1]*(1.-zd) + grid->fe[idx2]*zd};
        
        idx1 = toolkit::Index3d(grid->nx,grid->ny,grid->nz,xl,yl+1,zl);
        idx2 = toolkit::Index3d(grid->nx,grid->ny,grid->nz,xl,yl+1,zl+1);
        double i2 {grid->fe[idx1]*(1.-zd) + grid->fe[idx2]*zd};
        
        idx1 = toolkit::Index3d(grid->nx,grid->ny,grid->nz,xl+1,yl,zl);
        idx2 = toolkit::Index3d(grid->nx,grid->ny,grid->nz,xl+1,yl,zl+1);
        double j1 {grid->fe[idx1]*(1.-zd) + grid->fe[idx2]*zd};
        
        idx1 = toolkit::Index3d(grid->nx,grid->ny,grid->nz,xl+1,yl+1,zl);
        idx2 = toolkit::Index3d(grid->nx,grid->ny,grid->nz,xl+1,yl+1,zl+1);
        double j2 {grid->fe[idx1]*(1.-zd) + grid->fe[idx2]*zd};
        // interpolate along y direction, two interpolated vectors
        double w1 {i1*(1.-yd) + i2*yd};
        double w2 {j1*(1.-yd) + j2*yd};
        // interpolate along x direction
        density = w1*(1.-xd) + w2*xd;
    }
    // on the boundary
    else {
        std::size_t idx{toolkit::Index3d(grid->nx,grid->ny,grid->nz,xl,yl,zl)};
        density = grid->fe[idx];
    }
    return density;
}

void FErnd::write_grid(Param *,Grid_fernd *){
    assert(false);
}

// END
