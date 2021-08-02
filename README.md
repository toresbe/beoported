# beoported
Utility to use the Bang &amp; Olufsen Beoport PC2 on a Linux desktop

## Usage

### Installation

To pull in the newest version of libpc2, run

`git submodule init && git submodule update --recursive`

Then simply

`make && make install`

### Operation

`./pc2d [mode-char]`

Various mode characters have been used during development; 1 turns the device on, 0 turns it off.

You're most likely to want 'm', the main/monitor mode, which will listen for IR signal events and react appropriately.
