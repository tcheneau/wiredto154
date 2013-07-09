/*
* Conditions Of Use
*
* This software was developed by employees of the National Institute of
* Standards and Technology (NIST), and others.
* This software has been contributed to the public domain.
* Pursuant to title 15 Untied States Code Section 105, works of NIST
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
#include <iostream>
#include <boost/program_options.hpp>
namespace po = boost::program_options;
using namespace std;

int main(int argc, char const* argv[])
{
	string error_msg;
	string modulation_model;
	string pathloss_model;
	string filename("simulation-test-topology-simple.xml");

    try {
        int numnodes;
		int port;
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("numnodes,n", po::value<int>(&numnodes)->default_value(100),
                  "number of nodes that can participate in the simulation")
            ("verbose,v", po::value<int>()->implicit_value(1),
                  "enable verbosity (optionally specify level)")
            ("listen,l", po::value<int>(&port)->implicit_value(10000)
                  ->default_value(0,"no"),
                  "listen on a port.")
        ;

        po::positional_options_description p;
        p.add("input-file", -1);

        po::variables_map vm;
        po::store(po::command_line_parser(argc, const_cast<char **>(argv)).
                  options(desc).positional(p).run(), vm);
        po::notify(vm);

        if (vm.count("help")) {
            cout << "Usage: options_description [options]\n";
            cout << desc;
            return 0;
        }

        if (vm.count("verbose")) {
            cout << "Verbosity enabled.  Level is " << vm["verbose"].as<int>()
                 << "\n";
        }

        cout << "Listen port is " << port << "\n";
    }
    catch(std::exception& e)
    {
        cout << e.what() << "\n";
        return 1;
    }

	// TBD: read the topology from an XML file

	Simulation & sim = Simulation::get();

	try {
		sim.load("simulation-test-topology-simple.xml");
	} catch (exception & e) {
		cerr << "unable to load file " << filename
			 << " " << e.what()
			 << endl;
	}





	// instanciate the object appropriately
	// sim.set_modulation(modulation_model);
	// sim.set_pathloss_model(pathloss_model);

	if (!sim.is_properly_configured(error_msg)) {
		cerr << "simulation is not properly configured"
			 << error_msg
			 << endl;
		exit(EXIT_FAILURE);
	}

	// TBD: start the asynchronous server

	// wait for the program to be interrupted

    return 0;
}
