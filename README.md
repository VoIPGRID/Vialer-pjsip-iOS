# Vialer-pjsip-iOS

The Vialer PJSIP pod exposes the <a href="http://www.pjsip.org/">PJSIP</a> library as Cocoapod.

The most important part of the project is the "vialerbuild" script. It will:
- download a specified version of PJSIP from SVN.
- compile PJSIP for all iOS architectures.
- Decide whether to compile OpenSSL, OpenH264 and OPUS into the PJSIP source. By default they are included for all iOS architectures.
- build a fat static library.
- wrap the fat library in a framework.

## Status
Currently active maintained

### Usage:

## Requirements
For MacOS a newer version of nasm is required when you want to build openh264.
```bash
brew install nasm
```

## Running
Install via CocoaPods:

```ruby
platform :ios, '9.0'
pod 'Vialer-pjsip-iOS'
```

## Contributing

See the [CONTRIBUTING.md](CONTRIBUTING.md) file on how to contribute to this project.

## Contributors

See the [CONTRIBUTORS.md](CONTRIBUTORS.md) file for a list of contributors to the project.

## Roadmap

### Changelog

The changelog can be found in the [CHANGELOG.md](CHANGELOG.md) file.

#### Current issues:
When using the pod as a dependency in another podspec, linting generates the following error:
```
The 'Pods' target has transitive dependencies that include static binaries: VialerPJSIP.framework
```
As a workaround you can use
```
pod spec lint --use-libraries
```

The library is a fat static library and cannot be included in an use_frameworks! enviroment at this moment. We're trying to find a solution for this and implement a way to create a dynamic library.

### Future

- Make the libarary static build for Swift only projects.
- Staying up-to-date with PJSIP releases

## Get in touch with a developer

If you want to report an issue see the [CONTRIBUTING.md](CONTRIBUTING.md) file for more info.

We will be happy to answer your other questions at {opensource@wearespindle.com or insert alias}

## License

Vialer-pjsip-iOS is made available under the GNU General Public License v3.0 license. See the [LICENSE file](LICENSE) for more info.
