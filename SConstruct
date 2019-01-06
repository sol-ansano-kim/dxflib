import excons
from excons.tools import python
import sys
import os


env = excons.MakeBaseEnv()
out_libdir = excons.OutputBaseDirectory() + "/lib"
out_incdir = excons.OutputBaseDirectory() + "/include"
dxflib_static = (excons.GetArgument("dxflib-static", 0, int) != 0)
use_c11 = (excons.GetArgument("dxflib-c11", 1, int) != 0)


defs = []
prjs = []

cppflags = ""
if sys.platform != "win32":
    cppflags = " -Wno-unused-private-field -Wno-unused-parameter -Wno-deprecated-register"
    if use_c11:
        cppflags += " -std=c++11"
else:
    cppflags = " -wd4458 -wd4267 -wd4244"

if not dxflib_static:
    defs.append("DXFLIB_DLL")
    defs.append("DXFLIB_LIBRARY")


def DxflibName():
    return "dxf"


def DxflibPath():
    if sys.platform == "win32":
        return out_libdir + "/" + DxflibName() + ".lib"

    libname = "lib" + DxflibName() + (".a" if dxflib_static else excons.SharedLibraryLinkExt())

    return out_libdir + "/" + libname


def RequireDxflib(env):
    if not dxflib_static:
        env.Append(CPPDEFINES=["DXFLIB_DLL"])

    env.Append(CPPPATH=[out_incdir])
    env.Append(LIBPATH=[out_libdir])
    excons.Link(env, DxflibPath(), static=dxflib_static, force=True, silent=True)


prjs.append({"name": "dxf",
             "type": ("staticlib" if dxflib_static else "sharedlib"),
             "alias": "dxflib-lib",
             "defs": defs,
             "symvis": "default",
             "cppflags": cppflags,
             "incdirs": [out_incdir],
             "install": {out_incdir: excons.glob("src/*.h")},
             "srcs": excons.glob("src/*.cpp")})

prjs.append({"name": "_pydxf",
             "type": "dynamicmodule",
             "ext": python.ModuleExtension(),
             "alias": "dxflib-bind",
             "prefix": "py",
             "defs": [] if dxflib_static else ["DXFLIB_DLL"],
             "symvis": "default",
             "cppflags": cppflags,
             "incdirs": [out_incdir, "pybind11/include"],
             "srcs": excons.glob("bind/*.cpp"),
             "rpath": out_libdir,
             "custom": [RequireDxflib, python.SoftRequire],
             "install": {excons.OutputBaseDirectory() + "/py": excons.glob("bind/*.py")}
             })


for ed in os.listdir("examples"):
    prjs.append({"name": ed,
                 "type": "program",
                 "alias": "dxflib-test",
                 "symvis": "default",
                 "incdirs": [out_incdir, os.path.join("examples", ed)],
                 "srcs": excons.glob(os.path.join("examples", ed, "*.cpp")),
                 "cppflags": cppflags,
                 "custom": [RequireDxflib]})


excons.DeclareTargets(env, prjs)

excons.Export("DxflibName DxflibPath RequireDxflib")

excons.Default("dxf")

excons.Default("dxflib-bind")

