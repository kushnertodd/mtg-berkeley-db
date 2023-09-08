sh rm-db-samples.sh
for f in `cat ../json/request-names.txt` ; do
  if [ $f != card_select_all_decks ] ; then
  out="$f.txt"
  rm -f test/$out
  sh request.sh "$f"
  if diff results/$out test/$out >/dev/null ; then
    echo "succeed $f"
  else
    echo "fail    $f"
  fi
  fi
done
f=card_select_all_decks
out=$f.txt
rm -f test/$out
sh request.sh "$f"
if diff results/$out test/$out >/dev/null ; then
  echo "succeed $f"
else
  echo "fail    $f"
fi
