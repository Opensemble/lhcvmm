#version 120
#extension GL_ARB_texture_rectangle : enable


void main()
{
    
    vec4 pos = gl_Vertex;
    
    //--------------------------------
	gl_FrontColor =  gl_Color;
    gl_Position = pos;
}
