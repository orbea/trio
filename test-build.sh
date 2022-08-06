#!/bin/sh

set -euf

srcdir="$(cd -- "${0%/*}/" && pwd -P)"

cd -- "$srcdir"

if [ -d .git ]; then
  git clean -xdf
else
  rm -rf build
fi

check=

while [ $# -gt 0 ]; do
  check="${check} $1"
  shift
done

autoreconf -fi

compiler='cc clang gcc'

eval "set -- $compiler"
for cc do
  confdir="build/configure/$cc"
  mkdir -p -- "$confdir"
  (
    cd -- "$confdir"/
    printf %s\\n '' "Configuring for $cc" ''
    CC="$cc" "$srcdir"/configure --enable-doc
  )
done

build () {
  test="${1:-default}"
  cflags="${2:-}"
  rm -rf -- build/"$test"
  mkdir -p -- build/"$test"
  eval "set -- $compiler"
  for cc do
    [ "$cc" != 'clang' ] || ! check_asan "$test" || continue
    for standard in '' -std=c89 -std=c99 -std=c11; do
      # TODO: Add -O3 and -Ofast
      for optimization in '' -O1 -O2 -O0 -Os -Og; do
        build_string="$(printf %s "$standard" | tr = _)"
        dir="build/${test}/${cc}${optimization}${build_string}"
        rm -rf -- "$dir"
        cp -r -- build/configure/"$cc" "$dir"
        (
          cd -- "$dir"/
          build_flags="${standard} ${optimization}"
          all_flags="${build_flags} ${_CFLAGS} ${cflags}"
          printf %s\\n '' "Building ${cc} ${build_flags}: ${test}" ''
          make install V=1 CFLAGS="$all_flags" DESTDIR="$srcdir/$dir"/install
          make check V=1 CFLAGS="$all_flags"
        )
      done
    done
  done
}

check_asan () {
  case "$1" in
    *san ) return 0 ;;
    * ) return 1 ;;
  esac
}

###########
## Tests ##
###########

_CFLAGS='-Werror -Wall'
_CFLAGS_ASAN='-g -fno-omit-frame-pointer'

if [ -z "${check}" ]; then
  # TODO: Indeterminate non-reproducible failures with lsan
  check="default ansi lto asan tsan ubsan"
fi

for i in $(printf %s "${check}"); do
  case "$i" in
    ansi ) build 'ansi' '-ansi' ;;
    lto ) build 'lto' '-flto' ;;
    asan ) build 'asan' "${_CFLAGS_ASAN} -fsanitize=address" ;;
    lsan ) build 'lsan' "${_CFLAGS_ASAN} -fsanitize=leak" ;;
    tsan ) build 'tsan' "${_CFLAGS_ASAN} -fsanitize=thread" ;;
    ubsan ) build 'ubsan' "${_CFLAGS_ASAN} -fsanitize=undefined" ;;
    default ) build ;;
    * ) printf %s\\n "ERROR: Unknown '$i' test." >&2; exit 1 ;;
  esac
done
