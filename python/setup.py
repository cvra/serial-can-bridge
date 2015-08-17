#!/usr/bin/env python

from setuptools import setup

args = dict(
    name='serial_can_bridge',
    version='0.2',
    description='Implements a MessagePack API for our CAN-UART bridge.',
    packages=['can_bridge'],
    author='CVRA',
    author_email='info@cvra.ch',
    url='https://github.com/cvra',
    license='BSD',
    install_requires=[
        'msgpack-python'
        ]
)

setup(**args)
