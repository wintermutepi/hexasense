width = 150;
depth = 200;
bottom_thickness = 3;
wall_height = 30;
wall_thickness = 3;
roundness = 2;
cgal_border = 1;    // A border to fix cgal rendering bugs.

$fn=50;

echo("Rendering enclosure.");

// Bottom plate
translate([roundness, roundness,0]) {
  minkowski() {
    cube([width-(2*roundness), depth-(2*roundness), bottom_thickness]);
    cylinder(r=roundness, bottom_thickness);
  }
}

// Wall
translate([roundness, roundness, bottom_thickness]) {
  difference() {
    // outer wall
    minkowski() {
      cube([width-(2*roundness), depth-(2*roundness), wall_height]);
      cylinder(r=roundness, wall_height);
    }
    // inner wall
    translate([wall_thickness, wall_thickness, 0]) {
      minkowski() {
        cube([width-(2*roundness)-(2*wall_thickness)
            , depth-(2*roundness)-(2*wall_thickness)
            , wall_height+cgal_border]);
        cylinder(r=roundness, wall_height+cgal_border);
      }
    }
    // Lueftungsschlitze

  }
}

