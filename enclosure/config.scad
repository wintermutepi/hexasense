include <mcad/units.scad>;
width = 150*mm;
depth = 200*mm;
height = 40*mm;
wall_thickness = 3*mm;
roundness = 2;

bottom_thickness = wall_thickness;
wall_height = height - bottom_thickness;
airvent_height = wall_height * 0.8;
airvent_width = width * 0.8;
airvent_x_start = ((width-airvent_width)/2);
airvent_x_end = width - ((width-airvent_width)/2);


echo("Rendering enclosure:");
echo("- width: ", width);
echo("- depth: ", depth);
echo("- airvent_width: ", airvent_width);
echo("- airvent_x_start: ", airvent_x_start);
echo("- airvent_x_end: ", airvent_x_end);
