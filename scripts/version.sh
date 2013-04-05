#!/bin/bash
rm -f ../inc/version.h

LOCALVAR=`git rev-list HEAD | wc -l | awk '{print $1}'`

CODE_VERSION=$LOCALVAR
COMMIT_HASH=$(git rev-list HEAD -n 1 | cut -c 1-8)

cat ../inc/version.h.template | sed s/\$CODE_VERSION/$CODE_VERSION/g | sed s/\$COMMIT_HASH/$COMMIT_HASH/g > ../inc/version.h

echo "Generated" $CODE_VERSION
echo $CODE_VERSION
echo $COMMIT_HASH