//
//  Plane.cpp
//  CS432-FinalProject
//
//  Created by Mariana Quinde Garcia on 3/20/18.
//  Copyright © 2018 Roger White. All rights reserved.
//

#include "Plane.h"

Plane::Plane(vec4 vertices[4], string textureName) {
    build(vertices);
    glGenBuffers(1, &VBO);
    
    //move the data onto the buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexLocations)+sizeof(vertexTextureLocations), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexLocations), vertexLocations);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexLocations), sizeof(vertexTextureLocations), vertexTextureLocations);
    
    //set up stuff for the body of the cube
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO); //make this VAO active
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  //associate the VBO with the active VAO
    
    program = InitShader("vshaderTM_v150.glsl", "fshaderTM_v150.glsl");
    glUseProgram(program);
    
    //link the vertex attributes with the buffer
    GLuint vPosition, vTexture;
    
    assert((vPosition = glGetAttribLocation(program, "vPosition"))!=-1);
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    
    assert((vTexture = glGetAttribLocation(program, "vTexture"))!=-1);
    glEnableVertexAttribArray(vTexture);
    glVertexAttribPointer(vTexture, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertexLocations)));
    
    //get the texture data for the quad
    glGenTextures(1, &texture);
    int imgsize = 512;
    char *name = new char[textureName.length() + 1];
    strcpy(name, textureName.c_str());
    GLubyte *data = ppmRead(name, &imgsize, &imgsize);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgsize, imgsize, 0, GL_RGB, GL_UNSIGNED_BYTE, data);  //move the data onto the GPU
    delete[] data;  //dont' need this data now that its on the GPU
    
    //set the texturing parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    //get the locations of the uniform shader variables
    assert((mmLoc = glGetUniformLocation(program, "model_matrix")) != -1);
    assert((cmLoc = glGetUniformLocation(program, "cam_matrix")) != -1);
    assert((pmLoc = glGetUniformLocation(program, "proj_matrix")) != -1);
    assert((tex_loc = glGetUniformLocation(program, "textureID")) != -1);
    
}

Plane::~Plane(){
    glDeleteTextures(1, &texture);
}

void Plane::build(vec4 vertices[4]){
    
    index = 0;
    makeQuad(vertices[0], vertices[1], vertices[2], vertices[3]);
    
}

void Plane::setMaterial(vec4 a, vec4 d, vec4 s, float sh) {
}


void Plane::draw(Camera* cam, vector<Light> lights, vector<bool> enabled){
    glBindVertexArray(VAO);
    glUseProgram(program);
    
    //set uniform variable values
    glUniformMatrix4fv(mmLoc, 1, GL_TRUE, modelmatrix);
    glUniformMatrix4fv(cmLoc, 1, GL_TRUE, cam->camera_matrix);
    glUniformMatrix4fv(pmLoc, 1, GL_TRUE, cam->projection_matrix);
    glUniform1i(tex_loc, 0);
    
    //enable the necessary texturing
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    //draw
    glDrawArrays( GL_TRIANGLES, 0, 6);
    
    
}


void Plane::makeQuad(vec4 a, vec4 b, vec4 c, vec4 d){
    //Triangle #1
    vertexLocations[index] = a;
    vertexTextureLocations[index] = vec2(0,0);
    index++;
    
    vertexLocations[index] = b;
    vertexTextureLocations[index] = vec2(1,0);
    index++;
    
    vertexLocations[index] = c;
    vertexTextureLocations[index] = vec2(1,1);
    index++;
    
    //Triangle #2
    vertexLocations[index] = c;
    vertexTextureLocations[index] = vec2(1,1);
    index++;
    
    vertexLocations[index] = d;
    vertexTextureLocations[index] = vec2(0,1);
    index++;
    
    vertexLocations[index] = a;
    vertexTextureLocations[index] = vec2(0,0);
    index++;
    
}



