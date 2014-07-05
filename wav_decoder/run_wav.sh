#!/bin/bash
if [ $# -ne 1 ];then
echo $#
echo "Usage $0 <file_name/dir_name> "
exit
fi

if [ -d $1 ];then
iodir=$1
tests=`find $iodir -iname "*.wav"`
else
tests=$1
fi

#echo $tests
ref=ref.pcm
out=out.pcm
EXE=./a.out

for tst in ${tests[*]}
    do
    echo "Testing $tst"
    rm -f $out $ref
    gst-launch-0.10 filesrc location=$tst ! wavparse ! filesink location=$ref &> /dev/null
    $EXE $tst $out
    cmp $out $ref
    done

