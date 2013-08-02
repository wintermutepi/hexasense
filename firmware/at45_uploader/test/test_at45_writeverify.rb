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
DEFAULT_DEVICE = "/dev/ttyUSB1"

$bp = BusPirate.new(DEFAULT_DEVICE, DEFAULT_BAUDRATE, 
                    DEFAULT_DATABITS, DEFAULT_STOPBITS, DEFAULT_PARITY)
$at45 = AT45.new($bp, AT45::DB161D::PAGECOUNT, AT45::DB161D::PAGESIZE)

class TestAT45WriteVerify < Test::Unit::TestCase
  def setup
    @blankpage = [].fill(0x00, 0..AT45::DB161D::PAGESIZE-1);
  end

  def teardown
  end

  def test_write_onepage
    writebuf = readbuf = blankpage = [];
    (0..AT45::DB161D::PAGESIZE-1).each do |i|
      writebuf << (i.modulo(0x8e)).to_i;
    end
    assert_nothing_raised() {
      readbuf=$at45.upload_page(0, writebuf, :verify => true);
    }
    $at45.write_to_buf1(@blankpage);
    readbuf = $at45.read_from_buf1();
    assert_equal(readbuf, @blankpage, "Buffer 1 not cleared correctly.");
    $at45.mm_to_buf1(0);
    $at45.wait_for_ready();
    readbuf = $at45.read_from_buf1();
    assert_equal(readbuf, writebuf, "Page 0 not written correctly.");
  end

  def test_write_one_randompage
    (0..0).each do |page_idx|
      writebuf = readbuf = blankpage = [];
      (0..AT45::DB161D::PAGESIZE-1).each do |i|
        writebuf << (rand(256)).to_i;
      end
      assert_nothing_raised() {
        readbuf=$at45.upload_page(page_idx, writebuf, :verify => true);
      }
      $at45.write_to_buf1(@blankpage);
      readbuf = $at45.read_from_buf1();
      assert_equal(readbuf, @blankpage, "Buffer 1 not cleared correctly.");
      $at45.mm_to_buf1(page_idx);
      $at45.wait_for_ready();
      readbuf = $at45.read_from_buf1();
      assert_equal(readbuf, writebuf, "Page 0 not written correctly.");
    end
  end

  def test_write_four_randompage
    (0..4).each do |page_idx|
      writebuf = readbuf = blankpage = [];
      (0..AT45::DB161D::PAGESIZE-1).each do |i|
        writebuf << (rand(256)).to_i;
      end
      assert_nothing_raised() {
        readbuf=$at45.upload_page(page_idx, writebuf, :verify => true);
      }
      $at45.write_to_buf1(@blankpage);
      readbuf = $at45.read_from_buf1();
      assert_equal(readbuf, @blankpage, "Buffer 1 not cleared correctly.");
      $at45.mm_to_buf1(page_idx);
      $at45.wait_for_ready();
      readbuf = $at45.read_from_buf1();
      assert_equal(readbuf, writebuf, "Page 0 not written correctly.");
    end
  end

  def test_write_pagenumbers
    maxpage_idx = 70;
    (0..maxpage_idx).each do |page_idx|
      writebuf = [];
      (0..AT45::DB161D::PAGESIZE-1).each do |i|
        writebuf << (page_idx).to_i;
      end
      assert_nothing_raised() {
        readbuf=$at45.upload_page(page_idx, writebuf, :verify => true);
      }
    end
    (0..maxpage_idx).each do |page_idx|
      writebuf = readbuf = blankpage = [];
      (0..AT45::DB161D::PAGESIZE-1).each do |i|
        writebuf << (page_idx).to_i;
      end
      $at45.write_to_buf1(@blankpage);
      readbuf = $at45.read_from_buf1();
      assert_equal(readbuf, @blankpage, "Buffer 1 not cleared correctly.");
      $at45.mm_to_buf1(page_idx);
      $at45.wait_for_ready();
      readbuf = $at45.read_from_buf1();
      assert_equal(readbuf, writebuf, "Page #{page_idx} not stored correctly.");
    end
  end



  def test_write_offset_hypothesis
    (0..5).each do |page_idx|
      writebuf = readbuf = blankpage = [];
      (0..AT45::DB161D::PAGESIZE-1).each do |i|
        writebuf << (page_idx).to_i;
      end
      assert_nothing_raised() {
        readbuf=$at45.upload_page(page_idx, writebuf, :verify => true);
      }
      $at45.write_to_buf1(@blankpage);
      readbuf = $at45.read_from_buf1();
      assert_equal(readbuf, @blankpage, "Buffer 1 not cleared correctly.");
      $at45.mm_to_buf1(page_idx);
      $at45.wait_for_ready();
      readbuf = $at45.read_from_buf1();
      assert_equal(readbuf, writebuf, "Page #{page_idx} not written correctly.");
    end
  end

  def test_write_no_manual_verify
    (0..5).each do |page_idx|
      writebuf = readbuf = blankpage = [];
      (0..AT45::DB161D::PAGESIZE-1).each do |i|
        writebuf << (page_idx).to_i;
      end
      assert_nothing_raised() {
        readbuf=$at45.upload_page(page_idx, writebuf, :verify => true);
      }
      assert_equal(readbuf, writebuf, "Page #{page_idx} not written correctly.");
    end
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
      $at45.write_to_buf1(@blankpage);
      readbuf = $at45.read_from_buf1();
      assert_equal(readbuf, @blankpage, "Buffer 1 not cleared correctly.");
      $at45.mm_to_buf1(page);
      $at45.wait_for_ready();
      readbuf = $at45.read_from_buf1();
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

  # def test_write_allpages
  #   puts "\nErasing chip."
  #   $at45.chip_erase_wait()
  #   puts "\nFilling all pages - this can take a while."
  #   writebuf = readbuf = [];
  #   (0..AT45::DB161D::PAGESIZE-1).each do |i|
  #     writebuf << (i.modulo(0x8e)).to_i;
  #   end
  #   progressbar=ProgressBar.create(:title => "Page", :starting_at => 0, 
  #                                  :total => AT45::DB161D::PAGECOUNT,
  #                                  :format => '%t %c/%C |%B| %e')
  #   (0..AT45::DB161D::PAGECOUNT-1).each{ |page|
  #     assert_nothing_raised() {
  #       readbuf=$at45.upload_page(page, writebuf, :erase => false, :verify => false);
  #     }
  #     assert_equal(readbuf, writebuf, "Page #{page} not written correctly.");
  #     progressbar.increment
  #   }
  # end

end
