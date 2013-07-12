#!/usr/bin/env ruby
$LOAD_PATH.unshift(File.join(File.dirname(__FILE__), '..', 'ruby-buspirate', 'lib'))
$LOAD_PATH.unshift(File.join(File.dirname(__FILE__), '..', 'lib'))


require 'buspirate'
require 'pp'
require 'at45tools'

DEFAULT_BAUDRATE = 115200
DEFAULT_DATABITS = 8
DEFAULT_STOPBITS = 1
DEFAULT_PARITY = SerialPort::NONE
DEFAULT_DEVICE = "/dev/ttyUSB0"


begin
  bp = BusPirate.new(DEFAULT_DEVICE, DEFAULT_BAUDRATE, 
                     DEFAULT_DATABITS, DEFAULT_STOPBITS, DEFAULT_PARITY)
  at45 = AT45.new(bp, AT45::DB161D::PAGECOUNT, AT45::DB161D::PAGESIZE)

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

#  print "Erasing chip \t\t\t"
#  at45.chip_erase_wait() 
#  puts " done"

ensure
  puts "Closing buspirate device."
  bp.close unless bp.nil?
end





