#
# Check for existence of explicit_bzero
#
# This was introduced in glibc 2.25 in 2017
#

include(CheckFunctionExists)
check_function_exists(explicit_bzero HAVE_EXPLICIT_BZERO)
