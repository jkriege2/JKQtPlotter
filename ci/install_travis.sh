#!/usr/bin/env bash

# Exit on error
set -e
# Echo each command
set -x

export SOURCE_DIR=`pwd`
export our_install_dir="$HOME/our_usr"

if [[ ! -d $HOME/conda_root/pkgs ]]; then
    rm -rf $HOME/conda_root
    if [[ "${TRAVIS_OS_NAME}" == "osx" ]]; then
        wget https://repo.continuum.io/miniconda/Miniconda3-latest-MacOSX-x86_64.sh -O miniconda.sh
    else
        wget https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh -O miniconda.sh
    fi
    bash miniconda.sh -b -p $HOME/conda_root
fi
export PATH="$HOME/conda_root/bin:$PATH"
conda config --set always_yes yes --set changeps1 no
conda config --add channels conda-forge --force
# Useful for debugging any issues with conda
conda info -a

conda_pkgs="qt cmake opencv doxygen cimg"

conda create -q -p $our_install_dir ${conda_pkgs}

# Since this script is getting sourced, remove error on exit
set +e
set +x

source activate $our_install_dir

cd $SOURCE_DIR;

