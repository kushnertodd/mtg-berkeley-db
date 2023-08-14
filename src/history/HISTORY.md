# Version history

### tag v3.0_dev-bdb_inet_lookup-working-for-all-requests
bdb_inet_lookup runs for all scr/json requests in check.sh:
- check.sh kludged, greps "score" out of select name, title: why does it vary?
- needs to be dockerized

## v3.2

## v3.1
Google cloud installation in 'Google cloud install instructions.docx' works.

## v3.0
The prototype actor/movie screen works. 
Run from src/scripts with:
```
$ sh bdb_unix_lookup.sh &
$ sh imdb_app.sh &
```
## v2.0_oatpp
The web server version runs.
The programs `bdb_looup` and `imdb_app` must be run in that order,
then the script `src/scripts/request.sh` runs a load-name-sample
request.
## v1.0_oatpp
Integration of OATPP.
### v1.0_oatpp_dev
start of OATPP development.
## v1.0
App `bdb_unix_lookup` provides all requests.
The database configuration and request JSON are specified in files
on the command line. See the [v1.0](V1.0.md) description.
### v0.5_dev
  all load, lookup, select, search functions work for sample database
  branched off to berkeley-db-cpp-framework.
### v0.4_dev
  bdb_unix_lookup runs load/lookup/select/search for sample.
### v0.3_dev  
bdb_unix_lookup compiles and runs load name, title, principals, lookup name, title samples.

