#!/usr/bin/env ruby
$LOAD_PATH.unshift(File.join(File.dirname(__FILE__), '..', 'lib'))

require 'imagetools'
require 'optparse'

options = {}
OptionParser.new do |opts|
  opts.banner = "Usage: convert_images.rb [options]"

  opts.on("-v", "--[no-]verbose", "Run verbosely") do |v|
    options[:verbose] = v
  end

  opts.on("-o", "--output [FILENAME]", "flash image file to be produced") do |file|
    options[:file] = file
  end


end.parse!

$verbose=options[:verbose]
outfile = options[:file]
if (outfile == '')
  puts "No output file (-o) specified, aborting.";
  exit(-1);
end

puts "Using file name #{outfile}"
EPD27.dump_flash_config if $verbose;

basedir = File.join(File.dirname(__FILE__), "..", "pics")
converter = IMG::Converter.new()
flash=converter.munch_directory(basedir)
File.open(outfile, "wb") do |io|
  flash.write(io)
end
