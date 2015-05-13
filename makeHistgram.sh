pgmfiles=`ls *.pgm`

for pgmfile in $pgmfiles
do
    if [[ "$pgmfile" =~ _hist.pgm$ ]]; then
        continue
    fi
    ./makeHistgram $pgmfile
done
