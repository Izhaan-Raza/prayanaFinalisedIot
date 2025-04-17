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

## 📝 Postman Test Cases

1. **User Signup**
   - Method: `POST`
   - URL: `http://localhost:5000/signup`
   - Body (JSON): 
     ```json
     {
       "username": "izzu",
       "password": "password123",
       "re_password": "password123",
       "token": "test123"
     }
     ```
   - Expected Response: `{ "message": "Welcome to Prayana, izzu!" }` or appropriate error messages (e.g., "Invalid token", "Username already exists", etc.)

2. **User Login**
   - Method: `POST`
   - URL: `http://localhost:5000/login`
   - Body (JSON): `{ "username": "izzu", "password": "password123" }`
   - Expected Response: `{ "message": "Welcome to Prayana, izzu" }` or `{ "error": "Invalid credentials" }`

3. **Token Authentication**
   - Method: `POST`
   - URL: `http://localhost:5000/auth`
   - Body (JSON): `{ "token": "test123" }`
   - Expected Response: `{ "message": "access granted" }` or `{ "error": "Access denied" }`

---

Now you can easily run, debug, and manage the project using Docker! 🚀

---
