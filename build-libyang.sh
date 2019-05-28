#!/bin/sh

set -e

die() {
	echo "error: $*"
	echo "usage: $0 [--static] --src=DIR --build=DIR [--install=DIR]"
	exit 1
} >&2

enable_static=OFF

for arg in "$@"; do
	case "$arg" in
	--static)
		enable_static=ON
		;;
	--src=*)
		src_dir=${arg#--src=}
		;;
	--build=*)
		build_dir=${arg#--build=}
		;;
	--install=*)
		install_dir=${arg#--install=}
		;;
	*)
		die "Invalid argument: $arg"
		;;
	esac
done

if [ -z "$src_dir" ]; then
	die "Missing --src= argument"
fi
if [ -z "$build_dir" ]; then
	die "Missing --build= argument"
fi
if ! [ -d "$src_dir" ]; then
	die "Source directory $src_dir does not exist"
fi
if [ "$src_dir" = "$build_dir" ]; then
	die "Source and build directories must be different"
fi

cmake -S "$src_dir" -B "$build_dir" \
	-DENABLE_STATIC=$enable_static \
	-DCMAKE_BUILD_TYPE=release \
	-DCMAKE_C_FLAGS=-fPIC \
	-DENABLE_BUILD_TESTS=OFF \
	-DENABLE_VALGRIND_TESTS=OFF \
	-DENABLE_CALGRIND_TESTS=OFF \
	-DCMAKE_INSTALL_PREFIX=${install_dir:-/} \
	-DGEN_LANGUAGE_BINDINGS=0

make -C "$build_dir" -j$(nproc)

if [ -n "$install_dir" ]; then
	make -C "$build_dir" install
fi
