from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

ext_modules = [Extension('main',
                         ['main.pyx'],
                         libraries=['mal'],
                         library_dirs=['lib/'])]

setup(name = 'work extension module',
      cmdclass = {'build_ext': build_ext},
      ext_modules = ext_modules)