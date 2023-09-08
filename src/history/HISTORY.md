# Version history

### v0.1
first version

### v0.2
2023-09-05 version where the web page works displaying users, decks, cards, and editing card page (doesn't work)

To install docker version on Google cloud, do this:
- start instances tkllc-lucy-game-design-instance-1 and app-bdb-inet-lookup-instance-1 if not already started
- on tkllc-lucy-game-design-instance-1
  - upload all web files from src/web to tkllc-lucy-game-design-instance-1
  - ssh to the vm
  - modify request.js to select the request_url to 34.106.93.238:8000 instead of localhost

```
   //static request_url = "http://34.106.93.238:8000";
    static request_url = "http://localhost:8000";
```
  - sudo cp all the files to /var/www/html
- on app-bdb-inet-lookup-instance-1
  - run 'sh build.sh' in src/docker
  - upload all files in src/docker to app-bdb-inet-lookup-instance-1
  - run:
```
    $ sh stop_images.sh
    $ sh remove_images.sh
    $ sh build_api.sh
    $ sh run_api.sh
```

You should be able to connect from http://35.192.73.70/mtg-home.html and see the 'Welcome to the Magic Online Card Game!' page.
You can try:

- select any user from the top dropdown
- the user decks should show up, right-click on a deck
- the deck cards should show up, right-click on a card
- you should see a table with the card name and color, and a field to change them
- you can't actually change those, but select Save or Cancel and you should get a pop-up and have the card editing table disappear
- try selecting other users, decks, and cards and see that it works okay

### v0.3
2023-09-08 version where the web page works:
- displaying users, decks, cards ('Show Cards' button), and cards not in deck ('Add Cards' button)
- 'Add card' button works to move card from not in deck to in deck
- 'Remove card' button does not work
