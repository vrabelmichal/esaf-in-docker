#!/bin/bash
# based on thisroot.sh

drop_from_path()
{
   # Assert that we got enough arguments
   if test $# -ne 2 ; then
      echo "drop_from_path: needs 2 arguments"
      return 1
   fi

   p=$1
   drop=$2

   newpath=`echo $p | sed -e "s;:${drop}:;:;g" \
                          -e "s;:${drop};;g"   \
                          -e "s;${drop}:;;g"   \
                          -e "s;${drop};;g"`
}

if [ -n "${ESAFDIR}" ] ; then
   old_esafdir=${ESAFDIR}
fi

if [ "x${BASH_ARGV[0]}" = "x" ]; then
    if [ ! -f bin/thisesaf.sh ]; then
        echo ERROR: must "cd where/esaf/is" before calling ". bin/thisesaf.sh" for this version of bash!
        ESAFDIR=; export ESAFDIR
        return 1
    fi
    ESAFDIR="$PWD"; export ESAFDIR
else
    # get param to "."
    thisesaf=$(dirname ${BASH_ARGV[0]})
#     ESAFDIR=$(cd ${thisesaf}/..;pwd); export ESAFDIR
    ESAFDIR=$(cd ${thisesaf};pwd); export ESAFDIR
fi

ESAFINSTALL="$ESAFDIR"; export ESAFINSTALL
ESAFTMP="$ESAFDIR"/tmp; export ESAFTMP

manpathpart="man"
binpathpart="bin/`uname -m`"
libpathpart="lib/`uname -m`"

if [ -n "${old_esafdir}" ] ; then
   if [ -n "${PATH}" ]; then
      drop_from_path "$PATH" "${old_esafdir}/${binpathpart}"
      PATH=$newpath
   fi
   if [ -n "${LD_LIBRARY_PATH}" ]; then
      drop_from_path "$LD_LIBRARY_PATH" "${old_esafdir}/${libpathpart}"
      LD_LIBRARY_PATH=$newpath
   fi
#    if [ -n "${DYLD_LIBRARY_PATH}" ]; then
#       drop_from_path $DYLD_LIBRARY_PATH ${old_esafdir}/${libpathpart}
#       DYLD_LIBRARY_PATH=$newpath
#    fi
#    if [ -n "${SHLIB_PATH}" ]; then
#       drop_from_path $SHLIB_PATH ${old_esafdir}/${libpathpart}
#       SHLIB_PATH=$newpath
#    fi
#    if [ -n "${LIBPATH}" ]; then
#       drop_from_path $LIBPATH ${old_esafdir}/${libpathpart}
#       LIBPATH=$newpath
#    fi
#    if [ -n "${PYTHONPATH}" ]; then
#       drop_from_path $PYTHONPATH ${old_esafdir}/${libpathpart}
#       PYTHONPATH=$newpath
#    fi
#    if [ -n "${MANPATH}" ]; then
#       drop_from_path $MANPATH ${old_esafdir}/$manpathpart
#       MANPATH=$newpath
#    fi
fi

# if [ -z "${MANPATH}" ]; then
#    # Grab the default man path before setting the path to avoid duplicates 
#    if `which manpath > /dev/null 2>&1` ; then
#       default_manpath=`manpath`
#    else
#       default_manpath=`man -w 2> /dev/null`
#    fi
# fi

if [ -z "${PATH}" ]; then
   PATH="$ESAFDIR/$binpathpart"; export PATH
else
   PATH="$ESAFDIR/$binpathpart":$PATH; export PATH
fi

if [ -z "${LD_LIBRARY_PATH}" ]; then
   LD_LIBRARY_PATH="$ESAFDIR/$libpathpart"; export LD_LIBRARY_PATH       # Linux, ELF HP-UX
else
   LD_LIBRARY_PATH="$ESAFDIR/$libpathpart":$LD_LIBRARY_PATH; export LD_LIBRARY_PATH
fi

# if [ -z "${DYLD_LIBRARY_PATH}" ]; then
#    DYLD_LIBRARY_PATH="$ESAFDIR/$libpathpart"; export DYLD_LIBRARY_PATH   # Mac OS X
# else
#    DYLD_LIBRARY_PATH="$ESAFDIR/$libpathpart":$DYLD_LIBRARY_PATH; export DYLD_LIBRARY_PATH
# fi
# 
# if [ -z "${SHLIB_PATH}" ]; then
#    SHLIB_PATH="$ESAFDIR/$libpathpart"; export SHLIB_PATH                 # legacy HP-UX
# else
#    SHLIB_PATH="$ESAFDIR/$libpathpart":$SHLIB_PATH; export SHLIB_PATH
# fi
# 
# if [ -z "${LIBPATH}" ]; then
#    LIBPATH="$ESAFDIR/$libpathpart"; export LIBPATH                       # AIX
# else
#    LIBPATH="$ESAFDIR/$libpathpart":$LIBPATH; export LIBPATH
# fi
# 
# if [ -z "${PYTHONPATH}" ]; then
#    PYTHONPATH="$ESAFDIR/$libpathpart"; export PYTHONPATH
# else
#    PYTHONPATH="$ESAFDIR/$libpathpart":$PYTHONPATH; export PYTHONPATH
# fi
# 
# if [ -z "${MANPATH}" ]; then
#    MANPATH=`dirname $ESAFDIR/man`:${default_manpath}; export MANPATH
# else
#    MANPATH=`dirname $ESAFDIR/man`:$MANPATH; export MANPATH
# fi
# 
if [ "x`root-config --arch | grep -v win32gcc | grep -i win32`" != "x" ]; then
  ESAFDIR="`cygpath -w $ESAFDIR`"
fi

if [ "x`root-config --arch | grep -v win64gcc | grep -i win64`" != "x" ]; then
  ESAFDIR="`cygpath -w $ESAFDIR`"
fi

unset old_esafdir
unset thisesaf

