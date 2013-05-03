$LOAD_PATH.unshift(File.join(File.dirname(__FILE__), '..', 'lib'))

require 'RMagick'
include Magick

example = ImageList.new
[15..30].each {|temp|
  [5,10,15,20,25,30,35,40,45].each {|hum|
    puts "T: #{temp} H: #{hum}"
  }
}
