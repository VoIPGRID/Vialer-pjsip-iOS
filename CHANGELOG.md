# Changelog

All notable changes to this project will be documented in this file.

---
## [3.5](https://github.com/VoIPGRID/Vialer-pjsip-iOS/tree/3.5) (20/02/2020)

Released on Thursday, February 20, 2020

- Updated PJSIP to 2.10
- Updated h264 to 2.0.0
- Updated opus to 1.3.1
- Updated OpenSSL to 1.1.1d

## [3.4.0](https://github.com/VoIPGRID/Vialer-pjsip-iOS/tree/3.4) (09/09/2018)

Released on Tuesday, October 9, 2018

- Updated PJSIP to 2.8 [#28](https://github.com/VoIPGRID/Vialer-pjsip-iOS/pull/28)

## [3.3.6](https://github.com/VoIPGRID/Vialer-pjsip-iOS/tree/3.3.6) (06/04/2018)

Released on Friday, April 6, 2018

- Recompiled because of incomplete architectures.

## [3.3.5](https://github.com/VoIPGRID/Vialer-pjsip-iOS/tree/3.3.5) (13/03/2018)

Released on Tuesday, March 13, 2018

- Removed the compile script again causing errors.

## [3.3.4](https://github.com/VoIPGRID/Vialer-pjsip-iOS/tree/3.3.4) (13/03/2018)

Released on Tuesday, March 13, 2018

- Added a post compile script to the podspec to combine the VialerPJSIP split archives into one archive

## [3.3.2](https://github.com/VoIPGRID/Vialer-pjsip-iOS/tree/3.3.2) (06/03/2018)

Released on Tuesday, March 6, 2018

- Updated PJSIP to 2.7.2

## [3.3.0](https://github.com/VoIPGRID/Vialer-pjsip-iOS/tree/3.3.0) (11/01/2018)

Released on Monday, January 11, 2018

- Bitcode is enabled by default [#20](https://github.com/VoIPGRID/Vialer-pjsip-iOS/pull/20)
- IP v6 is compiled in to PJSIP

## [3.2.0](https://github.com/VoIPGRID/Vialer-pjsip-iOS/tree/3.2.0) (20/11/2017)

Released on Monday, November 20, 2017

- Updated PJSIP to 2.7.1
- Flag when compiling to ignore libsrtp when there is an independent webrtc build

## [3.1.0](https://github.com/VoIPGRID/Vialer-pjsip-iOS/tree/3.1.0) (20/10/2017)

Released on Friday, October 20, 2017

- Added the possibility to also use ZRTP

## [3.0.0](https://github.com/VoIPGRID/Vialer-pjsip-iOS/tree/3.0.0) (12/10/2017)

Released on Thursday, October 12, 2017

- Updated to PJSIP to release 2.7

## [2.0.1](https://github.com/VoIPGRID/Vialer-pjsip-iOS/tree/2.0.1) (16/02/2017)

Released on Thursday, February 16, 2017

### Changed

- Building on the iOS simulators works again [#12](https://github.com/VoIPGRID/Vialer-pjsip-iOS/issues/12)

## [2.0.0](https://github.com/VoIPGRID/Vialer-pjsip-iOS/tree/2.0.0) (15/02/2017)

Released on Wednesday, February 15, 2017

### Changed

- Updated to PJSIP to release 2.6
- OpenSSL is compiled into the pjsip source and is included into the VialerPJSIP.framework
- OPUS codec is compiled into the pjsip source and is included into the VialerPJSIP.framework
- OpenH264 is compiled into the pjsip source and is included into the VialerPJSIP.framework

## [1.1.0](https://github.com/VoIPGRID/Vialer-pjsip-iOS/tree/1.1.0) (12/08/2016)

Released on Thursday, December 8, 2016.

#### Changed

- Fixed the include issue for inttypes.h [#7](https://github.com/VoIPGRID/Vialer-pjsip-iOS/pull/7)
- PJSIP will no longer do symmetrical SDP negotiation. [#6](https://github.com/VoIPGRID/Vialer-pjsip-iOS/pull/6)
