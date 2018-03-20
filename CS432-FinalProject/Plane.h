//
//  Plane.hpp
//  CS432-FinalProject
//
//  Created by Mariana Quinde Garcia on 3/20/18.
//  Copyright © 2018 Roger White. All rights reserved.
//

#ifndef Plane_h
#define Plane_h

#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <string>
#include <vector>

using namespace std;

class Plane: public Drawable{
    
    
public:
    
    Plane(vec4 vertices[4], string textureName);
    ~Plane();
    
    void setMaterial(vec4, vec4, vec4, float);
    void draw(Camera*, vector<Light>, vector<bool>);
    
private:
    GLuint texture;
    void build(vec4 vertices[6]);
    vec4 vertexLocations[6];
    vec2 vertexTextureLocations[6];
    
    void makeQuad(vec4, vec4, vec4, vec4);
    
    unsigned int index;
    
    //uniform transformation matrix variable locations
    GLuint mmLoc, cmLoc, pmLoc;
    //uniform texture location
    GLuint tex_loc;
};
#endif

