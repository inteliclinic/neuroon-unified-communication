#!/bin/bash

echo -e "What would you like to do?\nBump [r]elease\nBump [m]ajor\nBump m[i]nor\nBump \
[p]atch\n[c]ancel"

read -n 1 ans

tag=$(git describe --tags --abbrev=0)

awk -F. '/[0-9]+\./{$(NF-1)++;print}' OFS=. <<< "$tag"

case "$ans" in
  "r")
    tag=$(awk -F. '/[0-9]+\./{$(NF-3)++;$(NF-2)=0;$(NF-1)=0;$NF=0;print}' OFS=. <<< "$tag");;
  "m")
    tag=$(awk -F. '/[0-9]+\./{$(NF-2)++;$(NF-1)=0;$NF=0;print}' OFS=. <<< "$tag");;
  "i")
    tag=$(awk -F. '/[0-9]+\./{$(NF-1)++;$NF=0;print}' OFS=. <<< "$tag");;
  "p")
    tag=$(awk -F. '/[0-9]+\./{$NF++;print}' OFS=. <<< "$tag");;
  *)
    echo "Cancel";;
esac

branch_name=$(git branch | grep "*" | sed "s/\* //" | sed -r "s/\x1B\[([0-9]{1,3}((;[0-9]{1,3})*)?)?[m|K]//g")

if [[ $branch_name == "master" ]]; then
  git tag $tag
  GIT_VERSION=`git describe --tags --abbrev=5 --dirty`

  echo "/**" > src/version.h
  echo "* @file    version.h" >> src/version.h
  echo "* @Author  atomatically generated header" >> src/version.h
  echo -e "* @brief   version informaction\n*\n*/\n" >> src/version.h
  echo "#ifndef VERSION_H" >> src/version.h
  echo -e "#define VERSION_H\n" >> src/version.h
  echo -e "#define GIT_VERSION \"$GIT_VERSION\"\n" >> src/version.h
  echo -e "#endif // VERSION_H" >> src/version.h

  git add src/version.h && git commit -m "Update version to $GIT_VERSION"
fi
