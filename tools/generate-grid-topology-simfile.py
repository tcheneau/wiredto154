#!/bin/env python

# Tony Cheneau <tony.cheneau@nist.gov>

def create_node(x, y, port, identity, description="", txPower=0, rxSensitivity=-85, ntype="contiki"):
    return "".join(['<node port="', str(port),
                    '" id="', str(identity),
                    '" type="', str(ntype),
                    '" description="', str(description),
                    '" x="', str(x),
                    '" y="', str(y),
                    '" txPower="', str(txPower),
                    '" rxSensitivity="', str(rxSensitivity),
                    '" />'])

def create_nodes_list(generator):
    header = "<nodes>\n"
    footer = "</nodes>\n"

    return header + \
           "\n".join([create_node(* node_config) for node_config in generator]) + \
           footer


def manathan_square(x,y, node_id=1, port=10001, scale_factor=1):
    for i in xrange(x):
        for j in xrange(y):
            yield (i * scale_factor, j * scale_factor, port, node_id)
            port += 1
            node_id += 1

def create_config(width, height, node_id, port, scale_factor):
    header = """<?xml version="1.0" encoding="UTF-8" ?>\n<simulation>\n"""
    footer = """</simulation>"""
    config = create_nodes_list(manathan_square(width, height, node_id, port, scale_factor = scale_factor))
    return header + config + footer

if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(usage= "Generate a grid of node (Manhattan topology")
    parser.add_argument("-f", "--filename", help="output file", type=str, default="simulation.xml")
    parser.add_argument("-x", "--width", help="width of the grid", type=int, default=10)
    parser.add_argument("-y", "--height", help="height of the grid", type=int, default=10)
    parser.add_argument("-s", "--scale-factor", help="spacing between nodes", type=int, default=10)
    parser.add_argument("-p", "--port", help="starting value for the port range", type=int, default=10001)
    parser.add_argument("-i", "--identifier", help="starting value for the identifier range", type=int, default=1)

    args = parser.parse_args()

    with open(args.filename, "w") as f:
        f.write(create_config(args.width,
                              args.height,
                              args.identifier,
                              args.port,
                              args.scale_factor))

