#!/bin/bash

## Command line parameters
if [[ $# != 2 ]]; then
    cat <<USAGE
usage:
  $0 <refspec> <version>

  Creates tar source package from <refspec> (e.g. a git branch).
  Files and directories are named after <version>.
  example:
    $0 origin/1.0.0 1.0.0-rc1
USAGE
    exit 1
fi

BRANCH=$1
VERSION=$2

echo "Creating tar archive..."
git archive --format=tar --prefix=XQueryEvaluator-${VERSION}-src/ ${BRANCH} | gzip > XQueryEvaluator-${VERSION}.tar.gz || exit 0