$LINES_PER_DISPLAY = 176;
$PIXEL_PER_LINE = 264;
$BYTES_PER_LINE = $PIXEL_PER_LINE/8;
$LINES_PER_PAGE = 564/$BYTES_PER_LINE 
$PAGES_PER_SCREEN = $LINES_PER_DISPLAY/$LINES_PER_PAGE

def dump_flash_config
  puts "Current EPD/Flash configuration:"
  puts "Lines per Display: #{$LINES_PER_DISPLAY}"
  puts "Bytes per Line: #{$BYTES_PER_LINE}"
  puts "Lines per Page: #{$LINES_PER_PAGE}"
  puts "Pages per Screen: #{$PAGES_PER_SCREEN}"
  puts "This gives us space for #{4096/$PAGES_PER_SCREEN} screens."
end
