# Precompiled firmware binaries

## for ATtiny2313, 115200bps

* device: ATtiny2313 (not A-version)
* fuse: FF-DF-6F
* clock: quartz crystal, 9.216MHz
* serial: 115200bps

## Notice

* Since the binary code was compiled by avr-gcc and linked with avr-libc, so it may contain its fragment of the code (startup-code, et al).
  please see https://github.com/avrdudes/avr-libc for details of avr-libc.
