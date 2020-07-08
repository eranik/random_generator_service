#!/usr/bin/env bash

#build rng
function build_rng() {
    #Save current directory
    current_dir="${PWD}"

    #The folder in which we will build rng
    build_dir='./build'
    if [ -d $build_dir ]; then
        echo "Deleted folder: ${build_dir}"
        rm -rf $build_dir
    fi

    #Create building folder
    echo "Created building folder: ${build_dir}"
    mkdir $build_dir

    echo "Entering folder: ${build_dir}"
    cd $build_dir

    echo "Start building rng..."
    if [ $1 -eq 1 ]; then
        cmake .. -DBUILD_TESTING=YES $cmake_gen
    else
        cmake .. $cmake_gen
    fi

    #If errors then exit
    if [ "$?" != "0" ]; then
        exit -1
    fi
    
    $make_program $make_flags
    
    #If errors then exit
    if [ "$?" != "0" ]; then
        exit -1
    fi

    echo "Installing ..."
    sudo $make_program install

    #Go back to the current directory
    cd $current_dir
    #Ok!
}

make_program=make
make_flags=''
cmake_gen=''
parallel=1
nproc=$(nproc)

# simulate ninja's parallelism
case nproc in
 1)
    parallel=$(( nproc + 1 ))
    ;;
 2)
    parallel=$(( nproc + 1 ))
    ;;
 *)
    parallel=$(( nproc + 2 ))
    ;;
esac

if [ -f /bin/ninja ]; then
    make_program=ninja
    cmake_gen='-G Ninja'
else
    make_flags="$make_flags -j$parallel"
fi

if [ "$1" = "-t" ]; then
    build_rng 1
else
    build_rng 0
fi
