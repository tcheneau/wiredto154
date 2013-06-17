import os

env = Environment()
boost_prefix = "/usr"
libs = [ 'boost_program_options' ]

sources = env.Glob("*.cpp")
env.Append(CPPPATH = [os.path.join(boost_prefix, "include")])
env.Append(LIBPATH = [os.path.join(boost_prefix, "lib")])
app = env.Program(target = "wiredto154", source = sources, LIBS = libs)
env.Default(app)
