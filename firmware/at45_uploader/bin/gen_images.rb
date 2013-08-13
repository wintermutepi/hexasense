$LOAD_PATH.unshift(File.join(File.dirname(__FILE__), '..', 'lib'))

require 'imagetools'
require 'RMagick'
include Magick
require 'ruby-progressbar'

temprange = (15..30).to_a
humrange = [10,20,30,40,50,60,70,80,90]

basedir = File.join(File.dirname(__FILE__), "..", "pics")
progressbar=ProgressBar.create(:title => "Images", :starting_at => 0, 
                               :total => temprange.length * humrange.length,
                               :format => '%t %c/%C |%B| %e')
temprange.each {|temp|
  humrange.each {|hum|
    draw = Magick::Draw.new()
    draw.gravity(Magick::CenterGravity)
    draw.fill('white');
    draw.stroke('black');
    draw.rectangle(3,3,EPD27::PIXEL_PER_LINE - 4,
                   EPD27::LINES_PER_DISPLAY - 4);

    draw.pointsize(42)
    draw.fill('black');
    draw.text(0, 0, "#{temp} -  #{hum}")
    img = Image.new(EPD27::PIXEL_PER_LINE, EPD27::LINES_PER_DISPLAY)
    draw.draw(img)
    filename = "img-#{temp}-#{hum}.png"
    img.write(File.join(basedir, filename))
    progressbar.increment
  }
}
