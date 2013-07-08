require 'at45tools/constants.rb'

class AT45
  def initialize(buspirate) 
    @bp = buspirate;
    configure_buspirate
  end

  def get_version
    retval = nil
    ensure_proper do
      @bp.spi_cs_block(true) do
        retval = @bp.spi_write_then_read(AT45::CMD::GET_VERSION, 4, false)
      end
    end
    return retval;
  end

  def get_status
    retval = 0x0F;
    ensure_proper do
      @bp.spi_cs_block(true) do
        #retval = @bp.spi_write_then_read(AT45::CMD::GET_STATUS, 1, true) 
        retval = @bp.spi_bulk_write_read(AT45::CMD::GET_STATUS) 
        retval = @bp.spi_bulk_write_read([0xFF]) 
      end
    end
    return retval;
  end

  def ready?
    foo = get_status
    #pp "Status: %08b" % foo
    return check_bitmask_set?(foo[0], AT45::STATUS::READY)
  end

  def chip_erase_wait
    retval = []
    ensure_proper do
      @bp.spi_cs_block(true) do
        #pp "Sending %02x%02x%02x%02x" % AT45::CMD::CHIP_ERASE
        #retval = @bp.spi_bulk_write_read(AT45::CMD::CHIP_ERASE)
        retval = @bp.spi_bulk_write_read(AT45::CMD::CHIP_ERASE);
        retval = @bp.spi_bulk_write_read([0x00,0x00,0x00,0x00]);
        #pp "returned %02x%02x%02x%02x" % retval;
      end
    end
    sleep(1)
    while (! ready?) do
      print(".")
      sleep(1)
    end
  end
  

  private
  def check_bitmask_set? (value, mask)
    return ((value & mask) == mask)
  end

  def configure_buspirate
    ensure_proper do 
      @bp.switch_mode(BusPirate::Mode::SPI);
      @bp.reset_console;

      print "entering bitbang mode..\t\t"
      if @bp.enter_bitbang
        puts "done"
      else
        puts "failed"
        exit
      end

      print "entering binary SPI mode...\t"
      if @bp.switch_mode(BusPirate::Mode::SPI)
        puts "done"
      else
        puts "failed"
        exit
      end

      print "setting speed...\t\t"
      if @bp.spi_set_speed(BusPirate::SPI::SPEED_30KHZ)
        puts "done"
      else
        puts "failed"
        exit
      end

      print "setting configuration...\t"
      if @bp.spi_set_config(BusPirate::SPI::PIN_OUTPUT_33V, 
                            BusPirate::SPI::CLOCK_IDLE_LOW, 
                            #BusPirate::SPI::CLOCK_EDGE_IDLE_TO_ACTIVE, 
                            BusPirate::SPI::CLOCK_EDGE_ACTIVE_TO_IDLE, 
                            BusPirate::SPI::SAMPLE_TIME_MIDDLE)
                            puts "done"
      else
        puts "failed"
        exit
      end

      print "configuring peripherals...\t"
      if (@bp.config_peripherals(true, false, true, true) &&
          @bp.configure_pins(BusPirate::PinMode::INPUT, BusPirate::PinMode::OUTPUT,
                             BusPirate::PinMode::OUTPUT,BusPirate::PinMode::INPUT,
                             BusPirate::PinMode::OUTPUT))

        puts "done"
      else
        puts "failed"
        exit
      end

      print "setting CS low...\t\t"
      if @bp.spi_set_cs(true)
        puts "done"
      else
        puts "failed"
        exit
      end
    end
  end

  # Run the provided block. If any exception occurs, properly close the
  # bus pirate device.
  def ensure_proper
    begin
      yield
    rescue => e
      puts "Error: #{e}"
    end
  end

end
