Wired (network) to (IEEE 802.)15.4
==================================

Wiredto154 is a real-time IEEE 802.15.4 PHY emulation software that works over wired links
(e.g. Ethernet). This "emulation" can be achieved because wired links are so
much faster (100Mbps, 1Gbps, etc.) than IEEE 802.15.4 wireless links (250Kbps).

This tool fills in the gap between real mote deployment and Discrete Event
Simulation (DES). It can runs the exact same code as the real motes, but does
not require you to buy and setup an expensive testbed to do so. Furthermore,
the integrated *pcap-dumper* enables you to capture the traffic for easier
debugging.

It implements realistic path loss fading model and Packet Error Rate mechanism.
Wiredo154 itself doesn't generate any IEEE 802.15.4 and does not even check if
the frame it carries has a valid format. Instead, it relies on some modified
virtual device driver to generate and interpret the frame. To this end,
Wiredto154 comes with a patch for the Contiki project to add such virtual
device driver.

Please note some functions are left to be implemented on the virtual device
driver. For rate limiting and delays are best implemented on the virtual device driver. 

Terminology
-----------

* *PHY emulator*: physical layer emulator is sometime used to refer to Wiredto154.
* *simulation*: we stretch this term from its general use a little. In our
  context, a simulation means running Wiredto154 with a set a virtual motes for
  a specific duration in order to gather some data and the associated
  configuration that makes this run possible.
* *virtual device driver*: a specific device driver for the virtual mote
* *virtual mote* or *virtual node*: a standalone software implementation of the IEEE
  802.15.4/6LoWPAN stack. In practice, it can be a Linux node (running under a
  virtualization technology such as [kvm][], or not) or a Contiki "native"
  node.

Purpose of the project
----------------------

* We believe that accurate large scale simulation of IEEE 802.15.4 networks (in order of hundreds) is important
* We want a better control on the propagation models
* We want a tool that enables us to connect Linux nodes and Contiki
  nodes together. Contiki offers proven implementations of the protocols that
  works on top of IEEE 802.15.4 (e.g. 6LoWPAN, RPL, etc.). We believe that this
  feature will help us work better on some shortcoming of the Linux network
  stack.

Architecture
------------

Virtual motes send data the PHY emulator using unicast UDP.
The PHY emulator then send a multicast packet with a header that indicates
which nodes should received the packet uncorrupted and which nodes should
receive a corrupted packet (thus keeping their radio busy). Using multicast for
broadcasting the packet saves a lot of bandwidth (a same packet reaches
multiple nodes) and enables decentralised simulation, where the motes are
scattered among different computers.

TBD: a picture that explain the configuration

From a practical standpoint, the PHY emulator and the virtual mote can be located on the same physical computer.

For more information on the packet format, look at [the associated document](documentation/packet_format.md).


Dependencies
------------

* [Boost library (C++)][boost]
* [pugixml][]


Building and installing
-----------------------

The build system relies on *scons*. If you have all the dependencies, all it
should take for you to build Wiredto154 is:

    scons

If everything builds fine, you should have two binaries: *wiredto154* and
*pcap-dumper*. You can copy these two files wherever you want on your system,
but it  does not make sense to make them system wide, so it's probably better
to use them directly in the build directory.

IEEE 802.15.4 model
-------------------

The various timing (CSMA-CA, CCA, etc) are not handled by Wiredto154.
However, the virtual device driver makes sure that frame are delivery in a timely manner.
Timings might not be that of a real device but are based on the standard.
It only captures the timing of a non beacon-enabled network with no acknowledgements.

Path loss model
---------------

For the moment, very few path loss models are implemented.

### Unit discs model

A standard unity discs implementation which models two discs around the sender.
These two discs that delimits three areas:

* in the area of the innermost disc, all packets are received. The size of this
  disc is controlled by the "good-radius" command line parameter.
* in the area between the innermost disc and the outermost disc, all received
  packets are damaged. The size of the outermost disc is controlled by the
  "damaged-radius" command line parameter.
* in the area outside of the outermost disc, no packet is ever received.

Note that this model is very basic and has only been implemented for prototype testing.

### Log-normal shadowing path loss model

The Log-normal shadowing path loss model is described in the 
["Wireless communication Principles & Practice" (Theodore S. Rappaport)](https://dl.acm.org/citation.cfm?id=559977) book.
As pointed in
["An Analysis of Unreliability and Asymmetry in
Low-Power Wireless Links" (Zamalloa et al.)](https://dl.acm.org/citation.cfm?id=1240227), 
the model does not behave well in the "transitional region" (where a node is
too far apart for reliably receiving communication from an other node, but not
far enough not to receive it).

#### Caveat

This model does not work at a distance below the reference distance (*d0*)
because the antenna effects affects the path loss computation.
A proper way extend the model would be to add a field path loss model for the
[0, d0) distance range.
If the topology contains two nodes below the reference distance, an exception
will be raised and Wiredto154 will exit.


Bit Error Rate (BER) computation
--------------------------------

Currently, the only supported modulation scheme is O-QPSK. The corresponding
BER (and thus PER) formula is one specified in the IEEE 802.15.4-2006
specification at the page 268.

Datasets
--------

### IEEE 802.15.4 nodes

The node's transceiver characteristics vary greatly between nodes' models and
manufacturers, and can even be modified dynamical when the node is being used.
For this reason, we only provide information on some common nodes, based
from the manufacturers' specifications and leave to the reader the choice of
the transceiver characteristics that is choose as a parameter of the
simulation.

#### RedBee Econotag

* transceiver power: between -30dBm or 4dBm. When running the Contiki firmware, an
  Econotag will run at a higher transmission power that is not documented in the specification. 
* antenna sensitivity: -96dBm in DCD (Differential Chip Detect) mode, -100dBm
  in NCD (Non-coherent Detect) mode. The latter requires more energy.

### Communication environment

* [Opportunities and Challenges of Wireless Sensor Networks in Smart Grid (Gungor et al.)][Gungor]
    - *500kVSub-LOS* (Log-Normal Shadowing), 500kV Substation (LOS)
    - *500kVSub-NLOS* (Log-Normal Shadowing), 500kV Substation (NLOS)
    - *MPR-LOS* (Log-Normal Shadowing), Main Power Room (LOS)
    - *MPR-NLOS* (Log-Normal Shadowing), Main Power Room (NLOS)
    - *UTV-LOS* (Log-Normal Shadowing), Underground Transformer Vault (LOS)
    - *UTV-NLOS* (Log-Normal Shadowing), Underground Transformer Vault (NLOS)

Ensuring that the simulation runs in a timely manner
----------------------------------------------------

It could be when number of nodes increases that the simulation will run to slowly to produce meaningful results.

Building networks using Wiredto154
----------------------------------

The node topology is described in a XML file that needs to be provided to the emulator.

TBA

Example of use
--------------

TBA

That's it! Virtual motes can now connect to the simulation on the associated port.


Usage
-----

    Allowed options:
      --help                            produce this help message
      --multicast-addr arg (=224.1.1.1) multicast address used for sending outgoing
                                        frames
      --multicast-port arg (=10000)     multicast port used for sending outgoing 
                                        frames
      --list-modulation                 list available modulation schemes
      --list-pathloss                   list available pathloss models
      --modulation arg                  modulation scheme
      --pathloss arg                    path loss model for the simulation
      -s [ --simulation ] arg           XML file that describes the simulation

A hidden argument, named *--help-pathloss* can be passed to the program. It
provides detailed information on the parameters accepted by the path loss
model. However, due to a limitation of the Boost Program Option library, it seems
that the parameters have to be passed in the "--param=value" format (e.g. other
format such as "--param value" won't be recognized).

Adding nodes to the simulation
------------------------------

### Contiki nodes

See the [dedicated documentation page](documentation/Contiki.md).

### Linux nodes

See the [dedicated project page](https://github.com/tcheneau/virtual-ieee802154-serial).
(note that the Virtual serial driver for Linux has not been updated to work with Wiredto154 yet)

Utilities
---------

### PCAP dumper

Wiredto154 ships with a small binary named *pcap-dumper*.
This is the tool you can use it to debug your network. 
This tool helps you debugging your simulation/network by generating pcap dumps
of the traffic. This dumps can easily be read with third party tools (e.g.
[Wireshark](https://www.wireshark.org/)).
The *pcap-dumper* listen on the same multicast group as the other nodes participating on
the network. This way, it does not incur any performance loss to the simulation.
It records each IEEE 802.15.4 frame that exits Wiredoto154 and create a
separate capture file for each nodes that should have received the frame.
For nodes that should receive a damaged frame, the FCS field is XOR'ed with 0xFFFF.
The frame is also saved within the capture file associated to sender of the
packet. This choice was not obvious, but was made because it makes ready
capture file easier.

Using the *pcap-dumper* is is pretty simple and does not require to provide the
network topology, you just have to connect to the same multicast group and port the
simulation is running on. For example, you can do the following:

    ./pcap-dumper --maddr 224.1.1.1 --mport 10000

This will create a new **pcapng** file for each nodes participating in the
simulation that is to receive a packet.

(Not so) Frequently Asked Questions
-----------------------------------

### Contiki comes with Cooja, why not using it?

[Cooja](https://github.com/contiki-os/contiki/wiki/An-Introduction-to-Cooja) is
a great tool, but does not seem suited for what we aim to do: having virtual
motes scattered on multiple computers and mixing different nodes type (e.g. Linux).

Contributors
------------

* Tony Cheneau
* M. Ranganathan (initial code for the UDP radio in Contiki)

Conditions Of Use
-----------------

<em>
This software was developed by employees of the National Institute of
Standards and Technology (NIST), and others.
This software has been contributed to the public domain.
Pursuant to title 15 United States Code Section 105, works of NIST
employees are not subject to copyright protection in the United States
and are considered to be in the public domain.
As a result, a formal license is not needed to use this software.

This software is provided "AS IS."
NIST MAKES NO WARRANTY OF ANY KIND, EXPRESS, IMPLIED
OR STATUTORY, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTY OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, NON-INFRINGEMENT
AND DATA ACCURACY.  NIST does not warrant or make any representations
regarding the use of the software or the results thereof, including but
not limited to the correctness, accuracy, reliability or usefulness of
this software.
</em>

[boost]: http://www.boost.org/
[contiki]: http://www.contiki-os.org/
[kvm]: http://www.linux-kvm.org/page/Main_Page
[pugixml]: https://code.google.com/p/pugixml/

[Gungor]: https://ieeexplore.ieee.org/xpls/abs_all.jsp?arnumber=5406152&tag=1
