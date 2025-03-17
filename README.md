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
   docker exec -it <mongodb_container_name> mongosh
   ```
   Replace `<mongodb_container_name>` with the actual name of your MongoDB container. You can get the name by running:
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
   docker logs -f <flask_container_name>
   ```
   Replace `<flask_container_name>` with the actual name of your Flask container.

2. **If the Flask App is Not Running, Restart It**
   ```sh
   docker restart <flask_container_name>
   ```

---

Now you can easily run, debug, and manage the project using Docker! 🚀
