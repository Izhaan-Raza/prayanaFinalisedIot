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

## 📡 API Endpoints

### 1. Check or Add Token
- **Endpoint:** `POST /check_token`
- **Request Body:**
  ```json
  {
    "token": "test123"
  }
  ```
- **Response:**
  - If the token exists: `{ "message": "Token already exists" }`
  - If the token does not exist: `{ "message": "Token added successfully" }`

### 2. Register User
- **Endpoint:** `POST /register`
- **Request Body:**
  ```json
  {
    "username": "izzu",
    "regno": "12345",
    "token": "test123"
  }
  ```
- **Response:**
  - If token does not exist: `{ "error": "Invalid token" }`
  - If token exists: `{ "message": "User registered successfully" }`

### 3. User Login
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
  - If login fails: `{ "error": "Invalid credentials" }`

## 📝 Postman Test Cases

1. **Check Token**
   - Method: `POST`
   - URL: `http://localhost:5000/check_token`
   - Body (JSON): `{ "token": "test123" }`
   - Expected Response: `{ "message": "Token already exists" }`

2. **Register User**
   - Method: `POST`
   - URL: `http://localhost:5000/register`
   - Body (JSON): `{ "username": "izzu", "regno": "12345", "token": "test123" }`
   - Expected Response: `{ "message": "User registered successfully" }`

3. **Login User**
   - Method: `POST`
   - URL: `http://localhost:5000/login`
   - Body (JSON): `{ "username": "izzu", "password": "password123" }`
   - Expected Response: `{ "message": "Welcome to Prayana, izzu" }`

---

Now you can easily run, debug, and manage the project using Docker! 🚀
