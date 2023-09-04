sh rm-db-samples.sh
for f in `cat ../json/request-names.txt` ; do
  out=$f.txt
  sh request.sh "$f"
  if diff results/$out test/$out >/dev/null ; then
    echo "succeed $f"
  else
    echo "fail    $f"
  fi
done
