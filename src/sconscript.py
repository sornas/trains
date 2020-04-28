platform = ARGUMENTS.get("OS", "linux")

inc = "../../inc"
lib = "../../lib/$PLATFORM"

env = Environment()

if platform == "windows":
    env.Replace(CC="x86_64-w64-mingw32-gcc")

env.Replace(PLATFORM=platform)
env.Append(INCDIR=inc)
env.Append(CPPPATH=[inc])
env.Append(LIBPATH=[lib])
env.Append(LIBS=Split("fog SDL2 SDL2main pthread"))

if platform != "windows":
    env.Append(LIBS=Split("c dl m"))

env.Append(CCFLAGS=Split("-std=c11 -Wall -ggdb -O0 -include src/config.h"))
env.Append(CPPDEFINES=["DEBUG"])

banverket = env.Program("banverket", source=Glob("*.c"))

Default(banverket)

Clean(banverket, Split(f"../../lib/linux/libfog.a"))
