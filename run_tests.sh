set -e

CMAKE_BUILD_TYPE_TEST="Debug"
REBUILD=""
BUILD_DIR="build"
DONT_BUILD=""
DONT_BUILD_TESTS=""
CMAKE_FLAGS="-DCMAKE_VERBOSE_MAKEFILE:BOOL=ON"
INSTALL_DIR="$(pwd)/_install"
DO_DOCS=""
DONT_TEST=""
LIST_TESTS=""
WITH_ASAN=""
WITH_COVERAGE=""
TEST_FLAGS="-C ${CMAKE_BUILD_TYPE_TEST}"
NJOBS="8"
RUN_EPHOTO=""
PREPROCESS=""

while [[ $# -gt 0 ]]; do
    case $1 in
	-j )
	    NJOBS="$2"
	    shift
	    shift
	    ;;
	--ephoto )
	    RUN_EPHOTO="TRUE"
	    DONT_BUILD_TESTS="TRUE"
	    DONT_TEST="TRUE"
	    shift
	    ;;
	--build-dir )
	    BUILD_DIR="$2"
	    shift
	    shift
	    ;;
	--rebuild )
	    REBUILD="TRUE"
	    shift
	    ;;
	--dont-build )
	    DONT_BUILD="TRUE"
	    shift # past argument with no value
	    ;;
	--dont-build-tests )
	    DONT_BUILD_TESTS="TRUE"
	    shift
	    ;;
	--install-dir )
	    INSTALL_DIR="$2"
	    shift
	    shift
	    ;;
	-DCMAKE_INSTALL_PREFIX=* )
	    INSTALL_DIR="${1#-DCMAKE_INSTALL_PREFIX=}"
	    shift
	    ;;
	--docs )
	    DO_DOCS="TRUE"
	    DONT_TEST="TRUE"
	    DONT_BUILD="TRUE"
	    shift
	    ;;
	--dont-test )
	    DONT_TEST="TRUE"
	    shift # past argument with no value
	    ;;
	--list-tests )
	    LIST_TESTS="TRUE"
	    DONT_TEST="TRUE"
	    shift # past argument with no value
	    ;;
	--verbose )
	    TEST_FLAGS="${TEST_FLAGS} --output-on-failure -VV"
	    shift # past argument with no value
	    ;;
	--with-coverage )
	    WITH_COVERAGE="TRUE"
	    DONT_TEST="TRUE"
	    CMAKE_FLAGS_LIB="${CMAKE_FLAGS_LIB} -DTEST_COVERAGE=ON"
	    shift # past argument with no value
	    ;;
	--with-asan )
	    WITH_ASAN="TRUE"
	    CMAKE_FLAGS_LIB="${CMAKE_FLAGS_LIB} -DWITH_ASAN=ON"
	    shift # past argument with no value
	    ;;
	--preprocess )
	    PREPROCESS="src/Variables.cpp"
	    DONT_BUILD="TRUE"
	    DONT_TEST="TRUE"
	    shift # past argument with no value
	    ;;
	--preprocess-file )
	    PREPROCESS="$2"
	    DONT_BUILD="TRUE"
	    DONT_TEST="TRUE"
	    shift
	    shift
	    ;;
    esac
done

if [ -n "$REBUILD" ]; then
    if [ -d "$BUILD_DIR" ]; then
	rm -rf "$BUILD_DIR"
    fi
fi
if [ ! -d "$BUILD_DIR" ]; then
    mkdir $BUILD_DIR
fi

if [ -n "$PREPROCESS" ]; then
   gcc -E $PREPROCESS -I include/
fi

if [ -n "$DONT_BUILD_TESTS" ]; then
    CMAKE_FLAGS="${CMAKE_FLAGS} -DBUILD_TESTS:BOOL=OFF"
else
    CMAKE_FLAGS="${CMAKE_FLAGS} -DBUILD_TESTS:BOOL=ON"
fi

cd $BUILD_DIR
if [ ! -n "$DONT_BUILD" ]; then
    cmake .. $CMAKE_FLAGS $CMAKE_FLAGS_LIB
    cmake --build . --config ${CMAKE_BUILD_TYPE_TEST} -- -j ${NJOBS}
    # Need install here to ensure that cmake config files are in place
    cmake --install . --prefix "$INSTALL_DIR"
fi

if [ ! -n "$DONT_TEST" ]; then
    ctest $TEST_FLAGS --stop-on-failure
fi
if [ -n "$LIST_TESTS" ]; then
    ctest -N
fi
if [ -n "$WITH_COVERAGE" ]; then
    make coverage
fi
if [ -n "$RUN_EPHOTO" ]; then
    cd ..
    # ./$BUILD_DIR/ePhoto -d 4 --enzyme InputEnzyme.txt --grn InputGRNC_MATLAB.txt --outputParam  # -T 25
    python utils/compare_matlab.py ../ePhotosynthesis -d 4 --matlab /Applications/MATLAB_R2024b.app/bin/matlab
    # diff ../ePhotosynthesis/EPS_init.txt EPS_init.txt &> diff_EPS_init.txt
    # diff ../ePhotosynthesis/EPS_rate.txt EPS_rate.txt &> diff_EPS_rate.txt
    diff ../ePhotosynthesis/EPS_final.txt EPS_final.txt &> diff_EPS_final.txt
    cd $BUILD_DIR
fi

if [ -n "$DO_DOCS" ]; then
    path_to_doxygen=$(which doxygen)
    if [ -x "$path_to_doxygen" ]; then
	cmake .. $CMAKE_FLAGS -DBUILD_DOCS=ON -DBUILD_TESTS=OFF -DDOXYGEN_CHECK_MISSING=ON
	cmake --build . $CONFIG_FLAGS --target docs --config ${CMAKE_BUILD_TYPE_TEST}
	# Need install here to ensure that cmake config files are in place
	# cmake --install . --prefix "$INSTALL_DIR" $CONFIG_FLAGS
    fi
fi
cd ..
