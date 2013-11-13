$LOAD_PATH.unshift(File.join(File.dirname(__FILE__), '..', 'lib'))

require 'geometry'
require 'imagetools'
require 'RMagick'
include Magick
require 'ruby-progressbar'

# values below 15 will be clamped to 14 by the firmware, values above 30 will be 
# clamped to 31. these screens should show "<15" and ">30".
temprange = (14..31).to_a
humrange = [10,20,30,40,50,60,70,80,90]

def pt(x, y)
	Geometry::Point.new(x, y)
end

# climate polygons: Y is relative humidity, X is temperature
# values taken from Pels Leusden and H. Freymark. Darstellungen der 
# Raumbehaglichkeit f ̈r den einfachen Praktischen Gebrauch. Der 
# Gesundheitsingenieur, 16(72):23–25, 1951.
# "good" corresponds to Zone I
good_climate = Geometry::Polygon.new([pt(17, 75), pt(21, 65), pt(22, 35), pt(19, 35)])
# "acceptable" corresponds to Zone II
acceptable_climate = Geometry::Polygon.new([pt(16, 75), pt(17, 85), pt(21, 80), pt(25, 60),
																					  pt(27, 30), pt(26, 20), pt(20, 20), pt(17, 35)])
# everything else is Zone III and therefore bad
gc_major = Geometry::Line.new(good_climate.vertices[1], good_climate.vertices[3])
gc_minor = Geometry::Line.new(good_climate.vertices[0], good_climate.vertices[2])

def left_of(line, m)
	a = line.point1
	b = line.point2
	(b.x - a.x) * (m.y - a.y) - (b.y - a.y) * (m.x - a.x) < 0
end

basedir = File.join(File.dirname(__FILE__), "..", "pics")
progressbar=ProgressBar.create(:title => "Images", :starting_at => 0, 
                               :total => temprange.length * humrange.length,
                               :format => '%t %c/%C |%B| %e')

temprange.each {|temp|
  humrange.each {|hum|
		point = pt(temp, hum)
		if good_climate.contains?(point)
			stat = "GOOD"
		elsif acceptable_climate.contains?(point)
			stat = if left_of(gc_major, point)
				if left_of(gc_minor, point) then "WARM" else "AIR" end
			else
				if left_of(gc_minor, point) then "DRY" else "COLD" end
			end
		else
			stat = "NOPE"
		end

		img_temp = Image.read(File.join(basedir, "t#{temp}.png"))[0].transparent('white')
		img_hum = Image.read(File.join(basedir, "h#{hum}.png"))[0].transparent('white')
		img_stat = Image.read(File.join(basedir, "s#{stat}.png"))[0].transparent('white')

    img = Image.new(EPD27::PIXEL_PER_LINE, EPD27::LINES_PER_DISPLAY)
		img.composite!(img_temp, 0, 0, CompositeOperator::OverCompositeOp)
		img.composite!(img_hum, 0, 0, CompositeOperator::OverCompositeOp)
		img.composite!(img_stat, 0, 0, CompositeOperator::OverCompositeOp)
		img.rotate!(180)

    filename = "img-#{temp}-#{hum}.png"
    img.write(File.join(basedir, filename))
    progressbar.increment
  }
}
