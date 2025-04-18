from flask import Flask, request, jsonify
from flask_cors import CORS
from pymongo import MongoClient
import bcrypt

app = Flask(__name__)
CORS(app)  # Enable CORS for all routes

# Connect to MongoDB
client = MongoClient("mongodb://mongo_container:27017/")
db = client["prayana"]

# Collections
tokens_collection = db["tokens"]
registered_users_collection = db["registered_users"]

@app.route('/signup', methods=['POST'])
def signup():
    data = request.json
    username = data.get("username")
    password = data.get("password")
    re_password = data.get("re_password")
    token = data.get("token")

    # Check if token exists in 'tokens' collection
    if not tokens_collection.find_one({"token": token}):
        return jsonify({"error": "Please go to the nearest charging station."}), 400
    
    # Check if username already exists
    if registered_users_collection.find_one({"username": username}):
        return jsonify({"error": "Username already exists"}), 409

    # Validate passwords match
    if password != re_password:
        return jsonify({"error": "Passwords do not match"}), 400

    # Hash password
    hashed_password = bcrypt.hashpw(password.encode('utf-8'), bcrypt.gensalt())

    # Insert user into database
    registered_users_collection.insert_one({
        "username": username,
        "password": hashed_password.decode('utf-8'),
        "token": token
    })

    return jsonify({"message": f"Welcome to Prayana, {username}!"}), 201

@app.route('/login', methods=['POST'])
def login():
    data = request.json
    username = data.get("username")
    password = data.get("password")

    # Find user
    user = registered_users_collection.find_one({"username": username})
    if not user:
        return jsonify({"error": "Invalid credentials"}), 401

    # Check password
    if not bcrypt.checkpw(password.encode('utf-8'), user["password"].encode('utf-8')):
        return jsonify({"error": "Invalid credentials"}), 401

    return jsonify({"message": f"Welcome to Prayana, {username}!"}), 200


@app.route('/auth', methods =['POST'])
def auth():
    data = request.json
    token = data.get("token")

    if not token:
        return jsonify({"error":"please send a token"}), 400
    if tokens_collection.find_one({"token": token }):
        return jsonify({"message":"access granted"}) , 201
    if not tokens_collection.find_one({"token": token}):
        return jsonify({"error": "Access denaid"}), 401

@app.route('/token', methods =['POST'])
def token():
    data = request.json
    token = data.get("token")

    if not token:
        return jsonify({"error": "Token value required"}), 400
    if tokens_collection.find_one({"token": token}):
        return jsonify({"error": "Token already exists"}), 409

    tokens_collection.insert_one({"token": token})
    return jsonify({"message": "Token registerd Succesully"}), 201

if __name__ == '__main__':
    print("""
██████╗ ███████╗   ███████╗   ██████╗    ███████╗
██╔══██╗██╔════╝   ██╔════╝   ██╔══██╗   ██╔════╝
██████╔╝█████╗     ███████╗   ██████╔╝   ███████╗
██╔═══╝ ██╔══╝     ╚════██║   ██╔══██╗   ╚════██║
██║██╗  ███████╗██╗███████║██╗██║  ██║██╗███████║
╚═╝╚═╝  ╚══════╝╚═╝╚══════╝╚═╝╚═╝  ╚═╝╚═╝╚══════╝
    """)
    app.run(host='0.0.0.0', port=5000, debug=True)
