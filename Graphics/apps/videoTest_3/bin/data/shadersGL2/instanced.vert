

#version 120
#extension GL_EXT_gpu_shader4 : require

uniform vec4 globalColor = vec4(1.0);

uniform float timeValue=0.0;
uniform float timeValue_b=0.0;

uniform int uMode; // Lineal=0 / Radial_Concentric=1 / Radial_Centrifuge=2
uniform float uDeformRad;
//resolution-spacing
uniform int uHres;
uniform int uWidth;
uniform int uVres;
uniform int uHeight;

//animation
uniform float uTimeNoise = 1.0;

uniform float uXnoiseFreq = 0.01;
uniform float uXnoiseAmp  = 5.0;
uniform float uXnoiseRug  = 1.0;

uniform float uYnoiseFreq = 0.01;
uniform float uYnoiseAmp  = 5.0;
uniform float uYnoiseRug  = 1.0;

uniform float uZnoiseFreq = 0.01;
uniform float uZnoiseAmp  = 2.0;
uniform float uZnoiseRug  = 1.0;
//shading
varying vec3 vN;
varying vec3 v;


//noise funcs---------------------------------------
vec3 mod289(vec3 x) {
	return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec2 mod289(vec2 x) {
	return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x) {
	return mod289(((x*34.0)+1.0)*x);
}

float snoise(vec2 v)
{
	const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
						0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
						-0.577350269189626,  // -1.0 + 2.0 * C.x
						0.024390243902439); // 1.0 / 41.0
											// First corner
	vec2 i  = floor(v + dot(v, C.yy) );
	vec2 x0 = v -   i + dot(i, C.xx);
	
	// Other corners
	vec2 i1;
	//i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
	//i1.y = 1.0 - i1.x;
	i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
	// x0 = x0 - 0.0 + 0.0 * C.xx ;
	// x1 = x0 - i1 + 1.0 * C.xx ;
	// x2 = x0 - 1.0 + 2.0 * C.xx ;
	vec4 x12 = x0.xyxy + C.xxzz;
	x12.xy -= i1;
	
	// Permutations
	i = mod289(i); // Avoid truncation effects in permutation
	vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
					 + i.x + vec3(0.0, i1.x, 1.0 ));
	
	vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
	m = m*m ;
	m = m*m ;
	
	// Gradients: 41 points uniformly over a line, mapped onto a diamond.
	// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)
	
	vec3 x = 2.0 * fract(p * C.www) - 1.0;
	vec3 h = abs(x) - 0.5;
	vec3 ox = floor(x + 0.5);
	vec3 a0 = x - ox;
	
	// Normalise gradients implicitly by scaling m
	// Approximation of: m *= inversesqrt( a0*a0 + h*h );
	m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );
	
	// Compute final noise value at P
	vec3 g;
	g.x  = a0.x  * x0.x  + h.x  * x0.y;
	g.yz = a0.yz * x12.xz + h.yz * x12.yw;
	return 130.0 * dot(m, g);
}

//other shader noise funcs
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

void main()
{

    vec4 vPos = gl_Vertex;
    
    //instanced---------------------
	float iCount = uHres;
    
    float instanceX = float (gl_InstanceID % int(iCount))/ iCount;
    
    float velocityX = timeValue;
	float timeDependentInstanceX = mod(instanceX + velocityX,  1.0);
    
    float instanceY =  floor(gl_InstanceID/iCount);
    
    //LINEAL------------------------------------------
    if(uMode==0){
        float vSpacing = uHeight / (uVres-1);
        vPos.x += timeDependentInstanceX* uWidth ;
        vPos.y += instanceY * vSpacing;
    }
    //RADIAL----------------------------------------
    else {

        float radius;
        float angle;
        
        //center the center
        vPos.x += uWidth*.5;
        
        float radialSpacing;
        if(uVres>1)
            radialSpacing = uHeight / (uVres-1);
        else
            radialSpacing = 0.0;
        
        float angularSpacing;
        if(uVres>1)
            angularSpacing = radians(360.0) / (uVres-1);
        else
            angularSpacing = 0.0;
        
        //Concentric-----------------------
        if(uMode == 1){
            angle = timeDependentInstanceX * radians(360.0);//animated
            radius = instanceY * radialSpacing;//fixed
        }
        //Centrifuge con deform-------------------------
        else if (uMode == 2){
            float deformation = timeDependentInstanceX * radians(360.0) * uDeformRad;
            angle  = instanceY * angularSpacing + deformation;
            radius = timeDependentInstanceX * uHeight;
        }
       
        
       
        //set coordinates-------------------------
                                    
        vPos.x += radius * cos (angle);
        vPos.y += radius * sin (angle);
    
    }
    //noise-------------------------------------

    
    float time_nz = timeValue_b * 0.001 *  uTimeNoise;

    float displacementX =  noise( vec2(time_nz + (vPos.y / uXnoiseRug), time_nz+ (vPos.y / uXnoiseRug)), uXnoiseFreq ) * uXnoiseAmp;
    float displacementY =  noise( vec2(time_nz + (vPos.x / uYnoiseRug), time_nz+ (vPos.x / uYnoiseRug)), uYnoiseFreq ) * uYnoiseAmp;
    float displacementZ =  noise( vec2(time_nz + (vPos.y / uZnoiseRug), time_nz+ (vPos.x / uZnoiseRug)), uZnoiseFreq ) * uZnoiseAmp;
    
    vPos.x += displacementX;
    vPos.y += displacementY;
    vPos.z += displacementZ;
   

    
    //output------
    vN = normalize(gl_NormalMatrix * gl_Normal);
    v = vec3(gl_ModelViewMatrix * vPos);
    gl_FrontColor =  gl_Color;
	gl_Position = gl_ModelViewProjectionMatrix * vPos ;
    
}