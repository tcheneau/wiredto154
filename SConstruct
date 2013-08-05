import os

boost_prefix = "/usr"
libs = [ 'boost_program_options', 'pthread', 'boost_system-mt', 'pugixml' ]

release_mode = ARGUMENTS.get("mode", "release")

if not (release_mode in [ "release", "debug" ]):
    print "mode should be set to release or debug"
    Exit(1)

releasecflags = [ "-O3" ]
debugcflags = [ "-O0", "-g", "-DDEBUG" ]

if release_mode == "release":
    cflags = releasecflags
else:
    cflags = debugcflags

env = Environment()
env.Append(CCFLAGS = cflags)

Export('env')

SConscript('models/SConscript')
SConscript('network/SConscript')
SConscript('core/SConscript')
SConscript('tests/SConscript')

Import('models_lib')
Import('core_lib')
Import('network_lib')

sources = env.Glob("*.cpp")
env.Append(CPPPATH = [os.path.join(boost_prefix, "include")])
env.Append(LIBPATH = [os.path.join(boost_prefix, "lib")])
env.Append(CCFLAGS = "-O3")
app = env.Program(target = "wiredto154", source = "wiredto154.cpp",
                  LIBS = libs + [ models_lib, core_lib, network_lib ],
                  CPPPATH=["."],
                  LIBPATH = [ 'models', 'network', 'core' ],
                  # the following line is a workaround to solve circular dependencies issues when linking the program
                  LINKFLAGS = "-Wl,-\(")
app2 = env.Program(target = "pcap-dumper",
                   source = "pcap-dumper.cpp",
                   LIBS = libs)
Default([app, app2])
