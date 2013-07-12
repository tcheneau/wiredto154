/*
* Conditions Of Use
*
* This software was developed by employees of the National Institute of
* Standards and Technology (NIST), and others.
* This software has been contributed to the public domain.
* Pursuant to title 15 United States Code Section 105, works of NIST
* employees are not subject to copyright protection in the United States
* and are considered to be in the public domain.
* As a result, a formal license is not needed to use this software.
*
* This software is provided "AS IS."
* NIST MAKES NO WARRANTY OF ANY KIND, EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTY OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, NON-INFRINGEMENT
* AND DATA ACCURACY.  NIST does not warrant or make any representations
* regarding the use of the software or the results thereof, including but
* not limited to the correctness, accuracy, reliability or usefulness of
* this software.
*/

/* Tony Cheneau <tony.cheneau@nist.gov> */

#include "core/simulation.hpp"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/program_options.hpp>
namespace po = boost::program_options;
using namespace std;

int main(int argc, char const* argv[])
{
	string error_msg;
	string simulation_file;
	string modulation;
	string pathloss;

	Simulation & sim = Simulation::get();

    try {
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce this help message")
			("list-modulation", "list available modulation schemes")
			("list-pathloss", "list available pathloss models")
			("modulation", po::value<string>(&modulation), "modulation scheme")
			("pathloss", po::value<string>(&pathloss), "path loss model for the simulation")
			("simulation,s", po::value<string>(&simulation_file), "XML file that describes the simulation")
        ;

        po::variables_map vm;
        po::store(po::command_line_parser(argc, const_cast<char **>(argv)).
                  options(desc).run(), vm);
        po::notify(vm);

        if (vm.count("help")) {
            cout << "Usage: options_description [options]\n";
            cout << desc;
            return 0;
        }

        if (vm.count("list-modulation")) {
			vector<string> modulations = Modulation::list_available_modulation();
			cout << "List of available modulation schemes:" << endl;
			for(vector<string>::const_iterator i = modulations.begin(); i != modulations.end(); ++i)
				cout << *i << endl;
            return 0;
        }

        if (vm.count("list-pathloss")) {
			vector<string> pathlossmodels = PathLossModel::list_available_model();
			cout << "List of available path loss models:" << endl;
			for(vector<string>::const_iterator i = pathlossmodels.begin(); i != pathlossmodels.end(); ++i)
				cout << *i << endl;
            return 0;
        }

		if (vm.count("pathloss")) {
			sim.set_pathloss_model(pathloss);
		}
		else {
			cerr << "no path loss model selected, you need to choose one using the --pathloss selector" << endl
				 << "for the list of path loss model, try --list-pathloss" << endl;
			exit(EXIT_FAILURE);
		}

		if (vm.count("modulation")) {
			sim.set_modulation(modulation);
		}
		else {
			std::string default_mod("O-QPSK");
			cout << "setting default modulation to " << default_mod << endl;
			sim.set_modulation(default_mod);
		}

        if (!vm.count("simulation")) {
			cerr << "\"simulation\" argument is mandatory" << endl;
			cout << desc;
			return 0;
        }
    }
    catch(std::exception& e)
    {
        cerr << e.what() << "\n";
        return 1;
    }

	// TBD: read the topology from an XML file

	try {
		sim.load(simulation_file.c_str());
	} catch (Simulation::exception_on_simulation_loading & e) {
		cerr << "unable to load file " << simulation_file;
		if (strlen(e.what()))
			cerr << " (" << e.what() << ")";
		cerr << endl;
		return 1;
	}





	// instanciate the object appropriately
	// sim.set_modulation(modulation_model);
	// sim.set_pathloss_model(pathloss_model);

	if (!sim.is_properly_configured(error_msg)) {
		cerr << "simulation is not properly configured: "
			 << error_msg
			 << endl;
		exit(EXIT_FAILURE);
	}

	// TBD: start the asynchronous server

	// wait for the program to be interrupted

    return 0;
}
