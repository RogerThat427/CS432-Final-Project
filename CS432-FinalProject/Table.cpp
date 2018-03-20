//
//  Table.cpp
//  CS432-FinalProject
//
//  Created by Mariana Quinde Garcia on 3/20/18.
//  Copyright © 2018 Roger White. All rights reserved.
//

#include "Table.h"

Table::Table() {
    buildTable();
    
}

Table::~Table(){
    
}

void Table::buildTable() {
   
    
    vec4 tableTopVertices1[4] = {vec4(0.6,0.3,0.5,1),vec4(0.6,0.3,-0.5,1),vec4(-0.6,0.3,-0.5,1),vec4(-0.6,0.3,0.5,1)};
    tableParts.push_back(new Plane(tableTopVertices1, textureName));
                    
    vec4 tableTopVertices2[4] = {vec4(0.6,0.2,0.5,1),vec4(.6,.2,-.5,1),vec4(-0.6,0.2,-0.5,1),vec4(-0.6,0.2,0.5,1)};
    tableParts.push_back(new Plane(tableTopVertices2, textureName));
    
    vec4 tableTopVertices3[4] = {vec4(0.6,0.3,0.5,1),vec4(-0.6,0.3,0.5,1),vec4(-0.6,0.2,0.5,1),vec4(0.6,0.2,0.5,1)};
    tableParts.push_back(new Plane(tableTopVertices3, textureName));
    
    vec4 tableTopVertices4[4] = {vec4(0.6,0.3,-0.5,1),vec4(-0.6,0.3,-0.5,1),vec4(-0.6,0.2,-0.5,1),vec4(0.6,0.2,-0.5,1)};
    tableParts.push_back(new Plane(tableTopVertices4, textureName));
    
    vec4 tableTopVertices5[4] = {vec4(0.6,0.3,0.5,1),vec4(0.6,0.3,-0.5,1),vec4(0.6,.2,-.5,1),vec4(0.6,0.2,0.5,1)};
    tableParts.push_back(new Plane(tableTopVertices5, textureName));
    
    vec4 tableTopVertices6[4] = {vec4(-0.6,0.3,-0.5,1),vec4(-0.6,0.3,0.5,1),vec4(-0.6,0.2,0.5,1),vec4(-0.6,0.2,-0.5,1)};
    tableParts.push_back(new Plane(tableTopVertices6, textureName));
    
    vec4 legOneVertices1[4] = {vec4(0.5,0.2,0.4,1), vec4(0.5,0.2,0.3,1),vec4(0.5,-0.3,0.3,1),vec4(0.5,-0.3,0.4,1)};
    tableParts.push_back(new Plane(legOneVertices1, textureName));

    vec4 legOneVertices2[4] = {vec4(0.4,0.2,0.3,1), vec4(0.4,0.2,0.4,1), vec4(0.4,-0.3,0.4,1),vec4(0.4,-0.3,0.3,1)};
    tableParts.push_back(new Plane(legOneVertices2, textureName));

    vec4 legOneVertices3[4] = {vec4(0.5,0.2,0.4,1),vec4(0.4,0.2,0.4,1), vec4(0.4,-0.3,0.4,1), vec4(0.5,-0.3,0.4,1)};
    tableParts.push_back(new Plane(legOneVertices3, textureName));

    vec4 legOneVertices4[4] = {vec4(0.5,0.2,0.3,1), vec4(0.4,0.2,0.3,1), vec4(0.4,-0.3,0.3,1), vec4(0.5,-0.3,0.3,1)};
    tableParts.push_back(new Plane(legOneVertices4, textureName));


    
    vec4 legTwoVertices1[4] = {vec4(-0.5,0.2,0.4,1), vec4(-0.5,0.2,0.3,1), vec4(-0.5,-0.3,0.3,1), vec4(-0.5,-0.3,0.4,1)};
    tableParts.push_back(new Plane(legTwoVertices1, textureName));

    vec4 legTwoVertices2[4] = {vec4(-0.4,0.2,0.3,1), vec4(-0.4,0.2,0.4,1), vec4(-0.4,-0.3,0.4,1), vec4(-0.4,-0.3,0.3,1)};
    tableParts.push_back(new Plane(legTwoVertices2, textureName));

    vec4 legTwoVertices3[4] = {vec4(-0.5,0.2,0.4,1),vec4(-0.4,0.2,0.4,1), vec4(-0.4,-0.3,0.4,1), vec4(-0.5,-0.3,0.4,1)};
    tableParts.push_back(new Plane(legTwoVertices3, textureName));

    vec4 legTwoVertices4[4] = {vec4(-0.5,0.2,0.3,1), vec4(-0.4,0.2,0.3,1), vec4(-0.4,-0.3,0.3,1),vec4(-0.5,-0.3,0.3,1)};
    tableParts.push_back(new Plane(legTwoVertices4, textureName));

    
    vec4 legThreeVertices1[4] = {vec4(-0.5,0.2,-0.4,1), vec4(-0.5,0.2,-0.3,1), vec4(-0.5,-0.3,-0.3,1), vec4(-0.5,-0.3,-0.4,1)};
    tableParts.push_back(new Plane(legThreeVertices1, textureName));

    vec4 legThreeVertices2[4] = {vec4(-0.4,0.2,-0.3,1), vec4(-0.4,0.2,-0.4,1), vec4(-0.4,-0.3,-0.4,1), vec4(-0.4,-0.3,-0.3,1)};
    tableParts.push_back(new Plane(legThreeVertices2, textureName));

    vec4 legThreeVertices3[4] = {vec4(-0.5,0.2,-0.4,1), vec4(-0.4,0.2,-0.4,1), vec4(-0.4,-0.3,-0.4,1), vec4(-0.5,-0.3,-0.4,1)};
    tableParts.push_back(new Plane(legThreeVertices3, textureName));

    vec4 legThreeVertices4[4] = {vec4(-0.5,0.2,-0.3,1), vec4(-0.4,0.2,-0.3,1), vec4(-0.4,-0.3,-0.3,1), vec4(-0.5,-0.3,-0.3,1)};
    tableParts.push_back(new Plane(legThreeVertices4, textureName));

    
    vec4 legFourVertices1[4] = {vec4(0.5,0.2,-0.4,1), vec4(0.5,0.2,-0.3,1), vec4(0.5,-0.3,-0.3,1), vec4(0.5,-0.3,-0.4,1)};
    tableParts.push_back(new Plane(legFourVertices1, textureName));
    
    vec4 legFourVertices2[4] = {vec4(0.4,0.2,-0.3,1), vec4(0.4,0.2,-0.4,1), vec4(0.4,-0.3,-0.4,1), vec4(0.4,-0.3,-0.3,1)};
    tableParts.push_back(new Plane(legFourVertices2, textureName));
    
    vec4 legFourVertices3[4] = {vec4(0.5,0.2,-0.4,1), vec4(0.4,0.2,-0.4,1), vec4(0.4,-0.3,-0.4,1), vec4(0.5,-0.3,-0.4,1)};
    tableParts.push_back(new Plane(legFourVertices3, textureName));
    
    vec4 legFourVertices4[4] = {vec4(0.5,0.2,-0.3,1), vec4(0.4,0.2,-0.3,1), vec4(0.4,-0.3,-0.3,1), vec4(0.5,-0.3,-0.3,1)};
    tableParts.push_back(new Plane(legFourVertices4, textureName));
    
    
    for(int i = 0; i < tableParts.size(); i++){
        tableParts.at(i)->setModelMatrix(Scale(2, 2, 2)*Translate(0, 0, 0));
    }
}


void Table::setMaterial(vec4 a, vec4 d, vec4 s, float sh) {
}


void Table::draw(Camera *cam, vector<Light> lights, vector<bool> enabled) {
    for(int i = 0; i < tableParts.size(); i++){
        tableParts.at(i)->draw(cam, lights, enabled);
    }
}
