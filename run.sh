#!/bin/bash

esy buildjs &&
JSFILE=`find . -name "IrminWebApp.js"`;
cp $JSFILE ~/public/. &&
node $JSFILE
