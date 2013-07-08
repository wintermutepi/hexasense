#include "buspirate.hpp"
#include <ctype.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

#include <libat45tools/error.hpp>

using namespace at45tools;


BusPirate::BusPirate (const bfs::path devicepath)
  : _devicepath(devicepath) 
{ }

BusPirate::~BusPirate() 
{ }

void BusPirate::bp_open() {
  std::cout << "Attempting to open device " << _devicepath << std::endl;
  struct termios tios;

  _fd= open(_devicepath.string().c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
  if (_fd < 0) {
    throw at45tools::IOException("Cannot open device.");
  }

  if (tcgetattr(_fd, &tios) < 0) {
    throw at45tools::IOException("Failed to get terminal settings of device.");
  }
  if (tcgetattr(_fd, &_saved_tios) < 0) {
    throw at45tools::IOException("Failed to save terminal settings.");
  }

  tios.c_cflag= CS8 | CLOCAL | CREAD;
  tios.c_iflag= IGNPAR | BRKINT;
  tios.c_oflag= 0;
  tios.c_lflag= 0;
  if (cfsetspeed(&tios, B115200) < 0) {
    throw at45tools::IOException("Failed to set device speed.");
  }

  if (tcsetattr(_fd, TCSANOW, &tios) < 0) {
    throw at45tools::IOException("Failed to set terminal configuration on device.");
  }

}

void BusPirate::bp_close() {
  std::cout << "Closing buspirate device." << std::endl;
 if (tcsetattr(_fd, TCSAFLUSH, &_saved_tios) < 0) {
    throw at45tools::IOException("Failed to reset terminal configuration on device.");
  }

  close(_fd);
}


void BusPirate::bp_reset() {
  std::cout << "NOT IMPLEMENTED Resetting buspirate device." << std::endl;
}


uint8_t BusPirate::read_byte() {
}

