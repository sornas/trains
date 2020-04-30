import subprocess
import sys

platform = ARGUMENTS.get("OS", sys.platform)

Execute(Mkdir(f"lib/{platform}"))

if platform in ("linux", "darwin"):
    engine = "libfog.a"
    cxx = "g++"
elif platform == "windows":
    engine = "libfog.lib"
    cxx = "x86_64-w64-mingw32-g++"
else:
    print(f"platform {platform} not supported")
    sys.exit(1)

Execute(Mkdir("inc"))

subprocess.run(f"make -C fog engine ENGINE_LIBRARY_NAME={engine} CXX={cxx}".split(" "))
subprocess.run("make -C fog rain mist".split(" "))

# syntax is Copy(target, source)
Execute(Copy(f"lib/{platform}/", f"fog/out/{engine}"))
Execute(Copy("inc/", "fog/out/fog.h"))

SConscript("src/sconscript.py", variant_dir=f"build/{platform}", duplicate=0)
