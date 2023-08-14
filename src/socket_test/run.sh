set d=$(docker ps -qa)
if [ -n "$d" ] ; then
  # docker rm -f $(docker ps -qa)
  docker rm -f $d
fi
docker run -d -p 8080:8080 server_app 
docker logs -f $(docker ps -qa) &
