
#version 120
#extension GL_EXT_geometry_shader4 : enable

uniform float thickness;

varying out vec3 vN;
varying out vec3 vertPos;


//---------------------------------
const int  cubeIndices[24]  = int [24](0,2,1,3,
                                       2,6,3,7,
                                       4,0,5,1,
                                       6,4,7,5,
                                       4,6,0,2,
                                       1,3,5,7);
/*
const vec3 normals[6] = vec3[6](
                                vec3(0.0 , 0.0, 1.0),//front face - 0
                                vec3(1.0 , 0.0, 0.0),//right face - 1
                                vec3(-1.0 , 0.0, 0.0),//left face - 2
                                vec3(0.0 , 0.0, -1.0),//back face - 3
                                vec3(0.0 , -1.0, 0.0),//bottom face - 4
                                vec3(0.0 , 1.0, 0.0)//top face - 5
                                );

*/

void main() {
	vec3 p0 = gl_PositionIn[0].xyz;
	vec3 p1 = gl_PositionIn[1].xyz;
	
	vec3 up = vec3(0, 0, 1);	// arbitrary up vector
	
	vec3 dir = normalize(p1 - p0);			// normalized direction vector from p0 to p1
	vec3 ancho = normalize(cross(dir, up));	// ancho vector
	vec3 norm = cross(ancho, dir);
	
    vec3 alto = norm;
    vec3 normalsV[6] = vec3[6](
                               normalize(dir),//front face - 0
                               normalize(ancho),//right face - 1
                               normalize(ancho * -1.),//left face - 2
                               normalize(dir * -1.),//back face - 3
                               normalize(alto * -1.),//bottom face - 4
                               normalize(alto)//top face - 5
                               );
    
	ancho *= thickness;
    alto  *= thickness;
    
    //--------------------------------------------------------------------
     vec3 cubeVerts[8] = vec3[8](
                                 
                                      p0 - ancho - alto, //LB 0
                                      p0 - ancho + alto, //LT 1
                                      p0 + ancho - alto, //RB 2
                                      p0 + ancho + alto, //RT 3
                                      //back face
                                      p1 - ancho - alto, //LB 4
                                      p1 - ancho + alto, //LT 5
                                      p1 + ancho - alto, //RB 6
                                      p1 + ancho + alto//RT 7
                                      );
    
    
    for (int j=0; j<6; j++) {
        
        for (int i=(j*4); i<(j+1)*4; i++)        {
            
            int iv = cubeIndices[i];
            vec3 v = cubeVerts[iv];
            
            if(iv==0||iv==1||iv==2||iv==3){
               gl_FrontColor = gl_FrontColorIn[0] ;
            }else{
                 gl_FrontColor = gl_FrontColorIn[1] ;
            }
            
            //if(allNormals==1)vN = normalsV[j];//tweak!
            //else if(allNormals==0) vN = norm;
            vN = normalsV[j];//tweak!
            
            
            vertPos = vec3 (gl_ModelViewMatrix * vec4(v, 1.));
            
            gl_Position = gl_ModelViewProjectionMatrix * vec4(v, 1.0);
            
            EmitVertex();
        }
        EndPrimitive();
    }
	
}