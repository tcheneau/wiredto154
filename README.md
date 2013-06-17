Wired to 15.4
=============

Wiredto154 is a real time IEEE 802.15.4 PHY emulation program over wired links
(e.g. Ethernet). It works because wired links are much faster (100Mbps, 1Gbps, etc.)
than IEEE 802.15.4 wireless links (250Kbps).
It implements realistic fading and Packet Error Rate mechanism.
This tool sits in between real deployment and Discrete Event Simulation (DES).

Dependencies
------------

* Boost library (C++)


Path loss model
---------------

For the moment, only one path loss model is implemented.

## Log-normal shadowing path loss model


Might not work after a certain distance (how to compute it), where the
measurements does not match the model accurately anymore.

### Caveat

It does not work at a distance below the reference distance (*d0*)
because the antenna effects affects the path loss computation.
A proper way extend the model would be to add a field path loss model for the
[0, d0) distance range.


Bit Error Rate (BER) computation
--------------------------------


Dataset
-------

## IEEE 802.15.4 nodes

* RedBee Econotag
* Mica2

TBA

## Communication environment

* Gungor et al.

TBA

TODO
----

* pcap dumper for every packet sent (easy)
* CLI argument parsing
* robust makefile system
* latency calibration tool



Acknowledgment
--------------

<em>
Conditions Of Use

This software was developed by employees of the National Institute of
Standards and Technology (NIST), and others.
This software has been contributed to the public domain.
Pursuant to title 15 Untied States Code Section 105, works of NIST
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
