#!/bin/sh

set -euf

srcdir="$(cd -- "${0%/*}/" && pwd -P)"

cd -- "$srcdir"
[ ! -d .git ] || git clean -xdf
autoreconf -fi

build () {
  test="${1:-default}"
  for compiler in cc clang gcc; do
    for standard in '' -std=c89 -std=c99 -std=c11; do
      for optimization in '' -O1 -O2 -O0 -Os -Og; do
        build_string="$(printf %s "$standard" | tr = _)"
        dir="build/${test}-${compiler}${optimization}${build_string}"
        rm -rf -- "$dir"
        mkdir -p -- "$dir"
        (
          build_flags="${standard} ${optimization}"
          cd -- "$dir"/
          printf %s\\n \
            '' "Building ${compiler} ${build_flags}: ${test}" ''
          CC="$compiler" CFLAGS="${build_flags} ${_CFLAGS} ${2:-}" \
          "$srcdir"/configure --enable-doc
          make install DESTDIR="$srcdir/$dir"/install V=1
          make check V=1
        )
      done
    done
  done
}

_CFLAGS='-Werror -Wall'

build
build 'ansi' '-ansi'
