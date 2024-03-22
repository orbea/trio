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
    if check_asan "$test" "$cc" "$musl"; then
      eval "set -- $standard"
      for std do
        cpp=
        libs=
        if check_cxx "$std"; then
          cpp='-xc++'
          libs='-lstdc++'
        fi
        # TODO: Add -O3 and -Ofast
        for optimization in '' -O1 -O2 -O0 -Os -Og; do
          build_string="$(printf %s "$std" | tr = _ | tr -d '[:space:]')"
          dir="build/${test}/${cc}${optimization}${build_string}"
          rm -rf -- "$dir"
          cp -r -- build/configure/"$cc" "$dir"
          (
            cd -- "$dir"/
            print_flags="${std} ${optimization}"
            build_flags="$(printf %s "${print_flags}" | sed 's/-xc++//')"
            all_flags="${build_flags} ${_CFLAGS} ${cflags}"
            printf %s\\n '' "Building ${cc} ${print_flags}: ${test}" ''
            make install V=1 CFLAGS="$all_flags" CPPFLAGS="$cpp" LDFLAGS="$libs" \
              DESTDIR="$srcdir/$dir"/install
            make check V=1 CFLAGS="$all_flags" CPPFLAGS="$cpp" LDFLAGS="$libs"
          )
        done
      done
    fi
  done
}

check_asan () {
  case "$1" in
    *san )
      # TODO: sanitizers are disabled with clang and musl
      if [ "$2" = 'clang' ] || [ -n "${3}" ]; then
        return 1
      fi
    ;;
  esac

  return 0
}

check_cxx () {
  case "$1" in
    *c++* ) return 0 ;;
    * ) return 1 ;;
  esac
}

if ldd "$(exists ls)" | grep -q musl; then
  printf %s\\n 'WARNING: Skipping the sanitizer tests on musl systems' >&2
  musl='1'
else
  musl=
fi

###########
## Tests ##
###########

_CFLAGS='-Werror -Wall -Wextra'
_CFLAGS_ASAN='-g -fno-omit-frame-pointer'

standard=
all_std='default c90 c99 c11 c17 c2x c++ c++98 c++11 c++14 c++17 c++20 c++23'

for i in $(printf %s "${all_std}"); do
  case "$i" in
    c[0-9]* ) standard="${standard} -std=$i -std=gnu${i#?}" ;;
    c++ ) standard="${standard} -x$i" ;;
    c++* ) standard="${standard} '-xc++ -std=$i' '-xc++ -std=gnu${i#?}'" ;;
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
    ansi ) build "$i" '-ansi' ;;
    lto ) build "$i" '-flto' ;;
    asan ) build "$i" "${_CFLAGS_ASAN} -fsanitize=address" ;;
    lsan ) build "$i" "${_CFLAGS_ASAN} -fsanitize=leak" ;;
    tsan ) build "$i" "${_CFLAGS_ASAN} -fsanitize=thread" ;;
    ubsan ) build "$i" "${_CFLAGS_ASAN} -fsanitize=undefined" ;;
    default ) build ;;
    * ) printf %s\\n "ERROR: Unknown '$i' test." >&2; exit 1 ;;
  esac
done
