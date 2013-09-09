#!/usr/bin/env ruby
$LOAD_PATH.unshift(File.join(File.dirname(__FILE__), '..', 'ruby-buspirate', 'lib'))
$LOAD_PATH.unshift(File.join(File.dirname(__FILE__), '..', 'lib'))


require 'buspirate'
require 'pp'
require 'at45tools'
require 'optparse'
require 'ruby-progressbar'

DEFAULT_BAUDRATE = 115200
DEFAULT_DATABITS = 8
DEFAULT_STOPBITS = 1
DEFAULT_PARITY = SerialPort::NONE
DEFAULT_DEVICE = "/dev/ttyUSB0"


options = {}
OptionParser.new do |opts|
  opts.banner = "Usage: flash_images.rb [options]"

  opts.on("-v", "--[no-]verbose", "Run verbosely") do |v|
    options[:verbose] = v
  end

  opts.on("-s", "--skip-erase", "Skip the initial erase cycle") do |s|
    options[:skip_erase] = s
  end

  opts.on("-n", "--no-verify", "Do not verify the written data") do |s|
    options[:no_verify] = false
  end

  opts.on("-p", "--power", "Enable bus pirate target power") do |p|
    options[:enable_power] = p
  end

  opts.on("-i", "--input [FILENAME]", "flash image file to be uploaded") do |file|
    options[:file] = file
  end

  opts.on("-d", "--serial-device [FILENAME]", "the serial port the buspirate is connected to") do |serial|
    options[:serial_port] = serial
  end

	opts.on("-l", "--limit [PAGE]", Numeric, "number of pages to write", "(implies -s)") do |limit|
		options[:limit] = limit
		options[:skip_erase] = true
	end
end.parse!

$verbose=options[:verbose]
infile = options[:file]
options[:enable_power] ||= false;
options[:no_verify] ||= false;
options[:serial_port] ||= DEFAULT_DEVICE;
if not infile 
  puts "No input file (-i) specified, aborting.";
  exit(-1);
end
# binary-read given file
print "Opening file " << infile << "\t\t"
flash_image = []
File.open(infile, "rb") {|file| 
  file.each_byte{|byte|
    flash_image << byte;
  }
}
puts "done"
# Check whether the image size is plausible
if (flash_image.length.modulo(AT45::DB161D::PAGESIZE) != 0)
  puts "Flash size is odd - aborting."
  exit(-1);
end
num_pages = flash_image.length/AT45::DB161D::PAGESIZE;
print "Image contains #{num_pages} pages.\t"
if (num_pages <= AT45::DB161D::PAGECOUNT) 
  puts "OK"
else
  puts "too many pages - aborting."
  exit;
end

if options[:limit]
	num_pages = options[:limit]
end


begin
  bp = BusPirate.new(options[:serial_port], DEFAULT_BAUDRATE, 
                     DEFAULT_DATABITS, DEFAULT_STOPBITS, 
                     DEFAULT_PARITY)
  bp.reset_console
  at45 = AT45.new(bp, AT45::DB161D::PAGECOUNT, 
                  AT45::DB161D::PAGESIZE, options[:enable_power])

  chip_version = at45.get_version()
  print  "Chip version: \t\t\t%02x:%02x:%02x:%02x - " % chip_version
  if (chip_version[0] != 0x1f) 
    puts "Non-Atmel chip, exiting"
    exit(AT45::EXITCODE::INVALID_TARGET)
  else
    printf "Found ATMEL chip, "
  end 
  if (chip_version[1] != 0x26) 
    puts "Density of chip is not 16M, exiting"
    exit(AT45::EXITCODE::INVALID_TARGET)
  else
    puts "16M dataflash"
  end

  print "Checking ready status \t\t"
  if (at45.ready?)
    puts "We're ready to go!"
  else 
    puts "Not ready - strange. Better exit."
    exit(AT45::EXITCODE::NOT_INITIALIZED)
  end

  print "Erasing chip\t\t\t"
  if not options[:skip_erase]
    at45.chip_erase_wait()
    puts " done."
  else
    puts "Skipping."
  end

  progressbar=ProgressBar.create(:title => "Page", :starting_at => 0, 
                                 :total => num_pages,
                                 :format => '%t %c/%C |%B| %e')
  (0..num_pages-1).each{ |page_idx|
    data = flash_image.slice(page_idx*AT45::DB161D::PAGESIZE, AT45::DB161D::PAGESIZE);
    begin 
      readbuf=at45.upload_page(page_idx, data, :erase => options[:skip_erase], :verify => !options[:no_verify]);
    rescue RuntimeError => e
      puts "Upload of page #{page_idx} failed - #{e}. Aborting.";
      exit
    end
    progressbar.increment
  }

ensure
  puts "Closing buspirate device."
  bp.close unless bp.nil?
end





