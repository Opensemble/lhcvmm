/*
 Shader based on https://www.shadertoy.com/view/4s2GRR
 */


#pragma once

#define STRINGIFY(A) #A

#include "ofMain.h"


class fxVariableFisheye{
public:
    
    string gl2FragmentShader;
    
    fxVariableFisheye(){

        gl2FragmentShader = "#version 120\n"; // For some reason "#version 120\n" makes this break.
        gl2FragmentShader += STRINGIFY(
                                       
                   uniform sampler2DRect tex0;
                   uniform float width;
                   uniform float height;
                   
                   uniform float amount = 0.1; // -0.5 a 0.5
                   
                   const float PI = 3.1415926535;
                   
                   void main(){
                       
                       vec2 texCoordNorm = vec2(gl_TexCoord[0].s/width ,gl_TexCoord[0].t/height);
                       vec2 p = texCoordNorm;
                       
                       float prop = 1.0;
                       vec2 m = vec2(0.5,0.5);
                       vec2 d = p - m;
                       float r = length (d);
                       
                       float amnt = clamp(amount, -0.5, 0.5);
                       float power = ( 2.0 * PI / (2.0 * sqrt(dot(m, m))) ) * amnt;//amount of effect
                       
                       float bind;
                       if (power > 0.0){
                           bind = sqrt(dot(m, m));//stick to corners
                       }else {
                           bind = m.y;//stick to borders
                       }
                       
                       
                       vec2 uv;
                       if (power > 0.0){
                           //fisheye
                           uv = m + normalize(d) * tan(r * power) * bind / tan( bind * power);
                       }else if (power < 0.0){
                           //antifisheye
                           uv = m + normalize(d) * atan(r * -power * 10.0) * bind / atan(-power * bind * 10.0);
                       }else{
                           uv = p;//no effect for power = 1.0
                       }
                       
                       vec2 texCoordRescaled= vec2(uv.x * width, uv.y*height);
                       vec4 color = texture2DRect(tex0, texCoordRescaled);
                       gl_FragColor = color;
                       
                   }
                
            );
    }
};
