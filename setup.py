try:
    from setuptools import setup
    from setuptools import find_packages
    packages = find_packages()
except ImportError:
    from distutils.core import setup
    import os
    packages = [x.strip('./').replace('/','.') for x in os.popen('find -name "__init__.py" | xargs -n1 dirname').read().strip().split('\n')]

if bytes is str:
    raise Exception("This module is designed for python 3 only.")

setup(
    name='syscall_agent',
    version='0.0.1',
    python_requires='>=3.5',
    packages=packages,
    package_data = {
        'syscall_agent': ['native/copy_qemu.sh',
                  'native/main.c',
                  'native/Makefile',
                  ]
    },
    install_requires=[
    ],
    extras_require = {
    },
    description='A collection of multi-host & multi-target syscall agents.',
    url='https://github.com/angr/syscall-agent',
)
