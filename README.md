---

# Prayana Electric Registration Slot System - Test Bench

## 🚗 Allocation System Workflow

The allocation system is designed to handle tokens for users at charging stations and ensure they can create an account for the dashboard and use the electric bike. Here's how it works:

### 1. Charging Station Token Registration

- **When a person inserts their ID card at the charging station:**
  - The server checks if the token (associated with the person's ID card) already exists in the `tokens` collection in MongoDB.
  - If the token doesn't exist, it is added to the `tokens` collection in the database.
  - This ensures that only registered tokens are used to create an account or access services.

### 2. Creating a Dashboard Account

- **When a person tries to create a dashboard account using their ID card and the token given to them at the charging station:**
  - The server checks if the token exists in the `tokens` collection.
  - If the token is valid and exists in the collection, the server allows the user to proceed with account creation for the dashboard.
  - If the token is not present, the server responds with an error, preventing account creation.

### 3. Using the E-Bike

- **When a person tries to use their ID card on the electric bike:**
  - The server checks if the token is present in the `registered_users` collection.
  - If the token is found in the `registered_users` collection (indicating the user has created an account), the user is allowed to use the bike.
  - If the token is not found, the server responds with an error, denying access to the bike.

This workflow ensures that only valid tokens allow users to register and use the system, providing a secure and streamlined process for managing electric bike rentals and user accounts.

---

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
   docker exec -it <use correct container name> mongosh
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

3. **Gunicorn Error Handling**

   If you're running the Flask app in production with Gunicorn, error handling is crucial. In case of issues with Gunicorn or server startup, you can view the logs for errors and troubleshooting:

   - **To view Gunicorn logs:**
     ```sh
     docker logs -f <your_gunicorn_container_name>
     ```

   - **Common issues:**
     - If Gunicorn fails to start, check for missing dependencies or application errors.
     - Ensure that the `requirements.txt` file is up to date, and all dependencies are installed.

   - **Error Handling in Gunicorn:**
     If an error occurs within the Flask app, Gunicorn will log the error and terminate the request. For detailed logs, make sure Gunicorn is set to log to stdout and stderr, as defined in the `CMD` directive in the `Dockerfile`.

---

## 📡 API Endpoints

### 1. User Signup
- **Endpoint:** `POST /signup`
- **Request Body:**
  ```json
  {
    "username": "izzu",
    "password": "password123",
    "re_password": "password123",
    "token": "test123"
  }
  ```
- **Response:**
  - If token is invalid: `{ "error": "Please go to the nearest charging station." }`
  - If username already exists: `{ "error": "Username already exists" }`
  - If passwords do not match: `{ "error": "Passwords do not match" }`
  - If successful: `{ "message": "Welcome to Prayana, izzu!" }`

- **Test with `curl`**:
   ```sh
   curl -X POST http://localhost:5000/signup \
   -H "Content-Type: application/json" \
   -d '{
         "username": "izzu",
         "password": "password123",
         "re_password": "password123",
         "token": "test123"
       }'
   ```

### 2. User Login
- **Endpoint:** `POST /login`
- **Request Body:**
  ```json
  {
    "username": "izzu",
    "password": "password123"
  }
  ```
- **Response:**
  - If login is successful: `{ "message": "Welcome to Prayana, izzu" }`
  - If login fails (invalid credentials): `{ "error": "Invalid credentials" }`

- **Test with `curl`**:
   ```sh
   curl -X POST http://localhost:5000/login \
   -H "Content-Type: application/json" \
   -d '{
         "username": "izzu",
         "password": "password123"
       }'
   ```

### 3. Token Authentication
- **Endpoint:** `POST /auth`
- **Request Body:**
  ```json
  {
    "token": "test123"
  }
  ```
- **Response:**
  - If token is valid: `{ "message": "access granted" }`
  - If token is invalid: `{ "error": "Access denied" }`

- **Test with `curl`**:
   ```sh
   curl -X POST http://localhost:5000/auth \
   -H "Content-Type: application/json" \
   -d '{
         "token": "test123"
       }'
   ```

### 4. Token Registration
- **Endpoint:** `POST /token`
- **Request Body:**
  ```json
  {
    "token": "test123"
  }
  ```
- **Response:**
  - If token is missing: `{ "error": "Token value required" }`
  - If token already exists: `{ "error": "Token already exists" }`
  - If successful: `{ "message": "Token registered successfully" }`

- **Test with `curl`**:
   ```sh
   curl -X POST http://localhost:5000/token \
   -H "Content-Type: application/json" \
   -d '{
         "token": "test123"
       }'
   ```

---

