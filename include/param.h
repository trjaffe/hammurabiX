// parameters (fixed or free) for physical models

#ifndef HAMMURABI_PARAM_H
#define HAMMURABI_PARAM_H

#include <string>
#include <vector>

#include <tinyxml2.h>
#include <vec3.h>

class Param {
    
public:
    
    Param (const std::string);
    
	Param () = default;
    
    virtual ~Param () = default;
	
    // observer
    vec3_t<double> SunPosition;
    
    // magnetic field
    
    // wmap lsa
    struct param_breg_wmap{
        double b0;
        double psi0;
        double psi1;
        double chi0;
    }breg_wmap;
    
    // test
#ifndef NDEBUG
    struct param_breg_test{
        double b0;
        double l0;
        double r;
    }breg_test;
#endif
    
    // jaffe
    struct param_breg_jaffe{
        bool quadruple,bss;
        double disk_amp,disk_z0;
        double halo_amp,halo_z0;
        double r_inner,r_scale,r_peak; // radial profile
        // ring/bar
        bool ring,bar;
        double ring_amp,bar_amp;
        double ring_r,bar_a,bar_b,bar_phi0;
        // spiral arms
        unsigned arm_num;
        std::vector<double> arm_amp,arm_phi0;
        double arm_pitch,arm_r0,arm_z0;
        // arm compress
        double comp_r,comp_c,comp_d,comp_p;
    }breg_jaffe;
    
    // random seed
    std::size_t brnd_seed;
    
    // global
    struct param_brnd_global_es{
        double rms;
        double k0;
        double a0;
        double rho;
        double r0,z0;
    }brnd_es;
    
    // local
    struct param_brnd_local_mhd{
        double pa0,pf0,ps0;
        double aa0,af0,as0;
        double k0;
        double ma,beta;
    }brnd_mhd;
    
    // FE
    
    // ymw16
    struct param_fereg_ymw16{
        double R_warp, R0;
        double t0_Gamma_w;
        double t1_Ad, t1_Bd, t1_n1, t1_H1;
        double t2_A2, t2_B2, t2_n2, t2_K2;
        double t3_B2s, t3_Ka, t3_narm[5], t3_warm[5], t3_Aa, t3_ncn, t3_wcn, t3_thetacn, t3_nsg, t3_wsg, t3_thetasg, t3_rmin[5], t3_phimin[5], t3_tpitch[5], t3_cpitch[5];
        double t4_ngc, t4_Agc, t4_Hgc;
        double t5_Kgn, t5_ngn, t5_Wgn, t5_Agn;
        double t6_J_LB, t6_nlb1, t6_detlb1, t6_wlb1, t6_hlb1, t6_thetalb1, t6_nlb2, t6_detlb2, t6_wlb2, t6_hlb2, t6_thetalb2;
        double t7_nLI, t7_RLI, t7_WLI, t7_detthetaLI, t7_thetaLI;
    }fereg_ymw16;
    
    // test
#ifndef NDEBUG
    struct param_fereg_test{
        double n0;
        double r0;
    }fereg_test;
#endif
    
    // random seed
    std::size_t fernd_seed;
    
    // isotropic
    struct param_fernd_global_dft{
        double rms;
        double k0;
        double a0;
        double r0;
        double z0;
    }fernd_dft;
    
    // CRE
    double sim_freq;
    
    // analytical
    struct param_cre_ana{
        double alpha,beta,theta;
        double r0,z0;
        double E0,j0;
    }cre_ana;
    
    // test
#ifndef NDEBUG
    struct param_cre_test{
        double alpha;
        double r0;
        double E0,j0;
    }cre_test;
#endif
    
protected:
    
    void b_param(tinyxml2::XMLDocument *);
    
    void fe_param(tinyxml2::XMLDocument *);
    
    void cre_param(tinyxml2::XMLDocument *);
};

#endif

// END
