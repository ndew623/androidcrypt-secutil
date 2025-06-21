# Change Log

v1.0.7

- Expand explicit\_bzero() use to any Unix-Like system (if present)

v1.0.6

- Updated SecureArray to not be constrained to trivial types (loosing the
  previous constraint)
- Changed the SecureArray to allow an initializer smaller than the array size,
  which aligns with the behavior of std::array
- Revised build options for Windows to enable additional warnings
- Updated dependencies
- Change the logic for checking for explicit\_bzero support

v1.0.5

- Corrected a bug in the logic that checks for explicit\_bzero due to
  the introduction of FreeBSD support

v1.0.4

- Updated build to check for explicit\_bzero() on FreeBSD

v1.0.3

- Updated to latest Simple Test Framework (STF)

v1.0.2

- Improved the SecureAllocator object

v1.0.1

- Test for explicit\_bzero() on Linux, as that was only introduced in 2017

v1.0.0

- Initial Release
