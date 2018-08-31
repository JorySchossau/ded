


#include"components.h"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <iostream>
#include <fstream>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>
#include <random>
#include <regex>

template <typename L, typename M>
void save(std::ofstream &f, std::string_view comp_type, L component_lister,
          M component_maker) {

  for (auto &comp_name : component_lister()) {
                         auto n  = component_maker(comp_name);
    std::cout <<
	   	comp_type
                          << " -- " << comp_name << "\n  " <<       std::endl;
	  std::cout <<std::setw(4) << n.publish_configuration()
                          << std::endl;
    f << "\n"
      << comp_type << " -- " << comp_name << "\n  " << std::setw(4)
      << component_maker(comp_name).publish_configuration() << std::endl;
  }
}
void save_configs() {

  std::ofstream file("configurations.cfg");

  save(file, "experiment", life::experiment_list, life::make_experiment);
  save(file, "entity", life::entity_list, life::make_entity);
  save(file, "selector", life::selector_list, life::make_selector);
  file.close();
}

life::configuration load_user_experiment(std::string file_name) {
  life::configuration con;
  std::ifstream ef(file_name);
  ef >> con;
//  std::cout << std::setw(4) << con << std::endl;
  return con;
}

int main(int argc, char **argv) {
  if (argc == 2 && std::string(argv[1]) == "-s") {
    save_configs();
  }

  if (argc == 3 && std::string(argv[1]) == "-s") {
    auto  con = load_user_experiment(argv[2]);
    std::string name = con[0];
    auto ex = life::make_experiment(name);
    ex.configure(con[1]);
    ex.run();
  }
}
