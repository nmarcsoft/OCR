# -*- coding: utf-8 -*-
"""
Created on Wed Nov 24 18:35:35 2021

@author: marti
"""

from mnist import MNIST

mndata = MNIST('samples')

images, labels = mndata.load_testing()
print(labels[0])