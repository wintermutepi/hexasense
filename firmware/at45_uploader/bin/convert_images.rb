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

require 'pp'

def imgrow2line(row)
  line=LineFormat.new()
  #puts "Got row with length #{row.length}: #{row}"
  if (row.length != $PIXEL_PER_LINE)
    raise "Invalid row size in row2line converter: #{row.length}"
  end
  (0..($BYTES_PER_LINE-1)).each{|byte_idx|
    curbyte = 0x00;
    ((byte_idx*8)..((byte_idx+1)*8)-1).each{ |pixel_idx|
      subidx = pixel_idx.modulo(8);
      pixel = row[byte_idx*8 + subidx]
      if pixel == ChunkyPNG::Color::BLACK # black pixel
        curbyte = curbyte | (1 << subidx)
        puts "pixel_idx: #{pixel_idx}, byte_idx: #{byte_idx}, subidx #{subidx}, pixel index: #{byte_idx*8+subidx}, pixel value: #{pixel}, black" if $verbose
      else # white pixel
        curbyte = curbyte & ~(1 << pixel_idx.modulo(8))
        puts "pixel_idx: #{pixel_idx}, byte_idx: #{byte_idx}, subidx #{subidx}, pixel index: #{byte_idx*8+subidx}, pixel value: #{pixel}, white" if $verbose
      end
    }
    line[byte_idx]=curbyte;
  }
  return line
end


def img2screen(image)
  screen = ScreenFormat.new();
  (0..($PAGES_PER_SCREEN-1)).each {|page_idx| 
    page = PageFormat.new()
    (0..($LINES_PER_PAGE-1)).each {|line_idx|
      current_row = page_idx * $LINES_PER_PAGE + line_idx
      if (current_row < $LINES_PER_DISPLAY)
        puts "Page #{page_idx}, line #{line_idx}, on image row #{current_row}"
        page.lines[line_idx] = imgrow2line(image.row(current_row));
      else
        puts "Inserting blank line for padding."
        page.lines[line_idx] = LineFormat.new();
      end
    }
    screen.pages[page_idx] = page;
  }
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
      flash.screens[0] = screen;
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

