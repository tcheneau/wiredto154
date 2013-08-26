#!/bin/env python

# Conditions Of Use
#
# This software was developed by employees of the National Institute of
# Standards and Technology (NIST), and others.
# This software has been contributed to the public domain.
# Pursuant to title 15 United States Code Section 105, works of NIST
# employees are not subject to copyright protection in the United States
# and are considered to be in the public domain.
# As a result, a formal license is not needed to use this software.
#
# This software is provided "AS IS."
# NIST MAKES NO WARRANTY OF ANY KIND, EXPRESS, IMPLIED
# OR STATUTORY, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTY OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, NON-INFRINGEMENT
# AND DATA ACCURACY.  NIST does not warrant or make any representations
# regarding the use of the software or the results thereof, including but
# not limited to the correctness, accuracy, reliability or usefulness of
# this software.

# Tony Cheneau <tony.cheneau@nist.gov>

from signal import signal, SIGINT
import os, select
import subprocess, shlex

process = None

def parse_xml(mcast_addr, mcast_port, phy_emu_addr, filename):
    import xml.etree.ElementTree as ET
    tree = ET.parse(os.path.expanduser(filename))
    root = tree.getroot()
    cli = []
    for node in root.find("nodes").getiterator("node"):
        if node.get("description") == "": # skip node with no description info
            print "node %s has no description field, skipping it" % node.get("id")
            continue
        args = { "binary": node.get("description"),
                "identifier": node.get("id"),
                "mcast_addr": mcast_addr,
                "mcast_port": mcast_port,
                "phy_emu_addr": phy_emu_addr,
                "phy_emu_port": node.get("port") }
        cli.append((args['identifier'], "{binary} -i {identifier} -m {mcast_addr} -l {mcast_port} -e {phy_emu_addr} -p {phy_emu_port}".format(** args)))

    return cli

def sig_handler(signal, frame):
    print "manually stopping the nodes"
    try:
        process.send_signal(SIGINT)
    except:
        pass

if __name__ == "__main__":
    # parse arguments
    import argparse
    parser = argparse.ArgumentParser(usage= "log events coming from a wiredto154 simulation",
                                     formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("-f", "--filename", help="simulation file", type=str, default="simulation.xml")
    parser.add_argument("--filename-prefix", help="prefix for the output file", type=str, default="output-node-")
    parser.add_argument("-e", "--emu-addr", help="IP address of the PHY emulator (wiredto154)", default="127.0.0.1")
    parser.add_argument("-a", "--mcast-addr", help="IP address of the multicast group", default="224.1.1.1")
    parser.add_argument("-p", "--mcast-port", help="port to listen on (for the multicast address)", type=int, default=10000)

    args = parser.parse_args()

    print "reading configuration file"
    # read XML file and prepare command line
    command_lines = parse_xml(args.mcast_addr, args.mcast_port, args.emu_addr, args.filename)

    # set the signal to end the process gracefully
    signal(SIGINT, sig_handler)

    print "starting processes"
    # launch subprocesses
    processes = []
    for (identifier, line) in command_lines:
        line = shlex.split(line)
        f = file(args.filename_prefix + identifier + ".txt", 'w')
        process = subprocess.Popen(line,
                                   stdin = subprocess.PIPE,
                                   stdout = f.fileno(),
                                   stderr = subprocess.STDOUT)
        process.stdin.close()
        processes.append((f, identifier, process))

    print "started %d processes" % len(command_lines)
    print "waiting for the simulation to end (or hit ctrl+c)"

    try:
        # wait for things to happen
        f, identifier, process = processes[0]
        ret = process.wait()
    except:
        pass

    print "cleaning up"

    # kill all the subprocesses upon exit
    for (f, identifier, process) in processes:
        try:
            print "stopping node %s" % identifier
            process.send_signal(SIGINT)
            process.wait()
            print "node %s stopped" % identifier
        except:
            pass

    print "program has exited gracefully"

