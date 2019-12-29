#include "colors.inc"   
#include "stones.inc"        
#include "textures.inc"//Ԥ����ĳ����?��  
#include "shapes.inc"
#include "glass.inc"
#include "metals.inc"
#include "woods.inc"

background { color Cyan }  

camera {
  location <0, 2, -3>
  look_at  <-3, -10,  30> 
  rotate y*240
}          

sky_sphere {
  pigment {
    gradient y
      color_map {
        [ 0.5  color CornflowerBlue ]
        [ 1.0  color MidnightBlue ]
        }
    scale 1
    translate -2
    }
  emission rgb <0.8,0.8,1>
}      

//���ϲ�     
box{
   <-0.6,0.5,0.3>,
   <0.6,0.7,-0.3>
   
   texture{ 
        T_Stone25     // Pre-defined from stones.inc
        scale 1       // Scale by the same amount in all  
   }   
   
}   

//���в�
box{
   <-0.6,0.3,0.9>,
   <0.6,0.5,-0.9>
   texture{
        T_Stone25     // Pre-defined from stones.inc
        scale 1       // Scale by the same amount in all   
   }
}

 
//��������
cylinder{
    <-0.6,0.5,0.3>,
    <0.6,0.5,0.3>,    
    0.2           
    texture {T_Stone25     // Pre-defined from stones.inc
        scale 1       // Scale by the same amount in all
        }

}
//���沿��
cylinder{
    <-0.6,0.5,-0.3>,
    <0.6,0.5,-0.3>,    
    0.2         
    texture {T_Stone25     // Pre-defined from stones.inc
        scale 1       // Scale by the same amount in all
        }

}     

//chechuang
box{
   <-0.6001,0.5,-0.3>,
   <0.6001,0.65,0.3>
   
   texture{pigment{color White}}
}



//�ĸ���̥    
cylinder{
  <0.55,0.3,0.7>,
  <0.45,0.3,0.7>,    
  0.15           
  texture {
    material_map {
      png "povmap.png"
        texture {  //used with index 0
          pigment {color red 0.3 green 0.1 blue 1}
          normal  {ripples 0.85 frequency 10 }
          finish  {specular 0.75}
          //scale 5
          }
        texture {  //used with index 1
          pigment {White}
          finish {
          ambient 0 diffuse 0 
          reflection 0.9 specular 0.75
          }
        }
        // used with index 2
        texture {pigment{NeonPink} finish{Luminous}}
        texture {  //used with index 3
          pigment {
            gradient y
            color_map {
              [0.00 rgb < 1 , 0 , 0>]
              [0.33 rgb < 0 , 0 , 1>]
              [0.66 rgb < 0 , 1 , 0>]
              [1.00 rgb < 1 , 0 , 0>]
              }
            }
          finish{specular 0.75}
          //scale 8
        }
      }
      //scale 30
      //translate <-15, -15, 0>
  }
}
 
cylinder{
    <0.55,0.3,-0.7>,
    <0.45,0.3,-0.7>,    
    0.15           
  texture {
    material_map {
      png "povmap.png"
        texture {  //used with index 0
          pigment {color red 0.3 green 0.1 blue 1}
          normal  {ripples 0.85 frequency 10 }
          finish  {specular 0.75}
          //scale 5
          }
        texture {  //used with index 1
          pigment {White}
          finish {
          ambient 0 diffuse 0 
          reflection 0.9 specular 0.75
          }
        }
        // used with index 2
        texture {pigment{NeonPink} finish{Luminous}}
        texture {  //used with index 3
          pigment {
            gradient y
            color_map {
              [0.00 rgb < 1 , 0 , 0>]
              [0.33 rgb < 0 , 0 , 1>]
              [0.66 rgb < 0 , 1 , 0>]
              [1.00 rgb < 1 , 0 , 0>]
              }
            }
          finish{specular 0.75}
          //scale 8
        }
      }
      //scale 30
      //translate <-15, -15, 0>
  }
}

cylinder{
    <-0.55,0.3,0.7>,
    <-0.45,0.3,0.7>,    
    0.15           
  texture {
    material_map {
      png "povmap.png"
        texture {  //used with index 0
          pigment {color red 0.3 green 0.1 blue 1}
          normal  {ripples 0.85 frequency 10 }
          finish  {specular 0.75}
          //scale 5
          }
        texture {  //used with index 1
          pigment {White}
          finish {
          ambient 0 diffuse 0 
          reflection 0.9 specular 0.75
          }
        }
        // used with index 2
        texture {pigment{NeonPink} finish{Luminous}}
        texture {  //used with index 3
          pigment {
            gradient y
            color_map {
              [0.00 rgb < 1 , 0 , 0>]
              [0.33 rgb < 0 , 0 , 1>]
              [0.66 rgb < 0 , 1 , 0>]
              [1.00 rgb < 1 , 0 , 0>]
              }
            }
          finish{specular 0.75}
          //scale 8
        }
      }
      //scale 30
      //translate <-15, -15, 0>
  }
}

cylinder{
    <-0.55,0.3,-0.7>,
    <-0.45,0.3,-0.7>,    
    0.15           
  texture {
    material_map {
      png "povmap.png"
        texture {  //used with index 0
          pigment {color red 0.3 green 0.1 blue 1}
          normal  {ripples 0.85 frequency 10 }
          finish  {specular 0.75}
          //scale 5
          }
        texture {  //used with index 1
          pigment {White}
          finish {
          ambient 0 diffuse 0 
          reflection 0.9 specular 0.75
          }
        }
        // used with index 2
        texture {pigment{NeonPink} finish{Luminous}}
        texture {  //used with index 3
          pigment {
            gradient y
            color_map {
              [0.00 rgb < 1 , 0 , 0>]
              [0.33 rgb < 0 , 0 , 1>]
              [0.66 rgb < 0 , 1 , 0>]
              [1.00 rgb < 1 , 0 , 0>]
              }
            }
          finish{specular 0.75}
          //scale 8
        }
      }
      //scale 30
      //translate <-15, -15, 0>
  }
}


plane { 
  <0, 1, 0>, 0.1
  pigment {                   //ľ������ģ��
        wood                    //ȷ����ɫģʽ��������ľ��
        color_map {             //ȷ����ɫ������������������?ֵ
          [0.0 color DarkTan]   //��DarkTan��DarkBrownռ�������?90%��
          [0.9 color DarkBrown] //����DarkBrown��VeryDarkBrownռ����10%
          [1.0 color VeryDarkBrown]
        }
        turbulence 0.07         //����ģʽ����������,����ǿ��
        scale <0.25,0.1,1>      //����ģʽ�Ĵ�С,�˴�ֵԽС����Խ�ܼ�
    } 
}    

light_source {
    <2, 10, -3>
    color White
    spotlight
    radius 15
    falloff 18
    tightness 10
    area_light <1, 0, 0>, <0, 0, 1>, 2, 2
    adaptive 1
    jitter
    point_at <0, 0, 0>
}

