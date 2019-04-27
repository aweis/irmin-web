#!/bin/bash

esy dune build &&
esy dune build executable/IrminWeb.re &&
esy dune build executable/IrminWeb.bc &&
esy buildjs &&
node ./_esy/default/store/b/irmin_web-74307f7a/default/executable/IrminWeb.js
