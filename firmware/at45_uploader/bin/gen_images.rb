$LOAD_PATH.unshift(File.join(File.dirname(__FILE__), '..', 'lib'))

require 'imagetools'
require 'RMagick'
include Magick
require 'ruby-progressbar'

temprange = (15..30).to_a
humrange = [10,20,30,40,50,60,70,80,90]
status = ["AIR", "WARM", "COLD", "NOPE", "GOOD"]

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

		stat = "GOOD"

		img_temp = Image.read(File.join(basedir, "t#{temp}.png"))[0]
		img_hum = Image.read(File.join(basedir, "h#{hum}.png"))[0]
		img_stat = Image.read(File.join(basedir, "s#{stat}.png"))[0]

    img = Image.new(EPD27::PIXEL_PER_LINE, EPD27::LINES_PER_DISPLAY)
    draw.draw(img)
		img.composite!(img_temp, 0, 0, CompositeOperator::OverCompositeOp)
		img.composite!(img_hum, 0, 0, CompositeOperator::OverCompositeOp)
		img.composite!(img_stat, 0, 0, CompositeOperator::OverCompositeOp)

    filename = "img-#{temp}-#{hum}.png"
    img.write(File.join(basedir, filename))
    progressbar.increment
  }
}
