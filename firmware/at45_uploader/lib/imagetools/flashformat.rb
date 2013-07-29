require "bindata"
require 'at45tools'
require 'imagetools'

module IMG

class LineFormat < BinData::Array
  uint8 :pixelbytes, :initial_length => EPD27::BYTES_PER_LINE
end

class PageFormat < BinData::Record
  array :lines, :type => LineFormat, :initial_length => EPD27::LINES_PER_PAGE
end

class ScreenFormat < BinData::Record
  array :pages, :type => PageFormat, :initial_length => 22
end

class LookupTableFormat < BinData::Record
  endian :little;
  uint8 :temperature, :initial_value => 0
  uint8 :humidity, :initial_value => 0
  uint16 :startpage, :initial_value => 0
end

LUTLINES_PER_PAGE = (AT45::DB161D::PAGESIZE / LookupTableFormat.new().num_bytes);
#puts " #{AT45::DB161D::PAGESIZE} #{LookupTableFormat.new().num_bytes} #{IMG::LUTLINES_PER_PAGE}"
LUT_PAGES = 4;

class LookupPageFormat < BinData::Record
  array :lines, :type => LookupTableFormat, :initial_length => IMG::LUTLINES_PER_PAGE
end

class FlashFormat < BinData::Record
  # A lookup table line consists of 4 bytes. We need (409*4)/564 = 2.9
  # pages for that - lets say 4 pages.
  array :lut_pages, :type => LookupPageFormat, :initial_length => IMG::LUT_PAGES
  array :screens, :type => ScreenFormat, :initial_length => 1
end

end
