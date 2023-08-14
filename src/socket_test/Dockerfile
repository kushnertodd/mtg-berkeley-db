# Use a base image with the desired C++ environment
FROM gcc:latest

# Create a new directory in the container
WORKDIR /app
EXPOSE 8080 8080

# Copy the C++ source code into the container
COPY server .
#COPY bdb-databases.json .

CMD ["./server"]
