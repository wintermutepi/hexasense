$LOAD_PATH.unshift(File.join(File.dirname(__FILE__), '..', 'lib'))

require 'imagetools'
require 'RMagick'
include Magick
require 'ruby-progressbar'

basedir = File.join(File.dirname(__FILE__), "..", "pics")
progressbar=ProgressBar.create(:title => "Images", :starting_at => 0, 
                               :total => (32-15)*10,
                               :format => '%t %c/%C |%B| %e')
(15..32).each {|temp|
  [10,20,30,40,50,60,70,80,90,100].each {|hum|
    draw = Magick::Draw.new()
    draw.gravity(Magick::CenterGravity)
    draw.pointsize(42)
    draw.text(0, 0, "#{temp} -  #{hum}")
    img = Image.new($PIXEL_PER_LINE, $LINES_PER_DISPLAY)
    draw.draw(img)
    filename = "img-#{temp}-#{hum}.png"
    img.write(File.join(basedir, filename))
    progressbar.increment
  }
}
