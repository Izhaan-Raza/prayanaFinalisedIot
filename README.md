```
░▒▓███████▓▒░░░▒▓████████▓▒░      ░▒▓███████▓▒░        ░▒▓██████▓▒░        ░▒▓███████▓▒░ 
░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░             ░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░        
░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░             ░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░        
░▒▓███████▓▒░░▒▓██████▓▒░        ░▒▓███████▓▒░       ░▒▓████████▓▒░       ░▒▓██████▓▒░  
░▒▓█▓▒░      ░▒▓█▓▒░             ░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░             ░▒▓█▓▒░ 
░▒▓█▓▒░▒▓██▓▒░▒▓█▓▒░      ░▒▓██▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓██▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓██▓▒░      ░▒▓█▓▒░ 
░▒▓█▓▒░▒▓██▓▒░▒▓████████▓▒░▒▓██▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓██▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓██▓▒░▒▓███████▓▒░  
                                                                                         
```

# Prayana Electric Slot Registration System (P.E.S.R.S.)

## Overview
Welcome to the **Prayana Electric Slot Registration System** (P.E.S.R.S.), a seamless and secure system designed for electric vehicle (EV) charging stations. The system manages user registration, authentication, and token validation through a simple and effective API, ensuring smooth access to charging stations and services.

### **P.E.S.R.S. stands for:**
- **P**rayana **E**lectric **S**lot **R**egistration **S**ystem

## System Workflow
- **Allocation System**: 
  - When a person inserts their ID card at a charging station and their ID card token is not in the tokens collection, the server automatically adds the token.
  - When a user tries to create a dashboard account from their ID card and the token, the server checks if the token exists in the tokens collection. Only then will it allow the user to create a dashboard account.
  - When a user attempts to use their ID card on the e-bike, the server checks if their token is present in the registered users collection.



## API Endpoints

### 1. `/signup` [POST]
This endpoint allows users to sign up and create an account for the dashboard.

**Request Body:**
```json
{
  "username": "john_doe",
  "password": "password123",
  "re_password": "password123",
  "token": "user_token_from_charging_station"
}
```

**Response:**
- Success:
  ```json
  {
    "message": "Welcome to Prayana, john_doe!"
  }
  ```
- Error (Token not found):
  ```json
  {
    "error": "Please go to the nearest charging station."
  }
  ```
- Error (Username exists):
  ```json
  {
    "error": "Username already exists"
  }
  ```

### 2. `/login` [POST]
This endpoint allows users to log in with their credentials.

**Request Body:**
```json
{
  "username": "john_doe",
  "password": "password123"
}
```

**Response:**
- Success:
  ```json
  {
    "message": "Welcome to Prayana, john_doe!"
  }
  ```
- Error:
  ```json
  {
    "error": "Invalid credentials"
  }
  ```

### 3. `/auth` [POST]
This endpoint checks if a valid token is provided.

**Request Body:**
```json
{
  "token": "user_token"
}
```

**Response:**
- Success:
  ```json
  {
    "message": "Access granted"
  }
  ```
- Error:
  ```json
  {
    "error": "Access denied"
  }
  ```

### 4. `/token` [POST]
This endpoint registers a new token for use.

**Request Body:**
```json
{
  "token": "new_token_from_charging_station"
}
```

**Response:**
- Success:
  ```json
  {
    "message": "Token registered successfully"
  }
  ```
- Error (Token exists):
  ```json
  {
    "error": "Token already exists"
  }
  ```

## How to Test the API Using `curl`

Here are some `curl` commands to test the API endpoints.

1. **Test Signup:**
```bash
curl -X POST http://localhost:5000/signup -H "Content-Type: application/json" -d '{
  "username": "john_doe",
  "password": "password123",
  "re_password": "password123",
  "token": "user_token_from_charging_station"
}'
```

2. **Test Login:**
```bash
curl -X POST http://localhost:5000/login -H "Content-Type: application/json" -d '{
  "username": "john_doe",
  "password": "password123"
}'
```

3. **Test Auth:**
```bash
curl -X POST http://localhost:5000/auth -H "Content-Type: application/json" -d '{
  "token": "user_token"
}'
```

4. **Test Token Registration:**
```bash
curl -X POST http://localhost:5000/token -H "Content-Type: application/json" -d '{
  "token": "new_token_from_charging_station"
}'
```

## Running the Application
### 1. Install dependencies:
```bash
pip install -r requirements.txt
```

### 2. Run the Flask Application:
```bash
python server.py
```

This will start the Flask application, and the ASCII art will appear in the terminal, indicating that the system is up and running.

## Error Handling and Gunicorn Configuration
Ensure that Gunicorn is set up properly for production deployment. Example Gunicorn command:
```bash
gunicorn -w 4 -b 0.0.0.0:5000 server:app
```

To view logs, check the standard output or configure a log file for more detailed logging.

---
