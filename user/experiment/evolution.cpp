
#include "evolution.h"
#include "../../core/utilities.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <regex>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

void evolution::run() {

  // generate the population
  auto pop = life::make_population(pop_name_);
  pop.configure(pop_config_);

  auto optimiser = life::make_environment(sel_name_);
  optimiser.configure(sel_config_);

  auto world = life::make_environment(world_name_);
  world.configure(world_config_);

  std::ofstream pop_file(Dir_ + "pop_" + Rep_ + ".csv");
  pop_file << "avg,max,update\n";

  std::ofstream lineage_file(Dir_ + "lineage_" + Rep_ + ".csv");
  lineage_file << "id,coalesced_at\n";

  for (auto i : util::rv3::view::iota(0, generations_)) {

    pop = world.evaluate(pop);

    auto stats =
	   	pop.get_stats();

    if (!(i%100))
      std::cout << stats << std::endl;

    for (auto &k : stats)
      pop_file << k << ",";
    pop_file << i << "\n";

    pop = optimiser.evaluate(pop);
  
  	auto new_ancs = pop.prune_lineage();
	if (!new_ancs.empty())
		for(auto& org: new_ancs)
			lineage_file << org.get_id() << "," << i << "\n";
 
  }
}

