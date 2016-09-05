
#version 120
#extension GL_EXT_geometry_shader4 : enable
#extension GL_ARB_texture_rectangle : enable


uniform float size;
/*
#define NUM_LIGHTS 1

varying out vec3 normal;
varying out vec3 lightDir;
varying out vec3 eyeVec;
varying out float numLights;
*/

varying out vec3 vN;
varying out vec3 vertPos;


///**************************************************


void main() {
    
    vec3 p0 = gl_PositionIn[0].xyz;
    vec3 p1 = gl_PositionIn[1].xyz;
    
   
    
    
    ///------------------
    vec3 up = vec3(0, 0, 1);	// arbitrary up vector
    
	
	//vec3 dir = normalize(p1 - p0);			// normalized direction vector from p0 to p1
	//vec3 right = normalize(cross(dir, up));	// right vector
    
    vec3 p1n = normalize((gl_PositionIn[1] - gl_PositionIn[0]).xyz);
    vec3 p2n = normalize((gl_PositionIn[2] - gl_PositionIn[0]).xyz);
    
	vec3 norm = normalize(cross(p1n, p2n));

    ///------------------
    
    const vec3 cubeVerts[8] = vec3[8](

                                      vec3(-0.5 , -0.5, 0.5), //LB 0
                                      vec3(-0.5 , 0.5, 0.5), //LT 1
                                      vec3(0.5 , -0.5, 0.5), //RB 2
                                      vec3(0.5 , 0.5, 0.5), //RT 3
                                      //back face
                                      vec3(-0.5 , -0.5, -0.5), //LB 4
                                      vec3(-0.5 , 0.5, -0.5), //LT 5
                                      vec3(0.5 , -0.5, -0.5), //RB 6
                                      vec3(0.5 , 0.5, -0.5)//RT 7
                                      );
    const vec3 normals[6] = vec3[6](
                                    vec3(0.0 , 0.0, 1.0),//front face - 0
                                    vec3(1.0 , 0.0, 0.0),//right face - 1
                                    vec3(-1.0 , 0.0, 0.0),//left face - 2
                                    vec3(0.0 , 0.0, -1.0),//back face - 3
                                    vec3(0.0 , -1.0, 0.0),//bottom face - 4
                                    vec3(0.0 , 1.0, 0.0)//top face - 5
                                    );
    
    const int  cubeIndices[24]  = int [24]
    (0,2,1,3,
     2,6,3,7,
     4,0,5,1,
     6,4,7,5,
     4,6,0,2,
     1,3,5,7);
    
    
    
    
    
    for (int j=0; j<6; j++) {
        
        for (int i=(j*4); i<(j+1)*4; i++)
        {
            gl_FrontColor = gl_FrontColorIn[0];
            
            //numLights = 1.0;
            //normal =   norm;
            //----------
            int iv = cubeIndices[i];
            
            vec3 v = p0 + cubeVerts[iv]*size;
            
            vN = normals[j];
            vertPos = vec3 (gl_ModelViewMatrix * vec4(v, 1.));
            
            gl_Position = gl_ModelViewProjectionMatrix * vec4(v, 1.0);
            ///-///-------
            /*
            vec3 vVertex = vec3(gl_ModelViewMatrix  * vec4(p0 + cubeVerts[iv]*size, 1.0));
            eyeVec = -vVertex.xyz;
            lightDir = vec3(gl_LightSource[0].position.xyz * -1.0   + vVertex.xyz );
            lightDir *= -1.0;
             */
            //------
            EmitVertex();
        }
        EndPrimitive();
        
    }
    
    
    

    
    

  


}