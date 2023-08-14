for f in `cat ../json/request-names.txt` ; do
  out=$f.txt
  sh request.sh $f
  #grep -v milliseconds <test/$out >test/$out
  grep -v '"score"' results/$out >/tmp/1
  grep -v '"score"' test/$out >/tmp/2
  if diff /tmp/1 /tmp/2 >/dev/null ; then
    echo "succeed $f"
  else
    echo "fail    $f"
  fi
done
