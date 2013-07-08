#include <libat45tools/common.hpp>
#include <libat45tools/buspirate.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/positional_options.hpp>
namespace po = boost::program_options;
#include <boost/filesystem.hpp>
namespace bfs = boost::filesystem; 
#include <iostream>
#include <signal.h>
#include <unistd.h>

#include <buspirate.h>
#include <spi.h>


static void signal_handler(int signum) {
  fprintf(stdout, "Ctrl-C received\n");
  fflush(stdout);
  exit(10);
}

//void show_pins_state(unsigned char value, int power, int pullup)
//{
//  printf("  Pin CS  : %s\n", (value & BP_PIN_CS)?"ON":"OFF");
//  printf("  Pin MISO: %s\n", (value & BP_PIN_MISO)?"ON":"OFF");
//  printf("  Pin CLK : %s\n", (value & BP_PIN_CLK)?"ON":"OFF");
//  printf("  Pin MOSI: %s\n", (value & BP_PIN_MOSI)?"ON":"OFF");
//  printf("  Pin AUX : %s\n", (value & BP_PIN_AUX)?"ON":"OFF");
//  if (pullup)
//    printf("  Pullup  : %s\n", (value & BP_PIN_PULLUP)?"ON":"OFF");
//  if (power)
//    printf("  Power   : %s\n", (value & BP_PIN_POWER)?"ON":"OFF");
//}
//
//void show_spi_speed(unsigned char speed)
//{
//  std::cout << "RAW SPEED: " << (uint16_t) speed << std::endl;
//  assert(speed <= 7);
//  const char* speeds[8]= {"30kHz", "125kHz", "250kHz", "1MHz",
//			  "2MHz", "2.6MHz", "4MHz", "8MHz"};
//  printf("  Speed: %s\n", speeds[speed]);
//}
//
//void show_spi_config(unsigned char config)
//{
//  assert(config <= 15);
//  printf("  Output level   : %s\n",
//	 (config & BP_BIN_SPI_LV_3V3?"3V3":"HiZ"));
//  printf("  Clock idle     : %s\n",
//	 (config & BP_BIN_SPI_CLK_IDLE_HIGH?"high":"low"));
//  printf("  Clock edge     : %s\n",
//	 (config & BP_BIN_SPI_CLK_EDGE_HIGH?"idle->active":"active->idle"));
//  printf("  Sample position: %s\n",
//	 (config & BP_BIN_SPI_SMP_END?"end":"middle"));
//}
//
//uint8_t enable_power(BP* bp) {
//  unsigned char data= BP_BIN_PINS_SET;
//  data|= BP_PIN_POWER;
//
//  if (bp_write(bp, &data, 1) != BP_SUCCESS)
//    return BP_FAILURE;
//
//  unsigned char buf;
//  if (bp_readc(bp, &buf) != BP_SUCCESS)
//    return BP_FAILURE;
//
//  if (buf == 0x01) 
//    return BP_SUCCESS;
//  else
//    return BP_FAILURE;
//}
//
int main (int argc, char const* argv[]) {
  std::ostringstream oss;
  oss << "Usage: " << argv[0] << " [-d] device [-f] file [options]";
  po::options_description desc(oss.str());
  desc.add_options()
    ("help,h", "produce help message")
    ("version", "print libat45tools version and exit")
    ("file,f", po::value<std::string>(), "which file to upload to an AT45 flash")
    ("device,d", po::value<std::string>(), "name of the buspirate device")
    ;
  po::positional_options_description p;
  p.add("device", 1);
  p.add("file", 1);
  po::variables_map vm;

  // Begin processing of commandline parameters.
  try {
    po::store(po::command_line_parser(argc, argv).
        options(desc).positional(p).run(), vm);
    po::notify(vm);
  } catch (std::exception& e) {
    std::cerr << "Cannot process commandline options: " << e.what() << std::endl;
    exit(-1);
  }

  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return 1;
  }

  if (vm.count("version")) {
    std::cout << "at45tools version " << at45tools::version() << std::endl;
    return 0;
  }

  std::string name_datafile;
  std::string name_buspirate_device;

  if (!vm.count("file")) {
    std::cerr << "You must specify a file to upload to flash. " << std::endl;
    std::cerr << desc << std::endl;
    return 1;
  } else {
    name_datafile=vm["file"].as<std::string>();
  }
  bfs::path datafile(name_datafile);
  if (! bfs::exists(datafile)) {
    std::cerr << "File " << datafile << " does not exist, exiting." << std::endl;
    return 2;
  }

  if (!vm.count("device")) {
    std::cerr << "You must specify the device of your bus pirate." << std::endl;
    std::cerr << desc << std::endl;
    return 1;
  } else {
    name_buspirate_device=vm["device"].as<std::string>();
  }
  bfs::path buspirate_device(name_buspirate_device);
  if (! bfs::exists(buspirate_device)) {
    std::cerr << "File " << buspirate_device << " does not exist, exiting." << std::endl;
    return 2;
  }

  assert(signal(SIGINT, signal_handler) != SIG_ERR);

  // now: everything is ready, start.
  BP* bp = bp_open(buspirate_device.string().c_str());
  if (bp == NULL) {
    std::cerr << "Could not open bus pirate - exiting." << std::endl;
    return 3;
  }

  uint8_t version;
  if (bp_bin_init(bp, &version) != BP_SUCCESS) {
    std::cout << "Could not enter binary mode - exiting." << std::endl;
    bp_close(bp);
    return 4;
  } 
  std::cout << "Binary IO version: " << (uint16_t) version << std::endl;
  
  printf("Entering SPI mode\n");
  if (bp_bin_mode_spi(bp, &version) != BP_SUCCESS)
    return -1;
  printf("Binary I/O SPI mode: %u\n", version);


  std::cout << "Enabling power: ";
  uint8_t mode=0x48;
  if (bp_write(bp, &mode, 1) != BP_SUCCESS) {
    std::cout << "Cannot enable power" << std::endl;
    bp_close(bp);
    return -1;
  }
  uint8_t command_state=0;
  if (bp_readc(bp, &command_state) != BP_SUCCESS) {
    std::cout << "Failed to read status." << std::endl;
    bp_close(bp);
    return -1;
  }
  if (command_state != 1) {
    std::cout << "Failed to enable power." << std::endl;
    bp_close(bp);
    return -1;
  } else {
    std::cout << "OK." << std::endl;
  }

  usleep(10);

  std::cout << "Reading AT45DB161D ID: ";
  uint8_t command=0x9f;
  if (bp_write(bp, &command, 1) != BP_SUCCESS) {
    std::cout << "Cannot query device" << std::endl;
    bp_close(bp);
    return -1;
  }
  for (uint8_t i=0; i<4; ++i) {
    uint8_t retval=0;
    if (bp_readc(bp, &retval) != BP_SUCCESS) {
      std::cout << "Failed to read return value." << std::endl;
      bp_close(bp);
      return -1;
    } else {
      printf("%x ", retval);
    }
  }
  std::cout << ";" << std::endl;


  // if (bp_bin_spi_version(bp, &version) != BP_SUCCESS)
  //   return -1;
  // printf("Binary I/O SPI version: %u\n", version);

  //   unsigned char speed;
  // printf("Reading SPI speed\n");
  // if (bp_bin_spi_get_speed(bp, &speed) != BP_SUCCESS)
  //   return -1;
  // printf("Binary I/O SPI speed:\n");
  // show_spi_speed(speed);

  // printf("Setting SPI speed\n");
  // if (bp_bin_spi_set_speed(bp, BP_BIN_SPI_SPEED_1M) != BP_SUCCESS)
  //   return -1;

  // printf("Reading SPI speed\n");
  // if (bp_bin_spi_get_speed(bp, &speed) != BP_SUCCESS)
  //   return -1;

  // printf("Updated Binary I/O SPI speed:\n");
  // show_spi_speed(speed);

  // unsigned char config;
  // printf("Reading SPI config\n");
  // if (bp_bin_spi_get_config(bp, &config) != BP_SUCCESS)
  //   return -1;

  // printf("Binary I/O SPI config (%u):\n", config);
  // show_spi_config(config);

  //   printf("Setting SPI config\n");
  // if (bp_bin_spi_set_config(bp, BP_BIN_SPI_LV_HIZ |
  // 		    BP_BIN_SPI_CLK_IDLE_HIGH |
  // 		    BP_BIN_SPI_CLK_EDGE_LOW |
  // 		    BP_BIN_SPI_SMP_MIDDLE) != BP_SUCCESS)
  //   return -1;

  // printf("Reading SPI config\n");
  // if (bp_bin_spi_get_config(bp, &config) != BP_SUCCESS)
  //   return -1;
  // printf("Binary I/O SPI config (%u):\n", config);
  // show_spi_config(config);


  // if (enable_power(bp) != BP_SUCCESS) {
  //   std::cout << "Failed to enable power." << std::endl;
  //   return -1;
  // }
  //
  // sleep(2);


  // // TODO: Write test.
  // bp_bin_spi_cs(bp, true);
  // uint8_t buffer[4];
  // buffer[0]=0x9f;
  // if (bp_bin_spi_bulk(bp, buffer, 4) != BP_SUCCESS) {
  //   std::cout << "Failed to write buffer!" << std::endl;
  // } else {
  //   std::cout << "Got result: " << buffer << std::endl;
  // }
  // bp_bin_spi_cs(bp, false);


  sleep(2);

  std::cout << "Resetting bus pirate to user terminal" << std::endl;
  if (bp_reset(bp) != BP_SUCCESS) {
    std::cout << "Failed to reset bus pirate." << std::endl;
  }


  bp_close(bp);
  exit(EXIT_SUCCESS);

}

