#!/bin/bash

WHERE_TO_PUT_CODE=$1

esy buildjs &&
JSFILE=`find . -name "IrminWebApp.bc.js"`;
sed -i '1s|^|/**\n * @haste-ignore\n * @nolint\n */\n|' $JSFILE
cp $JSFILE ~/public/. &&
if [ -d "$WHERE_TO_PUT_CODE" ]; then
  echo $WHERE_TO_PUT_CODE
  cp "$JSFILE" "$WHERE_TO_PUT_CODE/Hecate.js"
fi &&
node $JSFILE
