require "bindata"

class LineFormat < BinData::Array
  # A string is a fixed-length uint8 array, see
  # http://bindata.rubyforge.org/manual.html#strings
  uint8 :pixelbytes, :initial_length => $BYTES_PER_LINE
end

class PageFormat < BinData::Record
  array :lines, :type => LineFormat, :initial_length => $LINES_PER_PAGE
end

class ScreenFormat < BinData::Record
  array :pages, :type => PageFormat, :initial_length => 22
end

class FlashFormat < BinData::Record
  array :screens, :type => ScreenFormat, :initial_length => 1
end
