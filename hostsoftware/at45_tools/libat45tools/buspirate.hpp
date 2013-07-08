#ifndef LIBAT45TOOLS_BUSPIRATE_HPP
#define LIBAT45TOOLS_BUSPIRATE_HPP 1

#include <libat45tools/common.hpp>
#include <termios.h>
#include <boost/filesystem.hpp>
namespace bfs = boost::filesystem; 

#define BUFFER_SIZE         1024
#define TIMEOUT_MS          5
#define DEFAULT_NUM_RETRIES 3

namespace at45tools {
  class BusPirate {
    public:
      typedef boost::shared_ptr<BusPirate> Ptr;
      BusPirate (const bfs::path devicepath);
      virtual ~BusPirate();

      void bp_open();
      void bp_close();
      void bp_reset();

      uint8_t read_byte();

    private:
      BusPirate (const BusPirate& original);
      BusPirate& operator= (const BusPirate& rhs);
      bfs::path _devicepath;
      int _fd;
      char buffer[BUFFER_SIZE];
      struct termios _saved_tios;


  };
};


#endif /* LIBAT45TOOLS_BUSPIRATE_HPP */

