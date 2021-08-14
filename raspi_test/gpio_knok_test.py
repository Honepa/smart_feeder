#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Aug 14 16:14:06 2021

@author: honepa
"""

import gpiozero

b1 = gpiozero.LED(27)
b2 = gpiozero.LED(22)

b2.off()
b1.off()

knok = gpiozero.Button("GPIO17")

k = 0

while k < 10:
    b1.on()
    if not knok.value:
        k += 1
<<<<<<< HEAD
	while knok.value:
		b +=2
b1.off()
=======
        while knok.value:
            b += 2
b1.off()
>>>>>>> 38e7269d4e0a5df7f165873976decb12d1278552
