#!/usr/bin/env ruby
$LOAD_PATH.unshift(File.join(File.dirname(__FILE__), '..', 'lib'))

require 'imagetools'
require 'chunky_png'

$verbose = false

# lowest values.
tempstart, humstart = 16, 10
# highest values.
tempend, humend = 34, 100
# increment
tempinc, huminc = 1, 10

tempcount, humcount = 1, 1
m = []  
tempcount.times { m << Array.new( humcount ) }  


def imgrow2line(row)
  if (row.length != $PIXEL_PER_LINE)
    raise "Invalid row size in row2line converter: #{row.length}"
  end
  return nil
end


def img2screen(image)
  screen = ScreenFormat.new();

  page = PageFormat.new()
  page[0] = imgrow2line(image.row(0));
  screen[0] = page;
  return screen;
end

dump_flash_config;
# 1. Check and load images.
# TODO: Extend to use the full range of values from above
basedir = File.join(File.dirname(__FILE__), "..", "pics")
flash = FlashFormat.new()
for file in Dir.entries(basedir)
  if file =~ /^img-(\d+)-(\d+).png/ 
    temp, hum = $1, $2
    puts "File #{file}: Temp #{temp}, Hum #{hum}"
    image = nil;
    begin
      image = ChunkyPNG::Image.from_file(File.join(basedir, file));
    rescue
      puts "Failed to read PNG image data from file " << file << ". Exiting."
      exit;
    end
    if (image.dimension.height == $LINES_PER_DISPLAY &&
        image.dimension.width == $PIXEL_PER_LINE)
        screen = img2screen(image);
        flash[0] = screen;
        #TODO: Calculate addresses for lookup table
    else
      puts "Failure: file " << file << " has wrong dimensions.";
      exit
    end

  else 
    puts "Ignoring #{file}" if $verbose
  end
end

File.open("foo", "wb") do |io|
  flash.write(io)
end

#image=ChunkyPNG::Image.from_file("testbild.png");
#image[20,20] = ChunkyPNG::Color.rgb(255,0,0)
#image.save('output.png')
#

