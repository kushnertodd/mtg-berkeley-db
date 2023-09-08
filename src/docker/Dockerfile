#FROM ubuntu:latest
FROM ubuntu:23.04

# Create a new directory in the container
#WORKDIR /app

# Copy the C++ source code into the container
COPY ./mtg_app.sh /
RUN chmod 755 /mtg_app.sh 
COPY ./mtg_app .
RUN chmod 755 /mtg_app 
COPY ./bdb_databases.json .
COPY ./account.db .
COPY ./account_email.sdb .
COPY ./account_username.sdb .
COPY ./account_username_triplets.db .
COPY ./card.db .
COPY ./card_name.sdb .
COPY ./card_name_triplets.db .
COPY ./card_type_id.sdb .
COPY ./deck_account_id.sdb .
COPY ./deck_card_card_id.sdb .
COPY ./deck_card.db .
COPY ./deck_card_deck_id.sdb .
COPY ./deck.db .
COPY ./deck_name.sdb .
COPY ./deck_name_triplets.db .

# Expose any necessary ports.
EXPOSE 8000

# Set the command to run the executable when the container starts
ENTRYPOINT ["/mtg_app.sh"]
#CMD ["bdb_inet_lookup", "-d", "bdb_databases.json", "-h", "."] 
