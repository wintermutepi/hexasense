include <mcad/units.scad>;
include <mcad/boxes.scad>;
include <config.scad>;

$fn=10;


// Bottom plate
//translate([roundness, roundness,0]) {
//  minkowski() {
//    cube([width-(2*roundness), depth-(2*roundness), bottom_thickness]);
//    cylinder(r=roundness, bottom_thickness);
//  }
//}


// Wall
//translate([roundness, roundness, bottom_thickness]) {
//  difference() {
//    // outer wall
//    minkowski() {
//      // I have no idea why I need a 3 here. It seems to be right,
//      // though.
//      cube(size=[width-(2*roundness), depth-(2*roundness), 3], center=false);
//      cylinder(r=roundness, h=wall_height);
//    }
//    // inner wall
//    translate([wall_thickness, wall_thickness, 0]) {
//      minkowski() {
//        cube([width-(2*roundness)-(2*wall_thickness)
//            , depth-(2*roundness)-(2*wall_thickness)
//            , wall_height+epsilon]);
//        cylinder(r=roundness, h=wall_height+epsilon);
//      }
//    }
//    // Lueftungsschlitze
//    for (vent_offset = [0:(airvent_x_end -
//          airvent_x_start)/20:(airvent_x_end)]) {
//      translate([airvent_x_start+vent_offset, 
//          -10, ((wall_height - airvent_height)/2)]) {
//        cube([2, 20, airvent_height]);
//      }
//    }
//
//  }
//}

translate([width/2, depth/2, height/2]) {
  // hollow box
  difference () {
    translate([0,0,0]) roundedBox([width,depth,height],roundness,false);
    translate([0,0,wall_thickness]) {
      roundedBox([
          (width-(2*wall_thickness)),
          (depth-(2*wall_thickness)),
          (height+epsilon)],
          roundness, false);
    }
  }
  // lower air vents
  


}
