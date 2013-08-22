require 'at45tools/constants'

class EPD27

  LINES_PER_DISPLAY = 176;
  PIXEL_PER_LINE = 264;
  BYTES_PER_LINE = PIXEL_PER_LINE/8;
  LINES_PER_PAGE = (AT45::DB161D::PAGESIZE/BYTES_PER_LINE.to_f).ceil.to_i 
  PAGES_PER_SCREEN = (LINES_PER_DISPLAY/LINES_PER_PAGE.to_f).ceil.to_i

  def self.dump_flash_config
    puts "Current EPD/Flash configuration:"
    puts "Lines per Display: #{LINES_PER_DISPLAY}"
    puts "Bytes per Line: #{BYTES_PER_LINE}"
    puts "Lines per Page: #{LINES_PER_PAGE}"
    puts "Pages per Screen: #{PAGES_PER_SCREEN}"
    puts "This gives us space for #{4096/PAGES_PER_SCREEN} screens."
  end

end
