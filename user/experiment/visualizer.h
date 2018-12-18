
#include "../../components.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <regex>
#include <string>
#include <string_view>
#include <vector>

class visualizer {

  std::string pop_name_  = "null_population";
  std::string world_name_ = "null_environment";
  life::configuration pop_config_;
  life::configuration world_config_;
  std::string Rep_ = "arch";
  std::string Dir_ = "./data/vis_";
public:
  visualizer() {
	  configure(publish_configuration());
  }

  life::configuration publish_configuration() {
    life::configuration ec;
    ec["population"] = {pop_name_,{}};
    ec["world"] = {world_name_,{}};
    ec["REP"] = Rep_;
    ec["DIR"] = Dir_;
    return ec;
  }

  void configure(life::configuration con) {
    Rep_ = con["REP"];
    Dir_ = con["DIR"] ;
	pop_name_ = std::string(con["population"][0]);
	pop_config_ = con["population"][1];
	world_name_= std::string(con["world"][0]);
	world_config_ = con["world"][1];
  }

  void run();
};

