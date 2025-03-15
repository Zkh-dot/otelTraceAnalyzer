from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import sys
import shutil, os

ext_modules = [
    Extension(
        "otelanalyzer",
        sources=[
            "src/analyzer.c",

            "src/hashstructs/hashset_itr.c",
            "src/hashstructs/hashmap.c",
            "src/hashstructs/hashset.c",

            # "src/python_wrapper/py_struct_translator.c",
            "src/python_wrapper/py_structs.c",
            "src/python_wrapper/py_translator.c",
            "src/python_wrapper/py_analyzer.c",

            "src/structures/servicemap.c",
            "src/structures/service.c",
            "src/structures/span.c",
            "src/structures/trace.c",
            "src/structures/counters.c",
        ],
        include_dirs=["."],
        language="c",
        extra_compile_args=["-std=c23"],
    )
]

class BuildExt(build_ext):
    """Build extension with platform-specific flags"""
    def get_ext_filename(self, ext_name):
        return f"{ext_name}.so"
    
    def build_extensions(self):
        if sys.platform == "linux":
            for ext in self.extensions:
                ext.extra_link_args += ["-shared"]
        super().build_extensions()

        if os.path.exists("./otelanalyzer.so"):
            shutil.move("otelanalyzer.so", os.path.join("otelanalyzer", "otelanalyzer.so"))

setup(
    name="otelanalyzer",
    version="0.1",
    author="zkh-dot",
    author_email="kernzahar@gmail.com",
    python_requires=">=3.6",
    description="Pure C lib for python3 (or C if u wanna) to analyze traces check for compliance with the OpenTelemetry standard",
    classifiers=[
        'Programming Language :: Python :: 3',
        'License :: OSI Approved :: MIT License',  # License type
        'Operating System :: Linux',
    ],
    cmdclass={"build_ext": BuildExt},
    ext_modules=ext_modules,
)