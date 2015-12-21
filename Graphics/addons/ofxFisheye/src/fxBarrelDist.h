/*
 Shader based on http://github.prideout.net/barrel-distortion/
 */

#pragma once

#define STRINGIFY(A) #A

#include "ofMain.h"


class fxBarrelDist{
public:
    
    string gl2FragmentShader;
    
    fxBarrelDist(){

        gl2FragmentShader = "#version 120\n"; // For some reason "#version 120\n" makes this break.
        gl2FragmentShader += STRINGIFY(
                                       
               uniform sampler2DRect tex0;
               uniform float width;
               uniform float height;
               
               uniform float amount = 1.5;//barrelPower (0.0-2.0)
               
               const float PI = 3.1415926535;
               
               
               vec2 Distort(vec2 p)
                {
                    float theta  = atan(p.y, p.x);
                    float radius = length(p);
                    radius = pow(radius, amount);
                    p.x = radius * cos(theta);
                    p.y = radius * sin(theta);
                    return 0.5 * (p + 1.0);
                }
                                       
                                       
               void main(){
                   
                   vec2 texCoordNorm = vec2(gl_TexCoord[0].s/width ,gl_TexCoord[0].t/height);
                   
                   vec2 uv;
                   vec2 xy = 2.0 * texCoordNorm - 1.0;
                   
                   float d = length(xy);
                   
                   if (d < 1.0){
                       uv = Distort(xy);
                   }else{
                       uv = texCoordNorm;
                   }
                   
                   vec2 texCoordRescaled= vec2(uv.x * width, uv.y*height);
                   vec4 color = texture2DRect(tex0, texCoordRescaled);
                   gl_FragColor = color;
                   
               }
                
            );
    }
};
