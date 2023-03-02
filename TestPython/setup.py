from distutils.core import Extension, setup
from Cython.Build import cythonize
# define an extension that will be cythonized and compiled
ext1 = Extension(name="Math", sources=["Math.py"])
setup(ext_modules=cythonize(ext1, compiler_directives={'language_level' : "3"}))
