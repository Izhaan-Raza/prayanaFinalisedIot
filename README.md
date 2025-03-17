# Prayana Electric Registration Slot System - Test Bench

This repository contains the test bench code for the Prayana Electric Registration Slot System.

## 🚀 How to Run the Docker Container

1. **Build and Start the Docker Containers**
   ```sh
   docker-compose up --build -d
   ```
   The `-d` flag runs the containers in detached mode.

2. **Check Running Containers**
   ```sh
   docker ps
   ```

## 🗄️ Accessing MongoDB (`mongosh`)

1. **Enter the MongoDB Container**
   ```sh
   docker exec -it prayana_mongo mongosh
   ```
   You can verify the container name by running:
   ```sh
   docker ps
   ```

2. **Switch to the Database**
   ```sh
   use prayana
   ```

3. **Check Collections**
   ```sh
   show collections
   ```

## 📜 Viewing Flask Server Logs

1. **Check Logs of the Flask Server**
   ```sh
   docker logs -f prayana_flask
   ```

2. **If the Flask App is Not Running, Restart It**
   ```sh
   docker restart prayana_flask
   ```

---

Now you can easily run, debug, and manage the project using Docker! 🚀
