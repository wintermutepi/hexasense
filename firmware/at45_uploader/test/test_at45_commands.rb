$LOAD_PATH.unshift(File.join(File.dirname(__FILE__), '..', 'ruby-buspirate', 'lib'))
$LOAD_PATH.unshift(File.join(File.dirname(__FILE__), '..', 'lib'))

require "test/unit"
require 'buspirate'
require 'pp'
require 'at45tools'

DEFAULT_BAUDRATE = 115200
DEFAULT_DATABITS = 8
DEFAULT_STOPBITS = 1
DEFAULT_PARITY = SerialPort::NONE
DEFAULT_DEVICE = "/dev/ttyUSB0"

$bp = BusPirate.new(DEFAULT_DEVICE, DEFAULT_BAUDRATE, 
                    DEFAULT_DATABITS, DEFAULT_STOPBITS, DEFAULT_PARITY)
$at45 = AT45.new($bp, AT45::DB161D::PAGECOUNT, AT45::DB161D::PAGESIZE)

class TestAT45Commands < Test::Unit::TestCase
  def setup
    #sleep(1)
  end

  def teardown
  end

  def test_chipversion  
    chip_version = $at45.get_version()
    assert_equal(chip_version[0], 0x1f, "Non-Atmel chip");
    assert_equal(chip_version[1], 0x26, "Density is not 16Mbit");
  end

  def test_ready
    assert($at45.ready?, "Chip not ready.")
  end

  def test_buffer1
    zerobuf = [].fill(0x00, 0..AT45::DB161D::PAGESIZE-1);
    writebuf = [];
    (0..AT45::DB161D::PAGESIZE-1).each do |i|
      writebuf << (i.modulo(0xFF)).to_i;
    end
    #puts "Zerobuf: #{zerobuf.join(" ")}, element class #{zerobuf[0].class}, length #{zerobuf.length}"
    #puts "Writebuf: #{writebuf.join(" ")} , element class #{writebuf[0].class}, length #{writebuf.length}"
    $at45.write_to_buf1(zerobuf);
    $at45.write_to_buf1(writebuf);
    $at45.wait_for_ready();
    readbuf = $at45.read_from_buf1();
    #puts "Readbuf: #{readbuf.join(" ")}, element class #{readbuf[0].class}, length #{readbuf.length}"
    assert_equal(readbuf, writebuf, "buffer1 not written correctly .");
  end

  def test_buffer1_to_mm
    zerobuf = [].fill(0x00, 0..AT45::DB161D::PAGESIZE-1);
    writebuf = [];
    (0..AT45::DB161D::PAGESIZE-1).each do |i|
      writebuf << (i.modulo(0xff));
    end
    $at45.write_to_buf1(writebuf);
    readbuf = $at45.read_from_buf1();
    assert_equal(readbuf, writebuf, "buffer1 contained defective data.");
    # erase page - otherwise, the memory is not fully written.
    $at45.erase_page(23) 
    $at45.wait_for_ready();
    $at45.buf1_to_mm(23);
    $at45.wait_for_ready();
    # Zeroing buffer 1
    $at45.write_to_buf1(zerobuf);
    $at45.wait_for_ready();
    readbuf = $at45.read_from_buf1();
    assert_equal(readbuf, zerobuf, "buffer1 not cleared correctly.");
    $at45.wait_for_ready();
    $at45.mm_to_buf1(23);
    $at45.wait_for_ready();
    readbuf = $at45.read_from_buf1();
    #puts "Writebuf: #{writebuf.join(" ")} , element class #{writebuf[0].class}"
    #puts "Readbuf: #{readbuf.join(" ")}, element class #{readbuf[0].class}"
    assert_equal(readbuf, writebuf, "Read from MM: Data differs.");
  end

end

