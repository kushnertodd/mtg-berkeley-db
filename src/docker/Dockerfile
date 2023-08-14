FROM ubuntu:latest

# Create a new directory in the container
#WORKDIR /app

# Copy the C++ source code into the container
COPY ./bdb_inet_lookup.sh /
RUN chmod 755 /bdb_inet_lookup.sh 
COPY ./bdb_inet_lookup .
COPY ./bdb_databases.json .
COPY ./name.db .
COPY ./name_tripthongs.db .
COPY ./principals.db .
COPY ./principals_name_id.sdb .
COPY ./principals_title_id.sdb .
COPY ./ratings.db .
COPY ./title.db .
COPY ./title_tripthongs.db .

# Expose any necessary ports.
EXPOSE 8000

# Set the command to run the executable when the container starts
ENTRYPOINT ["/bdb_inet_lookup.sh"]
#CMD ["bdb_inet_lookup", "-d", "bdb_databases.json", "-h", "."] 
