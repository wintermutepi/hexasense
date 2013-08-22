class AT45

  module CMD
    GET_VERSION       = [0x9f];
    GET_STATUS        = [0xd7];
    CHIP_ERASE        = [0xc7, 0x94, 0x80, 0x9a];
    PAGE_ERASE        = [0x81];
    BUFFER1_READ      = [0xd4];
    BUFFER1_WRITE     = [0x84];
    #BUFFER1_TO_MM     = [0x83]; // erase each page separately.
    BUFFER1_TO_MM     = [0x88]; # do not erase each page separately.
    MM_TO_BUFFER1     = [0x53];
  end

  module STATUS
    READY             = 0b10000000;
    DENSITY_16M       = 0b00101100;
  end

  module DB161D
    PAGECOUNT         = 4096;
    PAGESIZE          = 528;
  end

  module EXITCODE
    INVALID_TARGET    = 1;
    NOT_INITIALIZED   = 2;
  end
end
