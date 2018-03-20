#version 150

//vertex attributes (position, normal, texture location)
in vec4 vPosition;	
in vec2 vTexture; 

out vec2 textureCoord;  //pass through texture location to fragment shader

uniform mat4 model_matrix;  //the model matrix to transpose vertices from object coord to world coord
uniform mat4 proj_matrix;
uniform mat4 cam_matrix;


void main() 
{ 

  	gl_Position = proj_matrix*cam_matrix*model_matrix*vPosition;  //assign the vertex position to the vPosition attribute multiplied by the model matrix
  	
  	textureCoord = vTexture; //pass through texture location to fragment shader

}