#!/usr/bin/sh
#
# Execute cmake and rebuild Visual Studio project.
#
# syntax:
#    rebuild.sh [-V <vs_version>][-T <vs_type>]
#
## parameters:
#   -V <vs_version> The Visual Studio version, 2019 or 2022.
#                   If not set the script looks for an installed VS version
#                   2019 or 2022, in this order. Visual Studio has to be 
#                   installed in the default path.
#   -T <vs_type>    The Visual Studio type, Enterprise, Professional or
#                   Community. If not set the script looks for an installed VS
#                   type Enterprise, Professional or Community, in this order.
#
#
# Appropriate Visual Studio versions and types:
vsversions="2019 2022"
vstypes="Enterprise Professional Community"

# DO NOT CHANGE ANYTHING BEYOND THIS LINE
#========================================

function usage() {
    echo "Execute cmake and rebuild Visual Studio project."
    echo ""
    echo "Syntax:"
    echo "   rebuild.sh [-V <vs_version>][-T <vs_type>]"
    echo "Options:"
    echo "   -V <vs_version> The Visual Studio version, 2019 or 2022."
    echo "                   If not set the script looks for an installed VS version"
    echo "                   2019 or 2022, in this order. Visual Studio has to be"
    echo "                   installed in the default path."
    echo "   -T <vs_type>    The Visual Studio type, Enterprise, Professional or"
    echo "                   Community. If not set the script looks for an installed VS"
    echo "                   type Enterprise, Professional or Community, in this order."
}

vsversion=
vstype=

# Convert an absolute path with Windows syntax into a path
# with MINGW syntax.
as_mingw_path() {
    expr='s:\\:/:g'
    path=`echo $1 | sed $expr`
    drive=`echo $path | sed -ne "s:\(.\).*:/\L\1:p"`
    path=`echo $path | sed "s/^..//"`
    echo ${drive}${path}
}

# Get the base path of the Microsoft Visual Studio installation.
# $1: Microsoft Visual Studio version ($vsversion)
get_vsbasedir() {
    if [ "$1" == "2019" ]; then
        echo "C:\Program Files (x86)\Microsoft Visual Studio"
    else
        echo "C:\Program Files\Microsoft Visual Studio"
    fi
}

# Convert an absolute path with MINGW syntax into a path
# with Windows syntax.
as_windows_path() {
    path=`echo $1 | sed "s/^.//"`
    expr='s:/:\\:g'
    path=`echo $path | sed $expr`
    drive=`echo $path | sed "s:\(^.\).*:\U\1:"`
    path=`echo $path | sed "s/^.//"`
    echo $drive:$path
}

# Convert a MINGW path relative to the current directory
# into an absolute path with Windows syntax.
as_absolute_windows_path() {
    absdir=`pwd`
    absdir="$absdir/$1"
    absdir=$( as_windows_path $absdir )
    echo $absdir
}

# check string against a list of supported strings.
# $1: The string to check.
# $2: Space separated list of supported strings.
check_value() {
    if [ "x$1" == "x" ]; then
        return 0; # no value is valid
    fi
    for value in $2
    do
        if [ "$value" == "$1" ]; then
            return 0
        fi
    done
    return 1
}

while :
do
    case "$1" in
        --) shift; break;;
        -h) usage; exit 0;;
        -V)
            if [ -n "$2" ]; then
                vsversion=$2
                shift
            else
                echo "**** Error: Argument for $1 is missing" >&2
                usage
                exit 1
            fi;;
        -T)
            if [ -n "$2" ]; then
                vstype=$2
                shift
            else
                echo "**** Error: Argument for $1 is missing" >&2
                usage
                exit 1
            fi;;
        *) break;;
    esac
    shift
done

# Look for an appropriate Visual Studio installation to set all needed
# variables:
check_value "$vsversion" "$vsversions"
if [ $? -ne 0 ]; then
    echo "**** Error: Visual Studio version \"$vsversion\" is not supported."
    usage
    exit 1
fi

check_value "$vstype" "$vstypes"
if [ $? -ne 0 ]; then
    echo "**** Error: Visual Studio type \"$vstype\" is not supported."
    usage
    exit 1
fi

check_value "$vsversion" "2019 2022"
if [ $? -ne 0 ]; then
    echo "**** File ${vsversion_file} has unsupported value for VS_TYPE."
    exit 1
fi
vsbasedir=$( get_vsbasedir $vsversion )

if [ "x$vstype" == "x" ]; then
    if [ "x$vsversion" == "x" ]; then
        # No vstype or vsversion specified, look for it.
        for vsversion in $vsversions
        do
            for vstype in $vstypes
            do
                vsbasedir=$( get_vsbasedir $vsversion )
                msvcscript="$vsbasedir\\$vsversion\\$vstype\VC\Auxiliary\Build\vcvarsall.bat"
                if [ -f "$msvcscript" ]; then
                    break
                fi
            done
            if [ -f "$msvcscript" ]; then
                break
            fi
        done
    else
        # vsversion specified, look for vstype.
        for vstype in $vstypes
        do
            vsbasedir=$( get_vsbasedir $vsversion )
            msvcscript="$vsbasedir\\$vsversion\\$vstype\VC\Auxiliary\Build\vcvarsall.bat"
            if [ -f "$msvcscript" ]; then
                break
            fi
        done
    fi
else
    if [ "x$vsversion" == "x" ]; then
        # vstype specified, look for vsversion.
        for vsversion in $vsversions
        do
            vsbasedir=$( get_vsbasedir $vsversion )
            msvcscript="$vsbasedir\\$vsversion\\$vstype\VC\Auxiliary\Build\vcvarsall.bat"
            if [ -f "$msvcscript" ]; then
                break
            fi
        done
    else
        # both vstype and vsversion specified.
        vsbasedir=$( get_vsbasedir $vsversion )
        msvcscript="$vsbasedir\\$vsversion\\$vstype\VC\Auxiliary\Build\vcvarsall.bat"
    fi
fi

VSMSBUILDDIR="$vsbasedir\\$vsversion\\$vstype\\MSBuild\\Current\\Bin"
if [ ! -d "$VSMSBUILDDIR" ]; then
    echo "**** Microsoft Visual Studio path not found:"
    echo "**** $VSMSBUILDDIR"
    echo "**** Check Your Microsoft Visual Studio installation."
    exit 1
fi
echo "Using Visual Studio $vsversion $vstype"
cmake_version=` cmake --version | sed -n "s/.*\([0-9]\+\.[0-9]\+\.[0-9]\+\).*/\1/p;q"`
echo "Using cmake V${cmake_version}"
echo

vsmsbuilddir=$( as_mingw_path "$VSMSBUILDDIR")

# Prepare for cmake build.
case "$vsversion" in
    2019) generator="Visual Studio 16 2019";;
    2022) generator="Visual Studio 17 2022";;
esac

# 1. Setup cmake to create a Visual Studio solution file (*.sln)
# 2. Rebuild the solution for all platforms and configurations.
platforms="Win32 x64"

exit_code=0
sln_file=""
builddir=""
for platform in $platforms
do
    builddir=build${platform}
    if [ -d $builddir ]; then
        rm -rf $builddir
    fi
    cmake.exe -S . -B $builddir -G "$generator" -A $platform
    if [[ $? != 0 ]]; then
        echo "**** cmake reported an error. Aborted."
        exit 1
    fi
    cd ${builddir}
    sln_file=`ls -1 *.sln`
    if [ "x$sln_file" == "x" ]; then
        echo "**** Visual Studio solution file (*.sln) not found. Aborted."
        exit 1
    fi

    for configuration in Debug Release
    do
        logfile=Build-${platform}-${configuration}.log
        echo
        echo ==== Rebuild solution $platform $configuration ...
        echo For details see log-file "$logfile".
        "$vsmsbuilddir/MSBuild.exe" $sln_file -t:Rebuild -m -v:quiet -clp:Summary -fl -flp:Verbosity=minimal\;LogFile=$logfile -property:Configuration=${configuration}\;Platform=${platform}
        if [[ $? != 0 ]]; then
            echo "**** There was a build error. Aborted."
            exit_code=1
        fi
    done
    cd ..
done
exit $exit_code
