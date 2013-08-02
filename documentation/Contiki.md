# Connecting Contiki with Wiredto154

The [Contiki][] operating system targets small communication devices with very little RAM and ROM known as Wireless Sensors.
This operating system currently already work on dozen of devices. Here, we
modified the *native* target to communicate to the other nodes through the PHY
emulator. Patching the sources of the Contiki operating system is required for this operation.

With our modification, a Contiki node will use its simulation identifier to
generate its layer 2 address, so that you never have to set it up yourself.
Because the identifier is on 2 bytes, and the IEEE 802.15.4 MAC address is on 8
bytes, the layer 2 address is padded with the 6 bytes of zeros on the Most
Significant Bytes.

## Retrieving Contiki sources

The Contiki sources can be retrieved through git with the following command:

    git clone https://github.com/contiki-os/contiki.git

All that is left is for us to apply the patch for our modifications:

    cd contiki
    git am pathtothepatch/contiki-udp-radio.patch

## Running an example: the rpl-udp example

The RPL-UDP example is a very useful example that can quickly demonstrates if two nodes can talk to
each other. The two nodes have different roles and thus runs different
code: one node is the server, while the other one is the client. Once the network is fully started (RPL
DODAG has been formed), the client will send UDP data packet to the server (RPL
DODAG root). The server will then send a reply to the client (if the server has
been compiled with the SERVER_REPLY flag).

In order to build the example, go to the rpl-udp example, and compile it with
the following options. From the contiki sources directory:

    cd examples/ipv6/rpl-udp
    make TARGET=native SERVER_REPLY=1

The example code will compile and you'll end up with two binaries *udp-client.native* and *udp-server.native*.

Supposing your simulation runs on the following (very simple) topology:

    <?xml version="1.0" encoding="UTF-8" ?>
    <simulation>
    <nodes>
    	<node port="10001" id="1" type="contiki" description="one" x="0.0" y="0.0" txPower="1" rxSensitivity="-85" />
    	<node port="10002" id="2" type="contiki" description="two" x="0.0" y="10.0" txPower="1" rxSensitivity="-85" />
    </nodes>
    </simulation>

If we assume that the PHY emulator sends multicast traffic on the IP address
224.1.1.1 and on port 10000 (the default behavior), you can then connect client
the following command line arguments:

    ./udp-client.native --identifier 1 -m 224.1.1.1 -l 10000 -e <PHY-emulator-IP> -p 10001

Starting the server is almost identical:

    ./udp-server.native --identifier 2 -m 224.1.1.1 -l 10000 -e <PHY-emulator-IP> -p 10002

If everything goes fine, you should see the following text on the *udp-server* after a few seconds:

    DATA recv 'Hello 1 from the client' from 1


## Usage

    usage: /tmp/udp-client.native -i ID -m MADDR -l MPORT -e DADDR -p DPORT
    -h,--help          this help message
    -i,--identifier    node's identifier for the simulation
    -m,--maddr         multicast address to listen on
    -l,--mport         multicast port to listen on
    -e,--emuaddr       address of the PHY emulator (wiredto154)
    -p,--emuport       PHY emulator (wiredto154) port to connect to


[contiki]: http://www.contiki-os.org/
