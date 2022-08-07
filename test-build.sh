#!/bin/sh

set -euf

srcdir="$(cd -- "${0%/*}/" && pwd -P)"

cd -- "$srcdir"

if [ -d .git ]; then
  git clean -xdf
else
  rm -rf build
fi

exists () {
  r=0; cwd="$(pwd)"
  while [ $# -gt 0 ]; do
    v=1; arg="$1"; shift
    case "$arg" in
      ''|*/ )
        :
      ;;
      /* )
        if [ -f "$arg" ] && [ -x "$arg" ]; then
          printf %s\\n "$arg"
          v=0
        fi
      ;;
      ./* )
        if [ -f "$arg" ] && [ -x "$arg" ]; then
          pre="$(cd -- "${arg%%/*}/" && pwd)"
          printf %s\\n "${pre%/}/$arg"
          v=0
        fi
      ;;
      */* )
        if [ -f "$arg" ] && [ -x "$arg" ]; then
          printf %s\\n "$(cd -- "${arg%%/*}/.." && pwd)/$arg"
          v=0
        fi
      ;;
      * )
        if [ -n "${PATH+x}" ]; then
          p=":${PATH:-$cwd}"
          while [ "$p" != "${p#*:}" ] && [ -n "${p#*:}" ]; do
            p="${p#*:}"; x="${p%%:*}"; z="${x:-$cwd}"; d="${z%/}/$arg"
            if [ -f "$d" ] && [ -x "$d" ]; then
              case "$d" in
                /* ) : ;;
                ./* ) pre="$(cd -- "${d%/*}/" && pwd)"; d="${pre%/}/$d" ;;
                * ) d="$(cd -- "${d%/*}/" && pwd)/$arg" ;;
              esac
              printf %s\\n "$d"
              v=0
              break
            fi
          done
        fi
      ;;
    esac
    [ $v = 0 ] || r=1
  done
  return $r
}

check=

while [ $# -gt 0 ]; do
  check="${check} $1"
  shift
done

if exists doxygen >/dev/null 2>&1; then
  doc='--enable-doc'
else
  printf %s\\n 'WARNING: Building without doxygen' >&2
  doc=
fi

autoreconf -fi

compiler='cc clang gcc'

eval "set -- $compiler"
for cc do
  confdir="build/configure/$cc"
  mkdir -p -- "$confdir"
  (
    cd -- "$confdir"/
    printf %s\\n '' "Configuring for $cc" ''
    CC="$cc" "$srcdir"/configure $doc
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
    eval "set -- $standard"
    for std do
      libs=
      ! check_cxx "$std" || libs='-lstdc++'
      # TODO: Add -O3 and -Ofast
      for optimization in '' -O1 -O2 -O0 -Os -Og; do
        build_string="$(printf %s "$std" | tr = _ | tr -d '[:space:]')"
        dir="build/${test}/${cc}${optimization}${build_string}"
        rm -rf -- "$dir"
        cp -r -- build/configure/"$cc" "$dir"
        (
          cd -- "$dir"/
          build_flags="${std} ${optimization}"
          all_flags="${build_flags} ${_CFLAGS} ${cflags}"
          printf %s\\n '' "Building ${cc} ${build_flags}: ${test}" ''
          make install V=1 CFLAGS="$all_flags" LDFLAGS="$libs" \
            DESTDIR="$srcdir/$dir"/install
          make check V=1 CFLAGS="$all_flags" LDFLAGS="$libs"
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

check_cxx () {
  case "$1" in
    *c++* ) return 0 ;;
    * ) return 1 ;;
  esac
}

###########
## Tests ##
###########

_CFLAGS='-Werror -Wall -Wextra'
_CFLAGS_ASAN='-g -fno-omit-frame-pointer'

standard=
all_std='default c89 c99 c11 c++ c++98 c++11 c++14 c++17'

for i in $(printf %s "${all_std}"); do
  case "$i" in
    c89 ) standard="${standard} -std=c89" ;;
    c99 ) standard="${standard} -std=c99" ;;
    c11 ) standard="${standard} -std=c11" ;;
    c++ ) standard="${standard} -xc++" ;;
    c++98 ) standard="${standard} '-xc++ -std=c++98'" ;;
    c++11 ) standard="${standard} '-xc++ -std=c++11'" ;;
    c++14 ) standard="${standard} '-xc++ -std=c++14'" ;;
    c++17 ) standard="${standard} '-xc++ -std=c++17'" ;;
    default ) standard="${standard} ''" ;;
    * ) printf %s\\n "ERROR: Unknown '$i' standard." >&2; exit 1 ;;
  esac
done

if [ -z "${check}" ]; then
  # TODO: Indeterminate non-reproducible failures with lsan
  check='default ansi lto asan tsan ubsan'
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
