#!/bin/bash

esy buildjs &&
JSFILE=`find . -name "IrminWebApp.bc.js"`;
sed -i '1s|^|/**\n * @haste-ignore\n * @nolint\n */\n|' $JSFILE
cp $JSFILE ~/public/. &&
node $JSFILE
