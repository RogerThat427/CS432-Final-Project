#include "Cube.h"

Cube::Cube() {
	
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

}

Cube::~Cube(){

}

//----------------------------------------------------------------------------
void Cube::makeQuad(int ind1, int ind2, int ind3, int ind4) {
    vec3 N1 = normalize(cross(vertices[ind2]-vertices[ind1],vertices[ind3]-vertices[ind1]));
    vec3 N2 = normalize(cross(vertices[ind4]-vertices[ind3],vertices[ind1]-vertices[ind3]));
    //triangle #1
	vertexLocations[index] = vertices[ind1];
    vertexNormals[index] = N1;
	index++;

    vertexLocations[index] = vertices[ind2];
    vertexNormals[index] = N1;
	index++;

    vertexLocations[index] = vertices[ind3];
    vertexNormals[index] = N1;
	index++;

	//Triangle #2
    vertexLocations[index] = vertices[ind3];
    vertexNormals[index] = N2;
	index++;

    vertexLocations[index] = vertices[ind4];
    vertexNormals[index] = N2;
	index++;

    vertexLocations[index] = vertices[ind1];
    vertexNormals[index] = N2;
	index++;
}

void Cube::setVertices(vec4 v0, vec4 v1, vec4 v2, vec4 v3, vec4 v4, vec4 v5, vec4 v6, vec4 v7){
    vertices[0] = v0;
    vertices[1] = v1;
    vertices[2] = v2;
    vertices[3] = v3;
    vertices[4] = v4;
    vertices[5] = v5;
    vertices[6] = v6;
    vertices[7] = v7;
    
    buildCube();
    program = InitShader("vshader01_v150.glsl", "fshader01_v150.glsl");
    glUseProgram(program);
    vpos = glGetAttribLocation(program, "vPosition");
    npos = glGetAttribLocation(program, "vNormal");
    mmpos = glGetUniformLocation(program, "model_matrix");
    cmpos = glGetUniformLocation(program, "cam_matrix");
    pmpos = glGetUniformLocation(program, "proj_matrix");
    diffuse_loc = glGetUniformLocation(program, "matDiffuse");
    spec_loc = glGetUniformLocation(program, "matSpecular");
    ambient_loc = glGetUniformLocation(program, "matAmbient");
    alpha_loc = glGetUniformLocation(program, "matAlpha");
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexLocations)+sizeof(vertexNormals), NULL,
                 GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexLocations), vertexLocations);
    glBufferSubData(GL_ARRAY_BUFFER, 0+sizeof(vertexLocations), sizeof(vertexNormals),
                    vertexNormals);
    glEnableVertexAttribArray(vpos);
    glVertexAttribPointer(vpos, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(npos);
    glVertexAttribPointer(npos, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertexLocations)));
}

void Cube::buildCube() {
	index = 0;

	makeQuad(1, 0, 3, 2);  //front
	makeQuad(2, 3, 7, 6);  //right
	makeQuad(3, 0, 4, 7);  //bottom
	makeQuad(6, 5, 1, 2);  //top
	makeQuad(4, 5, 6, 7);  //back
	makeQuad(5, 4, 0, 1);  //left
}

vec3 Cube::getCenter(){
    double xcenter = 1.0 * vertices[0][0];
    double ycenter = 1.0 * vertices[0][1];
    double zcenter = 1.0 * vertices[0][2];
    for(int i = 1; i <8; i++){
        xcenter = xcenter + vertices[i][0];
        ycenter = ycenter + vertices[i][1];
        zcenter = zcenter + vertices[i][2];
    }
    xcenter = xcenter / 8.0;
    ycenter = ycenter / 8.0;
    zcenter = zcenter / 8.0;
    vec3 centers = vec3(xcenter,ycenter,zcenter);
    return centers;
}

void Cube::setMaterial(vec4 a, vec4 d, vec4 s, float sh) {
    diffuse = d;
    specular = s;
    ambient = a;
    shine = sh;
}

void Cube::draw(Camera *cam, vector<Light> lights, vector<bool> enabled) {
    glBindVertexArray(VAO);
    glUseProgram(program);
    glUniformMatrix4fv(mmpos, 1, GL_TRUE, modelmatrix);
    glUniformMatrix4fv(cmpos, 1, GL_TRUE, cam->camera_matrix);
    glUniformMatrix4fv(pmpos, 1, GL_TRUE, cam->projection_matrix);
    glUniform4fv(diffuse_loc, 1, diffuse);
    glUniform4fv(spec_loc, 1, specular);
    glUniform4fv(ambient_loc, 1, ambient);
    glUniform1f(alpha_loc, shine);
    GLuint enabled_loc = glGetUniformLocation(program, "enabled1");
    if(enabled[0]){
        glUniform1i(enabled_loc, GLuint(1));
        GLuint light_loc = glGetUniformLocation(program, "lightPos1");
        glUniform4fv(light_loc, 1, lights[0].getPosition());
        GLuint ambient_loc2 = glGetUniformLocation(program, "lightAmbient1");
        glUniform4fv(ambient_loc2, 1, lights[0].getAmbient());
        GLuint diffuse_loc2 = glGetUniformLocation(program, "lightDiffuse1");
        glUniform4fv(diffuse_loc2, 1, lights[0].getDiffuse());
        GLuint specular_loc2 = glGetUniformLocation(program, "lightSpecular1");
        glUniform4fv(specular_loc2, 1, lights[0].getSpecular());
    } else {
        glUniform1i(enabled_loc, GLuint(0));
    }
    enabled_loc = glGetUniformLocation(program, "enabled2");
    if(enabled[1]){
        glUniform1i(enabled_loc, GLuint(1));
        GLuint light_loc = glGetUniformLocation(program, "lightPos2");
        glUniform4fv(light_loc, 1, lights[1].getPosition());
        GLuint ambient_loc2 = glGetUniformLocation(program, "lightAmbient2");
        glUniform4fv(ambient_loc2, 1, lights[1].getAmbient());
        GLuint diffuse_loc2 = glGetUniformLocation(program, "lightDiffuse2");
        glUniform4fv(diffuse_loc2, 1, lights[1].getDiffuse());
        GLuint specular_loc2 = glGetUniformLocation(program, "lightSpecular2");
        glUniform4fv(specular_loc2, 1, lights[1].getSpecular());
    } else {
        glUniform1i(enabled_loc, GLuint(0));
    }
    glDrawArrays(GL_TRIANGLES, 0,numVertices);
    
}
