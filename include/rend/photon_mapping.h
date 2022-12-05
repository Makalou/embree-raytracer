//
// Created by 王泽远 on 2022/6/15.
//

#ifndef EMBREE_RAYTRACER_PHOTON_MAPPING_H
#define EMBREE_RAYTRACER_PHOTON_MAPPING_H
#include "core/utility.h"
#include "core/vec3.h"

NAMESPACE_BEGIN(ert)
struct photon{
    vec3 position;
    vec3 power;
    float phi, theta;
    short flag;
};

class photonMap{
public:
    vec3 getPowerAt(vec3 pos){
        return {};
    }
    void add(photon p){

    }
    void build(){
        
    }
private:
    std::vector<photon> m_photonMap;
};
NAMESPACE_END(ert)

#endif //EMBREE_RAYTRACER_PHOTON_MAPPING_H
