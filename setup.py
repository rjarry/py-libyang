#!/usr/bin/env python
# Copyright (c) 2018 Robin Jarry
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

from distutils import log
from distutils.command.build_clib import build_clib
import multiprocessing
import os
import subprocess
import sys

import setuptools
from setuptools.command.build_ext import build_ext


CFFI_REQ = 'cffi>=1.7,!=1.11.3'
INSTALL_REQS = []
SETUP_REQS = ['wheel']
if '_cffi_backend' not in sys.builtin_module_names:
    INSTALL_REQS.append(CFFI_REQ)
    SETUP_REQS.append(CFFI_REQ)
HERE = os.path.abspath(os.path.dirname(__file__))


class BuildCLib(build_clib):

    def run(self):
        if not self.libraries:
            return
        log.info('Building libyang C library ...')
        cmd = [
            os.path.join(HERE, 'build-libyang.sh'),
            '--static',
            '--src=%s' % os.path.join(HERE, 'clib'),
            '--build=%s' % self.build_temp,
            '--install=%s' % self.build_temp,
        ]
        log.debug('+ %s' % ' '.join(cmd))
        subprocess.check_call(cmd)

    def get_library_names(self):
        if not self.libraries:
            return []
        return ['pcre', 'metadata', 'yangdata', 'nacm', 'user_date_and_time']


class BuildExt(build_ext):

    def run(self):
        if self.distribution.has_c_libraries():
            tmp = os.path.abspath(
                self.get_finalized_command('build_clib').build_temp)
            self.include_dirs.append(os.path.join(tmp, 'include'))
            self.library_dirs.append(tmp)
        return build_ext.run(self)


LIBRARIES = []
if os.environ.get('LIBYANG_INSTALL') != 'system':
    LIBRARIES.append(('yang', {'sources': ['clib']}))


setuptools.setup(
    name='libyang',
    version='0.16.78',
    description='CFFI bindings to libyang',
    long_description=open('README.rst').read(),
    url='https://github.com/rjarry/libyang-cffi',
    license='MIT',
    author='Robin Jarry',
    author_email='robin@jarry.cc',
    keywords=['libyang', 'cffi'],
    classifiers=[
        'Development Status :: 4 - Beta',
        'Environment :: Console',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: MIT License',
        'Operating System :: Unix',
        'Programming Language :: Python :: 2.7',
        'Programming Language :: Python :: 3.5',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: 3.7',
        'Topic :: Software Development :: Libraries',
    ],
    packages=['libyang'],
    zip_safe=False,
    include_package_data=True,
    setup_requires=SETUP_REQS,
    install_requires=INSTALL_REQS,
    cffi_modules=['cffi/build.py:BUILDER'],
    libraries=LIBRARIES,
    cmdclass={
        'build_clib': BuildCLib,
        'build_ext': BuildExt,
    },
)
