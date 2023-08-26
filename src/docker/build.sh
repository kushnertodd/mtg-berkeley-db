cp ../cmake-build-debug/bin/mtg_app .
cp ../json/bdb_databases.json .
cp ../data/db/*db .
docker build -t mtg_app .
#docker build -t us-west3-docker.pkg.dev/tkllc-lucy-game-design/tkllc-lucy-game-design-repo/app_bdb_inet_lookup .
