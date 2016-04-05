#version 150

// these are for the programmable pipeline system
uniform mat4 modelViewProjectionMatrix;
in vec4 position;

uniform float uTime;

uniform float uDispAmp;
uniform float uDispRug;
uniform float uDispFreq;

uniform float uHres;
uniform float uWidth;
uniform float uVspacing;

//---------------------------------

//noise funcs
vec4 rand(vec2 A,vec2 B,vec2 C,vec2 D){
    vec2 s=vec2(12.9898,78.233);
    vec4 tmp=vec4(dot(A,s),dot(B,s),dot(C,s),dot(D,s));
    return fract(sin(tmp) * 43758.5453)* 2.0 - 1.0;
}


float noise(vec2 coord,float d){
    vec2 C[4];
    float d1 = 1.0/d;
    C[0]=floor(coord*d)*d1;
    C[1]=C[0]+vec2(d1,0.0);
    C[2]=C[0]+vec2(d1,d1);
    C[3]=C[0]+vec2(0.0,d1);
    vec2 p=fract(coord*d);
    vec2 q=1.0-p;
    vec4 w=vec4(q.x*q.y,p.x*q.y,p.x*p.y,q.x*p.y);
    return dot(vec4(rand(C[0],C[1],C[2],C[3])),w);
}


//main
void main()
{
     vec4 mPos = modelViewProjectionMatrix * position;
    
    //Noise--------------------
/*
    float displacementY =  noise( vec2(uTime + (position.x / uDispRug), uTime+ (position.y / uDispRug)), uDispFreq ) * uDispAmp;
    float displacementX =  noise( vec2(uTime + (position.y / uDispRug), uTime+ (position.x / uDispRug)), uDispFreq ) * uDispAmp;
    
	mPos.y += displacementY;
    mPos.x += displacementX;
   
*/
    
    //Transversal---------------
    
    float iCount = uHres;//con uniform uHres
    float instanceX = float (gl_VertexID % int(iCount)) / iCount; //posicion relativa en X entre 0 y 1
    float velocityX = uTime;
    float timeDependentInstanceXValue = mod(instanceX + velocityX,  1.0);//posx desplazada por el tiempo, entre 0 y 1 con modulo
    
    float instanceY =  floor(gl_VertexID/iCount); //redondea a int para abajo
    
    
    mPos.x += timeDependentInstanceXValue * uWidth; //con uniform uWidth
    mPos.y += instanceY * uVspacing; //con uniform uVspacing
    
	
    
    gl_Position = mPos;
}