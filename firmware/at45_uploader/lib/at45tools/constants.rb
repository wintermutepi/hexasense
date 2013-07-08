class AT45

  module CMD
    GET_VERSION       = [0x9f];
    GET_STATUS        = [0xd7];
    CHIP_ERASE        = [0xc7, 0x94, 0x80, 0x9a];
  end

  module STATUS
    READY             = 0b10000000;
    DENSITY_16M       = 0b00101100;
  end

  module EXITCODE
    INVALID_TARGET    = 1;
    NOT_INITIALIZED   = 2;
  end
end
