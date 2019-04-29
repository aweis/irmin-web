#!/bin/bash

esy dune build &&
esy dune build executable/IrminWeb.re &&
esy dune build executable/IrminWeb.bc &&
esy buildjs &&
JSFILE=./_esy/default/store/b/irmin_web-74307f7a/default/executable/IrminWeb.js
cp $JSFILE ~/public/. &&
node $JSFILE
