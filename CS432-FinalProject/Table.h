//
//  Table.hpp
//  CS432-FinalProject
//
//  Created by Mariana Quinde Garcia on 3/20/18.
//  Copyright © 2018 Roger White. All rights reserved.
//

#ifndef Table_h
#define Table_h

#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <string>
#include <vector>
#include "Plane.h"
using namespace std;

class Table: public Drawable{
    
    
public:
    Table();
    ~Table();
    void setMaterial(vec4, vec4, vec4, float);
    void draw(Camera*, vector<Light>, vector<bool>);
    
private:
    void buildTable();
    vector<Plane*> tableParts;
    string textureName = "wood.ppm";
    
    
};

#endif /* Table_h */
