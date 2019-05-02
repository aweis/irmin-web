#!/bin/bash

esy buildjs &&
JSFILE=`find . -name "IrminWebApp.bc.js"`;
cp $JSFILE ~/public/. &&
node $JSFILE
