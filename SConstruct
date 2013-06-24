import os

env = Environment()
boost_prefix = "/usr"
libs = [ 'boost_program_options' ]


SConscript('models/SConscript', export='models_lib') #, variant_dir='build')
SConscript('tests/SConscript')

sources = env.Glob("*.cpp")
env.Append(CPPPATH = [os.path.join(boost_prefix, "include")])
env.Append(LIBPATH = [os.path.join(boost_prefix, "lib")])
env.Append(CCFLAGS = "-O3")
app = env.Program(target = "wiredto154", source = sources, LIBS = libs)
Default(app)
