
````
██████╗ ███████╗   ███████╗   ██████╗    ███████╗
██╔══██╗██╔════╝   ██╔════╝   ██╔══██╗   ██╔════╝
██████╔╝█████╗     ███████╗   ██████╔╝   ███████╗
██╔═══╝ ██╔══╝     ╚════██║   ██╔══██╗   ╚════██║
██║██╗  ███████╗██╗███████║██╗██║  ██║██╗███████║
╚═╝╚═╝  ╚══════╝╚═╝╚══════╝╚═╝╚═╝  ╚═╝╚═╝╚══════╝
````

# Prayana Electric Slot Registration System (P.E.S.R.S.)

## 🧠 What is P.E.S.R.S.?

**Prayana Electric Slot Registration System (P.E.S.R.S.)** is a token-based user authentication and access control system designed for electric vehicle (EV) charging stations and dashboard integration.

---

## ⚙️ Allocation System Logic

1. 🔌 **At Charging Station (Token Allocation):**
   - When a user inserts their ID card at the charging station, the station generates a token.
   - If the token does not already exist in the database (`tokens` collection), it is automatically added.

2. 🧑‍💻 **Dashboard Sign-Up:**
   - The user can use their token and credentials to sign up via the dashboard.
   - The server verifies the token in the `tokens` collection.
   - If the token exists and the username is not taken, the account is created and stored in `registered_users`.

3. 🛵 **E-Bike Usage (Access Check):**
   - When the user taps their card at the e-bike dock, the server checks if the token exists in the `registered_users` collection.
   - If it does, access is granted to the bike system.

---

## 🔌 API Endpoints

### `POST /signup`
Registers a new user (must use a valid pre-issued token).

**Request Body:**
```json
{
  "username": "john_doe",
  "password": "secret123",
  "re_password": "secret123",
  "token": "abc123"
}
```

**Responses:**
- ✅ Success:
```json
{ "message": "Welcome to Prayana, john_doe!" }
```
- ❌ Token not found:
```json
{ "error": "Please go to the nearest charging station." }
```
- ❌ Username exists:
```json
{ "error": "Username already exists" }
```
- ❌ Password mismatch:
```json
{ "error": "Passwords do not match" }
```

---

### `POST /login`
Log in with your existing credentials.

**Request Body:**
```json
{
  "username": "john_doe",
  "password": "secret123"
}
```

**Responses:**
- ✅ Success:
```json
{ "message": "Welcome to Prayana, john_doe!" }
```
- ❌ Invalid credentials:
```json
{ "error": "Invalid credentials" }
```

---

### `POST /auth`
Check if a token is authorized to access an e-bike.

**Request Body:**
```json
{ "token": "abc123" }
```

**Responses:**
- ✅ Valid:
```json
{ "message": "Access granted" }
```
- ❌ Invalid:
```json
{ "error": "Access denaid" }
```

---

### `POST /token`
Register a token from the charging station.

**Request Body:**
```json
{ "token": "abc123" }
```

**Responses:**
- ✅ Success:
```json
{ "message": "Token registerd Succesully" }
```
- ❌ Already exists:
```json
{ "error": "Token already exists" }
```

---

## 🧪 Testing the API with `curl`

### Signup
```bash
curl -X POST http://localhost:5000/signup -H "Content-Type: application/json" -d '{
  "username": "john_doe",
  "password": "secret123",
  "re_password": "secret123",
  "token": "abc123"
}'
```

### Login
```bash
curl -X POST http://localhost:5000/login -H "Content-Type: application/json" -d '{
  "username": "john_doe",
  "password": "secret123"
}'
```

### Auth Check
```bash
curl -X POST http://localhost:5000/auth -H "Content-Type: application/json" -d '{
  "token": "abc123"
}'
```

### Register Token
```bash
curl -X POST http://localhost:5000/token -H "Content-Type: application/json" -d '{
  "token": "abc123"
}'
```

---

## 🐳 Running with Docker

Make sure you have Docker and Docker Compose installed.

### 1. Build and Start the Containers:
```bash
docker-compose up --build
```

### 2. Access Logs with ASCII Banner:
You’ll see the banner and logs like this when your server is running:
```bash
docker logs <your_flask_container_name>
```

Example:
```bash
docker logs prayana_flask
```

---

## 🛠️ Additional Notes

- Ensure MongoDB is accessible at `mongo_container:27017` as per your `server.py`.
- The Flask server is running inside the container on port `5000`.
- You can modify `docker-compose.yml` for volume mappings or persistent data.

---

