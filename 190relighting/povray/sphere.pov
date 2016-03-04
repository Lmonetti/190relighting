#include "colors.inc"  
#include "textures.inc"

camera {
    location <0, 2, -3>
    look_at <0, 1, 3>
}

sphere {
    <0, 1, 3> 2
    texture {
        pigment {color Blue}
    }
} 

plane { 
  <0, 1, 0>, -1 //surface normal, displacement
  //alternative: y, -1
  pigment {
    hexagon
    pigment { Jade },
    pigment { White_Marble }
  }
} 

light_source { 
  <-1 * cos(360*clock), 4, 2*sin(360*clock)>
  color White
}