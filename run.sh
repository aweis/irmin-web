#!/bin/bash

esy dune build &&
esy dune build executable/IrminTestApp.re &&
esy dune build executable/IrminTestApp.bc &&
esy buildjs &&
node _esy/default/build/default/executable/IrminTestApp.js
