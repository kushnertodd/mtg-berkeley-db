cp ../cmake-build-debug/bin/bdb_inet_lookup .
cp ../json/bdb_databases.json .
cp ../data/db/sample/*db .
docker build -t app_bdb_inet_lookup .
#docker build -t us-west3-docker.pkg.dev/tkllc-lucy-game-design/tkllc-lucy-game-design-repo/app_bdb_inet_lookup .
