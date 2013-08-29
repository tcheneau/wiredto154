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
from copy import copy
import threading, os
import subprocess, shlex

processes = []
processes_lock = threading.Lock()

def parse_xml(mcast_addr, mcast_port, phy_emu_addr, filename):
    import xml.etree.ElementTree as ET
    tree = ET.parse(os.path.expanduser(filename))
    root = tree.getroot()
    cli = []
    delayed_cli = []
    for node in root.find("nodes").getiterator("node"):
        if node.get("description") == "": # skip node with no description info
            print "node %s has no description field, skipping it" % node.get("id")
            continue
        start_time = float(node.get("start-time", default = "0"))
        stop_time = float(node.get("stop-time", default = "0"))
        args = { "binary": node.get("description"),
                "identifier": node.get("id"),
                "mcast_addr": mcast_addr,
                "mcast_port": mcast_port,
                "phy_emu_addr": phy_emu_addr,
                "phy_emu_port": node.get("port") }
        if start_time == 0: # no delays
            cli.append((stop_time, args['identifier'], "{binary} -i {identifier} -m {mcast_addr} -l {mcast_port} -e {phy_emu_addr} -p {phy_emu_port}".format(** args)))
        else:
            delayed_cli.append((start_time, stop_time, args['identifier'], "{binary} -i {identifier} -m {mcast_addr} -l {mcast_port} -e {phy_emu_addr} -p {phy_emu_port}".format(** args)))

    return cli, delayed_cli

def process_start(identifier, filename, command):
    line = shlex.split(command)
    f = file(filename, 'w')
    t_process = subprocess.Popen(line,
                                stdin = subprocess.PIPE,
                                stdout = f.fileno(),
                                stderr = subprocess.STDOUT)
    with processes_lock:
        processes.append((f, identifier, t_process))

def delayed_process_start(identifier, *args, **kwargs):
    print "node %s is starting" % identifier
    process_start(identifier, *args, **kwargs)

def process_stop(identifier):
    with processes_lock:
        for i, data in enumerate(copy(processes)):
            (f, node_identifier, process) = data
            if identifier == node_identifier:
                print "stopping node %s" % identifier
                process.send_signal(SIGINT)
                process.wait()
                del processes[i]
                break

def kill_all_processes():
    with processes_lock:
        for i, data in enumerate(copy(processes)):
            try:
                (f, identifier, process) = data
                print "stopping node %s" % identifier
                process.send_signal(SIGINT)
                process.wait()
                print "node %s stopped" % identifier
                del processes[i]
            except:
                pass

def sig_handler(signal, frame):
    print "manually stopping the nodes"
    kill_all_processes()


if __name__ == "__main__":
    delayed_start = []
    delayed_stop = []

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
    command_lines, delayed_command_lines = parse_xml(args.mcast_addr, args.mcast_port, args.emu_addr, args.filename)

    # set the signal to end the process gracefully
    signal(SIGINT, sig_handler)

    print "starting processes"
    # launch subprocesses
    for (stop_time, identifier, line) in command_lines:
        filename = args.filename_prefix + identifier + ".txt"
        process_start(identifier, filename, line)
        if stop_time:
            t = threading.Timer(stop_time, process_stop, args = [ identifier ])
            delayed_stop.append(t)
            t.start()
            print "node %s will stop in %f seconds" % (identifier, stop_time)

    print "started %d processes immediately (%d processes scheduled for later)" % (len(command_lines),
                                                                                   len(delayed_command_lines))

    for (start_time, stop_time, identifier, cmd) in delayed_command_lines:
        filename = args.filename_prefix + identifier + ".txt"
        t = threading.Timer(start_time, delayed_process_start, args = [ identifier, filename, cmd ])
        delayed_start.append(t)
        t.start()
        print "node %s will start in %f seconds" % (identifier, start_time)
        if stop_time:
            t = threading.Timer(stop_time, process_stop, args = [ identifier ])
            delayed_stop.append(t)
            t.start()
            print "node %s will stop in %f seconds" % (identifier, stop_time)

    print "waiting for the simulation to end (or hit ctrl+c)"

    try:
        # wait for all processes to complete
        while processes:
            f, identifier, process = processes[0]
            process.wait()
            with processes_lock:
                del processes[0]
    except:
        pass

    print "cleaning up"

    for t in delayed_start + delayed_stop:
        t.cancel()
        t.join(.1)

    # kill all the subprocesses upon exit
    kill_all_processes()

    print "program has exited gracefully"

