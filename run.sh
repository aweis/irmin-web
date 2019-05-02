#!/bin/bash

esy dune build &&
esy dune build executable/IrminWebApp.re &&
esy dune build executable/IrminWebApp.bc &&
esy buildjs &&
JSFILE=`find . -name "IrminWebApp.js"`;
cp $JSFILE ~/public/. &&
node $JSFILE
