#!/usr/bin/env python
#-*- coding: utf-8 -*-

from __future__ import print_function

import sys

sys.path.append(sys.argv[1])

from PyQt5 import QtCore
from PyQt5 import QtWidgets

from PyKF5 import KCodecs

def main():
    app = QtWidgets.QApplication(sys.argv)

    assert(KCodecs.KEmailAddress.isValidSimpleAddress("konqi@kde.org"))
    assert(not KCodecs.KEmailAddress.isValidSimpleAddress("konqi"))

if __name__ == '__main__':
    sys.exit(main())
