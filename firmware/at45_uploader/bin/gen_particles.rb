$LOAD_PATH.unshift(File.join(File.dirname(__FILE__), '..', 'lib'))

require 'imagetools'
require 'RMagick'
include Magick
require 'ruby-progressbar'

temprange = (15..30).to_a
humrange = [10,20,30,40,50,60,70,80,90]
status = ["AIR", "WARM", "COLD", "NOPE", "GOOD"]

progressbar=ProgressBar.create(:title => "Images", :starting_at => 0, 
                               :total => temprange.length + humrange.length + status.length,
                               :format => '%t %c/%C |%B| %e')

def draw_img(x, y, text, file, gravity)
	basedir = File.join(File.dirname(__FILE__), "..", "pics")

	draw = Magick::Draw.new()
	draw.gravity(gravity)
	draw.stroke('black')
	draw.pointsize(42)
	draw.fill('black')
	draw.text(x, y, text)

	img = Image.new(EPD27::PIXEL_PER_LINE, EPD27::LINES_PER_DISPLAY)
	img.matte_reset!
	draw.draw(img)
	img.write(File.join(basedir, file))
end

temprange.each { |temp|
	draw_img(8, 16, "T:#{temp}", "t#{temp}.png", Magick::NorthWestGravity)
	progressbar.increment
}

humrange.each { |hum|
	draw_img(8, 16, "H:#{hum}", "h#{hum}.png", Magick::SouthWestGravity)
	progressbar.increment
}

status.each { |stat|
	draw_img(16, 0, stat.to_s, "s#{stat}.png", Magick::EastGravity)
	progressbar.increment
}
