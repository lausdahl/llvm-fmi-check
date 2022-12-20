#!/bin/sh

opt-15 -load-pass-plugin ./build/analysis/lib/libFmiCheckHE.so -passes=fmi-check -disable-output $1
#opt-15 -load-pass-plugin ./build/analysis/lib/libFmiCheck.so -passes=fmi-check -disable-output $1
