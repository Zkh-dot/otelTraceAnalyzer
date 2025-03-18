from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import sys
import shutil, os

SOURCE_FILES = [
    "src/analyzer.c",
    
    "src/plugin_manager.c",
    "src/plugins/example_plugin.c",

    "src/hashstructs/hashset_itr.c",
    "src/hashstructs/hashmap.c",
    "src/hashstructs/hashset.c",

    "src/python_wrapper/py_structs.c",
    "src/python_wrapper/py_translator.c",
    "src/python_wrapper/py_analyzer.c",
    "src/python_wrapper/py_plugin_manager.c",

    "src/python_wrapper/init_module.c",

    "src/structures/servicemap.c",
    "src/structures/service.c",
    "src/structures/span.c",
    "src/structures/trace.c",
    "src/structures/counters.c",
]

ext_modules = [
    Extension(
        "otelanalyzer",
        sources=SOURCE_FILES,
        include_dirs=["."],
        language="c",
        extra_compile_args=["-std=c2x"],
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
    version="0.1.3",
    author="zkh-dot",
    author_email="kernzahar@gmail.com",
    python_requires=">=3.10",
    description="Pure C lib for python3 (or C if u wanna) to analyze traces check for compliance with the OpenTelemetry standard",
    classifiers=[
        'Programming Language :: Python :: 3',
        'License :: OSI Approved :: MIT License',  # License type
        'Operating System :: POSIX :: Linux',
    ],
    package_data={"otelanalyzer": ["*.c", "*.h", "*.pyi"]},
    cmdclass={"build_ext": BuildExt},
    long_description=open("README.md").read(),
    long_description_content_type="text/markdown",
    ext_modules=ext_modules,
)