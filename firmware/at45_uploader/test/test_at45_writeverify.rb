$LOAD_PATH.unshift(File.join(File.dirname(__FILE__), '..', 'ruby-buspirate', 'lib'))
$LOAD_PATH.unshift(File.join(File.dirname(__FILE__), '..', 'lib'))

require "test/unit"
require 'buspirate'
require 'pp'
require 'at45tools'
require 'ruby-progressbar'

DEFAULT_BAUDRATE = 115200
DEFAULT_DATABITS = 8
DEFAULT_STOPBITS = 1
DEFAULT_PARITY = SerialPort::NONE
DEFAULT_DEVICE = "/dev/ttyUSB0"

$bp = BusPirate.new(DEFAULT_DEVICE, DEFAULT_BAUDRATE, 
                    DEFAULT_DATABITS, DEFAULT_STOPBITS, DEFAULT_PARITY)
$at45 = AT45.new($bp, AT45::DB161D::PAGECOUNT, AT45::DB161D::PAGESIZE)

class TestAT45WriteVerify < Test::Unit::TestCase
  def setup
    #sleep(1)
  end

  def teardown
  end

  def test_write_onepage
    writebuf = readbuf = [];
    (0..AT45::DB161D::PAGESIZE-1).each do |i|
      writebuf << (i.modulo(0x8e)).to_i;
    end
    assert_nothing_raised() {
      readbuf=$at45.upload_page(42, writebuf, :verify => true);
    }
    assert_equal(readbuf, writebuf, "Page 42 not written correctly.");
  end

  def test_write_cornercases
    writebuf = readbuf = [];
    (0..AT45::DB161D::PAGESIZE-1).each do |i|
      writebuf << (i.modulo(0x8e)).to_i;
    end
    page_idx=[0, AT45::DB161D::PAGECOUNT-1];
    page_idx.each{ |page|
      assert_nothing_raised() {
        readbuf=$at45.upload_page(page, writebuf, :verify => true);
      }
      assert_equal(readbuf, writebuf, "Page #{page} not written correctly.");
    }
    # Page index too large
    assert_raise(ArgumentError) {
      $at45.upload_page(AT45::DB161D::PAGECOUNT, writebuf, :verify => true);
    }
    # Page size invalid
    assert_raise(ArgumentError) {
      $at45.upload_page(0, writebuf << [0x00], :verify => true);
    }
  end

  def test_write_allpages
    puts "\nErasing chip."
    $at45.chip_erase_wait()
    puts "\nFilling all pages - this can take a while."
    writebuf = readbuf = [];
    (0..AT45::DB161D::PAGESIZE-1).each do |i|
      writebuf << (i.modulo(0x8e)).to_i;
    end
    progressbar=ProgressBar.create(:title => "Page", :starting_at => 0, 
                                   :total => AT45::DB161D::PAGECOUNT-1,
                                   :format => '%t %c/%C |%B| %e')
    (0..AT45::DB161D::PAGECOUNT-1).each{ |page|
      assert_nothing_raised() {
        readbuf=$at45.upload_page(page, writebuf, :erase => false, :verify => false);
      }
      assert_equal(readbuf, writebuf, "Page #{page} not written correctly.");
      progressbar.increment
    }
  end

end
