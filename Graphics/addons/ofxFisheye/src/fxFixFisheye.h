/*
 Shader based on http://paulbourke.net/dome/fisheye/
 */
#pragma once

#define STRINGIFY(A) #A

#include "ofMain.h"


class fxFixFisheye{
public:
    
    string gl2FragmentShader;
    
    fxFixFisheye(){

        gl2FragmentShader = "#version 120\n"; // For some reason "#version 120\n" makes this break.
        gl2FragmentShader += STRINGIFY(
                                       
                   uniform sampler2DRect tex0;
                   uniform float width;
                   uniform float height;
                   
                   uniform float amount = 178.0;//aperture (0.0 - 180.0)
                   
                   const float PI = 3.1415926535;
                   
                   
                   void main(){
                       
                       vec2 texCoordNorm = vec2(gl_TexCoord[0].s/width ,gl_TexCoord[0].t/height);
                       
                       float apertureHalf = 0.5 * amount * (PI / 180.0);
                       float maxFactor = sin(apertureHalf);
                       
                       vec2 uv;
                       vec2 xy = 2.0 * texCoordNorm - 1.0;//center coords
                       
                       float d = length(xy);
                       
                       if (d < 2.0-maxFactor){
                           d = length(xy * maxFactor);
                           float z = sqrt(1.0 - d * d);
                           float r = atan(d, z) / PI;
                           float phi = atan(xy.y, xy.x);
                           
                           uv.x = r * cos(phi) + 0.5;
                           uv.y = r * sin(phi) + 0.5;
                           
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
