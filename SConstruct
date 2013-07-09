import os

env = Environment()
boost_prefix = "/usr"
libs = [ 'boost_program_options', 'pthread', 'boost_system-mt', 'pugixml' ]

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
app = env.Program(target = "wiredto154", source = sources,
                  LIBS = libs + [ models_lib, core_lib, network_lib ],
                  CPPPATH=["core", "models", "network"],
                  LIBPATH = [ 'models', 'network', 'core' ],
                  # the following line is a workaround to solve circular dependencies issues when linking the program
                  LINKFLAGS = "-Wl,-\(")
Default(app)
