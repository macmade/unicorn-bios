unicorn-bios
============

[![Build Status](https://img.shields.io/travis/macmade/unicorn-bios.svg?branch=master&style=flat)](https://travis-ci.org/macmade/unicorn-bios)
[![Coverage Status](https://img.shields.io/coveralls/macmade/unicorn-bios.svg?branch=master&style=flat)](https://coveralls.io/r/macmade/unicorn-bios?branch=master)
[![Issues](http://img.shields.io/github/issues/macmade/unicorn-bios.svg?style=flat)](https://github.com/macmade/unicorn-bios/issues)
![Status](https://img.shields.io/badge/status-active-brightgreen.svg?style=flat)
![License](https://img.shields.io/badge/license-mit-brightgreen.svg?style=flat)
[![Contact](https://img.shields.io/badge/contact-@macmade-blue.svg?style=flat)](https://twitter.com/macmade)  
[![Donate-Patreon](https://img.shields.io/badge/donate-patreon-yellow.svg?style=flat)](https://patreon.com/macmade)
[![Donate-Gratipay](https://img.shields.io/badge/donate-gratipay-yellow.svg?style=flat)](https://www.gratipay.com/macmade)
[![Donate-Paypal](https://img.shields.io/badge/donate-paypal-yellow.svg?style=flat)](https://paypal.me/xslabs)

About
-----

Basic BIOS emulator for Unicorn Engine.

![Screenshot](Assets/Screen.png "Screenshot")

### Usage:

    Usage: unicorn-bios [OPTIONS] BOOT_IMG
    
    Options:
        
        --help   / -h:  Displays help.
        --memory / -m:  The amount of memory to allocate for the virtual machine
                        (in megabytes). Defaults to 64.
        --break-int:    Breaks on interrupt calls.
        --break-iret:   Breaks on interrupt returns.
        --debug-video:  Turns on debug output for video services.
        --single-step:  Breaks on every instruction.

### Installation:

    brew install --HEAD macmade/tap/unicorn-bios

License
-------

unicorn-bios is released under the terms of the MIT license.

Repository Infos
----------------

    Owner:          Jean-David Gadina - XS-Labs
    Web:            www.xs-labs.com
    Blog:           www.noxeos.com
    Twitter:        @macmade
    GitHub:         github.com/macmade
    LinkedIn:       ch.linkedin.com/in/macmade/
    StackOverflow:  stackoverflow.com/users/182676/macmade
