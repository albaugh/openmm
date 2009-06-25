// -----------------------------------------------------------------------------
//       OpenMM(tm) PROTOTPYE C and Fortran wrapper functions (June 2009)
// -----------------------------------------------------------------------------
// This is the C++ implementation of the C wrappers for the OpenMM workshop.
// The functions here convert between C types and OpenMM's C++ objects
// and then call the appropriate OpenMM methods.
//
// Each C function comes in several forms -- one is intended to be called from C
// main programs, and the others from Fortran main programs. The Fortran ones
// typically just translates Fortran naming and argument conventions into C 
// and then calls the C function. In particular, gcc's gfortran compiler
// translates names into all-lowercase and appends an underscore, while
// Intel's ifort compiler tranlates names into all-uppercase and does not
// append an underscore.
//
// A C main program can use this just by including the OpenMM_CWrapper.h 
// header file that is included here as well. A Fortran 95 program can use
// the "use OpenMM" module which defines an interface to the Fortran-callable
// functions defined here. Fortran 77 programs have to call these directly;
// you can use an integer*8 to hold the pointers on any platform.
// -----------------------------------------------------------------------------

#include "OpenMM_CWrapper.h"
#include <cstring>

// Suppress irrelevant warnings from Microsoft's compiler.
#ifdef _MSC_VER
    #pragma warning(disable:4996)   // sprintf is unsafe 
    #pragma warning(disable:4251)   // no dll interface for some classes
#endif

#include "OpenMM.h"
using namespace OpenMM;


static inline Vec3 toVec3(const OpenMM_Vec3 src) {
    return Vec3(src[0], src[1], src[2]);
}
static inline Vec3 scaleToVec3(const OpenMM_Vec3 src, double s) {
    return Vec3(s*src[0], s*src[1], s*src[2]);
}
static inline void fromVec3(const Vec3& src, OpenMM_Vec3 dest) {
    dest[0] = src[0]; dest[1] = src[1]; dest[2] = src[2];
}
static inline void scaleFromVec3(const Vec3& src, double s, OpenMM_Vec3 dest) {
    dest[0] = s*src[0]; dest[1] = s*src[1]; dest[2] = s*src[2];
}

extern "C" {

    ///////////////////////////////
    // std::vector<OpenMM::Vec3> //
    ///////////////////////////////

// This provides std::vector<Vec3> functionality to the
// C program. It isn't as elegant as in C++ but is still better
// than doing it in C. Also, this allows the C program to
// communicate with OpenMM without having to copy arrays of
// Vec3s to and from std::vectors.

OpenMM_Vec3Array* OpenMM_Vec3Array_create(int n) 
{   return (OpenMM_Vec3Array*)new std::vector<Vec3>(n); }
void openmm_vec3array_create_(OpenMM_Vec3Array*& a, const int& n)
{   a = OpenMM_Vec3Array_create(n); }

int OpenMM_Vec3Array_size(const OpenMM_Vec3Array* a) 
{   return (int)((const std::vector<Vec3>*)a)->size(); }
int openmm_vec3array_size_(const OpenMM_Vec3Array*& a)
{   return OpenMM_Vec3Array_size(a); }

void OpenMM_Vec3Array_resize(OpenMM_Vec3Array* a, int n) 
{   ((std::vector<Vec3>*)a)->resize(n); }
void openmm_vec3array_resize_(OpenMM_Vec3Array* const& a, const int& n)
{   OpenMM_Vec3Array_resize(a, n); }

void OpenMM_Vec3Array_destroy(OpenMM_Vec3Array* doomed) 
{   delete ((std::vector<Vec3>*)doomed); }
void openmm_vec3array_destroy_(OpenMM_Vec3Array*& doomed)
{   OpenMM_Vec3Array_destroy(doomed); doomed = 0; }

void OpenMM_Vec3Array_append(OpenMM_Vec3Array* a, const OpenMM_Vec3 v) 
{   ((std::vector<Vec3>*)a)->push_back(*(const Vec3*)v); }
void openmm_vec3array_append_(OpenMM_Vec3Array* const& a, const OpenMM_Vec3 v)
{   OpenMM_Vec3Array_append(a, v); }

// Get a single Vec3 element from the array. Index is 0-relative in C, 1-relative in Fortran.
void OpenMM_Vec3Array_get(const OpenMM_Vec3Array* a, int i0, OpenMM_Vec3 ov3) {
    fromVec3((*(const std::vector<Vec3>*)a)[i0], ov3);
}
void openmm_vec3array_get_(const OpenMM_Vec3Array* const& a, const int& i1, OpenMM_Vec3 ov3)
{   OpenMM_Vec3Array_get(a, i1-1, ov3); }

// Get a single Vec3 element from the array and scale it. Index is 0-relative in C, 
// 1-relative in Fortran.
void OpenMM_Vec3Array_getScaled(const OpenMM_Vec3Array* a, int i0, double s, OpenMM_Vec3 ov3) {
    scaleFromVec3((*(const std::vector<Vec3>*)a)[i0], s, ov3);
}
void openmm_vec3array_getscaled_(const OpenMM_Vec3Array* const& a, const int& i1, const double& s, OpenMM_Vec3 ov3)
{   OpenMM_Vec3Array_getScaled(a, i1-1, s, ov3); }

// Set a single Vec3 element in the array. Index is 0-relative in C, 1-relative in Fortran.
void OpenMM_Vec3Array_set(OpenMM_Vec3Array* a, int i0, const OpenMM_Vec3 v3)
{   (*(std::vector<Vec3>*)a)[i0] = toVec3(v3); }
void openmm_vec3array_set_(OpenMM_Vec3Array* const& a, const int& i1, const OpenMM_Vec3 v3)
{   OpenMM_Vec3Array_set(a, i1-1, v3); }

// Set a single Vec3 element in the array to a scaling of the input vector. 
// Index is 0-relative in C, 1-relative in Fortran.
void OpenMM_Vec3Array_setScaled(OpenMM_Vec3Array* a, int i0, const OpenMM_Vec3 v3, double s)
{   (*(std::vector<Vec3>*)a)[i0] = scaleToVec3(v3, s); }
void openmm_vec3array_setscaled_(OpenMM_Vec3Array* const& a, const int& i1, const OpenMM_Vec3 v3, const double& s)
{   OpenMM_Vec3Array_setScaled(a, i1-1, v3, s); }

void OpenMM_Vec3_scale(const double in[3], double s, double out[3])
{   out[0]=s*in[0]; out[1]=s*in[1]; out[2]=s*in[2]; }
void openmm_vec3_scale_(const double in[3], const double& s, double out[3])
{   out[0]=s*in[0]; out[1]=s*in[1]; out[2]=s*in[2]; }



    ///////////////////////////////////////
    // std::vector< std::pair<int,int> > //
    ///////////////////////////////////////

// This provides std::vector< std::pair<int,int> > functionality to the
// C program, for use as a list of bond pairs.

typedef std::vector< std::pair<int,int> > IntPairs;

OpenMM_BondArray* OpenMM_BondArray_create(int n) 
{   return (OpenMM_BondArray*)new IntPairs(n); }
void openmm_bondarray_create_(OpenMM_BondArray*& a, const int& n)
{   a = OpenMM_BondArray_create(n); }
void OPENMM_BONDARRAY_CREATE(OpenMM_BondArray*& a, const int& n)
{   a = OpenMM_BondArray_create(n); }

int OpenMM_BondArray_size(const OpenMM_BondArray* a) 
{   return (int)((const IntPairs*)a)->size(); }
int openmm_bondarray_size_(const OpenMM_BondArray*& a)
{   return OpenMM_BondArray_size(a); }
int OPENMM_BONDARRAY_SIZE(const OpenMM_BondArray*& a)
{   return OpenMM_BondArray_size(a); }

void OpenMM_BondArray_resize(OpenMM_BondArray* a, int n) 
{   ((IntPairs*)a)->resize(n); }
void openmm_bondarray_resize_(OpenMM_BondArray* const& a, const int& n)
{   OpenMM_BondArray_resize(a, n); }
void OPENMM_BONDARRAY_RESIZE(OpenMM_BondArray* const& a, const int& n)
{   OpenMM_BondArray_resize(a, n); }

void OpenMM_BondArray_destroy(OpenMM_BondArray* doomed) 
{   delete ((IntPairs*)doomed); }
void openmm_bondarray_destroy_(OpenMM_BondArray*& doomed)
{   OpenMM_BondArray_destroy(doomed); doomed = 0; }
void OPENMM_BONDARRAY_DESTROY(OpenMM_BondArray*& doomed)
{   OpenMM_BondArray_destroy(doomed); doomed = 0; }

void OpenMM_BondArray_append(OpenMM_BondArray* a, int p1, int p2) 
{   ((IntPairs*)a)->push_back(std::make_pair(p1,p2)); }
void openmm_bondarray_append_(OpenMM_BondArray* const& a, int const& p1, int const& p2)
{   OpenMM_BondArray_append(a, p1, p2); }
void OPENMM_BONDARRAY_APPEND(OpenMM_BondArray* const& a, int const& p1, int const& p2)
{   OpenMM_BondArray_append(a, p1, p2); }

// Get a single int pair from the array. Index is 0-relative in C, 1-relative in Fortran.
void OpenMM_BondArray_get(const OpenMM_BondArray* a, int i0, int* p1, int* p2) {
    *p1 = (*(const IntPairs*)a)[i0].first;
    *p2 = (*(const IntPairs*)a)[i0].second;
}
void openmm_bondarray_get_(const OpenMM_BondArray* const& a, const int& i1, int& p1, int& p2)
{   OpenMM_BondArray_get(a, i1-1, &p1, &p2); }
void OPENMM_BONDARRAY_GET(const OpenMM_BondArray* const& a, const int& i1, int& p1, int& p2)
{   OpenMM_BondArray_get(a, i1-1, &p1, &p2); }

// Set a single Vec3 element in the array. Index is 0-relative in C, 1-relative in Fortran.
void OpenMM_BondArray_set(OpenMM_BondArray* a, int i0, int p1, int p2)
{   (*(IntPairs*)a)[i0] = std::make_pair(p1,p2); }
void openmm_bondarray_set_(OpenMM_BondArray* const& a, const int& i1, int const& p1, int const& p2)
{   OpenMM_BondArray_set(a, i1-1, p1, p2); }
void OPENMM_BONDARRAY_SET(OpenMM_BondArray* const& a, const int& i1, int const& p1, int const& p2)
{   OpenMM_BondArray_set(a, i1-1, p1, p2); }


    /////////////////
    // std::string //
    /////////////////

// This is an interface to std::string primarily for Fortran. You
// can use null-terminated char arrays directly in C.

OpenMM_String* OpenMM_String_create(const char* nullTerminatedInitVal) {
    OpenMM_String* os = (OpenMM_String*)new std::string(nullTerminatedInitVal);
    return os;
}
void openmm_string_create_(OpenMM_String*& os, const char* init, int len) {
    std::string* s = new std::string();
    os = (OpenMM_String*)s;
    if (len > 0) {
        s->resize(len);
        std::strncpy(&(*s)[0], init, len);
    }
}
void OpenMM_String_destroy(OpenMM_String* os) {
    delete ((std::string*)os);
}
void openmm_string_destroy_(OpenMM_String*& os) {OpenMM_String_destroy(os);}
const char* OpenMM_String_getAsC(const OpenMM_String* os) {
    return ((const std::string*)os)->c_str();
}
int OpenMM_String_length(const OpenMM_String* os) {
    return (int)((const std::string*)os)->size();
}
int openmm_string_length_(const OpenMM_String* const& os) {
    return OpenMM_String_length(os);
}
// Copy out as a null-terminated C string.
void OpenMM_String_get(const OpenMM_String* os, char* buf, int buflen) {
    if (buflen <= 0) return;
    const std::string& s = *(const std::string*)os;
    const int minlen = std::min((int)s.size(), buflen);
    for (int i=0; i < minlen; ++i)
        buf[i] = s[i];
    const int nullpos = std::min(minlen, buflen-1);
    buf[nullpos] = '\0';
}
// Copy out as a blank-padded Fortran string.
void openmm_string_get_(const OpenMM_String* const& os, char* buf, int buflen) {
    if (buflen <= 0) return;
    const std::string& s = *(const std::string*)os;
    const int minlen = std::min((int)s.size(), buflen);
    for (int i=0; i < minlen; ++i)
        buf[i] = s[i];
    for (int i=minlen; i < buflen; ++i)
        buf[i] = ' ';
}

// Set string from a null-terminated C string, stripping trailing blanks.
void OpenMM_String_set(OpenMM_String* os, const char* in) {
    std::string& s = *(std::string*)os;
    int len = std::strlen(in);
    s = std::string(in, in+len);
    while (len > 0 && s[len-1]==' ')
        --len;
    s.erase(len);
}
// Set string from a fix-sized Fortran character array, 
// stripping trailing blanks.
void openmm_string_set_(OpenMM_String*& os, const char* in, int len) {
    std::string& s = *(std::string*)os;
    s = std::string(in, in+len);
    while (len > 0 && s[len-1]==' ')
        --len;
    s.erase(len);
}


    //////////////////////
    // OpenMM::Platform //
    //////////////////////
void OpenMM_Platform_loadPluginsFromDirectory(const char* dir) {
    OpenMM::Platform::loadPluginsFromDirectory(std::string(dir));
}
const char* OpenMM_Platform_getDefaultPluginsDirectory() {
    static std::string dir;
    dir = OpenMM::Platform::getDefaultPluginsDirectory();
    const char* out = dir.c_str();
    return dir.c_str();
}

void openmm_platform_loadpluginsfromdirectory_(const OpenMM_String* const& dir)
{    OpenMM_Platform_loadPluginsFromDirectory(OpenMM_String_getAsC(dir)); }

void openmm_platform_getdefaultpluginsdirectory_(OpenMM_String* const& dir)
{    OpenMM_String_set(dir, OpenMM_Platform_getDefaultPluginsDirectory()); }


    ////////////////////
    // OpenMM::System //
    ////////////////////
OpenMM_System* 
OpenMM_System_create() {
    return (OpenMM_System*)new System();
}
void openmm_system_create_(OpenMM_System*& sys) {sys=OpenMM_System_create();}

void OpenMM_System_destroy(OpenMM_System* doomed) {
    delete (System*)doomed;
}
void openmm_system_destroy_(OpenMM_System*& doomed) 
{OpenMM_System_destroy(doomed); doomed=0;}

void OpenMM_System_addForce(OpenMM_System* sys, OpenMM_Force* frc) {
    ((System*)sys)->addForce((NonbondedForce*)frc);
}
void openmm_system_addforce_(OpenMM_System*& sys, OpenMM_Force*& frc) 
{OpenMM_System_addForce(sys,frc);}

void OpenMM_System_addParticle(OpenMM_System* sys, double mass) {
    ((System*)sys)->addParticle(mass);
}
void openmm_system_addparticle_(OpenMM_System*& sys, const double& mass) 
{OpenMM_System_addParticle(sys,mass);}

int OpenMM_System_getNumParticles(const OpenMM_System* sys) {
    return ((const System*)sys)->getNumParticles();
}
int openmm_system_getnumparticles_(const OpenMM_System*& sys)
{return OpenMM_System_getNumParticles(sys);}

    ////////////////////////////
    // OpenMM::NonbondedForce //
    ////////////////////////////

// create
OpenMM_NonbondedForce* OpenMM_NonbondedForce_create() 
{   return (OpenMM_NonbondedForce*)new NonbondedForce(); }
void openmm_nonbondedforce_create_(OpenMM_NonbondedForce*& frc)
{   frc = OpenMM_NonbondedForce_create();}
void OPENMM_NONBONDEDFORCE_CREATE(OpenMM_NonbondedForce*& frc)
{   frc = OpenMM_NonbondedForce_create();}

// destroy
void OpenMM_NonbondedForce_destroy(OpenMM_NonbondedForce* doomed) 
{   delete (NonbondedForce*)doomed; }
void openmm_nonbondedforce_destroy_(OpenMM_NonbondedForce*& doomed) 
{   OpenMM_NonbondedForce_destroy(doomed); doomed = 0;}
void OPENMM_NONBONDEDFORCE_DESTROY(OpenMM_NonbondedForce*& doomed) 
{   OpenMM_NonbondedForce_destroy(doomed); doomed = 0;}

// Fortran only: recast NonbondedForce as a Force.
void openmm_nonbondedforce_asforce_(OpenMM_NonbondedForce* const& nonbond,
									OpenMM_Force*&                force)
{   force = (OpenMM_Force*)nonbond; }
void OPENMM_NONBONDEDFORCE_ASFORCE(OpenMM_NonbondedForce* const& nonbond,
									OpenMM_Force*&                force)
{   force = (OpenMM_Force*)nonbond; }

// setNonbondedMethod
void OpenMM_NonbondedForce_setNonbondedMethod(OpenMM_NonbondedForce* nbf, 
                                              OpenMM_NonbondedForce_NonbondedMethod method) 
{    ((NonbondedForce*)nbf)->setNonbondedMethod(NonbondedForce::NonbondedMethod(method)); }
void openmm_nonbondedforce_setnonbondedmethod_(OpenMM_NonbondedForce* const& nbf, const int& method)
{   OpenMM_NonbondedForce_setNonbondedMethod(nbf,OpenMM_NonbondedForce_NonbondedMethod(method)); }
void OPENMM_NONBONDEDFORCE_SETNONBONDEDMETHOD(OpenMM_NonbondedForce* const& nbf, const int& method)
{   OpenMM_NonbondedForce_setNonbondedMethod(nbf,OpenMM_NonbondedForce_NonbondedMethod(method)); }

// getNonbondedMethod
OpenMM_NonbondedForce_NonbondedMethod
OpenMM_NonbondedForce_getNonbondedMethod(const OpenMM_NonbondedForce* nbf)
{   return OpenMM_NonbondedForce_NonbondedMethod(((const NonbondedForce*)nbf)->getNonbondedMethod()); }
int openmm_nonbondedforce_getnonbondedmethod_(const OpenMM_NonbondedForce* const& nbf)
{   return (int)OpenMM_NonbondedForce_getNonbondedMethod(nbf); }
int OPENMM_NONBONDEDFORCE_GETNONBONDEDMETHOD(const OpenMM_NonbondedForce* const& nbf)
{   return (int)OpenMM_NonbondedForce_getNonbondedMethod(nbf); }

// setCutoffDistance
void OpenMM_NonbondedForce_setCutoffDistance(OpenMM_NonbondedForce* nbf, double d) 
{   ((NonbondedForce*)nbf)->setCutoffDistance(d); }
void openmm_nonbondedforce_setcutoffdistance_(OpenMM_NonbondedForce* const& nbf, const double& d)
{   OpenMM_NonbondedForce_setCutoffDistance(nbf,d);}
void OPENMM_NONBONDEDFORCE_SETCUTOFFDISTANCE(OpenMM_NonbondedForce* const& nbf, const double& d)
{   OpenMM_NonbondedForce_setCutoffDistance(nbf,d);}

// getCutoffDistance
double OpenMM_NonbondedForce_getCutoffDistance(const OpenMM_NonbondedForce* nbf) 
{   return ((NonbondedForce*)nbf)->getCutoffDistance(); }
double openmm_nonbondedforce_getcutoffdistance_(const OpenMM_NonbondedForce* const& nbf)
{   return OpenMM_NonbondedForce_getCutoffDistance(nbf);}
double OPENMM_NONBONDEDFORCE_GETCUTOFFDISTANCE(const OpenMM_NonbondedForce* const& nbf)
{   return OpenMM_NonbondedForce_getCutoffDistance(nbf);}

// setPeriodicBoxVectors
void OpenMM_NonbondedForce_setPeriodicBoxVectors(OpenMM_NonbondedForce* nbf, 
                                                 const OpenMM_Vec3 a,const OpenMM_Vec3 b,const OpenMM_Vec3 c) 
{   ((NonbondedForce*)nbf)->setPeriodicBoxVectors(*(const Vec3*)a, *(const Vec3*)b, *(const Vec3*)c); }
void openmm_nonbondedforce_setperiodicboxvectors_(OpenMM_NonbondedForce* const& nbf, 
                                                  const OpenMM_Vec3 a,const OpenMM_Vec3 b,const OpenMM_Vec3 c) 
{   OpenMM_NonbondedForce_setPeriodicBoxVectors(nbf,a,b,c);}
void OPENMM_NONBONDEDFORCE_SETPERIODICBOXVECTORS(OpenMM_NonbondedForce* const& nbf, 
                                                  const OpenMM_Vec3 a,const OpenMM_Vec3 b,const OpenMM_Vec3 c) 
{   OpenMM_NonbondedForce_setPeriodicBoxVectors(nbf,a,b,c);}

// getPeriodicBoxVectors
void OpenMM_NonbondedForce_getPeriodicBoxVectors(const OpenMM_NonbondedForce* nbf, 
                                                 OpenMM_Vec3 a,OpenMM_Vec3 b,OpenMM_Vec3 c) 
{   ((const NonbondedForce*)nbf)->getPeriodicBoxVectors(*(Vec3*)a, *(Vec3*)b, *(Vec3*)c); }
void openmm_nonbondedforce_getperiodicboxvectors_(const OpenMM_NonbondedForce* const& nbf, 
                                                  OpenMM_Vec3 a, OpenMM_Vec3 b, OpenMM_Vec3 c) 
{   OpenMM_NonbondedForce_getPeriodicBoxVectors(nbf,a,b,c);}
void OPENMM_NONBONDEDFORCE_GETPERIODICBOXVECTORS(const OpenMM_NonbondedForce* const& nbf, 
                                                  OpenMM_Vec3 a, OpenMM_Vec3 b, OpenMM_Vec3 c) 
{   OpenMM_NonbondedForce_getPeriodicBoxVectors(nbf,a,b,c);}

// addParticle
int OpenMM_NonbondedForce_addParticle(OpenMM_NonbondedForce* nbf, 
                                      double charge, double sigmaInNm, double vdwEnergyInKJ)
{   return ((NonbondedForce*)nbf)->addParticle(charge, sigmaInNm, vdwEnergyInKJ); }
void openmm_nonbondedforce_addparticle_(OpenMM_NonbondedForce* const& nbf, 
                                        const double& charge, const double& sigmaInNm, const double& vdwEnergyInKJ)
{   OpenMM_NonbondedForce_addParticle(nbf,charge, sigmaInNm, vdwEnergyInKJ);}
void OPENMM_NONBONDEDFORCE_ADDPARTICLE(OpenMM_NonbondedForce* const& nbf, 
                                        const double& charge, const double& sigmaInNm, const double& vdwEnergyInKJ)
{   OpenMM_NonbondedForce_addParticle(nbf,charge, sigmaInNm, vdwEnergyInKJ);}

// setParticleParameters
void OpenMM_NonbondedForce_setParticleParameters(OpenMM_NonbondedForce* nbf, int index,
                                        double charge, double sigmaInNm, double vdwEnergyInKJ)
{   ((NonbondedForce*)nbf)->setParticleParameters(index, charge, sigmaInNm, vdwEnergyInKJ); }
void openmm_nonbondedforce_setparticleparameters_(OpenMM_NonbondedForce* const& nbf, const int& index,
                                        const double& charge, const double& sigmaInNm, const double& vdwEnergyInKJ)
{   OpenMM_NonbondedForce_setParticleParameters(nbf, index, charge, sigmaInNm, vdwEnergyInKJ);}
void OPENMM_NONBONDEDFORCE_SETPARTICLEPARAMETERS(OpenMM_NonbondedForce* const& nbf, const int& index,
                                        const double& charge, const double& sigmaInNm, const double& vdwEnergyInKJ)
{   OpenMM_NonbondedForce_setParticleParameters(nbf, index, charge, sigmaInNm, vdwEnergyInKJ);}

// getParticleParameters
void OpenMM_NonbondedForce_getParticleParameters(const OpenMM_NonbondedForce* nbf, int index,
                                        double* charge, double* sigmaInNm, double* vdwEnergyInKJ)
{   ((const NonbondedForce*)nbf)->getParticleParameters(index, *charge, *sigmaInNm, *vdwEnergyInKJ); }
void openmm_nonbondedforce_getparticleparameters_(const OpenMM_NonbondedForce* const& nbf, const int& index,
                                        double& charge, double& sigmaInNm, double& vdwEnergyInKJ)
{   OpenMM_NonbondedForce_getParticleParameters(nbf, index, &charge, &sigmaInNm, &vdwEnergyInKJ);}
void OPENMM_NONBONDEDFORCE_GETPARTICLEPARAMETERS(const OpenMM_NonbondedForce* const& nbf, const int& index,
                                        double& charge, double& sigmaInNm, double& vdwEnergyInKJ)
{   OpenMM_NonbondedForce_getParticleParameters(nbf, index, &charge, &sigmaInNm, &vdwEnergyInKJ);}

// getNumParticles
int OpenMM_NonbondedForce_getNumParticles(const OpenMM_NonbondedForce* nbf)
{   return ((const NonbondedForce*)nbf)->getNumParticles(); }
int openmm_nonbondedforce_getnumparticles_(const OpenMM_NonbondedForce* const& nbf)
{   return OpenMM_NonbondedForce_getNumParticles(nbf); }
int OPENMM_NONBONDEDFORCE_GETNUMPARTICLES(const OpenMM_NonbondedForce* const& nbf)
{   return OpenMM_NonbondedForce_getNumParticles(nbf); }

// getNumExceptions
int OpenMM_NonbondedForce_getNumExceptions(const OpenMM_NonbondedForce* nbf)
{   return ((const NonbondedForce*)nbf)->getNumExceptions(); }
int openmm_nonbondedforce_getnumexceptions_(const OpenMM_NonbondedForce* const& nbf)
{   return OpenMM_NonbondedForce_getNumExceptions(nbf); }
int OPENMM_NONBONDEDFORCE_GETNUMEXCEPTIONS(const OpenMM_NonbondedForce* const& nbf)
{   return OpenMM_NonbondedForce_getNumExceptions(nbf); }

// addException
int OpenMM_NonbondedForce_addException(OpenMM_NonbondedForce* nbf, int p1, int p2, 
                                      double chargeProd, double sigmaInNm, double vdwEnergyInKJ)
{   return ((NonbondedForce*)nbf)->addException(p1, p2, chargeProd, sigmaInNm, vdwEnergyInKJ); }
void openmm_nonbondedforce_addexception_(OpenMM_NonbondedForce* const& nbf, const int& p1, const int& p2,
                                         const double& chargeProd, const double& sigmaInNm, const double& vdwEnergyInKJ)
{   OpenMM_NonbondedForce_addException(nbf, p1, p2, chargeProd, sigmaInNm, vdwEnergyInKJ);}
void OPENMM_NONBONDEDFORCE_ADDEXCEPTION(OpenMM_NonbondedForce* const& nbf, const int& p1, const int& p2,
                                         const double& chargeProd, const double& sigmaInNm, const double& vdwEnergyInKJ)
{   OpenMM_NonbondedForce_addException(nbf, p1, p2, chargeProd, sigmaInNm, vdwEnergyInKJ);}

// setExceptionParameters
void OpenMM_NonbondedForce_setExceptionParameters(OpenMM_NonbondedForce* nbf, int index, int p1, int p2, 
                                                  double chargeProd, double sigmaInNm, double vdwEnergyInKJ)
{   return ((NonbondedForce*)nbf)->setExceptionParameters(index, p1, p2, chargeProd, sigmaInNm, vdwEnergyInKJ); }
void openmm_nonbondedforce_setexceptionparameters_(OpenMM_NonbondedForce* const& nbf, const int& index, const int& p1, const int& p2,
                                         const double& chargeProd, const double& sigmaInNm, const double& vdwEnergyInKJ)
{   OpenMM_NonbondedForce_setExceptionParameters(nbf, index, p1, p2, chargeProd, sigmaInNm, vdwEnergyInKJ);}
void OPENMM_NONBONDEDFORCE_SETEXCEPTIONPARAMETERS(OpenMM_NonbondedForce* const& nbf, const int& index, const int& p1, const int& p2,
                                         const double& chargeProd, const double& sigmaInNm, const double& vdwEnergyInKJ)
{   OpenMM_NonbondedForce_setExceptionParameters(nbf, index, p1, p2, chargeProd, sigmaInNm, vdwEnergyInKJ);}

// getExceptionParameters
void OpenMM_NonbondedForce_getExceptionParameters(const OpenMM_NonbondedForce* nbf, int index, int* p1, int* p2, 
                                                  double* chargeProd, double* sigmaInNm, double* vdwEnergyInKJ)
{   return ((const NonbondedForce*)nbf)->getExceptionParameters(index, *p1, *p2, *chargeProd, *sigmaInNm, *vdwEnergyInKJ); }
void openmm_nonbondedforce_getexceptionparameters_(const OpenMM_NonbondedForce* const& nbf, const int& index, int& p1, int& p2,
                                         double& chargeProd, double& sigmaInNm, double& vdwEnergyInKJ)
{   OpenMM_NonbondedForce_getExceptionParameters(nbf, index, &p1, &p2, &chargeProd, &sigmaInNm, &vdwEnergyInKJ);}
void OPENMM_NONBONDEDFORCE_GETEXCEPTIONPARAMETERS(const OpenMM_NonbondedForce* const& nbf, const int& index, int& p1, int& p2,
                                         double& chargeProd, double& sigmaInNm, double& vdwEnergyInKJ)
{   OpenMM_NonbondedForce_getExceptionParameters(nbf, index, &p1, &p2, &chargeProd, &sigmaInNm, &vdwEnergyInKJ);}

// createExceptionsFromBonds
void OpenMM_NonbondedForces_createExceptionsFromBonds(OpenMM_NonbondedForce* nbf, const OpenMM_BondArray* ba,
                                                      double coulomb14Scale, double lj14Scale)
{   typedef std::vector< std::pair<int,int> > IntPairs;
    const IntPairs& bonds = *(const IntPairs*)ba;
    ((NonbondedForce*)nbf)->createExceptionsFromBonds(bonds, coulomb14Scale, lj14Scale); 
}
void openmm_nonbondedforces_createexceptionsfrombonds_(OpenMM_NonbondedForce* const& nbf, const OpenMM_BondArray* const& ba,
                                                       double const& coulomb14Scale, double const& lj14Scale)
{   OpenMM_NonbondedForces_createExceptionsFromBonds(nbf, ba, coulomb14Scale, lj14Scale); }
void OPENMM_NONBONDEDFORCES_CREATEEXCEPTIONSFROMBONDS(OpenMM_NonbondedForce* const& nbf, const OpenMM_BondArray* const& ba,
                                                      double const& coulomb14Scale, double const& lj14Scale)
{   OpenMM_NonbondedForces_createExceptionsFromBonds(nbf, ba, coulomb14Scale, lj14Scale); }

    //////////////////////////
    // OpenMM::GBSAOBCForce //
    //////////////////////////

// create
OpenMM_GBSAOBCForce* OpenMM_GBSAOBCForce_create() 
{   return (OpenMM_GBSAOBCForce*)new GBSAOBCForce(); }
void openmm_gbsaobcforce_create_(OpenMM_GBSAOBCForce*& frc)
{   frc = OpenMM_GBSAOBCForce_create(); }

// destroy
void OpenMM_GBSAOBCForce_destroy(OpenMM_GBSAOBCForce* doomed) 
{   delete (GBSAOBCForce*)doomed; }
void openmm_gbsaobcforce_destroy_(OpenMM_GBSAOBCForce*& doomed) 
{   OpenMM_GBSAOBCForce_destroy(doomed); doomed = 0; }

// Fortran only: recast GBSAOBCForce as a Force.
void openmm_gbsaobcforce_asforce_(OpenMM_GBSAOBCForce* const& gbsa,
							      OpenMM_Force*&              force)
{   force = (OpenMM_Force*)gbsa; }

// setSolventDielectric
void OpenMM_GBSAOBCForce_setSolventDielectric(OpenMM_GBSAOBCForce* gbsa, double d) 
{   ((GBSAOBCForce*)gbsa)->setSolventDielectric(d); }
void openmm_gbsaobcforce_setsolventdielectric_(OpenMM_GBSAOBCForce*& gbsa, const double& d)
{   OpenMM_GBSAOBCForce_setSolventDielectric(gbsa,d); }

// setSoluteDielectric
void OpenMM_GBSAOBCForce_setSoluteDielectric(OpenMM_GBSAOBCForce* gbsa, double d) 
{   ((GBSAOBCForce*)gbsa)->setSoluteDielectric(d); }
void openmm_gbsaobcforce_setsolutedielectric_(OpenMM_GBSAOBCForce*& gbsa, const double& d)
{   OpenMM_GBSAOBCForce_setSoluteDielectric(gbsa,d); }

// addParticle
void OpenMM_GBSAOBCForce_addParticle(OpenMM_GBSAOBCForce* gbsa, 
                                     double charge, double radiusInNm, double scalingFactor)
{   ((GBSAOBCForce*)gbsa)->addParticle(charge, radiusInNm, scalingFactor); }
void openmm_gbsaobcforce_addparticle_(OpenMM_GBSAOBCForce*& gbsa, 
                                      const double& charge, const double& radiusInNm, const double& scalingFactor)
{   OpenMM_GBSAOBCForce_addParticle(gbsa,charge, radiusInNm, scalingFactor); }


    ///////////////////////////////
    // OpenMM::HarmonicBondForce //
    ///////////////////////////////

// create
OpenMM_HarmonicBondForce* OpenMM_HarmonicBondForce_create()
{   return (OpenMM_HarmonicBondForce*)new HarmonicBondForce(); }
void openmm_harmonicbondforce_create_(OpenMM_HarmonicBondForce*& frc)
{   frc = OpenMM_HarmonicBondForce_create();}
void OPENMM_HARMONICBONDFORCE_CREATE(OpenMM_HarmonicBondForce*& frc)
{   frc = OpenMM_HarmonicBondForce_create();}

// destroy
void OpenMM_HarmonicBondForce_destroy(OpenMM_HarmonicBondForce* doomed)
{   delete (HarmonicBondForce*)doomed; }
void openmm_harmonicbondforce_destroy_(OpenMM_HarmonicBondForce*& doomed) 
{   OpenMM_HarmonicBondForce_destroy(doomed); doomed = 0;}
void OPENMM_HARMONICBONDFORCE_DESTROY(OpenMM_HarmonicBondForce*& doomed) 
{   OpenMM_HarmonicBondForce_destroy(doomed); doomed = 0;}

// getNumBonds
int OpenMM_HarmonicBondForce_getNumBonds(const OpenMM_HarmonicBondForce* hbf)
{   return ((const HarmonicBondForce*)hbf)->getNumBonds(); }
int openmm_harmonicbondforce_getnumbonds_(const OpenMM_HarmonicBondForce* const& hbf)
{   return OpenMM_HarmonicBondForce_getNumBonds(hbf); }
int OPENMM_HARMONICBONDFORCE_GETNUMBONDS(const OpenMM_HarmonicBondForce* const& hbf)
{   return OpenMM_HarmonicBondForce_getNumBonds(hbf); }

// addBond
int OpenMM_HarmonicBondForce_addBond(OpenMM_HarmonicBondForce* hbf, int p1, int p2, double len, double k)
{   return ((HarmonicBondForce*)hbf)->addBond(p1,p2,len,k); }
int openmm_harmonicbondforce_addbond_(OpenMM_HarmonicBondForce* const& hbf, int const& p1, int const& p2, double const& len, double const& k)
{   return OpenMM_HarmonicBondForce_addBond(hbf,p1,p2,len,k); }
int OPENMM_HARMONICBONDFORCE_ADDBOND(OpenMM_HarmonicBondForce* const& hbf, int const& p1, int const& p2, double const& len, double const& k)
{   return OpenMM_HarmonicBondForce_addBond(hbf,p1,p2,len,k); }

// setBondParameters
void OpenMM_HarmonicBondForce_setBondParameters(OpenMM_HarmonicBondForce* hbf, int ix, int p1, int p2, double len, double k)
{   ((HarmonicBondForce*)hbf)->setBondParameters(ix,p1,p2,len,k); }
void openmm_harmonicbondforce_setbondparameters_(OpenMM_HarmonicBondForce* const& hbf, int const& ix, int const& p1, int const& p2, double const& len, double const& k)
{   OpenMM_HarmonicBondForce_setBondParameters(hbf,ix,p1,p2,len,k); }
void OPENMM_HARMONICBONDFORCE_SETBONDPARAMETERS(OpenMM_HarmonicBondForce* const& hbf, int const& ix, int const& p1, int const& p2, double const& len, double const& k)
{   OpenMM_HarmonicBondForce_setBondParameters(hbf,ix,p1,p2,len,k); }

// getBondParameters
void OpenMM_HarmonicBondForce_getBondParameters(const OpenMM_HarmonicBondForce* hbf, int ix, 
                                                int* p1, int* p2, double* len, double* k)
{   ((const HarmonicBondForce*)hbf)->getBondParameters(ix,*p1,*p2,*len,*k); }
void openmm_harmonicbondforce_getbondparameters_(const OpenMM_HarmonicBondForce* const& hbf, int const& ix, int& p1, int& p2, double& len, double& k)
{   OpenMM_HarmonicBondForce_getBondParameters(hbf,ix,&p1,&p2,&len,&k); }
void OPENMM_HARMONICBONDFORCE_GETBONDPARAMETERS(const OpenMM_HarmonicBondForce* const& hbf, int const& ix, int& p1, int& p2, double& len, double& k)
{   OpenMM_HarmonicBondForce_getBondParameters(hbf,ix,&p1,&p2,&len,&k); }



    ////////////////////////////////
    // OpenMM::HarmonicAngleForce //
    ////////////////////////////////

// create
OpenMM_HarmonicAngleForce* OpenMM_HarmonicAngleForce_create()
{   return (OpenMM_HarmonicAngleForce*)new HarmonicAngleForce(); }
void openmm_harmonicangleforce_create_(OpenMM_HarmonicAngleForce*& frc)
{   frc = OpenMM_HarmonicAngleForce_create();}
void OPENMM_HARMONICANGLEFORCE_CREATE(OpenMM_HarmonicAngleForce*& frc)
{   frc = OpenMM_HarmonicAngleForce_create();}

// destroy
void OpenMM_HarmonicAngleForce_destroy(OpenMM_HarmonicAngleForce* doomed)
{   delete (HarmonicAngleForce*)doomed; }
void openmm_harmonicangleforce_destroy_(OpenMM_HarmonicAngleForce*& doomed) 
{   OpenMM_HarmonicAngleForce_destroy(doomed); doomed = 0;}
void OPENMM_HARMONICANGLEFORCE_DESTROY(OpenMM_HarmonicAngleForce*& doomed) 
{   OpenMM_HarmonicAngleForce_destroy(doomed); doomed = 0;}

// getNumAngles
int OpenMM_HarmonicAngleForce_getNumAngles(const OpenMM_HarmonicAngleForce* haf)
{   return ((const HarmonicAngleForce*)haf)->getNumAngles(); }
int openmm_harmonicangleforce_getnumangles_(const OpenMM_HarmonicAngleForce* const& haf)
{   return OpenMM_HarmonicAngleForce_getNumAngles(haf); }
int OPENMM_HARMONICANGLEFORCE_GETNUMANGLES(const OpenMM_HarmonicAngleForce* const& haf)
{   return OpenMM_HarmonicAngleForce_getNumAngles(haf); }

// addAngle
int OpenMM_HarmonicAngleForce_addAngle(OpenMM_HarmonicAngleForce* haf, int p1, int p2, int p3, double angle, double k)
{   return ((HarmonicAngleForce*)haf)->addAngle(p1,p2,p3,angle,k); }
int openmm_harmonicangleforce_addangle_(OpenMM_HarmonicAngleForce* const& haf, int const& p1, int const& p2, int const& p3, double const& angle, double const& k)
{   return OpenMM_HarmonicAngleForce_addAngle(haf,p1,p2,p3,angle,k); }
int OPENMM_HARMONICANGLEFORCE_ADDANGLE(OpenMM_HarmonicAngleForce* const& haf, int const& p1, int const& p2, int const& p3, double const& angle, double const& k)
{   return OpenMM_HarmonicAngleForce_addAngle(haf,p1,p2,p3,angle,k); }

// setAngleParameters
void OpenMM_HarmonicAngleForce_setAngleParameters(OpenMM_HarmonicAngleForce* haf, int ix, int p1, int p2, int p3, double angle, double k)
{   ((HarmonicAngleForce*)haf)->setAngleParameters(ix,p1,p2,p3,angle,k); }
void openmm_harmonicangleforce_setangleparameters_(OpenMM_HarmonicAngleForce* const& haf, int const& ix, int const& p1, int const& p2, int const& p3, double const& angle, double const& k)
{   OpenMM_HarmonicAngleForce_setAngleParameters(haf,ix,p1,p2,p3,angle,k); }
void OPENMM_HARMONICANGLEFORCE_SETANGLEPARAMETERS(OpenMM_HarmonicAngleForce* const& haf, int const& ix, int const& p1, int const& p2, int const& p3, double const& angle, double const& k)
{   OpenMM_HarmonicAngleForce_setAngleParameters(haf,ix,p1,p2,p3,angle,k); }

// getAngleParameters
void OpenMM_HarmonicAngleForce_getAngleParameters(const OpenMM_HarmonicAngleForce* haf, int ix, 
                                                int* p1, int* p2, int* p3, double* angle, double* k)
{   ((const HarmonicAngleForce*)haf)->getAngleParameters(ix,*p1,*p2,*p3,*angle,*k); }
void openmm_harmonicangleforce_getangleparameters_(const OpenMM_HarmonicAngleForce* const& haf, int const& ix, int& p1, int& p2, int& p3, double& angle, double& k)
{   OpenMM_HarmonicAngleForce_getAngleParameters(haf,ix,&p1,&p2,&p3,&angle,&k); }
void OPENMM_HARMONICANGLEFORCE_GETANGLEPARAMETERS(const OpenMM_HarmonicAngleForce* const& haf, int const& ix, int& p1, int& p2, int& p3, double& angle, double& k)
{   OpenMM_HarmonicAngleForce_getAngleParameters(haf,ix,&p1,&p2,&p3,&angle,&k); }




    //////////////////////////////////
    // OpenMM::PeriodicTorsionForce //
    //////////////////////////////////

// create
OpenMM_PeriodicTorsionForce* OpenMM_PeriodicTorsionForce_create()
{   return (OpenMM_PeriodicTorsionForce*)new PeriodicTorsionForce(); }
void openmm_periodictorsionforce_create_(OpenMM_PeriodicTorsionForce*& frc)
{   frc = OpenMM_PeriodicTorsionForce_create();}
void OPENMM_PERIODICTORSIONFORCE_CREATE(OpenMM_PeriodicTorsionForce*& frc)
{   frc = OpenMM_PeriodicTorsionForce_create();}

// destroy
void OpenMM_PeriodicTorsionForce_destroy(OpenMM_PeriodicTorsionForce* doomed)
{   delete (PeriodicTorsionForce*)doomed; }
void openmm_periodictorsionforce_destroy_(OpenMM_PeriodicTorsionForce*& doomed) 
{   OpenMM_PeriodicTorsionForce_destroy(doomed); doomed = 0;}
void OPENMM_PERIODICTORSIONFORCE_DESTROY(OpenMM_PeriodicTorsionForce*& doomed) 
{   OpenMM_PeriodicTorsionForce_destroy(doomed); doomed = 0;}

// getNumTorsions
int OpenMM_PeriodicTorsionForce_getNumTorsions(const OpenMM_PeriodicTorsionForce* haf)
{   return ((const PeriodicTorsionForce*)haf)->getNumTorsions(); }
int openmm_periodictorsionforce_getnumangles_(const OpenMM_PeriodicTorsionForce* const& haf)
{   return OpenMM_PeriodicTorsionForce_getNumTorsions(haf); }
int OPENMM_PERIODICTORSIONFORCE_GETNUMANGLES(const OpenMM_PeriodicTorsionForce* const& haf)
{   return OpenMM_PeriodicTorsionForce_getNumTorsions(haf); }

// addTorsion
int OpenMM_PeriodicTorsionForce_addTorsion(OpenMM_PeriodicTorsionForce* haf, int p1, int p2, int p3, int p4, int periodicity, double phase, double k)
{   return ((PeriodicTorsionForce*)haf)->addTorsion(p1,p2,p3,p4,periodicity,phase,k); }
int openmm_periodictorsionforce_addtorsion_(OpenMM_PeriodicTorsionForce* const& haf, int const& p1, int const& p2, int const& p3, int const& p4, int const& periodicity, double const& phase, double const& k)
{   return OpenMM_PeriodicTorsionForce_addTorsion(haf,p1,p2,p3,p4,periodicity,phase,k); }
int OPENMM_PERIODICTORSIONFORCE_ADDTORSION(OpenMM_PeriodicTorsionForce* const& haf, int const& p1, int const& p2, int const& p3, int const& p4, int const& periodicity, double const& phase, double const& k)
{   return OpenMM_PeriodicTorsionForce_addTorsion(haf,p1,p2,p3,p4,periodicity,phase,k); }

// setTorsionParameters
void OpenMM_PeriodicTorsionForce_setTorsionParameters(OpenMM_PeriodicTorsionForce* haf, int ix, int p1, int p2, int p3, int p4, int periodicity, double phase, double k)
{   ((PeriodicTorsionForce*)haf)->setTorsionParameters(ix,p1,p2,p3,p4,periodicity,phase,k); }
void openmm_periodictorsionforce_settorsionparameters_(OpenMM_PeriodicTorsionForce* const& haf, int const& ix, int const& p1, int const& p2, int const& p3, int const& p4, int const& periodicity, double const& phase, double const& k)
{   OpenMM_PeriodicTorsionForce_setTorsionParameters(haf,ix,p1,p2,p3,p4,periodicity,phase,k); }
void OPENMM_PERIODICTORSIONFORCE_SETTORSIONPARAMETERS(OpenMM_PeriodicTorsionForce* const& haf, int const& ix, int const& p1, int const& p2, int const& p3, int const& p4, int const& periodicity, double const& phase, double const& k)
{   OpenMM_PeriodicTorsionForce_setTorsionParameters(haf,ix,p1,p2,p3,p4,periodicity,phase,k); }

// getTorsionParameters
void OpenMM_PeriodicTorsionForce_getTorsionParameters(const OpenMM_PeriodicTorsionForce* haf, int ix, 
                                                int* p1, int* p2, int* p3, int* p4, int* periodicity, double* phase, double* k)
{   ((const PeriodicTorsionForce*)haf)->getTorsionParameters(ix,*p1,*p2,*p3,*p4,*periodicity,*phase,*k); }
void openmm_periodictorsionforce_gettorsionparameters_(const OpenMM_PeriodicTorsionForce* const& haf, int const& ix, int& p1, int& p2, int& p3, int& p4, int& periodicity, double& phase, double& k)
{   OpenMM_PeriodicTorsionForce_getTorsionParameters(haf,ix,&p1,&p2,&p3,&p4,&periodicity,&phase,&k); }
void OPENMM_PERIODICTORSIONFORCE_GETTORSIONPARAMETERS(const OpenMM_PeriodicTorsionForce* const& haf, int const& ix, int& p1, int& p2, int& p3, int& p4, int& periodicity, double& phase, double& k)
{   OpenMM_PeriodicTorsionForce_getTorsionParameters(haf,ix,&p1,&p2,&p3,&p4,&periodicity,&phase,&k); }


    ////////////////////////
    // OpenMM::Integrator //
    ////////////////////////
void OpenMM_Integrator_step(OpenMM_Integrator* integ, int numSteps) 
{    ((Integrator*)integ)->step(numSteps); }
void openmm_integrator_step_(OpenMM_Integrator* const& integ, int& numSteps) 
{    OpenMM_Integrator_step(integ, numSteps); }

void OpenMM_Integrator_destroy(OpenMM_Integrator* doomed) 
{   delete ((Integrator*)doomed); }
void openmm_integrator_destroy_(OpenMM_Integrator*& doomed)
{   OpenMM_Integrator_destroy(doomed); doomed = 0; }

    // OpenMM::VerletIntegrator
OpenMM_VerletIntegrator* OpenMM_VerletIntegrator_create(double stepSzInPs) 
{   return (OpenMM_VerletIntegrator*)new VerletIntegrator(stepSzInPs); }
void openmm_verletintegrator_create_(OpenMM_VerletIntegrator*& verlet, double& stepSzInPs)
{   verlet = OpenMM_VerletIntegrator_create(stepSzInPs); }

void OpenMM_VerletIntegrator_destroy(OpenMM_VerletIntegrator* doomed) 
{   delete (VerletIntegrator*)doomed; }
void openmm_verletintegrator_destroy_(OpenMM_VerletIntegrator*& doomed)
{   OpenMM_VerletIntegrator_destroy(doomed); doomed = 0; }

// Fortran only: recast VerletIntegrator as an Integrator.
void openmm_verletintegrator_asintegrator_(OpenMM_VerletIntegrator* const& verlet,
										   OpenMM_Integrator*&             integ)
{   integ = (OpenMM_Integrator*)verlet; }

void OpenMM_VerletIntegrator_step(OpenMM_VerletIntegrator* verlet, int numSteps) 
{   ((VerletIntegrator*)verlet)->step(numSteps); }
void openmm_verletintegrator_step_(OpenMM_VerletIntegrator* const& verlet, int& numSteps)
{   OpenMM_VerletIntegrator_step(verlet, numSteps); }


    // OpenMM::LangevinIntegrator
OpenMM_LangevinIntegrator* OpenMM_LangevinIntegrator_create(double temperature, double frictionInPerPs, double stepSzInPs) 
{   return (OpenMM_LangevinIntegrator*)new LangevinIntegrator(temperature, frictionInPerPs, stepSzInPs); }
void openmm_langevinintegrator_create_(OpenMM_LangevinIntegrator*& langevin, double& temperature, double& frictionInPerPs, double& stepSzInPs)
{   langevin = OpenMM_LangevinIntegrator_create(temperature, frictionInPerPs, stepSzInPs); }

void OpenMM_LangevinIntegrator_destroy(OpenMM_LangevinIntegrator* doomed) 
{   delete (LangevinIntegrator*)doomed; }
void openmm_langevinintegrator_destroy_(OpenMM_LangevinIntegrator*& doomed)
{   OpenMM_LangevinIntegrator_destroy(doomed); doomed = 0; }

// Fortran only: recast LangevinIntegrator as an Integrator.
void openmm_langevinintegrator_asintegrator_(OpenMM_LangevinIntegrator* const& langevin,
										     OpenMM_Integrator*&               integ)
{   integ = (OpenMM_Integrator*)langevin; }

void OpenMM_LangevinIntegrator_step(OpenMM_LangevinIntegrator* langevin, int numSteps) 
{   ((LangevinIntegrator*)langevin)->step(numSteps); }
void openmm_langevinintegrator_step_(OpenMM_LangevinIntegrator* const& langevin, int& numSteps)
{   OpenMM_LangevinIntegrator_step(langevin, numSteps); }

    /////////////////////
    // OpenMM::Context //
    /////////////////////
OpenMM_Context* OpenMM_Context_create(OpenMM_System* sys, OpenMM_Integrator* integ) {
    return (OpenMM_Context*)new OpenMM::OpenMMContext(*(System*)sys, *(Integrator*)integ);
}
void openmm_context_create_(OpenMM_Context*& context, OpenMM_System*& sys, OpenMM_Integrator*& integ)
{   context = OpenMM_Context_create(sys, integ); }

void OpenMM_Context_destroy(OpenMM_Context* doomed) {
    delete (OpenMMContext*)doomed;
}
void openmm_context_destroy_(OpenMM_Context*& doomed) 
{    OpenMM_Context_destroy(doomed); }

void OpenMM_Context_setPositions(OpenMM_Context* context, const OpenMM_Vec3Array* positions) {
    ((OpenMMContext*)context)->setPositions(*(const std::vector<Vec3>*)positions);
}
void openmm_context_setpositions_(OpenMM_Context*& context, const OpenMM_Vec3Array* const& positions)
{    OpenMM_Context_setPositions(context, positions); }

void OpenMM_Context_setVelocities(OpenMM_Context* context, const OpenMM_Vec3Array* velocities) {
    ((OpenMMContext*)context)->setVelocities(*(const std::vector<Vec3>*)velocities);
}
void openmm_context_setvelocities_(OpenMM_Context*& context, const OpenMM_Vec3Array* const& velocities)
{    OpenMM_Context_setVelocities(context, velocities); }

// Note that a Context creates the OpenMM::State object, but you have to destroy
// it using OpenMM_State_destroy.
OpenMM_State* OpenMM_Context_createState(const OpenMM_Context* context, int types) {
    return (OpenMM_State*)new State(((OpenMMContext*)context)->getState(types));
}
void openmm_context_createstate_(const OpenMM_Context* const& context, const int& types, OpenMM_State*& state)
{   state=OpenMM_Context_createState(context, types); }

// Return a reference to a static null terminated C string containing the
// Platform name.
const char* OpenMM_Context_getPlatformName(const OpenMM_Context* context) {
    static std::string platform;
    platform = ((const OpenMMContext*)context)->getPlatform().getName();
    return platform.c_str();
}
// Return a blank-padded Fortran string containing the Platform name. There
// is no terminating null.
void openmm_context_getplatformname_(const OpenMM_Context* const& context, char* buf, int len) {
    const std::string name = ((const OpenMMContext*)context)->getPlatform().getName();
    const int minLen = std::min((int)name.size(), len);
    for (int i=0; i<minLen; ++i) buf[i] = name[i];
    for (int i=minLen; i<len; ++i) buf[i] = ' ';
}

    ///////////////////
    // OpenMM::State //
    ///////////////////
void OpenMM_State_destroy(OpenMM_State* doomed) 
{   delete (State*)doomed; }
void openmm_state_destroy_(OpenMM_State*& doomed)
{   OpenMM_State_destroy(doomed); doomed=0; }

double OpenMM_State_getTime(const OpenMM_State* state) 
{   return ((const State*)state)->getTime(); }
double openmm_state_gettime_(const OpenMM_State* const& state)
{   return OpenMM_State_getTime(state); }

double OpenMM_State_getPotentialEnergy(const OpenMM_State* state) 
{   return ((const State*)state)->getPotentialEnergy(); }
double openmm_state_getpotentialenergy_(const OpenMM_State* const& state)
{   return OpenMM_State_getPotentialEnergy(state); }

double OpenMM_State_getKineticEnergy(const OpenMM_State* state) 
{   return ((const State*)state)->getKineticEnergy(); }
double openmm_state_getkineticenergy_(const OpenMM_State* const& state)
{   return OpenMM_State_getKineticEnergy(state); }

const OpenMM_Vec3Array* OpenMM_State_getPositions(const OpenMM_State* state) 
{   return (const OpenMM_Vec3Array*)&((const State*)state)->getPositions(); }
void openmm_state_getpositions_(const OpenMM_State* const& state, const OpenMM_Vec3Array*& positions)
{   positions = OpenMM_State_getPositions(state); }

const OpenMM_Vec3Array*  OpenMM_State_getVelocities(const OpenMM_State* state) 
{   return (const OpenMM_Vec3Array*)&((const State*)state)->getVelocities(); }
void openmm_state_getvelocities_(const OpenMM_State* const& state, const OpenMM_Vec3Array*& velocities)
{   velocities = OpenMM_State_getVelocities(state); }


    ///////////////////////////
    // OpenMM_RuntimeObjects //
    ///////////////////////////
OpenMM_RuntimeObjects* OpenMM_RuntimeObjects_create() {
    OpenMM_RuntimeObjects* ommrt = new OpenMM_RuntimeObjects();
    ommrt->system     = 0;
    ommrt->integrator = 0;
    ommrt->context    = 0;
    return ommrt;
}
void openmm_runtimeobjects_create_(OpenMM_RuntimeObjects*& ommrt) 
{   ommrt = OpenMM_RuntimeObjects_create(); }

void OpenMM_RuntimeObjects_clear(OpenMM_RuntimeObjects* ommrt) {
    if (!ommrt) return;
    OpenMM_Context_destroy(ommrt->context);       ommrt->context    = 0;
    OpenMM_Integrator_destroy(ommrt->integrator); ommrt->integrator = 0;
    OpenMM_System_destroy(ommrt->system);         ommrt->system     = 0;
}
void openmm_runtimeobjects_clear_(OpenMM_RuntimeObjects*& ommrt)
{   OpenMM_RuntimeObjects_clear(ommrt); }

void OpenMM_RuntimeObjects_destroy(OpenMM_RuntimeObjects* ommrt)
{    OpenMM_RuntimeObjects_clear(ommrt); delete ommrt; }
void openmm_runtimeobjects_destroy_(OpenMM_RuntimeObjects*& ommrt)
{    OpenMM_RuntimeObjects_destroy(ommrt); ommrt = 0; }

void OpenMM_RuntimeObjects_setSystem(OpenMM_RuntimeObjects* ommrt, OpenMM_System* sys)
{    OpenMM_System_destroy(ommrt->system); ommrt->system = sys; }
void openmm_runtimeobjects_setsystem_(OpenMM_RuntimeObjects*& ommrt, OpenMM_System*& sys)
{    OpenMM_RuntimeObjects_setSystem(ommrt, sys); }
OpenMM_System* OpenMM_RuntimeObjects_getSystem(OpenMM_RuntimeObjects* ommrt)
{    return ommrt->system; }
void openmm_runtimeobjects_getsystem_(OpenMM_RuntimeObjects*& ommrt, OpenMM_System*& sys)
{    sys = OpenMM_RuntimeObjects_getSystem(ommrt); }

void OpenMM_RuntimeObjects_setIntegrator(OpenMM_RuntimeObjects* ommrt, OpenMM_Integrator* integ)
{    OpenMM_Integrator_destroy(ommrt->integrator); ommrt->integrator = integ; }
void openmm_runtimeobjects_setintegrator_(OpenMM_RuntimeObjects*& ommrt, OpenMM_Integrator*& integ)
{    OpenMM_RuntimeObjects_setIntegrator(ommrt, integ); }
OpenMM_Integrator* OpenMM_RuntimeObjects_getIntegrator(OpenMM_RuntimeObjects* ommrt)
{    return ommrt->integrator; }
void openmm_runtimeobjects_getintegrator_(OpenMM_RuntimeObjects*& ommrt, OpenMM_Integrator*& integ)
{    integ = OpenMM_RuntimeObjects_getIntegrator(ommrt); }

void OpenMM_RuntimeObjects_setContext(OpenMM_RuntimeObjects* ommrt, OpenMM_Context* context)
{    OpenMM_Context_destroy(ommrt->context); ommrt->context = context; }
void openmm_runtimeobjects_setcontext_(OpenMM_RuntimeObjects*& ommrt, OpenMM_Context*& context)
{    OpenMM_RuntimeObjects_setContext(ommrt, context); }
OpenMM_Context* OpenMM_RuntimeObjects_getContext(OpenMM_RuntimeObjects* ommrt)
{    return ommrt->context; }
void openmm_runtimeobjects_getcontext_(OpenMM_RuntimeObjects*& ommrt, OpenMM_Context*& context)
{    context = OpenMM_RuntimeObjects_getContext(ommrt); }

} // extern "C"
